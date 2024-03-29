/* -------------------------------------------------------------------------------
* Copyright (c) 2018-2021, OLogN Technologies AG
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


#ifndef Q_BASED_INFRASTRUCTURE_H
#define Q_BASED_INFRASTRUCTURE_H

#include "../include/nodecpp/common.h"
#include "../include/nodecpp/common_structs.h"
#include "../include/nodecpp/nls.h"

#include "../../src/interthread_comm_impl.h"

#include "ev_queue.h"

#include "../include/nodecpp/timers.h"
#include <functional>

#include <gmqueue.h>

/*#ifdef NODECPP_RECORD_AND_REPLAY
#include "tcp_socket/tcp_socket_replaying_loop.h"
#endif // NODECPP_RECORD_AND_REPLAY*/

#include "timeout_manager.h"

int getPollTimeout(uint64_t nextTimeoutAt, uint64_t now);
uint64_t infraGetCurrentTime();

extern thread_local TimeoutManager* timeoutManager;
extern thread_local EvQueue* inmediateQueue;
extern globalmq::marshalling::GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;

#ifndef NODECPP_NO_COROUTINES
inline
auto a_timeout_impl(uint32_t ms) { 

    struct timeout_awaiter {

        std::experimental::coroutine_handle<> who_is_awaiting;
		uint32_t duration = 0;
		nodecpp::Timeout to;

        timeout_awaiter(uint32_t ms) {duration = ms;}

        timeout_awaiter(const timeout_awaiter &) = delete;
        timeout_awaiter &operator = (const timeout_awaiter &) = delete;

        timeout_awaiter(timeout_awaiter &&) = delete;
        timeout_awaiter &operator = (timeout_awaiter &&) = delete;

        ~timeout_awaiter() {}

        bool await_ready() {
            return false;
        }

        void await_suspend(std::experimental::coroutine_handle<> awaiting) {
			nodecpp::initCoroData(awaiting);
            who_is_awaiting = awaiting;
			to = timeoutManager->appSetTimeout(awaiting, duration, infraGetCurrentTime());
        }

		auto await_resume() {
			if ( nodecpp::isCoroException(who_is_awaiting) )
				throw nodecpp::getCoroException(who_is_awaiting);
		}
    };
    return timeout_awaiter(ms);
}
#endif // NODECPP_NO_COROUTINES

template<typename NodeT, typename MessageT> concept has_global_mq_message_handler_call = requires { { std::declval<NodeT>().onGlobalMQMessage(std::declval<MessageT&>()) }; };
template<typename NodeT, typename MessageT> concept has_infrastructure_message_handler_call = requires { { std::declval<NodeT>().onInfrastructuralMessage(std::declval<MessageT&>()) }; };
template<typename NodeT> concept has_invariant_checker_call = requires { { std::declval<NodeT>().dbgInvariantChecker() }; };

template<class Node>
struct InfraNodeWrapper
{
	using NlsT = nodecpp::NLS;
	using NodeT = Node;
	static constexpr bool has_global_mq_message_handler = has_global_mq_message_handler_call<NodeT, nodecpp::platform::internal_msg::InternalMsg>;
	static constexpr bool has_infrastructure_message_handler = has_infrastructure_message_handler_call<NodeT, nodecpp::platform::internal_msg::InternalMsg>;
	static constexpr bool has_invariant_checker = has_invariant_checker_call<NodeT>;
	NlsT nls;
#ifdef NODECPP_USE_IIBMALLOC
		nodecpp::iibmalloc::ThreadLocalAllocatorT allocManager;
#endif
	safememory::owning_ptr<NodeT> node;

	globalmq::marshalling::GMQTransportBase<GMQueueStatePublisherSubscriberTypeInfo> transport;
	InfraNodeWrapper() : transport( gmqueue ) {
		nls.transport = &transport;
	}
};

template<class UserNodeT>
class NodeProcessor
{
	using NodeType = InfraNodeWrapper<UserNodeT>;
	template<class InfraT, class Node> 
	friend class Runnable;
	
	TimeoutManager timeout;
	EvQueue immediateEvQueue;

	NodeType node;

public:
	NodeProcessor() {}
	~NodeProcessor() { deinit(); }

	safememory::soft_ptr<UserNodeT> getNode() { return node.node; }

