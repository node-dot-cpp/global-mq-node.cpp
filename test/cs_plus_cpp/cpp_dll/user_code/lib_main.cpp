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

EXPORT_API ErrorCodeT initEngine()
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

EXPORT_API ErrorCodeT deinitEngine()
{
    try {
		gmqueue.~GMQueue();
		new (&gmqueue) decltype(gmqueue)();
        return 0;
    }
    catch (...) { return 1; /*unspecified error*/ }
}

#if defined(NODECPP_MSVC)
#include <Windows.h>
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            deinitEngine();
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#elif ((defined NODECPP_CLANG) || (defined NODECPP_GCC)) && (defined NODECPP_MAC) 
__attribute__((destructor))

static void finalizer(void) {
    deinitEngine();
}
#else
#error Unsupported configuration
#endif
