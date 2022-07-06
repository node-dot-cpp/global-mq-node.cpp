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


class InterInterThreadCommData
{
	static thread_local BasicThreadInfo thisThreadDescriptor;
	ThreadCommData threadQueues[MAX_THREADS];

public:
	void setThisThreadBasicInfo(BasicThreadInfo& startupData)
	{
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, startupData.slotId < MAX_THREADS ); 
		thisThreadDescriptor = startupData;
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
	MsgQueue& getThisThreadQueue()
	{
		return threadQueues[thisThreadDescriptor.slotId].queue;
	}

	void acquireBasicThreadInfoForNewThread( BasicThreadInfo& startupData )
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
};
thread_local BasicThreadInfo InterInterThreadCommData::thisThreadDescriptor;
static InterInterThreadCommData threadQueues;


void setThisThreadBasicInfo(BasicThreadInfo& startupData)
{
	threadQueues.setThisThreadBasicInfo(startupData);
}

MsgQueue& getThreadQueue( size_t slotId )
{ 
	return threadQueues.getThreadQueue(slotId);
}

MsgQueue& getThisThreadQueue( size_t slotId )
{ 
	return threadQueues.getThisThreadQueue();
}

std::pair<bool, size_t> popFrontFromThisThreadQueue( InterThreadMsg* messages, size_t count )
{
	return threadQueues.popFrontFromThisThreadQueue( messages, count );
}

std::pair<bool, size_t> popFrontFromThisThreadQueue( InterThreadMsg* messages, size_t count, uint64_t timeout )
{
	return threadQueues.popFrontFromThisThreadQueue( messages, count, timeout );
}

void runThreadTerminationCleanupRoutines()
{
}

/*uintptr_t initInterThreadCommSystemAndGetReadHandleForMainThread()
{
	return interThreadCommInitializer.init();
}*/

globalmq::marshalling::GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;

globalmq::marshalling::GMQTransportBase<GMQueueStatePublisherSubscriberTypeInfo>* getTransport() {
	return ((nodecpp::NLS*)(nodecpp::nodeLocalData))->transport;
}


void acquireBasicThreadInfoForNewThread( BasicThreadInfo& startupData )
{
	threadQueues.acquireBasicThreadInfoForNewThread( startupData );
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

