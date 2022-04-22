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


#ifndef ALT_COMM_IFACE_H
#define ALT_COMM_IFACE_H

#include "../nodecpp/common.h"
#include "q_based_infrastructure.h"
#include "inproc_queue.h"

#if defined(NODECPP_MSVC)
// Visual Studio needs annotating exported functions with this
#define EXPORT_API extern "C" __declspec(dllexport)
#elif (defined NODECPP_CLANG) || (defined NODECPP_GCC)
#define EXPORT_API extern "C"
#else
#pragma message( "Unknown compiler. Annotation for exported functions is unknown" )
#endif

struct ThreadCommBasicData : public nodecpp::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo>
{
	size_t threadIdx;
	ThreadCommBasicData( size_t threadIdx_, ::globalmq::marshalling::GMQueue<GMQueueStatePublisherSubscriberTypeInfo>& gmq, MsgQueue& queue, int recipientID ) : 
		nodecpp::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo>( gmq, queue, recipientID ), threadIdx( threadIdx_ ) {}
};

using ErrorCodeT = int32_t;


// exported staff

EXPORT_API ErrorCodeT getThisThreadCommMeans( void** h ); // initializes, if necessary, comm means and returns "handle" to transport

EXPORT_API ErrorCodeT releaseThisThreadForCommMeans( void* handle ); // complemetary to prepareThisThreadForCommunication() on thread termination

EXPORT_API ErrorCodeT getNextMessageSize( void* handle, int32_t* requiredBufferSize );

EXPORT_API ErrorCodeT getNextMessage( void* handle, uint8_t* buff, int32_t buffsz, int32_t* bytesCopied ); // returns number of bytes copied to the buff (insufficient buffer is an error)

EXPORT_API ErrorCodeT postMessage( void* handle, uint8_t* buff, int32_t buffsz );

/* QUICK IMPLEMENTATION NOTES to build a module that can be used from C#, etc
* 
* 1. In addtion to the above exported calls a call to the init() that
*    - initializes GMQueue,
*    - runs nodes, etc, should also be implemented, and
*    - prepares callig thread for communication
*    In most cases such a function should return immediately (that is, not running infinite message loops or alike; in particular, for that all nodes shold be run via runNodeInAnotherThread<>())
*    Sample implementation:
*    ErrorCodeT init() {
*      try {
*        using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
*        using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
*        gmqueue.template initStateConcentratorFactory<basic_test::StateConcentratorFactory<BufferT, ComposerT>>();
*        gmqueue.setAuthority( "" ); // current process only
*        runNodeInAnotherThread<SomeNode>( SomeNodeName );
*        return 0; // ok
*      }
*      catch (...) { return 1; } // unspecified error
*    }
* 
* 2. On a calling (say, c#) side the sequence of calls (up to language specifics and error handling) may look like
*    {
*      // ...
*      init();
*      // uintptr_t hComm is defined somewhere
*      getThisThreadCommMeans(&hComm);
*      // ... then inside any suitable kind of message loop:
*          // size_t requiredBufferSize and bytesCopied are defined somewhere
*          getNextMessageSize( hComm, requiredBufferSize );
*          if ( requiredBufferSize != 0 ) {
*            // prepare buffer (whatever it means in the target language) and pass a pointer to it
*            getNextMessage( hComm, buff, buffsz, & bytesCopied );
*            // parse and process incoming message; compose outgiong message
*            postMessage( hComm, buff_out, sz_out );
*          }
*    }
*          
* 3. Regarding message processing see also c# port of Global-MQ library and code generation         
*          
*********************************************************************************************/


#endif // ALT_COMM_IFACE_H