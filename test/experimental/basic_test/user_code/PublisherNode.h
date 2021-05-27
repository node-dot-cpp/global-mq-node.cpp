// NetSocket.h : sample of user-defined code for an http server

#ifndef PUBLISHER_NODE_H
#define PUBLISHER_NODE_H

#include <nodecpp/common.h>
#include <log.h>
#include <nodecpp/fs.h>
#include "generated/publishable_state.h"

constexpr const char* PublisherNodeName = "PublisherNode";

using namespace nodecpp;

class PublisherNode : public NodeBase
{
	using PoolType = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
	PoolType mqPool;
	struct PublishedState
	{
		std::string text;
		size_t id = 0;
	};
	basic_test::publishable_sample_NodecppWrapperForPublisher<PublishedState, PoolType> publishedStateWrapper;

	log::Log log;

	class Connection: public globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		PublisherNode* node = nullptr;
	public:
		Connection( PublisherNode* node_ ) : node( node_ ) {}
	};

	class ConnFactory : public globalmq::marshalling::ConnectionFactoryBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		PublisherNode* node = nullptr;
	public:
		ConnFactory( PublisherNode* node_ ) : node( node_ ) {}
		virtual globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>* create()
		{
			assert( node != nullptr );
			return new Connection( node );
		}
	};
	ConnFactory connFactory;

	class ConnNotifier : public globalmq::marshalling::ServerNotifierBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;
		PublisherNode* node = nullptr;
	public:
		ConnNotifier( PublisherNode* node_ ) : node( node_ ) {}
		virtual void onNewConnection( uint64_t connID ) {
			log::default_log::log( log::LogLevel::fatal, "New connection {} accepted\n", connID );
		}
		virtual void onMessage( uint64_t connID, globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>* connection_, ParserT& parser ) {
			Connection* connection = dynamic_cast<Connection*>( connection_ );
			std::string s;
			parser.readStringFromJson( &s );
			log::default_log::log( log::LogLevel::fatal, "Connection {}: message received:\n", connID );
			log::default_log::log( log::LogLevel::fatal, "     {}\n", s );

			// reply once
			platform::internal_msg::InternalMsg msg;
			msg.append( "\"Happy to hear about your happiness\"", 36 );
			connection->postMessage( std::move( msg ) );
		}
	};
	ConnNotifier connNotifier;

	int ctr = 0;

	void republish() // sample code with timeouts
	{
		publishedStateWrapper.set_id( ++ctr );
		publishedStateWrapper.set_text( fmt::format( " ---> {}. Hello Node.Cpp!", ctr ) );

		// GlobalMQ: at the end of each handler cause pools to post all updates
		mqPool.postAllUpdates();

		setTimeout( [this]() { 
			republish();
		}, 500 );
	}


public:
	PublisherNode() : publishedStateWrapper( mqPool ), connFactory( this ), connNotifier( this ) {}

	handler_ret_type main()
	{
		log.level = log::LogLevel::info;
		log.add( stdout );
		logging_impl::currentLog = &log;

		mqPool.setTransport( getTransport() );
		mqPool.setConnectionFactory( &connFactory );
		mqPool.setNotifier( &connNotifier );

		// TODO: place some code here, for instance...

		republish();

		CO_RETURN;
	}

	void onGlobalMQMessage( platform::internal_msg::InternalMsg& msg )
	{
		mqPool.onMessage( msg );
		// GlobalMQ: at the end of each handler cause pools to post all updates
		mqPool.postAllUpdates();
	}

	void onInfrastructuralMessage( platform::internal_msg::InternalMsg& msg )
	{
		ctr = 0;
		// GlobalMQ: at the end of each handler cause pools to post all updates
		auto riter = msg.getReadIter();
		log::default_log::log( log::LogLevel::fatal, "   >>> {}\n", riter.directRead( riter.directlyAvailableSize() ) );
		mqPool.postAllUpdates();
	}
};

#endif // PUBLISHER_NODE_H
