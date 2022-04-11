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


struct ThreadCommBasicData : public nodecpp::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo>
{
	size_t threadIdx;
	ThreadCommBasicData( size_t threadIdx_, ::globalmq::marshalling::GMQueue<GMQueueStatePublisherSubscriberTypeInfo>& gmq, MsgQueue& queue, int recipientID ) : 
		nodecpp::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo>( gmq, queue, recipientID ), threadIdx( threadIdx_ ) {}
};

using ErrorCodeT = int;


// exported staff

uintptr_t prepareThisThreadForCommunication(); // initializes comm means and returns "handle" to transport

ErrorCodeT releaseThisThreadForCommunicationData( uintptr_t handle ); // complemetary to prepareThisThreadForCommunication() on thread termination

ErrorCodeT getNextMessageSize( uintptr_t handle, size_t* requiredBufferSize );

ErrorCodeT getNextMessage( uintptr_t handle, void* buff, size_t buffsz, size_t* bytesCopied ); // returns number of bytes copied to the buff (insufficient buffer is an error)

ErrorCodeT postMessage( uintptr_t handle, uint8_t* buff, size_t sz );


#endif // ALT_COMM_IFACE_H