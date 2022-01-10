#ifndef _publishable_state_h_91f76d69_guard
#define _publishable_state_h_91f76d69_guard

#include <marshalling.h>
#include <publishable_impl.h>
using namespace globalmq::marshalling;
namespace basic_test {

#ifdef METASCOPE_basic_test_ALREADY_DEFINED
#error metascope must reside in a single idl file
#endif
#define METASCOPE_basic_test_ALREADY_DEFINED

// Useful aliases:
//     (note: since clang apparently too often requires providing template arguments for aliased type ctors we use wrappers instead of type aliasing)
using Buffer = globalmq::marshalling::Buffer;
using FileReadBuffer = globalmq::marshalling::FileReadBuffer;
template<class BufferT>
class GmqComposer : public globalmq::marshalling::GmqComposer<BufferT> { public: GmqComposer( BufferT& buff_ ) : globalmq::marshalling::GmqComposer<BufferT>( buff_ ) {} };
template<class BufferT>
class GmqParser : public globalmq::marshalling::GmqParser<BufferT> { public: /*GmqParser( BufferT& buff_ ) : globalmq::marshalling::GmqParser<BufferT>( buff_ ) {}*/ GmqParser( typename BufferT::ReadIteratorT& iter ) : globalmq::marshalling::GmqParser<BufferT>( iter ) {} GmqParser( const GmqParser<BufferT>& other ) : globalmq::marshalling::GmqParser<BufferT>( other ) {} GmqParser& operator = ( const GmqParser<BufferT>& other ) { globalmq::marshalling::GmqParser<BufferT>::operator = ( other ); return *this; }};
template<class BufferT>
class JsonComposer : public globalmq::marshalling::JsonComposer<BufferT> { public: JsonComposer( BufferT& buff_ ) : globalmq::marshalling::JsonComposer<BufferT>( buff_ ) {} };
template<class BufferT>
class JsonParser : public globalmq::marshalling::JsonParser<BufferT> { public: /*JsonParser( BufferT& buff_ ) : globalmq::marshalling::JsonParser<BufferT>( buff_ ) {}*/ JsonParser( typename BufferT::ReadIteratorT& iter ) : globalmq::marshalling::JsonParser<BufferT>( iter ) {} JsonParser( const JsonParser<BufferT>& other ) : globalmq::marshalling::JsonParser<BufferT>( other ) {} JsonParser& operator = ( const JsonParser<BufferT>& other ) { globalmq::marshalling::JsonParser<BufferT>::operator = ( other ); return *this; } };
template<class T>
class SimpleTypeCollectionWrapper : public globalmq::marshalling::SimpleTypeCollectionWrapper<T> { public: SimpleTypeCollectionWrapper( T& coll ) : globalmq::marshalling::SimpleTypeCollectionWrapper<T>( coll ) {} };
template<class LambdaSize, class LambdaNext>
class CollectionWrapperForComposing : public globalmq::marshalling::CollectionWrapperForComposing<LambdaSize, LambdaNext> { public: CollectionWrapperForComposing(LambdaSize &&lsize, LambdaNext &&lnext) : globalmq::marshalling::CollectionWrapperForComposing<LambdaSize, LambdaNext>(std::forward<LambdaSize>(lsize), std::forward<LambdaNext>(lnext)) {} };
template<class LambdaCompose>
class MessageWrapperForComposing : public globalmq::marshalling::MessageWrapperForComposing<LambdaCompose> { public: MessageWrapperForComposing(LambdaCompose &&lcompose) : globalmq::marshalling::MessageWrapperForComposing<LambdaCompose>( std::forward<LambdaCompose>(lcompose) ) {} };
template<class LambdaSize, class LambdaNext>
class CollectionWrapperForParsing : public globalmq::marshalling::CollectionWrapperForParsing<LambdaSize, LambdaNext> { public: CollectionWrapperForParsing(LambdaSize &&lsizeHint, LambdaNext &&lnext) : globalmq::marshalling::CollectionWrapperForParsing<LambdaSize, LambdaNext>(std::forward<LambdaSize>(lsizeHint), std::forward<LambdaNext>(lnext)) {} };
template<typename msgID_, class LambdaHandler>
MessageHandler<msgID_, LambdaHandler> makeMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeMessageHandler<msgID_, LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }
template<class LambdaHandler>
DefaultMessageHandler<LambdaHandler> makeDefaultMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeDefaultMessageHandler<LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }

