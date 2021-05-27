// NetSocket.h : sample of user-defined code for an http server

#ifndef SUBSCRIBER_NODE_H
#define SUBSCRIBER_NODE_H

#include <nodecpp/common.h>
#include <log.h>
#include <nodecpp/fs.h>
#include "generated/publishable_state.h"

constexpr const char* SubscriberNodeName = "SubscriberNode";

using namespace nodecpp;

class SubscriberNode : public NodeBase
{
	using PoolType = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
	PoolType mqPool;
	struct SubscriptionState
	{
		std::string text;
		size_t id = 0;
		void notifyUpdated_id() const { 
			log::default_log::log( log::LogLevel::fatal, "id = {}\n", id );
			if ( id == 6 )
			{
			}
		}
		void notifyUpdated_text() const { log::default_log::log( log::LogLevel::fatal, "text = {}\n", text ); }
	};
	basic_test::publishable_sample_NodecppWrapperForSubscriber<SubscriptionState, PoolType> subscribedStateWrapper;

	log::Log log;

	class Connection: public globalmq::marshalling::ClientConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		SubscriberNode* node = nullptr;
	public:
		Connection( SubscriberNode* node_ ) : node( node_ ) {}
	};
	Connection connection;

	class ConnNotifier : public globalmq::marshalling::ClientNotifierBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		SubscriberNode* node = nullptr;
	public:
		ConnNotifier( SubscriberNode* node_ ) : node( node_ ) {}
		virtual void onConnectionAccepted( uint64_t connID ) {
			log::default_log::log( log::LogLevel::fatal, "Connection {} confirmed accepted\n", connID );
			platform::internal_msg::InternalMsg msg;
			msg.append( "\"Happy to be accepted\"", 22 );
			node->connection.postMessage( std::move( msg ) );
		}
		virtual void onMessage( uint64_t connID, ParserT& parser ) {
			std::string s;
			parser.readStringFromJson( &s );
			log::default_log::log( log::LogLevel::fatal, "Connection {}: message received:\n", connID );
			log::default_log::log( log::LogLevel::fatal, "     {}\n", s );
		}
	};
	ConnNotifier connNotifier;

public:
	SubscriberNode() : subscribedStateWrapper( mqPool ), connection( this ), connNotifier( this ) {}

	handler_ret_type main()
	{
		log.level = log::LogLevel::info;
		log.add( stdout );
		logging_impl::currentLog = &log;

		mqPool.setTransport( getTransport() );
		mqPool.setNotifier( &connNotifier );

		globalmq::marshalling::GmqPathHelper::PathComponents pc;

		pc.authority = "";
		pc.nodeName = "PublisherNode";
		pc.statePublisherOrConnPeerName = basic_test::publishable_sample_NodecppWrapperForSubscriber<SubscriptionState, PoolType>::stringTypeID;
		GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose( globalmq::marshalling::GmqPathHelper::Type::subscriptionRequest, pc );
		subscribedStateWrapper.subscribe( path );

		mqPool.add( &connection );
		connection.connect( path );


		CO_RETURN;
	}

	void onGlobalMQMessage( platform::internal_msg::InternalMsg& msg )
	{
		mqPool.onMessage( msg );
		// GlobalMQ: at the end of each handler cause pools to post all updates
		mqPool.postAllUpdates();
	}
};

#endif // SUBSCRIBER_NODE_H
