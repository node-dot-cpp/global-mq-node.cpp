// User-defined sample code
///////////////////////////

#include <nodecpp/common.h>
#include <infrastructure/q_based_infrastructure.h>
#include <chrono>
#include <thread>
#include "PublisherNode.h"
#include "SubscriberNode.h"
#include <infrastructure/node_thread_creation.h>
#include <infrastructure/alt_comm_iface.h>

EXPORT_API ErrorCodeT initEngine ()
{
    try {
        auto isInit = gmqueue.isStateConcentratorFactoryInitialized();
        if ( !isInit.first )
        {
            // Init GMGueue
            using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
            using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
            gmqueue.template initStateConcentratorFactory<basic_test::StateConcentratorFactory<BufferT, ComposerT>>();
            gmqueue.setAuthority( "" ); // current process only
        }
        else
        {
            if ( isInit.second != "" )
                return 1; /*unspecified error*/
        }

        runNodeInAnotherThread<PublisherNode>( PublisherNodeName );
            return 0;
    }
    catch (...) { return 1; /*unspecified error*/ }
}