//////////////////////////////////////////////////////////////
//
//  Scopes:
//
//  scope_test_exchange
//  {
//    cl_request
//    srv_response
//  }
//
//////////////////////////////////////////////////////////////

using ordinal_Type = NamedParameter<struct ordinal_Struct>;
using replied_on_Type = NamedParameter<struct replied_on_Struct>;
using text_from_server_Type = NamedParameter<struct text_from_server_Struct>;
using text_to_server_Type = NamedParameter<struct text_to_server_Struct>;
using value_Type = NamedParameter<struct value_Struct>;

constexpr ordinal_Type::TypeConverter ordinal;
constexpr replied_on_Type::TypeConverter replied_on;
constexpr text_from_server_Type::TypeConverter text_from_server;
constexpr text_to_server_Type::TypeConverter text_to_server;
constexpr value_Type::TypeConverter value;


// member name presence checkers
template<typename T> concept has_id_member = requires { { T::id }; };
template<typename T> concept has_text_member = requires { { T::text }; };


// member update notifier presence checks
using index_type_for_array_notifiers = size_t&;
template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };
template<typename T> concept has_void_update_notifier_call_for_currentVariant = requires(T t) { { t.notifyUpdated_currentVariant() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_currentVariant = requires { { std::declval<StateT>().notifyUpdated_currentVariant(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_id = requires(T t) { { t.notifyUpdated_id() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_id = requires { { std::declval<StateT>().notifyUpdated_id(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_text = requires(T t) { { t.notifyUpdated_text() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_text = requires { { std::declval<StateT>().notifyUpdated_text(std::declval<MemberT>()) }; };

//===============================================================================
// C-structures for idl STRUCTs, DISCRIMINATED_UNIONs, MESSAGEs and PUBLISHABLEs

namespace structures {


namespace scope_test_exchange {
struct MESSAGE_cl_request
{
	int64_t ordinal;
	GMQ_COLL string text_to_server;
};
} // namespace scope_test_exchange

namespace scope_test_exchange {
struct MESSAGE_srv_response
{
	int64_t replied_on;
	int64_t value;
	GMQ_COLL string text_from_server;
};
} // namespace scope_test_exchange

struct publishable_sample
{
	int64_t id;
	GMQ_COLL string text;
};


} // namespace structures

//===============================================================================


namespace scope_test_exchange {

using cl_request = ::globalmq::marshalling::impl::MessageName<2>;
using srv_response = ::globalmq::marshalling::impl::MessageName<3>;

template<class ParserT, class ... HandlersT >
void implHandleMessage( ParserT& parser, HandlersT ... handlers )
{
	uint64_t msgID;

	static_assert( ParserT::proto == Proto::GMQ, "According to IDL GMQ parser is expected" );
	parser.parseUnsignedInteger( &msgID );
	bool ok = false;

	switch ( msgID )
	{
		case cl_request::id: ok = ::globalmq::marshalling::impl::implHandleMessage<cl_request>( parser, handlers... ); break;
		case srv_response::id: ok = ::globalmq::marshalling::impl::implHandleMessage<srv_response>( parser, handlers... ); break;
		default: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;
	}

}

template<class BufferT, class ... HandlersT >
void handleMessage( BufferT& buffer, HandlersT ... handlers )
{
	auto riter = buffer.getReadIter();
	GmqParser<BufferT> parser( riter );
	implHandleMessage( parser, handlers... );
}

template<class ReadIteratorT, class ... HandlersT >
void handleMessage2( ReadIteratorT& riter, HandlersT ... handlers )
{
	GmqParser<typename ReadIteratorT::BufferT> parser( riter );
	implHandleMessage( parser, handlers... );
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args );

//**********************************************************************
// MESSAGE "cl_request" Targets: GMQ (2 parameters)
//  1. INTEGER ordinal (REQUIRED)
//  2. CHARACTER_STRING text_to_server (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_cl_request_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, ordinal_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::StringType, text_to_server_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this MESSAGE assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)0>(composer, arg_2_type::nameAndTypeID, args...);
}

template<class ParserT>
structures::scope_test_exchange::MESSAGE_cl_request MESSAGE_cl_request_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::scope_test_exchange::MESSAGE_cl_request;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ordinal)>( parser, &(t.ordinal), "ordinal" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::text_to_server)>( parser, &(t.text_to_server), "text_to_server" );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

//**********************************************************************
// MESSAGE "srv_response" Targets: GMQ (3 parameters)
//  1. INTEGER replied_on (REQUIRED)
//  2. INTEGER value (REQUIRED)
//  3. CHARACTER_STRING text_from_server (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_srv_response_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, replied_on_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, value_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::StringType, text_from_server_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this MESSAGE assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, uint64_t, uint64_t, (uint64_t)0>(composer, arg_3_type::nameAndTypeID, args...);
}

template<class ParserT>
structures::scope_test_exchange::MESSAGE_srv_response MESSAGE_srv_response_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::scope_test_exchange::MESSAGE_srv_response;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::replied_on)>( parser, &(t.replied_on), "replied_on" );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::value)>( parser, &(t.value), "value" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::text_from_server)>( parser, &(t.text_from_server), "text_from_server" );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	globalmq::marshalling::GmqComposer composer( buffer );
	::globalmq::marshalling::impl::composeUnsignedInteger( composer, msgID::id );
	if constexpr ( msgID::id == cl_request::id )
		MESSAGE_cl_request_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == srv_response::id )
		MESSAGE_srv_response_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
}

} // namespace scope_test_exchange 

//**********************************************************************
// PUBLISHABLE publishable_sample (2 parameters)
// 1. INTEGER id
// 2. CHARACTER_STRING text
//**********************************************************************

template<class T, class ComposerT>
class publishable_sample_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;
	static constexpr bool has_id = has_id_member<T>;
	static_assert( has_id, "type T must have member T::id of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_text = has_text_member<T>;
	static_assert( has_text, "type T must have member T::text of a type corresponding to IDL type CHARACTER_STRING" );


public:
	static constexpr uint64_t numTypeID = 11;
	static constexpr const char* stringTypeID = "publishable_sample";

	template<class ... ArgsT>
	publishable_sample_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}
	const T& getState() { return t; }
	ComposerT& getComposer() { return composer; }
	void startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}
	BufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }
	const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }
	auto get_id() { return t.id; }
	void set_id( decltype(T::id) val) { 
		t.id = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( composer, t.id );
	}
	const auto& get_text() { return t.text; }
	void set_text( decltype(T::text) val) { 
		t.text = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( composer, t.text );
	}

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.id, "id", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.text, "text", false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}
};

