/* -------------------------------------------------------------------------------
* Copyright (c) 2018, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the OLogN Technologies AG nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL OLogN Technologies AG BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

#include "../include/infrastructure/q_based_infrastructure.h"

#include "../include/nodecpp/nls.h"

#include <time.h>
#include <climits>

#ifdef _MSC_VER
#include <Windows.h>
#else
#define _GNU_SOURCE
#include <poll.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>
/* pid_t */
#include <sys/types.h>
/* open */
#include <sys/stat.h>
#include <fcntl.h>
/* strrchr */
#include <string.h>
#ifndef gettid
#define gettid() syscall(SYS_gettid)
#endif
#include <sys/resource.h>
#endif
#include <chrono>


namespace nodecpp {

//thread_local NLS threadLocalData;
	thread_local void* nodeLocalData;
} // namespace nodecpp

//thread_local MsgQueue interThreadMessageQueues[1000]; // TODO:  way to customize

thread_local TimeoutManager* timeoutManager;


uint64_t infraGetCurrentTime()
{
#ifdef _MSC_VER
//	return GetTickCount64() * 1000; // mks
	auto now = ::std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
#else
    struct timespec ts;
//    timespec_get(&ts, TIME_UTC);
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000; // mks
#endif
}

thread_local EvQueue* inmediateQueue;

int getPollTimeout(uint64_t nextTimeoutAt, uint64_t now)
{
	 	if(nextTimeoutAt == TimeOutNever)
	 		return -1;
	 	else if(nextTimeoutAt <= now)
	 		return 0;
	 	else if((nextTimeoutAt - now) / 1000 + 1 <= uint64_t(INT_MAX))
	 		return static_cast<int>((nextTimeoutAt - now) / 1000 + 1);
	 	else
	         return INT_MAX;
}

namespace nodecpp {
	nodecpp::Timeout setTimeout(std::function<void()> cb, int32_t ms)
	{
		return timeoutManager->appSetTimeout(cb, ms, infraGetCurrentTime());
	}

#ifndef NODECPP_NO_COROUTINES
	nodecpp::Timeout setTimeoutForAction(awaitable_handle_t h, int32_t ms)
	{
		return timeoutManager->appSetTimeoutForAction(h, ms, infraGetCurrentTime());
	}
#endif // NODECPP_NO_COROUTINES

	void refreshTimeout(Timeout& to)
	{
		return timeoutManager->appRefresh(to.getId(), infraGetCurrentTime());
	}

	void clearTimeout(const Timeout& to)
	{
		return timeoutManager->appClearTimeout(to);
	}

	Timeout::~Timeout()
	{
		if ( id != 0 )
			timeoutManager->appTimeoutDestructor(id);
	}

	void setInmediate(std::function<void()> cb)
	{
		inmediateQueue->add(std::move(cb));
	}

} // namespace nodecpp


//thread_local NodeBase* thisThreadNode = nullptr;

// InterThread communication (low level)


class InterThreadCommData
{
	std::mutex mx; // covers adding and removing threads
	static thread_local BasicThreadInfo thisThreadDescriptor;
	ThreadCommData threadQueues[MAX_THREADS];

public:
	void setThisThreadBasicInfo(BasicThreadInfo& startupData)
	{
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, startupData.slotId < MAX_THREADS ); 
		thisThreadDescriptor = startupData;
	}

	std::pair<bool, InterThreadMsg> popFrontFromThisThreadQueue()
	{
		return threadQueues[thisThreadDescriptor.slotId].queue.pop_front();
	}

	std::pair<bool, size_t> popFrontFromThisThreadQueue( InterThreadMsg* messages, size_t count )
	{
		return threadQueues[thisThreadDescriptor.slotId].queue.pop_front( messages, count );
	}

	std::pair<bool, size_t> popFrontFromThisThreadQueue( InterThreadMsg* messages, size_t count, uint64_t timeout )
	{
		return threadQueues[thisThreadDescriptor.slotId].queue.pop_front( messages, count, timeout );
	}

	MsgQueue& getThreadQueue( size_t slotId )
	{ 
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, slotId < MAX_THREADS ); 
		return threadQueues[slotId].queue;
	}

	ThreadQueueID getThisThreadQueueID()
	{
		return ThreadQueueID( thisThreadDescriptor.slotId );
	}

	MsgQueue& getThisThreadQueue()
	{
		return threadQueues[thisThreadDescriptor.slotId].queue;
	}

	void implAcquireBasicThreadInfoForNewThread( BasicThreadInfo& startupData )
	{
		for ( size_t slotIdx = 1; slotIdx < MAX_THREADS; ++slotIdx )
		{
			auto ret = threadQueues[slotIdx].acquireForReuse();
			if ( ret.first )
			{
				startupData.reincarnation = ret.second;
				startupData.slotId = slotIdx;
				break;
			}
		}
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, startupData.slotId != BasicThreadInfo::InvalidSlotID ); 
		startupData.defaultLog = nodecpp::logging_impl::currentLog;
	}

	bool acquireBasicThreadInfoForNewThread( BasicThreadInfo& startupData )
	{
		std::unique_lock<std::mutex> lock(mx);
		if ( thisThreadDescriptor.slotId == BasicThreadInfo::InvalidSlotID )
		{
			implAcquireBasicThreadInfoForNewThread( startupData );
			return true;
		}
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, !threadQueues[thisThreadDescriptor.slotId].isUnused() ); 
		if ( !threadQueues[thisThreadDescriptor.slotId].isTerminating() )
		{
			implAcquireBasicThreadInfoForNewThread( startupData );
			return true;
		}
		startupData.invalidate();
		return false;
	}

	void setTerminatingFlag( size_t slotId, uint64_t reincarnation )
	{
		if ( slotId != BasicThreadInfo::InvalidSlotID )
		{
			NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, slotId < MAX_THREADS );
			NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, !threadQueues[slotId].isUnused() );
			threadQueues[slotId].setTerminating( reincarnation );
		}
	}

	void thisThreadHasTerminated()
	{
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, threadQueues[thisThreadDescriptor.slotId].isTerminating() ); // in current version it is the only exit condition and that's why we're here
		static constexpr size_t maxMsgCnt = 8;
		std::pair<bool, size_t> fromQ;
		do
		{
			InterThreadMsg thq[maxMsgCnt];
			fromQ = popFrontFromThisThreadQueue( thq, maxMsgCnt, 0 );
			NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, !fromQ.first ); // in current version it is the only exit condition and that's why we're here
		}
		while ( fromQ.second != 0 );
		threadQueues[thisThreadDescriptor.slotId].setUnused( thisThreadDescriptor.reincarnation );
		thisThreadDescriptor.invalidate();
	}

	void setAllThreadsTerminating() // Status: EXPERIMENTAL
	{
		std::unique_lock<std::mutex> lock(mx);
		for ( size_t i=0; i<MAX_THREADS; ++i )
			if ( !threadQueues[i].isUnused() )
				threadQueues[i].setTerminating();
	}

	bool haveAllThreadsTerminated() // Status: EXPERIMENTAL
	{
		std::unique_lock<std::mutex> lock(mx);
		for ( size_t i=0; i<MAX_THREADS; ++i )
			if ( !threadQueues[i].isUnused() )
				return false;
		return true;
	}
};
thread_local BasicThreadInfo InterThreadCommData::thisThreadDescriptor;
static InterThreadCommData threadQueues;

