/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*	 * Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	 * Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	 * Neither the name of the OLogN Technologies AG nor the
*	   names of its contributors may be used to endorse or promote products
*	   derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL OLogN Technologies AG BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

using System;
using System.Diagnostics;
using System.Text;
using System.Threading;
using globalmq.marshalling;


class SubscriberNodeFactory : INodeFactory
{
	public virtual NodeBase create() { return new SubscriberNode(); }
}
class SubscriberNode : NodeBase
{

	public const String SubscriberNodeName = "SubscriberNode";

	public override string getNodeName() { return SubscriberNodeName; }

	class SubscriptionState : basic_test.publishable_sample_subscriber {
		public override void notifyUpdated_id(Int64 old) { /*assert( getCurrentNode<SubscriberNode>() != nullptr );*/ Console.Write( "id   = {0}\n", id ); }
		public override void notifyUpdated_text(string old) { Console.Write("text = {0}\n", text ); }
		public override void notifyUpdated_html() { Console.Write( "html = {0}\n", html_text_tag_converter.htmltag_to_htmlstring(html) ); }
	}


	private SubscriptionState subscribedStateWrapper;


	class Connection: globalmq.marshalling.ClientConnectionBase
	{
		SubscriberNode node = null;

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


	public SubscriberNode()
	{
		this.subscribedStateWrapper = new SubscriptionState();
		this.connection = new Connection(this);
	}

    public override void init()
    {
		globalmq.marshalling.GmqPathHelper.PathComponents pc = new globalmq.marshalling.GmqPathHelper.PathComponents();
		pc.type = globalmq.marshalling.PublishableStateMessageHeader.MsgType.subscriptionRequest;
		pc.authority = "";
		pc.nodeName = PublisherNode.PublisherNodeName;
		pc.statePublisherOrConnectionType = "publishable_sample";
		string path = globalmq.marshalling.GmqPathHelper.compose(pc);

		Pool.add(subscribedStateWrapper);
		Pool.subscribe(subscribedStateWrapper, path);

		globalmq.marshalling.GmqPathHelper.PathComponents pc2 = new globalmq.marshalling.GmqPathHelper.PathComponents();
		pc2.type = globalmq.marshalling.PublishableStateMessageHeader.MsgType.connectionRequest;
		pc2.statePublisherOrConnectionType = "local";
		pc2.nodeName = PublisherNode.PublisherNodeName;
		string path2 = globalmq.marshalling.GmqPathHelper.compose(pc2);
		Pool.add(connection);
		connection.connect(path2);
	}

	public override void dbgInvariantChecker()
	{
		Console.WriteLine("dbgInvariantChecker");
	}
};
