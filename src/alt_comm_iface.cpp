/* -------------------------------------------------------------------------------
* Copyright (c) 2022, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the <organization> nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

#include "../include/infrastructure/alt_comm_iface.h"
#include "../include/infrastructure/q_based_infrastructure.h"
#include "../include/infrastructure/node_thread_creation.h"

// [quick and dirty] Staff to support interfaces with languages other than c/cpp (like c#)

uintptr_t prepareThisThreadForCommunication() 
// NOTE: just initializes comm means and returns "handle" to transport
// TODO: ensure the call is made once per thread
{
	BasicThreadInfo data;
	acquireBasicThreadInfoForNewThread( data );
	size_t threadIdx = data.slotId;
	setThisThreadBasicInfo(data);
	ThreadCommBasicData* transport = new ThreadCommBasicData( threadIdx, gmqueue, threadQueues[threadIdx].queue, 0 ); // NOTE: recipientID = 0 is by default; TODO: revise
	return reinterpret_cast<uintptr_t>( transport );
}

ErrorCodeT releaseThisThreadForCommunicationData( uintptr_t handle ) 
// NOTE: complemetary to prepareThisThreadForCommunication() on thread termination
// TODO: ensure calling on thread termination
{
	try {
		ThreadCommBasicData* transport = reinterpret_cast<ThreadCommBasicData*>( handle );
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, transport != nullptr ); 
		delete transport;
		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}

ErrorCodeT getNextMessageSize( uintptr_t handle, size_t* requiredBufferSize )
{
	try {
		ThreadCommBasicData* transport = reinterpret_cast<ThreadCommBasicData*>( handle );
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, transport != nullptr );
		*requiredBufferSize = threadQueues[transport->threadIdx].queue.test_front()->msg.size();
		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}

ErrorCodeT getNextMessage( uintptr_t handle, void* buff, size_t buffsz, size_t* bytesCopied )
{
	try {
		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}

ErrorCodeT postMessage( uintptr_t handle, uint8_t* buff, size_t sz )
{
	try {
		GMQueueStatePublisherSubscriberTypeInfo::BufferT msg;
		msg.append( buff, sz );
		ThreadCommBasicData* transport = reinterpret_cast<ThreadCommBasicData*>( handle );
		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, transport != nullptr ); 
		transport->postMessage( std::move( msg ) );
		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}
