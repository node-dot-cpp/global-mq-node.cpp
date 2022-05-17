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

static thread_local ThreadCommBasicData* threadLocalTransport = nullptr;

EXPORT_API ErrorCodeT getThisThreadCommMeans( void** h )
// NOTE: just initializes comm means and returns "handle" to transport
// TODO: ensure the call is made once per thread
{
    *h = nullptr;
	try {
		if ( threadLocalTransport == nullptr ) {
		    BasicThreadInfo data;
		    acquireBasicThreadInfoForNewThread( data );
		    size_t threadIdx = data.slotId;
		    setThisThreadBasicInfo(data);
		    threadLocalTransport = new ThreadCommBasicData( threadIdx, gmqueue, getThreadQueue(threadIdx), 0 ); // NOTE: recipientID = 0 is by default; TODO: revise
		}
		*h = threadLocalTransport;
		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}

EXPORT_API ErrorCodeT releaseThisThreadForCommMeans( void* handle ) 
// NOTE: complemetary to prepareThisThreadForCommunication() on thread termination
// TODO: ensure calling on thread termination
{
	try {
		if(handle == 0)
			return 0; // release on null is no-op

		ThreadCommBasicData* transport = static_cast<ThreadCommBasicData*>( handle );
		if(transport != threadLocalTransport)
			return 3;

		delete threadLocalTransport;
		threadLocalTransport = nullptr;

		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}

EXPORT_API ErrorCodeT getNextMessageSize( void* handle, int32_t* requiredBufferSize )
{
	*requiredBufferSize = 0;
	try {
		if(handle == 0)
			return 2;

		ThreadCommBasicData* transport = static_cast<ThreadCommBasicData*>( handle );
		if(transport != threadLocalTransport)
			return 3;

		auto ptr = getThreadQueue(transport->threadIdx).test_front();
		if(ptr)
			*requiredBufferSize = ptr->msg.size();

		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}

EXPORT_API ErrorCodeT getNextMessage( void* handle, uint8_t* buff, int32_t buffsz, int32_t* bytesCopied )
{
	*bytesCopied = 0;
	try {
		if(handle == 0)
			return 2;

		ThreadCommBasicData* transport = static_cast<ThreadCommBasicData*>( handle );

		if(transport != threadLocalTransport)
			return 3;

		auto msg = getThreadQueue(transport->threadIdx).pop_front();

		if(!msg.first)
			return 1; // shouldn't happend

		auto iter = msg.second.msg.getReadIter();
		*bytesCopied = static_cast<int32_t>(iter.read(buff, buffsz));

		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}

EXPORT_API ErrorCodeT postMessage( void* handle, uint8_t* buff, int32_t buffsz )
{
	try {
		if(handle == 0)
			return 2;

		ThreadCommBasicData* transport = static_cast<ThreadCommBasicData*>( handle );

		if(transport != threadLocalTransport)
			return 3;

		GMQueueStatePublisherSubscriberTypeInfo::BufferT msg;
		msg.append( buff, buffsz );
		transport->postMessage( std::move( msg ) );

		return 0;
	}
	catch (...) { return 1; /*unspecified error*/ }
}