template<class T, class RegistrarT>
class publishable_sample_NodecppWrapperForPublisher : public publishable_sample_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>
{
	using ComposerT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	RegistrarT& registrar;
public:
	using BufferT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT::BufferType;
	template<class ... ArgsT>
	publishable_sample_NodecppWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_sample_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_sample_NodecppWrapperForPublisher()
	{ 
		registrar.remove( this );
	}

	virtual void startTick( BufferT&& buff ) { publishable_sample_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }
	virtual BufferT&& endTick() { return  publishable_sample_WrapperForPublisher<T, ComposerT>::endTick(); }
	virtual void generateStateSyncMessage(ComposerT& composer) { publishable_sample_WrapperForPublisher<T, ComposerT>::compose(composer); }
	virtual const char* name() { return publishable_sample_WrapperForPublisher<T, ComposerT>::name(); }
};

template<class T, class BufferT>
class publishable_sample_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
	T t;
	static constexpr bool has_id = has_id_member<T>;
	static_assert( has_id, "type T must have member T::id of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_text = has_text_member<T>;
	static_assert( has_text, "type T must have member T::text of a type corresponding to IDL type CHARACTER_STRING" );

	static constexpr bool has_void_update_notifier_for_id = has_void_update_notifier_call_for_id<T>;
	static constexpr bool has_update_notifier_for_id = has_update_notifier_call_for_id<T, decltype(T::id)>;
	static constexpr bool has_any_notifier_for_id = has_void_update_notifier_for_id || has_update_notifier_for_id;
	static constexpr bool has_void_update_notifier_for_text = has_void_update_notifier_call_for_text<T>;
	static constexpr bool has_update_notifier_for_text = has_update_notifier_call_for_text<T, decltype(T::text)>;
	static constexpr bool has_any_notifier_for_text = has_void_update_notifier_for_text || has_update_notifier_for_text;
	static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;

public:
	static constexpr uint64_t numTypeID = 11;
	static constexpr const char* stringTypeID = "publishable_sample";

	template<class ... ArgsT>
	publishable_sample_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
	const T& getState() { return t; }
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }

	template<typename ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );
		GMQ_COLL vector<size_t> addr;
		while( ::globalmq::marshalling::impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )
		{
			GMQ_ASSERT( addr.size() );
			switch ( addr[0] )
			{
				case 0:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					if constexpr( has_any_notifier_for_id )
					{
						decltype(T::id) oldVal = t.id;
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::id)>( parser, &(t.id) );
						bool currentChanged = oldVal != t.id;
						if ( currentChanged )
						{
							if constexpr ( has_void_update_notifier_for_id )
								t.notifyUpdated_id();
							if constexpr ( has_update_notifier_for_id )
								t.notifyUpdated_id( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::id)>( parser, &(t.id) );
					break;
				}
				case 1:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					if constexpr( has_any_notifier_for_text )
					{
						decltype(T::text) oldVal = t.text;
						::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::text)>( parser, &(t.text) );
						bool currentChanged = oldVal != t.text;
						if ( currentChanged )
						{
							if constexpr ( has_void_update_notifier_for_text )
								t.notifyUpdated_text();
							if constexpr ( has_update_notifier_for_text )
								t.notifyUpdated_text( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::text)>( parser, &(t.text) );
					break;
				}
				default:
					throw std::exception(); // bad format, TODO: ...
			}
			addr.clear();
		}
	}


	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::id)>( parser, &(t.id), "id" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::text)>( parser, &(t.text), "text" );

		::globalmq::marshalling::impl::parseStructEnd( parser );

		if constexpr ( has_full_update_notifier )
			t.notifyFullyUpdated();
	}
	auto get_id() { return t.id; }
	const auto& get_text() { return t.text; }
};