	int init( const globalmq::marshalling::InProcTransferrable<GMQueueStatePublisherSubscriberTypeInfo>& tdata ) {
		node.transport.restore( tdata, gmqueue );
		nodecpp::nodeLocalData = &(node.nls);

#ifdef NODECPP_USE_IIBMALLOC
		node.allocManager.initialize();
		::nodecpp::iibmalloc::ThreadLocalAllocatorT* formerAlloc = ::nodecpp::iibmalloc::setCurrneAllocator( &(node.allocManager) );
#endif
		timeoutManager = &(getTimeout());
		inmediateQueue = &(getInmediateQueue());

		node.node = safememory::make_owning<typename NodeType::NodeT>();

#ifdef NODECPP_RECORD_AND_REPLAY
		if ( replayMode == nodecpp::record_and_replay_impl::BinaryLog::Mode::recording )
			node.node->binLog.initForRecording( 26 );
		::nodecpp::threadLocalData.binaryLog = &(node.node->binLog);
#endif // NODECPP_RECORD_AND_REPLAY
		// NOTE!!! 
		// By coincidence it so happened that both void Node::main() and nodecpp::handler_ret_type Node::main() are currently treated in the same way.
		// If, for any reason, treatment should be different, to check exactly which one is present, see, for instance
		// http://www.gotw.ca/gotw/071.htm and 
		// https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature
#ifdef NODECPP_RECORD_AND_REPLAY
		if ( replayMode == nodecpp::record_and_replay_impl::BinaryLog::Mode::recording )
			::nodecpp::threadLocalData.binaryLog->addFrame( record_and_replay_impl::BinaryLog::FrameType::node_main_call, nullptr, 0 );
#endif // NODECPP_RECORD_AND_REPLAY
		node.node->main();

		uint64_t nextTimeoutAt = nextTimeout();
		uint64_t now = infraGetCurrentTime();
		int timeoutToUse = getPollTimeout(nextTimeoutAt, now);

		timeoutManager = nullptr;
		inmediateQueue = nullptr;
		safememory::detail::killAllZombies();

#ifdef NODECPP_USE_IIBMALLOC
		::nodecpp::iibmalloc::setCurrneAllocator( formerAlloc );
#endif

		nodecpp::nodeLocalData = nullptr;

		return timeoutToUse;
	}

	void deinit()
	{
#ifdef NODECPP_USE_IIBMALLOC
		::nodecpp::iibmalloc::ThreadLocalAllocatorT* formerAlloc = ::nodecpp::iibmalloc::setCurrneAllocator( &(node.allocManager) );
#endif
		node.node = nullptr;
		timeout.clearForGracefulTerminaion();
#ifdef NODECPP_THREADLOCAL_INIT_BUG_GCC_60702
		nodecpp::net::SocketBase::DataForCommandProcessing::userHandlerClassPattern.destroy();
		nodecpp::net::ServerBase::DataForCommandProcessing::userHandlerClassPattern.destroy();
#endif // NODECPP_THREADLOCAL_INIT_BUG_GCC_60702
		safememory::detail::killAllZombies();
#ifdef NODECPP_USE_IIBMALLOC
		::nodecpp::iibmalloc::setCurrneAllocator( formerAlloc );
#endif
	}

public:
//	bool running = true;
	uint64_t nextTimeoutAt = 0;
	TimeoutManager& getTimeout() { return timeout; }
	EvQueue& getInmediateQueue() { return immediateEvQueue; }
	void emitInmediates() { immediateEvQueue.emit(); }

	bool refedTimeout() const noexcept
	{
		return !immediateEvQueue.empty() || timeout.infraRefedTimeout();
	}

	uint64_t nextTimeout() const noexcept
	{
		return immediateEvQueue.empty() ? timeout.infraNextTimeout() : 0;
	}

