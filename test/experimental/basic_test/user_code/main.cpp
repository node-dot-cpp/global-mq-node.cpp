// User-defined sample code
///////////////////////////

#include <nodecpp/common.h>
#include <infrastructure/q_based_infrastructure.h>
#include <chrono>
#include <thread>
#include "PublisherNode.h"
#include "SubscriberNode.h"
#include <infrastructure/node_thread_creation.h>


int main( int argc, char *argv_[] )
{
	for ( int i=0; i<argc; ++i )
		argv.push_back( argv_[i] );

	// Init GMGueue
	using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
	using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	gmqueue.template initStateConcentratorFactory<basic_test::StateConcentratorFactory<BufferT, ComposerT>>();
	gmqueue.setAuthority( "" ); // current process only

	runNodeInAnotherThread<PublisherNode>( PublisherNodeName );
		
	runNodeInThisThread<SubscriberNode>(); // returns on exit from node loop

	return 0;
}
