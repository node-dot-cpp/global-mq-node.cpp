// NetSocket.h : sample of user-defined code for an http server

#ifndef SUBSCRIBER_NODE_H
#define SUBSCRIBER_NODE_H

#include <nodecpp/common.h>
#include <log.h>
#include <nodecpp/fs.h>
#include "generated/publishable_state.h"
#include <nls.h>

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
		basic_test::structures::HtmlTag html;
		void notifyUpdated_id() const { /*assert( getCurrentNode<SubscriberNode>() != nullptr );*/ log::default_log::log( log::LogLevel::fatal, "id   = {}\n", id ); }
		void notifyUpdated_text() const { log::default_log::log( log::LogLevel::fatal, "text = {}\n", text ); }
		void notifyUpdated_html() const { log::default_log::log( log::LogLevel::fatal, "html = {}\n", htmltag_to_htmlstring(html) ); }
	};
	basic_test::publishable_sample_NodecppWrapperForSubscriber<SubscriptionState, PoolType> subscribedStateWrapper;

	log::Log log;

	using SrvReply = basic_test::structures::scope_test_exchange::MESSAGE_srv_response;

	class Connection: public globalmq::marshalling::ClientConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		using ReadIteratorT = typename GMQueueStatePublisherSubscriberTypeInfo::BufferT::ReadIteratorT;
		SubscriberNode* node = nullptr;
	public:
		Connection( SubscriberNode* node_ ) : node( node_ ) {}
		void onConnectionAccepted() override {
			log::default_log::log( log::LogLevel::fatal, "Connection confirmed accepted\n" );
			node->sendRequest();
		}
		void onMessage( ReadIteratorT& riter ) override {
			basic_test::scope_test_exchange::handleMessage2( riter, 
				basic_test::makeMessageHandler<basic_test::scope_test_exchange::srv_response>([&](auto& parser){ 
					auto reply = basic_test::scope_test_exchange::MESSAGE_srv_response_parse( parser );
					assert( node != nullptr );
					node->onServerReply( getConnID(), reply );
				}),
				basic_test::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
			);
		}
	};
	Connection connection;

	void onServerReply( uint64_t connID, const SrvReply& reply )
	{
		log::default_log::log( log::LogLevel::fatal, "Server reply received (connID = {}):\n", connID );
		log::default_log::log( log::LogLevel::fatal, "     replied_on      : {}\n", reply.replied_on );
		log::default_log::log( log::LogLevel::fatal, "     value           : {}\n", reply.value );
		log::default_log::log( log::LogLevel::fatal, "     text_from_server: {}\n", reply.text_from_server );
		sendRequest();
	}

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
	SubscriberNode() : subscribedStateWrapper( mqPool ), connection( this ) {}

	handler_ret_type main()
	{
		log.level = log::LogLevel::info;
		log.add( stdout );
		logging_impl::currentLog = &log;

		mqPool.setTransport( getTransport() );

		globalmq::marshalling::GmqPathHelper::PathComponents pc;

		pc.type = globalmq::marshalling::PublishableStateMessageHeader::MsgType::subscriptionRequest;
		pc.authority = "";
		pc.nodeName = "PublisherNode";
		pc.statePublisherOrConnectionType = basic_test::publishable_sample_NodecppWrapperForSubscriber<SubscriptionState, PoolType>::stringTypeID;
		GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose( pc );
		subscribedStateWrapper.subscribe( path );

		mqPool.add( &connection );
		pc.type = globalmq::marshalling::PublishableStateMessageHeader::MsgType::connectionRequest;
		pc.statePublisherOrConnectionType = "local";
		path = globalmq::marshalling::GmqPathHelper::compose( pc );
		connection.connect( path );

		CO_RETURN;
	}

	void onGlobalMQMessage( platform::internal_msg::InternalMsg& msg )
	{
		mqPool.onMessage( msg );
		// GlobalMQ: at the end of each handler cause pools to post all updates
		mqPool.postAllUpdates();
	}

	void dbgInvariantChecker()
	{
//		log::default_log::log( log::LogLevel::fatal, "    OUT OF HANDLERS\n" );
	}
};

#endif // SUBSCRIBER_NODE_H