	uint64_t processMessagesAndOrTimeout( InterThreadMsg* thq )
	{
		nodecpp::nodeLocalData = &(node.nls);

#ifdef NODECPP_USE_IIBMALLOC
		::nodecpp::iibmalloc::ThreadLocalAllocatorT* formerAlloc = ::nodecpp::iibmalloc::setCurrneAllocator( &(node.allocManager) );
#endif
		timeoutManager = &(getTimeout());
		inmediateQueue = &(getInmediateQueue());

		EvQueue queue;
		uint64_t now = infraGetCurrentTime();
		timeout.infraTimeoutEvents(now, queue);
		queue.emit();

		if ( thq )
		{
			switch( thq->msgType )
			{
				case InterThreadMsgType::GlobalMQ:
				{
					if constexpr ( NodeType::has_global_mq_message_handler )
						node.node->onGlobalMQMessage( thq->msg );
					else
						throw std::exception(); // unexpected / unhandled message type
					break;
				}
				case InterThreadMsgType::Infrastructural:
				{
					if constexpr ( NodeType::has_infrastructure_message_handler )
						node.node->onInfrastructuralMessage( thq->msg );
					else
						throw std::exception(); // unexpected / unhandled message type
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
		}

		emitInmediates();

		bool refed = refedTimeout();
		uint64_t nextTimeoutAt = nextTimeout();
		now = infraGetCurrentTime();
//		int timeoutToUse = getPollTimeout(nextTimeoutAt, now);
		uint64_t timeoutToUse = nextTimeoutAt == TimeOutNever ? TimeOutNever : (nextTimeoutAt > now ? nextTimeoutAt - now : 0);

		timeoutManager = nullptr;
		inmediateQueue = nullptr;
		safememory::detail::killAllZombies();
#ifdef NODECPP_USE_IIBMALLOC
		::nodecpp::iibmalloc::setCurrneAllocator( formerAlloc );
#endif

		nodecpp::nodeLocalData = nullptr;
					
		if constexpr ( NodeType::has_invariant_checker )
			node.node->dbgInvariantChecker();

		return timeoutToUse;
	}

	void run() {}
};

bool acquireBasicThreadInfoForNewThread( BasicThreadInfo& startupData );
MsgQueue& getThreadQueue( size_t slotId );
MsgQueue& getThisThreadQueue();
void thisThreadHasTerminated();
void setAllThreadsTerminating(); // Status: EXPERIMENTAL
bool haveAllThreadsTerminated(); // Status: EXPERIMENTAL

class CommThreadHandle
{
	friend class RaiiCommThreadHandle;
	size_t slotId = BasicThreadInfo::InvalidSlotID;
	uint64_t reincarnation = BasicThreadInfo::InvalidReincarnation;
public:
	void setTerminatingFlag();
	CommThreadHandle( size_t slotId_, uint64_t reincarnation_ ) noexcept : slotId( slotId_ ), reincarnation( reincarnation_ ) {}
};

class RaiiCommThreadHandle : public CommThreadHandle
{
public:
	RaiiCommThreadHandle( CommThreadHandle h ) : CommThreadHandle( h ) {}
	RaiiCommThreadHandle( size_t slotId, uint64_t reincarnation ) : CommThreadHandle( slotId, reincarnation ) {}
	RaiiCommThreadHandle( const RaiiCommThreadHandle& ) = delete;
	RaiiCommThreadHandle& operator = ( const RaiiCommThreadHandle& ) = delete;
	RaiiCommThreadHandle( RaiiCommThreadHandle&& other ) noexcept : CommThreadHandle( other.slotId, other.reincarnation )
	{
		if ( &other != this )
		{
			other.slotId = BasicThreadInfo::InvalidSlotID;
			other.reincarnation = BasicThreadInfo::InvalidReincarnation;
		}
	}
	RaiiCommThreadHandle& operator = ( RaiiCommThreadHandle&& other ) noexcept
	{
		slotId = other.slotId;
		other.slotId = BasicThreadInfo::InvalidSlotID;
		reincarnation = other.reincarnation;
		other.reincarnation = BasicThreadInfo::InvalidReincarnation;
		return *this;
	}
	CommThreadHandle detach()
	{
		CommThreadHandle ret = *this;
		slotId = BasicThreadInfo::InvalidSlotID;
		reincarnation = BasicThreadInfo::InvalidReincarnation;
		return ret;
	}
	~RaiiCommThreadHandle()
	{
		setTerminatingFlag();
	}
};
	
template<class NodeT>
class QueueBasedInfrastructure : public NodeProcessor<NodeT>
{
//	bool running = true;
	uint64_t timeoutToUse = 0;
public:
	QueueBasedInfrastructure() : NodeProcessor<NodeT>() {}

	void init()
	{
		timeoutToUse = NodeProcessor<NodeT>::init();
	}

	void run()
	{
		static constexpr size_t maxMsgCnt = 8;
		for (;;) // TODO: exit condition except kill flag at queue
		{
			InterThreadMsg thq[maxMsgCnt];
			auto actualFromQueue = timeoutToUse == TimeOutNever ? popFrontFromThisThreadQueue( thq, maxMsgCnt ) : popFrontFromThisThreadQueue( thq, maxMsgCnt, timeoutToUse );
//if ( timeoutToUse == TimeOutNever )
//	nodecpp::log::default_log::info( nodecpp::log::ModuleID(nodecpp::nodecpp_module_id),"timeoutToUse == never, msg cnt = {}", actualFromQueue.second);

			if ( actualFromQueue.first )
			{
				if ( actualFromQueue.second )
					for ( size_t i=0; i<actualFromQueue.second; ++i )
					{
						NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, thq[i].recipientID == 0 ); // current version limitation: just a single node per thread
						timeoutToUse = NodeProcessor<NodeT>::processMessagesAndOrTimeout( thq + i );
					}
				else
					timeoutToUse = NodeProcessor<NodeT>::processMessagesAndOrTimeout( nullptr );
			}
			else
				break;
		}
	}

	~QueueBasedInfrastructure()
	{
		thisThreadHasTerminated();
	}
};

template<class NodeT>
class NodeLoopBase
{
public:
	class Initializer
	{
		BasicThreadInfo data;
		friend class NodeLoopBase;
		bool acquire() {
			return acquireBasicThreadInfoForNewThread( data );
		}
	public:
		globalmq::marshalling::InProcTransferrable<GMQueueStatePublisherSubscriberTypeInfo> transportData;

