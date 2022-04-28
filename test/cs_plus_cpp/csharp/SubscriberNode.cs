// NetSocket.h : sample of user-defined code for an http server

//#ifndef SUBSCRIBER_NODE_H
//#define SUBSCRIBER_NODE_H

//#include <nodecpp/common.h>
//#include <log.h>
//#include <nodecpp/fs.h>
//#include "generated/publishable_state.h"
//#include <nls.h>


//using namespace nodecpp;

using System;
using System.Diagnostics;
using System.Text;
using System.Threading;
using globalmq.marshalling;

class SubscriberNode : NodeBase
{

	public const String SubscriberNodeName = "SubscriberNode";
	public const String PublisherNodeName = "PublisherNode";

	public override String getNodeName() { return SubscriberNodeName; }

	class SubscriptionState : basic_test.publishable_sample_subscriber {
		public override void notifyUpdated_id(Int64 old) { /*assert( getCurrentNode<SubscriberNode>() != nullptr );*/ Console.Write( "id   = {0}\n", id ); }
		public override void notifyUpdated_text(string old) { Console.Write("text = {0}\n", text ); }
		public override void notifyUpdated_html() { Console.Write( "html = {0}\n", html_text_tag_converter.htmltag_to_htmlstring(html) ); }
	}


	private SubscriptionState subscribedStateWrapper;

	//log::Log log;

	//using SrvReply = basic_test::structures::scope_test_exchange::MESSAGE_srv_response;

	class Connection: globalmq.marshalling.ClientConnectionBase
	{
		//using ReadIteratorT = typename GMQueueStatePublisherSubscriberTypeInfo::BufferT::ReadIteratorT;
		SubscriberNode node = null;
	//public:
		public Connection( SubscriberNode node_ ) { this.node = node_; }
		public override void onConnectionAccepted(){
			Console.Write( "Connection confirmed accepted\n" );
			node.sendRequest();
		}
		public override void onMessage( ReadIteratorT riter ) {
			basic_test.scope_test_exchange.handleMessage( riter, 
				basic_test.scope_test_exchange.makeMessageHandler(basic_test.scope_test_exchange.MsgId.srv_response,
					(JsonParser parser, ulong id) => {
						basic_test.scope_test_exchange.srv_response reply = basic_test.scope_test_exchange.parseMessage_srv_response( parser );
					//assert( node != nullptr );
						node.onServerReply( getConnID(), reply );
				}),
				basic_test.scope_test_exchange.makeDefaultMessageHandler((JsonParser parser, ulong id) => { Console.Write( "Unhandled message\n", id ); })
			);
		}
	};
	Connection connection;

	void onServerReply( UInt64 connID, basic_test.scope_test_exchange.srv_response reply )
	{
		Console.Write( "Server reply received (connID = {0}):\n", connID );
		Console.Write( "     replied_on      : {0}\n", reply.replied_on );
		Console.Write( "     value           : {0}\n", reply.value );
		Console.Write( "     text_from_server: {0}\n", reply.text_from_server );
		sendRequest();
	}

	// data for message exchange
	long ordinal = 0;
	void sendRequest()
	{
		++ordinal;
		string txt = string.Format( "from client # {0}", ordinal );
		BufferT msg = Platform.makeBuffer();
		basic_test.scope_test_exchange.composeMessage_cl_request( msg, ordinal : ordinal, text_to_server : txt );
		connection.postMessage( msg );
	}

	DllTransport dllTr = null;
	public SubscriberNode()
	{
		this.subscribedStateWrapper = new SubscriptionState();
		this.connection = new Connection(this);
	}

	public void initDllTransport(DllTransport tr)
	{
		Debug.Assert(dllTr == null);
		this.dllTr = tr;
		initTransport(tr);
	}

	//this.platformSupport = platform;
	//this.transport = GmqTransport.getThreadTransport();
	//this.mqPool = new MetaPool();

	public override void init()
	{ 
		//mqPool.setPlatform(platform);
		//mqPool.setTransport(transport);


		globalmq.marshalling.GmqPathHelper.PathComponents pc = new globalmq.marshalling.GmqPathHelper.PathComponents();
		pc.type = globalmq.marshalling.PublishableStateMessageHeader.MsgType.subscriptionRequest;
		pc.authority = "";
		pc.nodeName = PublisherNodeName;
		pc.statePublisherOrConnectionType = "publishable_sample";
		string path = globalmq.marshalling.GmqPathHelper.compose(pc);

		Pool.add(subscribedStateWrapper);
		Pool.subscribe(subscribedStateWrapper, path);

		globalmq.marshalling.GmqPathHelper.PathComponents pc2 = new globalmq.marshalling.GmqPathHelper.PathComponents();
		pc2.type = globalmq.marshalling.PublishableStateMessageHeader.MsgType.connectionRequest;
		pc2.statePublisherOrConnectionType = "local";
		pc2.nodeName = PublisherNodeName;
		string path2 = globalmq.marshalling.GmqPathHelper.compose(pc2);
		Pool.add(connection);
		connection.connect(path2);
	}

	//public void onGlobalMQMessage( BufferT msg )
	//{
	//	mqPool.onMessage( msg );
	//	// GlobalMQ: at the end of each handler cause pools to post all updates
	//	mqPool.postAllUpdates();
	//}

	public void checkIncomingMessages()
	{
		int size = 0;
		int bytesCopied = 0;

		dllTr.getNextMessageSize(out size);

		while (size != 0)
		{
			byte[] arr = new byte[size];
			dllTr.getNextMessage(arr, out bytesCopied);

			Debug.Assert(bytesCopied == size);

			BufferT buff = Platform.makeBuffer();
			buff.append(arr);

			onGlobalMQMessage(buff);
			dllTr.getNextMessageSize(out size);
		}

	}


	public override void dbgInvariantChecker()
	{
		Console.Write( "    OUT OF HANDLERS\n" );
	}
};
