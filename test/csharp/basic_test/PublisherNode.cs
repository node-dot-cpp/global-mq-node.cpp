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

class PublisherNodeFactory : INodeFactory
{
    public virtual NodeBase create() { return new PublisherNode(); }
}

class PublisherNode : NodeBase
{

    public const String PublisherNodeName = "PublisherNode";
    public const String dummyHtml = "<html><head><title>Untitled</title></head><body><p>body text</p></body></html>";

    public override String getNodeName() { return PublisherNodeName; }

    //using PoolType = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
    //PoolType mqPool;
    //struct PublishedState
    //{
    //	public string text;
    //	size_t id = 0;
    //	basic_test::structures::HtmlTag html;
    //};

    basic_test.publishable_sample_publisher publishedStateWrapper;


    //log::Log log;

    //using ClientRequest = basic_test::structures::scope_test_exchange::MESSAGE_cl_request;
    //using SrvReply = basic_test::structures::scope_test_exchange::MESSAGE_srv_response;

    public class ConnectionInSCScope : globalmq.marshalling.ServerConnectionBase
    {
        //using ReadIteratorT = typename GMQueueStatePublisherSubscriberTypeInfo::BufferT::ReadIteratorT;
        PublisherNode node;
        //public:
        public int replyCtr = 1000;
        public ConnectionInSCScope(PublisherNode node_) { this.node = node_; }
        public override void onMessage(ReadIteratorT riter)
        {
            basic_test.scope_test_exchange.handleMessage(riter,
                basic_test.scope_test_exchange.makeMessageHandler(basic_test.scope_test_exchange.MsgId.cl_request,
                (JsonParser parser, ulong msgID) =>
                {
                    var request = basic_test.scope_test_exchange.parseMessage_cl_request(parser);
                    node.onClientRequest(getConnID(), request, this);
                }),
                        basic_test.scope_test_exchange.makeDefaultMessageHandler(
                            (JsonParser parser, ulong msgID) => { Console.Write("Unhandled message {0}\n", msgID); })
                    );
        }
    };

    class ConnFactory : globalmq.marshalling.IConnectionFactory
    {
        PublisherNode node;
        //public:
        public ConnFactory(PublisherNode node_) { this.node = node_; }
        public virtual ServerConnectionBase create()
        {
            //auto conn = GMQueueStatePublisherSubscriberTypeInfo::make_owning<ConnectionInSCScope>(node);
            Console.Write("New connection accepted\n");
            // TODO: upon necessity we can notify node about a new connection, etc
            return new ConnectionInSCScope(node);
        }
    };
    ConnFactory connFactory;

    public void onClientRequest(UInt64 connID, basic_test.scope_test_exchange.Icl_request rq, ConnectionInSCScope connection)
    {
        Console.Write("Client request (connID = {0}):\n", connID);
        Console.Write("     ordinal                : {0}\n", rq.ordinal);
        //		log::default_log::log( log::LogLevel::fatal, "     text                   : {}\n", rq.text );

        basic_test.scope_test_exchange.srv_response reply = new basic_test.scope_test_exchange.srv_response();
        reply.replied_on = rq.ordinal;
        reply.value = ++(connection.replyCtr);
        reply.text_from_server = String.Format("srv reply {0} -> {1}", rq.ordinal, reply.value);
        BufferT buff = Platform.makeBuffer();
        basic_test.scope_test_exchange.composeMessage_srv_response(
            buff, replied_on: reply.replied_on, value: reply.value,
            text_from_server: reply.text_from_server);
        connection.postMessage(buff);

    }

    int ctr = 0;

    void republish() // sample code with timeouts
    {
        publishedStateWrapper.id = ++ctr;
        publishedStateWrapper.text = String.Format(" ---> {0}. Hello Node.Cpp!", ctr);
        if (ctr == 1)
            publishedStateWrapper.html = html_text_tag_converter.htmlstring_to_htmltag(dummyHtml);
        else
        {
            basic_test.IHtmlTag tag = publishedStateWrapper.html;
            if (tag.tags.currentVariant() == basic_test.HtmlTextOrTags_variants.text)
            {
                publishedStateWrapper.html.tags.str = String.Format("now my number is {0}", ctr + 10000);
            }
            else
            {
                publishedStateWrapper.html.name = String.Format("html_{0}", ctr + 2000);
            }
        }

        // GlobalMQ: at the end of each handler cause pools to post all updates
        Pool.postAllUpdates();

        //setTimeout( [this]() {
        //	republish();
        //}, 10 );
    }


    //public:
    public PublisherNode()
    {
        this.publishedStateWrapper = new basic_test.publishable_sample_publisher();
        this.connFactory = new ConnFactory(this);
    }

    public override void init()
    {
        Pool.addSimpleConnectionFactory(connFactory, "local");
        Pool.add(publishedStateWrapper);

        republish();

    }
}

