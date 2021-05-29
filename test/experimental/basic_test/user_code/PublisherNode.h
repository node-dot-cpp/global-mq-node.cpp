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

	struct ClientRequest
	{
		int ordinal;
		std::string text;
	};

	struct SrvReply
	{
		int replied_on;
		int value;
		std::string text_from_server;
	};

	class ConnectionInSCScope: public globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		using ReadIteratorT = typename GMQueueStatePublisherSubscriberTypeInfo::BufferT::ReadIteratorT;
		PublisherNode* node = nullptr;
	public:
		int replyCtr = 1000;
		ConnectionInSCScope( PublisherNode* node_ ) : node( node_ ) {}
		virtual void onMessage( ReadIteratorT& riter ) {
			ClientRequest request;
			basic_test::scope_test_exchange::handleMessage2( riter, 
				basic_test::makeMessageHandler<basic_test::scope_test_exchange::cl_request>([&](auto& parser){ 
					basic_test::scope_test_exchange::MESSAGE_cl_request_parse( parser, basic_test::ordinal = &(request.ordinal), basic_test::text_to_server = &(request.text) );
					assert( node != nullptr );
					node->onClientRequest( connID, request, this );
				}),
				basic_test::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
			);
		}
	};

	class ConnFactory : public globalmq::marshalling::ConnectionFactoryBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		PublisherNode* node = nullptr;
	public:
		ConnFactory( PublisherNode* node_ ) : node( node_ ) {}
		virtual globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>* create()
		{
			return new ConnectionInSCScope( node );
		}
	};
	ConnFactory connFactory;

	void onClientRequest( uint64_t connID, const ClientRequest& rq, ConnectionInSCScope* connection )
	{
		log::default_log::log( log::LogLevel::fatal, "Client request (connID = {}):\n", connID );
		log::default_log::log( log::LogLevel::fatal, "     ordinal                : {}\n", rq.ordinal );
//		log::default_log::log( log::LogLevel::fatal, "     text                   : {}\n", rq.text );

		SrvReply reply;
		reply.replied_on = rq.ordinal;
		reply.value = ++(connection->replyCtr);
		reply.text_from_server = fmt::format( "srv reply {} -> {}", rq.ordinal, reply.value );
		typename GMQueueStatePublisherSubscriberTypeInfo::BufferT buff;
		basic_test::scope_test_exchange::composeMessage<basic_test::scope_test_exchange::srv_response>(buff, basic_test::replied_on = reply.replied_on, basic_test::value = reply.value, basic_test::text_from_server = reply.text_from_server );
		connection->postMessage( std::move( buff ) );

	}

	class ConnNotifier : public globalmq::marshalling::ServerNotifierBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;
		PublisherNode* node = nullptr;
	public:
		ConnNotifier( PublisherNode* node_ ) : node( node_ ) {}
		virtual void onNewConnection( uint64_t connID ) {
			log::default_log::log( log::LogLevel::fatal, "New connection {} accepted\n", connID );
		};
		virtual void onMessage( globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>* connection, ReadIteratorT& riter ) {
			connection->onMessage( riter );
		};
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
		mqPool.addSimpleConnectionFactory( &connFactory, "local" );
		mqPool.setNotifier( &connNotifier );

		republish();

		CO_RETURN;
	}

	void onGlobalMQMessage( platform::internal_msg::InternalMsg& msg )
	{
		mqPool.onMessage( msg );
		// GlobalMQ: at the end of each handler cause pools to post all updates
		mqPool.postAllUpdates();
	}
};

#endif // PUBLISHER_NODE_H