		Initializer() {}
		Initializer( const Initializer& ) = default;
		Initializer& operator = ( const Initializer& ) = default;
		Initializer( Initializer&& ) = default;
		Initializer& operator = ( Initializer&& ) = default;

		const BasicThreadInfo& getBasicThreadInfo() { return data; }
	};

private:
	BasicThreadInfo threadInfo;
	globalmq::marshalling::InProcTransferrable<GMQueueStatePublisherSubscriberTypeInfo> transportData;
	bool initialized = false;
	bool entered = false;

public:
	NodeLoopBase() {}
	NodeLoopBase( Initializer i ) { 
		threadInfo = i.data;
		transportData = i.transportData;
		setThisThreadBasicInfo( i.data ); 
		nodecpp::logging_impl::currentLog = i.data.defaultLog;
		nodecpp::logging_impl::instanceId = i.data.slotId;
		nodecpp::log::default_log::info( nodecpp::log::ModuleID(nodecpp::nodecpp_module_id), "starting Node thread with threadID = {}", i.data.slotId );
		initialized = true;
	}

	static std::pair<Initializer, uint64_t> getInitializer()
	{
		Initializer i;
		i.acquire();
		return std::make_pair(i, i.data.slotId);
	}

protected:
	template<class InfraT>
	int init( InfraT& infra, uint64_t nodeID )
	{
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, !entered ); 
		entered = true;
		// note: startup data must be allocated using std allocator (reason: freeing memory will happen at a new thread)
		if ( !initialized )
		{
			bool ok = acquireBasicThreadInfoForNewThread( threadInfo );
			NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, ok ); 
			initialized = true;
		}
		size_t threadIdx = threadInfo.slotId;
		nodecpp::log::default_log::info( nodecpp::log::ModuleID(nodecpp::nodecpp_module_id),"about to start Listener thread with threadID = {}...", threadIdx );

		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, threadInfo.slotId != 0 ); 
		setThisThreadBasicInfo( threadInfo );
		nodecpp::logging_impl::currentLog = threadInfo.defaultLog;
		nodecpp::logging_impl::instanceId = threadInfo.slotId;
		nodecpp::log::default_log::info( nodecpp::log::ModuleID(nodecpp::nodecpp_module_id), "starting Node thread with threadID = {}", threadInfo.slotId );

		return infra.init( transportData );
	}

	template<class InfraT>
	void run( InfraT& infra)
	{
		infra.run();
	}
};


template<class NodeT>
class NoNodeLoop : public NodeLoopBase<NodeT>
{
	NodeProcessor<NodeT> infra;
public:
	NoNodeLoop() {}
	NoNodeLoop( typename NodeLoopBase<NodeT>::Initializer i ) : NodeLoopBase<NodeT>( i ) {}

	safememory::soft_ptr<NodeT> getNode() { return infra.getNode(); }
	
	int init( uint64_t nodeID )
	{
		return NodeLoopBase<NodeT>::template init<NodeProcessor<NodeT>>(infra, nodeID);
	}

	int onInfrastructureMessage( InterThreadMsg&& thq )
	{
		return infra.processMessagesAndOrTimeout( &thq );
	}

	int onTimeout()
	{
		return infra.processMessagesAndOrTimeout( nullptr );
	}
};

template<class NodeT>
class QueueBasedNodeLoop : public NodeLoopBase<NodeT>
{
	QueueBasedInfrastructure<NodeT> infra;
public:
	QueueBasedNodeLoop() {}
	QueueBasedNodeLoop( typename NodeLoopBase<NodeT>::Initializer i ) : NodeLoopBase<NodeT>( i ) {
	}

	safememory::soft_ptr<NodeT> getNode() { return infra.getNode(); }
	
	int init( uint64_t nodeID )
	{
		return NodeLoopBase<NodeT>::template init<NodeProcessor<NodeT>>(infra, nodeID);
	}
	
	int init()
	{
		return NodeLoopBase<NodeT>::template init<NodeProcessor<NodeT>>(infra, (uint64_t)(-1));
	}

	void run()
	{
		NodeLoopBase<NodeT>::template run<QueueBasedInfrastructure<NodeT>>(infra);
	}
};

#endif // Q_BASED_INFRASTRUCTURE_H
