// User-defined sample code
///////////////////////////

#include <nodecpp/common.h>
#include <infrastructure/q_based_infrastructure.h>
#include <chrono>
#include <thread>
#include "PublisherNode.h"
#include "SubscriberNode.h"
#include <infrastructure/node_thread_creation.h>
#include <chrono>

void test_1()
{
	// Init GMGueue
	using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
	using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	gmqueue.template initStateConcentratorFactory<basic_test::StateConcentratorFactory<BufferT, ComposerT>>();
	gmqueue.setAuthority( "" ); // current process only

	runNodeInAnotherThread<PublisherNode>( PublisherNodeName );
		
	runNodeInThisThread<SubscriberNode>(); // returns on exit from node loop
}

void test_2()
{
	// Init GMGueue
	using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
	using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	gmqueue.template initStateConcentratorFactory<basic_test::StateConcentratorFactory<BufferT, ComposerT>>();
	gmqueue.setAuthority( "" ); // current process only

	for (;;)
	{
		runNodeInAnotherThread<PublisherNode>( PublisherNodeName );		
		runNodeInAnotherThread<SubscriberNode>(); // returns on exit from node loop
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		fmt::print( "   --->> Stopping all threads...\n" );
		setAllThreadsTerminating();
		while (!haveAllThreadsTerminated())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			fmt::print( "   --->> --->> stopping in progress...\n" );
		}
		fmt::print( "   --->> --->> done\n" );
		std::this_thread::sleep_for(std::chrono::milliseconds(20000));
	}
}

int main( int argc, char *argv_[] )
{
	/*auto tag = htmlstring_to_htmltag(dummyHtml);
	auto str = htmltag_to_htmlstring(tag);
	fmt::print( "{}\n", str );
	return 0;*/

	FILE* outf = fopen("log.txt", "wb" );
	::nodecpp::log::Log log;
	log.add(outf);
	nodecpp::logging_impl::currentLog = &log;

	for ( int i=0; i<argc; ++i )
		argv.push_back( argv_[i] );

	test_2();

	return 0;
}
