// NetSocket.h : sample of user-defined code for an http server

#ifndef PUBLISHER_NODE_H
#define PUBLISHER_NODE_H

#include <nodecpp/common.h>
#include <log.h>
#include <nodecpp/fs.h>
#include "generated/publishable_state.h"
#include "html_text_tag_converter.h"

constexpr const char* PublisherNodeName = "PublisherNode";

constexpr const char* dummyHtml = "<html><head><title>Untitled</title></head><body><p>body text</p></body></html>";

using namespace nodecpp;

class PublisherNode : public NodeBase
{
	using PoolType = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
	PoolType mqPool;
	struct PublishedState
	{
		std::string text;
		size_t id = 0;
		basic_test::structures::HtmlTag html;
	};
	basic_test::publishable_sample_NodecppWrapperForPublisher<PublishedState, PoolType> publishedStateWrapper;


	log::Log log;

	using ClientRequest = basic_test::structures::scope_test_exchange::MESSAGE_cl_request;
	using SrvReply = basic_test::structures::scope_test_exchange::MESSAGE_srv_response;

	class ConnectionInSCScope: public globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>
	{
		using ReadIteratorT = typename GMQueueStatePublisherSubscriberTypeInfo::BufferT::ReadIteratorT;
		PublisherNode* node = nullptr;
	public:
		int replyCtr = 1000;
		ConnectionInSCScope( PublisherNode* node_ ) : node( node_ ) {}
		void onMessage( ReadIteratorT& riter ) override {
			basic_test::scope_test_exchange::handleMessage2( riter, 
				basic_test::makeMessageHandler<basic_test::scope_test_exchange::cl_request>([&](auto& parser){ 
					auto request = basic_test::scope_test_exchange::MESSAGE_cl_request_parse( parser );
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
		GMQueueStatePublisherSubscriberTypeInfo::OwningPtrT<globalmq::marshalling::ServerConnectionBase<GMQueueStatePublisherSubscriberTypeInfo>> create() override
		{
			auto conn = GMQueueStatePublisherSubscriberTypeInfo::make_owning<ConnectionInSCScope>( node );
			log::default_log::log( log::LogLevel::fatal, "New connection accepted\n" );
			// TODO: upon necessity we can notify node about a new connection, etc
			return conn;
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

	int ctr = 0;

	void republish() // sample code with timeouts
	{
		publishedStateWrapper.set_id( ++ctr );
		publishedStateWrapper.set_text( fmt::format( " ---> {}. Hello Node.Cpp!", ctr ) );
		if ( ctr == 1 )
			publishedStateWrapper.set_html( htmlstring_to_htmltag(dummyHtml) );
		else
		{
			const auto tag = publishedStateWrapper.get_html();
			if ( tag.tags.currentVariant() == basic_test::structures::HtmlTextOrTags::text )
			{
				publishedStateWrapper.get4set_html().get4set_tags().set_str( fmt::format( "now my number is {}", ctr + 10000 ) );
			}
			else
			{
				publishedStateWrapper.get4set_html().set_name( fmt::format( "html_{}", ctr + 2000 ) );
			}
		}

		// GlobalMQ: at the end of each handler cause pools to post all updates
		mqPool.postAllUpdates();

		setTimeout( [this]() { 
			republish();
		}, 10 );
	}


public:
	PublisherNode() : publishedStateWrapper( mqPool ), connFactory( this ) {}

	handler_ret_type main()
	{
		log.level = log::LogLevel::info;
		log.add( stdout );
		logging_impl::currentLog = &log;

		mqPool.setTransport( getTransport() );
		mqPool.addSimpleConnectionFactory( &connFactory, "local" );

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