void CommThreadHandle::setTerminatingFlag()
{
	threadQueues.setTerminatingFlag( slotId, reincarnation);
}

void setAllThreadsTerminating() // Status: EXPERIMENTAL
{
	threadQueues.setAllThreadsTerminating();
}

bool haveAllThreadsTerminated() // Status: EXPERIMENTAL
{
	return threadQueues.haveAllThreadsTerminated();
}

void setThisThreadBasicInfo(BasicThreadInfo& startupData)
{
	threadQueues.setThisThreadBasicInfo(startupData);
}

ThreadQueueID getThisThreadQueueID()
{
	return threadQueues.getThisThreadQueueID();
}

MsgQueue& getThreadQueue( size_t slotId )
{ 
	return threadQueues.getThreadQueue(slotId);
}

MsgQueue& getThisThreadQueue()
{ 
	return threadQueues.getThisThreadQueue();
}

void postInfrastructuralMessage( ThreadQueueID id, nodecpp::platform::internal_msg::InternalMsg&& msg )
{
	auto& queue = threadQueues.getThreadQueue( id.id );
	queue.push_back( InterThreadMsg( std::move( msg ), InterThreadMsgType::Infrastructural ) );
}

std::pair<bool, InterThreadMsg> popFrontFromThisThreadQueue()
{
	return threadQueues.popFrontFromThisThreadQueue();
}

std::pair<bool, size_t> popFrontFromThisThreadQueue( InterThreadMsg* messages, size_t count )
{
	return threadQueues.popFrontFromThisThreadQueue( messages, count );
}

std::pair<bool, size_t> popFrontFromThisThreadQueue( InterThreadMsg* messages, size_t count, uint64_t timeout )
{
	return threadQueues.popFrontFromThisThreadQueue( messages, count, timeout );
}

void thisThreadHasTerminated()
{
	threadQueues.thisThreadHasTerminated();
}

globalmq::marshalling::GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;

globalmq::marshalling::GMQTransportBase<GMQueueStatePublisherSubscriberTypeInfo>* getTransport() {
	return ((nodecpp::NLS*)(nodecpp::nodeLocalData))->transport;
}




bool acquireBasicThreadInfoForNewThread( BasicThreadInfo& startupData )
{
	return threadQueues.acquireBasicThreadInfoForNewThread( startupData );
}

/*void postInfrastructuralMsg(nodecpp::Message&& msg, NodeAddress threadId )
{
	postInterThreadMsg( std::move( msg ), InterThreadMsgType::Infrastructural, threadId );
}

void internalPostlGlobalMQ(nodecpp::Message&& msg, NodeAddress threadId )
{
	postInterThreadMsg( std::move( msg ), InterThreadMsgType::GlobalMQ, threadId );
}*/


#ifndef NODECPP_NO_COROUTINES
nodecpp::handler_ret_type nodecpp::a_timeout(uint32_t ms)
{
	co_await ::a_timeout_impl( ms );
	co_return;
}
nodecpp::handler_ret_type nodecpp::a_sleep(uint32_t ms)
{
	co_await ::a_timeout_impl( ms );
	co_return;
}
#endif