template<class T, class RegistrarT>
class publishable_sample_NodecppWrapperForSubscriber : public publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
	RegistrarT& registrar;
public:
	template<class ... ArgsT>
	publishable_sample_NodecppWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_sample_NodecppWrapperForSubscriber()
	{ 
		registrar.remove( this );
	}

	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}
	virtual const char* name()
	{
		return publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::name();
	}
	void subscribe(GMQ_COLL string path)
	{
		registrar.subscribe( this, path );
	}
};

template<class T, class InputBufferT, class ComposerT>
class publishable_sample_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	static constexpr bool has_id = has_id_member<T>;
	static_assert( has_id, "type T must have member T::id of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_text = has_text_member<T>;
	static_assert( has_text, "type T must have member T::text of a type corresponding to IDL type CHARACTER_STRING" );


public:
	static constexpr uint64_t numTypeID = 11;

	publishable_sample_WrapperForConcentrator() {}
	const char* name() {return "publishable_sample";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.id, "id", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.text, "text", false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}

	// Acting as subscriber
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser ) { parseStateSyncMessage(parser); }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser ) { parseStateSyncMessage(parser); }

	template<typename ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );
		GMQ_COLL vector<size_t> addr;
		while( ::globalmq::marshalling::impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )
		{
			GMQ_ASSERT( addr.size() );
			switch ( addr[0] )
			{
				case 0:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::id)>( parser, &(t.id) );
					break;
				}
				case 1:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::text)>( parser, &(t.text) );
					break;
				}
				default:
					throw std::exception(); // bad format, TODO: ...
			}
			addr.clear();
		}
	}

	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::id)>( parser, &(t.id), "id" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::text)>( parser, &(t.text), "text" );

		::globalmq::marshalling::impl::parseStructEnd( parser );
	}
};

template<class InputBufferT, class ComposerT>
class StateConcentratorFactory : public ::globalmq::marshalling::StateConcentratorFactoryBase<InputBufferT, ComposerT>
{
public:
	virtual StateConcentratorBase<InputBufferT, ComposerT>* createConcentrator( uint64_t typeID )
	{
		switch( typeID )
		{
			case 11:
				return new publishable_sample_WrapperForConcentrator<structures::publishable_sample, InputBufferT, ComposerT>;
			default:
				return nullptr;
		}
	}
};

//===============================================================================


} // namespace basic_test

#endif // _publishable_state_h_91f76d69_guard
