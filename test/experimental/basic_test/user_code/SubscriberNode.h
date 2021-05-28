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

	struct SrvReply
	{
		int replied_on;
		int value;
		std::string text_from_server;
	};

	void onServerReply( uint64_t connID, const SrvReply& reply )
	{
		log::default_log::log( log::LogLevel::fatal, "Server reply received (connID = {}):\n", connID );
		log::default_log::log( log::LogLevel::fatal, "     replied_on      : {}\n", reply.replied_on );
		log::default_log::log( log::LogLevel::fatal, "     value           : {}\n", reply.value );
		log::default_log::log( log::LogLevel::fatal, "     text_from_server: {}\n", reply.text_from_server );
		sendRequest();
	}

	class ConnNotifier : public globalmq::marshalling::ClientNotifierBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		SubscriberNode* node = nullptr;
	public:
		ConnNotifier( SubscriberNode* node_ ) : node( node_ ) {}
		virtual void onConnectionAccepted( uint64_t connID ) {
			log::default_log::log( log::LogLevel::fatal, "Connection {} confirmed accepted\n", connID );
			/*platform::internal_msg::InternalMsg msg;
			msg.append( "\"Happy to be accepted\"", 22 );
			node->connection.postMessage( std::move( msg ) );*/
			node->sendRequest();
		}
		virtual void onMessage( uint64_t connID, ReadIteratorT& iter ) {
			/*std::string s;
			parser.readStringFromJson( &s );
			log::default_log::log( log::LogLevel::fatal, "Connection {}: message received:\n", connID );
			log::default_log::log( log::LogLevel::fatal, "     {}\n", s );*/
			SrvReply reply;
			basic_test::scope_test_exchange::handleMessage2( iter, 
				basic_test::makeMessageHandler<basic_test::scope_test_exchange::srv_response>([&](auto& parser){ 
					basic_test::scope_test_exchange::MESSAGE_srv_response_parse( parser, basic_test::replied_on = &(reply.replied_on), basic_test::value = &(reply.value), basic_test::text_from_server = &(reply.text_from_server) );
					assert( node != nullptr );
					node->onServerReply( connID, reply );
				}),
				basic_test::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
			);
//			node->onServerReply( connID, reply );
		}
	};
	ConnNotifier connNotifier;

	// data for message exchange
	uint64_t ordinal = 0;
	void sendRequest()
	{
		++ordinal;
		auto txt = fmt::format( "from client # {}", ordinal );
		platform::internal_msg::InternalMsg msg;
		basic_test::scope_test_exchange::composeMessage<basic_test::scope_test_exchange::cl_request>( msg, basic_test::ordinal = ordinal, basic_test::text_to_server = txt );
		connection.postMessage( std::move( msg ) );
	}

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
