#ifndef _publishable_state_h_8057e425_guard
#define _publishable_state_h_8057e425_guard

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
template<typename T> concept has_html_member = requires { { T::html }; };
template<typename T> concept has_id_member = requires { { T::id }; };
template<typename T> concept has_name_member = requires { { T::name }; };
template<typename T> concept has_properties_member = requires { { T::properties }; };
template<typename T> concept has_str_member = requires { { T::str }; };
template<typename T> concept has_tags_member = requires { { T::tags }; };
template<typename T> concept has_text_member = requires { { T::text }; };
template<typename T> concept has_value_member = requires { { T::value }; };


// member update notifier presence checks
using index_type_for_array_notifiers = size_t&;
template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };
template<typename T> concept has_update_notifier_call = requires(T t) { { t.notifyUpdated() }; };
template<typename T> concept has_void_update_notifier_call_for_currentVariant = requires(T t) { { t.notifyUpdated_currentVariant() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_currentVariant = requires { { std::declval<StateT>().notifyUpdated_currentVariant(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_html = requires(T t) { { t.notifyUpdated_html() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_html = requires { { std::declval<StateT>().notifyUpdated_html(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_id = requires(T t) { { t.notifyUpdated_id() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_id = requires { { std::declval<StateT>().notifyUpdated_id(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_name = requires(T t) { { t.notifyUpdated_name() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_name = requires { { std::declval<StateT>().notifyUpdated_name(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_properties = requires(T t) { { t.notifyUpdated_properties() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_properties = requires { { std::declval<StateT>().notifyUpdated_properties(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_properties = requires(T t) { { t.notifyElementUpdated_properties() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_properties = requires { { std::declval<StateT>().notifyElementUpdated_properties(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_properties = requires { { std::declval<StateT>().notifyElementUpdated_properties(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_properties = requires(T t) { { t.notifyInserted_properties() }; };
template<typename StateT> concept has_insert_notifier_call2_for_properties = requires { { std::declval<StateT>().notifyInserted_properties(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_properties = requires { { std::declval<StateT>().notifyInserted_properties(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_properties = requires(T t) { { t.notifyErased_properties() }; };
template<typename StateT> concept has_erased_notifier_call2_for_properties = requires { { std::declval<StateT>().notifyErased_properties(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_properties = requires { { std::declval<StateT>().notifyErased_properties(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_str = requires(T t) { { t.notifyUpdated_str() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_str = requires { { std::declval<StateT>().notifyUpdated_str(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_tags = requires(T t) { { t.notifyUpdated_tags() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_tags = requires { { std::declval<StateT>().notifyUpdated_tags(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_tags = requires(T t) { { t.notifyElementUpdated_tags() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_tags = requires { { std::declval<StateT>().notifyElementUpdated_tags(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_tags = requires { { std::declval<StateT>().notifyElementUpdated_tags(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_tags = requires(T t) { { t.notifyInserted_tags() }; };
template<typename StateT> concept has_insert_notifier_call2_for_tags = requires { { std::declval<StateT>().notifyInserted_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_tags = requires { { std::declval<StateT>().notifyInserted_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_tags = requires(T t) { { t.notifyErased_tags() }; };
template<typename StateT> concept has_erased_notifier_call2_for_tags = requires { { std::declval<StateT>().notifyErased_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_tags = requires { { std::declval<StateT>().notifyErased_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_text = requires(T t) { { t.notifyUpdated_text() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_text = requires { { std::declval<StateT>().notifyUpdated_text(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_value = requires(T t) { { t.notifyUpdated_value() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_value = requires { { std::declval<StateT>().notifyUpdated_value(std::declval<MemberT>()) }; };

//===============================================================================
// C-structures for idl STRUCTs, DISCRIMINATED_UNIONs, MESSAGEs and PUBLISHABLEs

namespace structures {

struct Property;
struct HtmlTag;
class HtmlTextOrTags;

class HtmlTextOrTags : public ::globalmq::marshalling::impl::DiscriminatedUnionType
{
public:
	enum Variants { text=21, taglists=22, unknown };
private:
	Variants v = Variants::unknown;
	struct Case_text
	{
		GMQ_COLL string str;
	};

	struct Case_taglists
	{
		GMQ_COLL vector<HtmlTag> tags;
	};

public:
	using Case_text_str_T = decltype( Case_text::str );
	using Case_taglists_tags_T = decltype( Case_taglists::tags );

private:
	static constexpr size_t text_taglists_memsz = sizeof( Case_taglists ) > ( sizeof( Case_text ) ) ? sizeof( Case_taglists ) : ( sizeof( Case_text ) );
	uint8_t text_taglists_mem[text_taglists_memsz];
	void implDeinit() {
		if ( v != Variants::unknown ) // then destruct existing value
		{
			switch ( v )
			{
				case Variants::text: reinterpret_cast<Case_text*>( text_taglists_mem ) -> ~Case_text(); break;
				case Variants::taglists: reinterpret_cast<Case_taglists*>( text_taglists_mem ) -> ~Case_taglists(); break;
			}
			v = Variants::unknown;
		}
	}

	void implCopyFrom( const HtmlTextOrTags& other ) {
		if ( v != other.v )
			implDeinit();
		switch ( other.v )
		{
			case Variants::text:
				new ( text_taglists_mem ) Case_text( *reinterpret_cast<const Case_text*>( other.text_taglists_mem ) );
				break;
			case Variants::taglists:
				new ( text_taglists_mem ) Case_taglists( *reinterpret_cast<const Case_taglists*>( other.text_taglists_mem ) );
				break;
			case Variants::unknown: break;
		}
		v = other.v;
	}

	void implMoveFrom( HtmlTextOrTags&& other ) {
		if ( v != other.v )
			implDeinit();
		switch ( other.v )
		{
			case Variants::text:
				new ( text_taglists_mem ) Case_text( std::move( *reinterpret_cast<Case_text*>( other.text_taglists_mem ) ) );
				break;
			case Variants::taglists:
				new ( text_taglists_mem ) Case_taglists( std::move( *reinterpret_cast<Case_taglists*>( other.text_taglists_mem ) ) );
				break;
			case Variants::unknown: break;
		}
		v = other.v;
		other.v = Variants::unknown;
	}

public:
	HtmlTextOrTags() {}
	HtmlTextOrTags( const HtmlTextOrTags &other ) {
		implCopyFrom( other );
	}
	HtmlTextOrTags& operator = ( const HtmlTextOrTags &other) {
		implCopyFrom( other );
		return *this;
	}
	HtmlTextOrTags( HtmlTextOrTags&& other) noexcept {
		implMoveFrom( std::move( other ) );
	}
	HtmlTextOrTags& operator = ( HtmlTextOrTags&& other) noexcept {
		implMoveFrom( std::move( other ) );
		return *this;
	}
	virtual ~HtmlTextOrTags() {
		implDeinit();
	}
	Variants currentVariant() const { return v; }
	void initAs( Variants v_ ) {
		implDeinit();
		switch ( v_ ) // init for a new type
		{
			case Variants::text: new ( text_taglists_mem ) Case_text; break;
			case Variants::taglists: new ( text_taglists_mem ) Case_taglists; break;
		}
		v = v_;
	}

	// IDL CASE text:
	Case_text_str_T& str() {
		if ( v != Variants::text )
			throw std::exception();
		return reinterpret_cast<Case_text*>( text_taglists_mem )->str;
	}
	const Case_text_str_T& str() const {
		if ( v != Variants::text )
			throw std::exception();
		return reinterpret_cast<const Case_text*>( text_taglists_mem )->str;
	}
	

	// IDL CASE taglists:
	Case_taglists_tags_T& tags() {
		if ( v != Variants::taglists )
			throw std::exception();
		return reinterpret_cast<Case_taglists*>( text_taglists_mem )->tags;
	}
	const Case_taglists_tags_T& tags() const {
		if ( v != Variants::taglists )
			throw std::exception();
		return reinterpret_cast<const Case_taglists*>( text_taglists_mem )->tags;
	}
	
};

struct HtmlTag
{
	GMQ_COLL string name;
	GMQ_COLL vector<Property> properties;
	HtmlTextOrTags tags;
};

struct Property
{
	GMQ_COLL string name;
	GMQ_COLL string value;
};

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
	HtmlTag html;
};


} // namespace structures

//===============================================================================

struct publishable_STRUCT_Property;
template<class T> class Property_RefWrapper;
template<class T, class RootT> class Property_RefWrapper4Set;

struct publishable_STRUCT_HtmlTag;
template<class T> class HtmlTag_RefWrapper;
template<class T, class RootT> class HtmlTag_RefWrapper4Set;

struct publishable_STRUCT_Property;
template<class T> class Property_RefWrapper;
template<class T, class RootT> class Property_RefWrapper4Set;

struct publishable_STRUCT_HtmlTag;
template<class T> class HtmlTag_RefWrapper;
template<class T, class RootT> class HtmlTag_RefWrapper4Set;


struct publishable_DISCRIMINATED_UNION_HtmlTextOrTags : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		uint64_t caseId;
		::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &(caseId), "caseId" );
		t.initAs( (typename T::Variants)(caseId) );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "caseData" );
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					::globalmq::marshalling::impl::publishableParseString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()), "str" );

					break;
				}
				case 22: // IDL CASE taglists
				{
					::globalmq::marshalling::impl::parseKey( parser, "tags" );
					PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag, true>( parser, t.tags() );

					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		}
	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		uint64_t caseId = t.currentVariant();
		::globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, caseId, "caseId", true );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::composePublishableStructBegin( composer, "caseData" );
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					::globalmq::marshalling::impl::publishableStructComposeString( composer, t.str(), "str", false );

					break;
				}
				case 22: // IDL CASE taglists
				{
					PublishableVectorProcessor::compose<ComposerT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( composer, t.tags(), "tags", false );

					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );
		}
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_str = has_void_update_notifier_call_for_str<T>;
		static constexpr bool has_update_notifier_for_str = has_update_notifier_call_for_str<T, typename T::Case_text_str_T>;
		static constexpr bool has_any_notifier_for_str = has_void_update_notifier_for_str || has_update_notifier_for_str;
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, typename T::Case_taglists_tags_T>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
		using tagsT = typename T::Case_taglists_tags_T;
		static constexpr bool has_void_insert_notifier_for_tags = has_void_insert_notifier_call_for_tags<T>;
		static constexpr bool has_insert_notifier2_for_tags = has_insert_notifier_call2_for_tags<T>;
		static constexpr bool has_insert_notifier3_for_tags = has_insert_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_erased_notifier_for_tags = has_void_erased_notifier_call_for_tags<T>;
		static constexpr bool has_erased_notifier2_for_tags = has_erased_notifier_call2_for_tags<T>;
		static constexpr bool has_erased_notifier3_for_tags = has_erased_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_element_updated_notifier_for_tags = has_element_updated_void_notifier_call_for_tags<T>;
		static constexpr bool has_element_updated_notifier_for_tags = has_element_updated_notifier_call_for_tags<T>;
		static constexpr bool has_full_element_updated_notifier_for_tags = has_full_element_updated_notifier_call_for_tags<T, tagsT&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_update_notifier = has_update_notifier_call<T>;

		uint64_t caseId;
		::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &(caseId), "caseId" );
		t.initAs( (typename T::Variants)(caseId) );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "caseData" );
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					if constexpr( has_any_notifier_for_str || reportChanges || has_update_notifier )
					{
						typename T::Case_text_str_T oldVal = t.str();
						::globalmq::marshalling::impl::publishableParseString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()), "str" );
						bool currentChanged = oldVal != t.str();
						if ( currentChanged )
						{
							if constexpr ( reportChanges || has_update_notifier )
								changed = true;
							if constexpr ( has_void_update_notifier_for_str )
								t.notifyUpdated_str();
							if constexpr ( has_update_notifier_for_str )
								t.notifyUpdated_str( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()), "str" );

					break;
				}
				case 22: // IDL CASE taglists
				{
					if constexpr( reportChanges || has_update_notifier )
					{
						typename T::Case_taglists_tags_T oldVectorVal;
						::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVectorVal );
						::globalmq::marshalling::impl::parseKey( parser, "tags" );
						PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );
						bool currentChanged = !::globalmq::marshalling::impl::isSameVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( oldVectorVal, t.tags() );
						changed = changed || currentChanged;
					}
					else
					{
						::globalmq::marshalling::impl::parseKey( parser, "tags" );
						PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );
					}

					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		}

		if constexpr ( has_update_notifier )
		{
			if ( changed )
				t.notifyUpdated();
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_str = has_void_update_notifier_call_for_str<T>;
		static constexpr bool has_update_notifier_for_str = has_update_notifier_call_for_str<T, typename T::Case_text_str_T>;
		static constexpr bool has_any_notifier_for_str = has_void_update_notifier_for_str || has_update_notifier_for_str;
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, typename T::Case_taglists_tags_T>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
		using tagsT = typename T::Case_taglists_tags_T;
		static constexpr bool has_void_insert_notifier_for_tags = has_void_insert_notifier_call_for_tags<T>;
		static constexpr bool has_insert_notifier2_for_tags = has_insert_notifier_call2_for_tags<T>;
		static constexpr bool has_insert_notifier3_for_tags = has_insert_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_erased_notifier_for_tags = has_void_erased_notifier_call_for_tags<T>;
		static constexpr bool has_erased_notifier2_for_tags = has_erased_notifier_call2_for_tags<T>;
		static constexpr bool has_erased_notifier3_for_tags = has_erased_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_element_updated_notifier_for_tags = has_element_updated_void_notifier_call_for_tags<T>;
		static constexpr bool has_element_updated_notifier_for_tags = has_element_updated_notifier_call_for_tags<T>;
		static constexpr bool has_full_element_updated_notifier_for_tags = has_full_element_updated_notifier_call_for_tags<T, tagsT&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_update_notifier = has_update_notifier_call<T>;

		static constexpr bool has_void_update_notifier_for_currentVariant = has_void_update_notifier_call_for_currentVariant<T>;
		static constexpr bool has_update_notifier_for_currentVariant = has_update_notifier_call_for_currentVariant<T, typename T::Variants>;
		static constexpr bool has_any_notifier_for_currentVariant = has_void_update_notifier_for_currentVariant || has_update_notifier_for_currentVariant;
		GMQ_ASSERT( addr.size() );
		if ( addr[offset] == 0 ) // changing current variant
		{
			if ( addr.size() > offset + 1 )
				throw std::exception(); // bad format, TODO: ...
			uint64_t newVar = T::Variants::unknown;
			::globalmq::marshalling::impl::publishableParseLeafeUnsignedInteger<ParserT, uint64_t>( parser, &newVar );
			if constexpr( has_any_notifier_for_currentVariant || reportChanges || has_update_notifier )
			{
				auto oldVal = t.currentVariant();
				t.initAs( (typename T::Variants)( newVar ) );
				bool currentChanged = oldVal != t.currentVariant();
				if ( currentChanged )
				{
					if constexpr ( reportChanges || has_update_notifier )
						changed = true;
					if constexpr ( has_void_update_notifier_for_currentVariant )
						t.notifyUpdated_currentVariant();
					if constexpr ( has_update_notifier_for_currentVariant )
						t.notifyUpdated_currentVariant( oldVal );
				}
			}
			else
				t.initAs( (typename T::Variants)( newVar ) );
		}
		else // updating particular members in particular cases
		{
			switch ( t.currentVariant() )
			{
				case 21: // IDL CASE text
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() > offset + 1 )
								throw std::exception(); // bad format, TODO: ...
							if constexpr( has_any_notifier_for_str || reportChanges || has_update_notifier )
							{
								typename T::Case_text_str_T oldVal = t.str();
								::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()) );
								bool currentChanged = oldVal != t.str();
								if ( currentChanged )
								{
									if constexpr ( reportChanges || has_update_notifier )
										changed = true;
									if constexpr ( has_void_update_notifier_for_str )
										t.notifyUpdated_str();
									if constexpr ( has_update_notifier_for_str )
										t.notifyUpdated_str( oldVal );
								}
							}
							else
								::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()) );
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				case 22: // IDL CASE taglists
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							{
								typename T::Case_taglists_tags_T oldVectorVal;
								bool currentChanged = false;
								constexpr bool alwaysCollectChanges = has_any_notifier_for_tags;
								if constexpr( alwaysCollectChanges )
									::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVectorVal );
								if ( addr.size() > offset + 1 ) // one of actions over elements of the vector
								{
									size_t pos = addr[offset + 1];
									if ( pos > t.tags().size() )
										throw std::exception();
									if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
									{
										typename T::Case_taglists_tags_T::value_type& value = t.tags()[pos];
										if constexpr ( has_full_element_updated_notifier_for_tags )
										{
											typename T::Case_taglists_tags_T::value_type oldValue;
											publishable_STRUCT_HtmlTag::copy( value, oldValue );
											currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											if ( currentChanged )
											{
												changed = true;
												t.notifyElementUpdated_tags( pos, oldValue );
												if constexpr ( has_element_updated_notifier_for_tags )
													t.notifyElementUpdated_tags();
												if constexpr ( has_void_element_updated_notifier_for_tags )
													t.notifyElementUpdated_tags();
											}
										}
										else if constexpr ( has_element_updated_notifier_for_tags )
										{
											currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											if ( currentChanged )
											{
												changed = true;
												t.notifyElementUpdated_tags( pos );
												if constexpr ( has_void_element_updated_notifier_for_tags )
													t.notifyElementUpdated_tags();
											}
										}
										else if constexpr ( has_void_element_updated_notifier_for_tags )
										{
											currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											if ( currentChanged )
											{
												changed = true;
												t.notifyElementUpdated_tags();
											}
										}
										else
										{
											if constexpr ( alwaysCollectChanges )
												currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											else
												publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type>( parser, value, addr, offset + 2 );
										}
									}
									else // update of one or more elelments as a whole
									{
										size_t action;
										::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
										switch ( action )
										{
											case ActionOnVector::remove_at:
											{
												typename T::Case_taglists_tags_T oldVal;
												::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVal );
												t.tags().erase( t.tags().begin() + pos );
												if constexpr ( has_erased_notifier3_for_tags )
													t.notifyErased_tags( pos, oldVal );
												if constexpr ( has_erased_notifier2_for_tags )
													t.notifyErased_tags( pos );
												if constexpr ( has_void_erased_notifier_for_tags )
													t.notifyErased_tags();
												if constexpr ( alwaysCollectChanges )
													currentChanged = true;
												break;
											}
											case ActionOnVector::update_at:
											{
												::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
												typename T::Case_taglists_tags_T::value_type& value = t.tags()[pos];
												typename T::Case_taglists_tags_T::value_type oldValue;
												publishable_STRUCT_HtmlTag::copy( value, oldValue );
												if constexpr ( has_full_element_updated_notifier_for_tags )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
													if ( currentChanged )
													{
														changed = true;
														t.notifyElementUpdated_tags( pos, oldValue );
														if constexpr ( has_element_updated_notifier_for_tags )
															t.notifyElementUpdated_tags( pos );
														if constexpr ( has_void_element_updated_notifier_for_tags )
															t.notifyElementUpdated_tags();
													}
												}
												else if constexpr ( has_element_updated_notifier_for_tags )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
													if ( currentChanged )
													{
														changed = true;
														t.notifyElementUpdated_tags( pos );
														if constexpr ( has_void_element_updated_notifier_for_tags )
															t.notifyElementUpdated_tags();
													}
												}
												else if constexpr ( has_void_element_updated_notifier_for_tags )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
													if ( currentChanged )
														changed = true;
														t.notifyElementUpdated_tags();
												}
												else
												{
													if constexpr ( alwaysCollectChanges )
												{
														currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
														changed = true;
												}
													else
														PublishableVectorProcessor::parseSingleValue<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value );
												}
												break;
											}
											case ActionOnVector::insert_single_before:
											{
												::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
												typename T::Case_taglists_tags_T::value_type value;
												PublishableVectorProcessor::parseSingleValue<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value );
												if constexpr ( has_insert_notifier3_for_tags )
												{
													typename T::Case_taglists_tags_T oldVal;
													::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVal );
													t.notifyInserted_tags( pos, oldVal );
												}
												if constexpr ( has_insert_notifier2_for_tags )
													t.notifyInserted_tags( pos );
												if constexpr ( has_void_insert_notifier_for_tags )
													t.notifyInserted_tags();
												t.tags().insert( t.tags().begin() + pos, value );
												if constexpr ( alwaysCollectChanges )
													currentChanged = true;
												break;
											}
											default:
												throw std::exception();
										}
										::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );
									}
								}
								else // replacement of the whole vector
								{
									::globalmq::marshalling::impl::publishableParseLeafeVectorBegin( parser );

									if constexpr( alwaysCollectChanges )
									{
										PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );
										currentChanged = !::globalmq::marshalling::impl::isSameVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( oldVectorVal, t.tags() );
									}
									else
										PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );

									::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
								}

								if ( currentChanged )
								{
									changed = true;
									if constexpr( has_void_update_notifier_for_tags )
										t.notifyUpdated_tags();
									if constexpr( has_update_notifier_for_tags )
										t.notifyUpdated_tags( oldVectorVal );
								}
							}
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
		}
		if constexpr ( has_update_notifier )
		{
			if ( changed )
				t.notifyUpdated();
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<typename UserT>
	static void copy(const UserT& src, UserT& dst) {
		auto srcCaseId = src.currentVariant();
		dst.initAs( srcCaseId );
		if ( srcCaseId != UserT::Variants::unknown )
			switch ( srcCaseId )
			{
				case 21: // IDL CASE text
				{
					dst.str() = src.str();
					break;
				}
				case 22: // IDL CASE taglists
				{
					::globalmq::marshalling::impl::copyVector<typename UserT::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( src.tags(), dst.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.currentVariant() != s2.currentVariant() )
			return false;
		if ( s1.currentVariant() != UserT::Variants::unknown )
			switch ( s1.currentVariant() )
			{
				case 21: // IDL CASE text
				{
					if ( s1.str() != s2.str() ) return false;
					break;
				}
				case 22: // IDL CASE taglists
				{
					if ( !::globalmq::marshalling::impl::isSameVector<typename UserT::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( s1.tags(), s2.tags() ) ) return false;
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
		return true;
	}
};

struct publishable_STRUCT_HtmlTag : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parseKey( parser, "properties" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::properties), publishable_STRUCT_Property, true>( parser, t.properties );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( parser, t.tags );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::properties), publishable_STRUCT_Property>( composer, t.properties, "properties", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( composer, t.tags );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
		static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
		static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
		static constexpr bool has_void_update_notifier_for_properties = has_void_update_notifier_call_for_properties<T>;
		static constexpr bool has_update_notifier_for_properties = has_update_notifier_call_for_properties<T, decltype(T::properties)>;
		static constexpr bool has_any_notifier_for_properties = has_void_update_notifier_for_properties || has_update_notifier_for_properties;
		using propertiesT = decltype(T::properties);
		static constexpr bool has_void_insert_notifier_for_properties = has_void_insert_notifier_call_for_properties<T>;
		static constexpr bool has_insert_notifier2_for_properties = has_insert_notifier_call2_for_properties<T>;
		static constexpr bool has_insert_notifier3_for_properties = has_insert_notifier_call3_for_properties<T, GMQ_COLL vector<propertiesT>&>;
		static constexpr bool has_void_erased_notifier_for_properties = has_void_erased_notifier_call_for_properties<T>;
		static constexpr bool has_erased_notifier2_for_properties = has_erased_notifier_call2_for_properties<T>;
		static constexpr bool has_erased_notifier3_for_properties = has_erased_notifier_call3_for_properties<T, GMQ_COLL vector<propertiesT>&>;
		static constexpr bool has_void_element_updated_notifier_for_properties = has_element_updated_void_notifier_call_for_properties<T>;
		static constexpr bool has_element_updated_notifier_for_properties = has_element_updated_notifier_call_for_properties<T>;
		static constexpr bool has_full_element_updated_notifier_for_properties = has_full_element_updated_notifier_call_for_properties<T, propertiesT&>;
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, decltype(T::tags)>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_update_notifier = has_update_notifier_call<T>;

		if constexpr( has_any_notifier_for_name || reportChanges || has_update_notifier )
		{
			decltype(T::name) oldVal = t.name;
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );
			bool currentChanged = oldVal != t.name;
			if ( currentChanged )
			{
				if constexpr ( reportChanges || has_update_notifier )
					changed = true;
				if constexpr ( has_void_update_notifier_for_name )
					t.notifyUpdated_name();
				if constexpr ( has_update_notifier_for_name )
					t.notifyUpdated_name( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		if constexpr( reportChanges || has_update_notifier )
		{
			decltype(T::properties) oldVectorVal;
			::globalmq::marshalling::impl::copyVector<decltype(T::properties), publishable_STRUCT_Property>( t.properties, oldVectorVal );
			::globalmq::marshalling::impl::parseKey( parser, "properties" );
			PublishableVectorProcessor::parse<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, t.properties );
			bool currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::properties), publishable_STRUCT_Property>( oldVectorVal, t.properties );
			changed = changed || currentChanged;
		}
		else
		{
			::globalmq::marshalling::impl::parseKey( parser, "properties" );
			PublishableVectorProcessor::parse<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, t.properties );
		}

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "tags" );
		if constexpr( has_update_notifier_for_tags )
		{
			decltype(T::tags) temp_tags;
			publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy<decltype(T::tags)>( t.tags, temp_tags );
			bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
			if ( changedCurrent )
			{
				changed = true;
				if constexpr ( reportChanges || has_update_notifier )
					changed = true;
				if constexpr( has_void_update_notifier_for_tags )
					t.notifyUpdated_tags();
				if constexpr( has_update_notifier_for_tags )
					t.notifyUpdated_tags( temp_tags );
			}
		}
		else if constexpr( has_void_update_notifier_for_tags )
		{
			bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges || has_update_notifier )
					changed = true;
				t.notifyUpdated_tags();
			}
		}
		else if constexpr ( reportChanges || has_update_notifier )
			changed = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
		else
			publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse( parser, t.tags );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		if constexpr ( has_update_notifier )
		{
			if ( changed )
				t.notifyUpdated();
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
		static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
		static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
		static constexpr bool has_void_update_notifier_for_properties = has_void_update_notifier_call_for_properties<T>;
		static constexpr bool has_update_notifier_for_properties = has_update_notifier_call_for_properties<T, decltype(T::properties)>;
		static constexpr bool has_any_notifier_for_properties = has_void_update_notifier_for_properties || has_update_notifier_for_properties;
		using propertiesT = decltype(T::properties);
		static constexpr bool has_void_insert_notifier_for_properties = has_void_insert_notifier_call_for_properties<T>;
		static constexpr bool has_insert_notifier2_for_properties = has_insert_notifier_call2_for_properties<T>;
		static constexpr bool has_insert_notifier3_for_properties = has_insert_notifier_call3_for_properties<T, GMQ_COLL vector<propertiesT>&>;
		static constexpr bool has_void_erased_notifier_for_properties = has_void_erased_notifier_call_for_properties<T>;
		static constexpr bool has_erased_notifier2_for_properties = has_erased_notifier_call2_for_properties<T>;
		static constexpr bool has_erased_notifier3_for_properties = has_erased_notifier_call3_for_properties<T, GMQ_COLL vector<propertiesT>&>;
		static constexpr bool has_void_element_updated_notifier_for_properties = has_element_updated_void_notifier_call_for_properties<T>;
		static constexpr bool has_element_updated_notifier_for_properties = has_element_updated_notifier_call_for_properties<T>;
		static constexpr bool has_full_element_updated_notifier_for_properties = has_full_element_updated_notifier_call_for_properties<T, propertiesT&>;
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, decltype(T::tags)>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_update_notifier = has_update_notifier_call<T>;

		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_name || reportChanges || has_update_notifier )
				{
					decltype(T::name) oldVal = t.name;
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
					bool currentChanged = oldVal != t.name;
					if ( currentChanged )
					{
						if constexpr ( reportChanges || has_update_notifier )
							changed = true;
						if constexpr ( has_void_update_notifier_for_name )
							t.notifyUpdated_name();
						if constexpr ( has_update_notifier_for_name )
							t.notifyUpdated_name( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
				break;
			}
			case 1:
			{
				{
					decltype(T::properties) oldVectorVal;
					bool currentChanged = false;
					constexpr bool alwaysCollectChanges = has_any_notifier_for_properties;
					if constexpr( alwaysCollectChanges )
						::globalmq::marshalling::impl::copyVector<decltype(T::properties), publishable_STRUCT_Property>( t.properties, oldVectorVal );
					if ( addr.size() > offset + 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[offset + 1];
						if ( pos > t.properties.size() )
							throw std::exception();
						if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
						{
							typename decltype(T::properties)::value_type& value = t.properties[pos];
							if constexpr ( has_full_element_updated_notifier_for_properties )
							{
								typename decltype(T::properties)::value_type oldValue;
								publishable_STRUCT_Property::copy( value, oldValue );
								currentChanged = publishable_STRUCT_Property::parse<ParserT, typename decltype(T::properties)::value_type, bool>( parser, value, addr, offset + 2 );
								if ( currentChanged )
								{
									changed = true;
									t.notifyElementUpdated_properties( pos, oldValue );
									if constexpr ( has_element_updated_notifier_for_properties )
										t.notifyElementUpdated_properties();
									if constexpr ( has_void_element_updated_notifier_for_properties )
										t.notifyElementUpdated_properties();
								}
							}
							else if constexpr ( has_element_updated_notifier_for_properties )
							{
								currentChanged = publishable_STRUCT_Property::parse<ParserT, typename decltype(T::properties)::value_type, bool>( parser, value, addr, offset + 2 );
								if ( currentChanged )
								{
									changed = true;
									t.notifyElementUpdated_properties( pos );
									if constexpr ( has_void_element_updated_notifier_for_properties )
										t.notifyElementUpdated_properties();
								}
							}
							else if constexpr ( has_void_element_updated_notifier_for_properties )
							{
								currentChanged = publishable_STRUCT_Property::parse<ParserT, typename decltype(T::properties)::value_type, bool>( parser, value, addr, offset + 2 );
								if ( currentChanged )
								{
									changed = true;
									t.notifyElementUpdated_properties();
								}
							}
							else
							{
								if constexpr ( alwaysCollectChanges )
									currentChanged = publishable_STRUCT_Property::parse<ParserT, typename decltype(T::properties)::value_type, bool>( parser, value, addr, offset + 2 );
								else
									publishable_STRUCT_Property::parse<ParserT, typename decltype(T::properties)::value_type>( parser, value, addr, offset + 2 );
							}
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							switch ( action )
							{
								case ActionOnVector::remove_at:
								{
									decltype(T::properties) oldVal;
									::globalmq::marshalling::impl::copyVector<decltype(T::properties), publishable_STRUCT_Property>( t.properties, oldVal );
									t.properties.erase( t.properties.begin() + pos );
									if constexpr ( has_erased_notifier3_for_properties )
										t.notifyErased_properties( pos, oldVal );
									if constexpr ( has_erased_notifier2_for_properties )
										t.notifyErased_properties( pos );
									if constexpr ( has_void_erased_notifier_for_properties )
										t.notifyErased_properties();
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::properties)::value_type& value = t.properties[pos];
									typename decltype(T::properties)::value_type oldValue;
									publishable_STRUCT_Property::copy( value, oldValue );
									if constexpr ( has_full_element_updated_notifier_for_properties )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, value, oldValue );
										if ( currentChanged )
										{
											changed = true;
											t.notifyElementUpdated_properties( pos, oldValue );
											if constexpr ( has_element_updated_notifier_for_properties )
												t.notifyElementUpdated_properties( pos );
											if constexpr ( has_void_element_updated_notifier_for_properties )
												t.notifyElementUpdated_properties();
										}
									}
									else if constexpr ( has_element_updated_notifier_for_properties )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, value, oldValue );
										if ( currentChanged )
										{
											changed = true;
											t.notifyElementUpdated_properties( pos );
											if constexpr ( has_void_element_updated_notifier_for_properties )
												t.notifyElementUpdated_properties();
										}
									}
									else if constexpr ( has_void_element_updated_notifier_for_properties )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, value, oldValue );
										if ( currentChanged )
											changed = true;
											t.notifyElementUpdated_properties();
									}
									else
									{
										if constexpr ( alwaysCollectChanges )
									{
											currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, value, oldValue );
											changed = true;
									}
										else
											PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, value );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::properties)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, value );
									if constexpr ( has_insert_notifier3_for_properties )
									{
										decltype(T::properties) oldVal;
										::globalmq::marshalling::impl::copyVector<decltype(T::properties), publishable_STRUCT_Property>( t.properties, oldVal );
										t.notifyInserted_properties( pos, oldVal );
									}
									if constexpr ( has_insert_notifier2_for_properties )
										t.notifyInserted_properties( pos );
									if constexpr ( has_void_insert_notifier_for_properties )
										t.notifyInserted_properties();
									t.properties.insert( t.properties.begin() + pos, value );
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								default:
									throw std::exception();
							}
							::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );
						}
					}
					else // replacement of the whole vector
					{
						::globalmq::marshalling::impl::publishableParseLeafeVectorBegin( parser );

						if constexpr( alwaysCollectChanges )
						{
							PublishableVectorProcessor::parse<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, t.properties );
							currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::properties), publishable_STRUCT_Property>( oldVectorVal, t.properties );
						}
						else
							PublishableVectorProcessor::parse<ParserT, decltype(T::properties), publishable_STRUCT_Property>( parser, t.properties );

						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					if ( currentChanged )
					{
						changed = true;
						if constexpr( has_void_update_notifier_for_properties )
							t.notifyUpdated_properties();
						if constexpr( has_update_notifier_for_properties )
							t.notifyUpdated_properties( oldVectorVal );
					}
				}
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );
					if constexpr( has_update_notifier_for_tags )
					{
						decltype(T::tags) temp_tags;
						publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy<decltype(T::tags)>( t.tags, temp_tags );
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges || has_update_notifier )
								changed = true;
							if constexpr( has_void_update_notifier_for_tags )
								t.notifyUpdated_tags();
							if constexpr( has_update_notifier_for_tags )
								t.notifyUpdated_tags( temp_tags );
						}
					}
					else if constexpr( has_void_update_notifier_for_tags )
					{
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges || has_update_notifier )
								changed = true;
							t.notifyUpdated_tags();
						}
					}
					else
					{
						if constexpr ( reportChanges || has_update_notifier )
							changed = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
						else
							publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse( parser, t.tags );
					}
					::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
				}
				else // let child continue parsing
				{
					if constexpr( has_update_notifier_for_tags )
					{
						decltype(T::tags) temp_tags;
						publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy<decltype(T::tags)>( t.tags, temp_tags );
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags, addr, offset + 1 );
						if ( changedCurrent )
						{
							changed = true;
							if constexpr ( reportChanges || has_update_notifier )
								changed = true;
							if constexpr( has_void_update_notifier_for_tags )
								t.notifyUpdated_tags();
							if constexpr( has_update_notifier_for_tags )
								t.notifyUpdated_tags( temp_tags );
						}
					}
					else if constexpr( has_void_update_notifier_for_tags )
					{
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges || has_update_notifier )
								changed = true;
							t.notifyUpdated_tags();
						}
					}
					else if constexpr ( reportChanges || has_update_notifier )
						changed = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags, addr, offset + 1 );
					else
						publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse( parser, t.tags, addr, offset + 1 );
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if constexpr ( has_update_notifier )
		{
			if ( changed )
				t.notifyUpdated();
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<typename UserT>
	static void copy(const UserT& src, UserT& dst) {
		dst.name = src.name;
		::globalmq::marshalling::impl::copyVector<decltype(UserT::properties), publishable_STRUCT_Property>( src.properties, dst.properties );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy( src.tags, dst.tags );
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.name != s2.name ) return false;
		if ( !::globalmq::marshalling::impl::isSameVector<decltype(UserT::properties), publishable_STRUCT_Property>( s1.properties, s2.properties ) ) return false;
		if( ! publishable_DISCRIMINATED_UNION_HtmlTextOrTags::isSame( s1.tags, s2.tags ) ) return false;
		return true;
	}
};

struct publishable_STRUCT_Property : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::value)>( parser, &(t.value), "value" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.value, "value", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
		static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
		static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
		static constexpr bool has_void_update_notifier_for_value = has_void_update_notifier_call_for_value<T>;
		static constexpr bool has_update_notifier_for_value = has_update_notifier_call_for_value<T, decltype(T::value)>;
		static constexpr bool has_any_notifier_for_value = has_void_update_notifier_for_value || has_update_notifier_for_value;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_update_notifier = has_update_notifier_call<T>;

		if constexpr( has_any_notifier_for_name || reportChanges || has_update_notifier )
		{
			decltype(T::name) oldVal = t.name;
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );
			bool currentChanged = oldVal != t.name;
			if ( currentChanged )
			{
				if constexpr ( reportChanges || has_update_notifier )
					changed = true;
				if constexpr ( has_void_update_notifier_for_name )
					t.notifyUpdated_name();
				if constexpr ( has_update_notifier_for_name )
					t.notifyUpdated_name( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		if constexpr( has_any_notifier_for_value || reportChanges || has_update_notifier )
		{
			decltype(T::value) oldVal = t.value;
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::value)>( parser, &(t.value), "value" );
			bool currentChanged = oldVal != t.value;
			if ( currentChanged )
			{
				if constexpr ( reportChanges || has_update_notifier )
					changed = true;
				if constexpr ( has_void_update_notifier_for_value )
					t.notifyUpdated_value();
				if constexpr ( has_update_notifier_for_value )
					t.notifyUpdated_value( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::value)>( parser, &(t.value), "value" );


		if constexpr ( has_update_notifier )
		{
			if ( changed )
				t.notifyUpdated();
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
		static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
		static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
		static constexpr bool has_void_update_notifier_for_value = has_void_update_notifier_call_for_value<T>;
		static constexpr bool has_update_notifier_for_value = has_update_notifier_call_for_value<T, decltype(T::value)>;
		static constexpr bool has_any_notifier_for_value = has_void_update_notifier_for_value || has_update_notifier_for_value;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_update_notifier = has_update_notifier_call<T>;

		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_name || reportChanges || has_update_notifier )
				{
					decltype(T::name) oldVal = t.name;
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
					bool currentChanged = oldVal != t.name;
					if ( currentChanged )
					{
						if constexpr ( reportChanges || has_update_notifier )
							changed = true;
						if constexpr ( has_void_update_notifier_for_name )
							t.notifyUpdated_name();
						if constexpr ( has_update_notifier_for_name )
							t.notifyUpdated_name( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
				break;
			}
			case 1:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_value || reportChanges || has_update_notifier )
				{
					decltype(T::value) oldVal = t.value;
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::value)>( parser, &(t.value) );
					bool currentChanged = oldVal != t.value;
					if ( currentChanged )
					{
						if constexpr ( reportChanges || has_update_notifier )
							changed = true;
						if constexpr ( has_void_update_notifier_for_value )
							t.notifyUpdated_value();
						if constexpr ( has_update_notifier_for_value )
							t.notifyUpdated_value( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::value)>( parser, &(t.value) );
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if constexpr ( has_update_notifier )
		{
			if ( changed )
				t.notifyUpdated();
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<typename UserT>
	static void copy(const UserT& src, UserT& dst) {
		dst.name = src.name;
		dst.value = src.value;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.name != s2.name ) return false;
		if ( s1.value != s2.value ) return false;
		return true;
	}
};

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
// PUBLISHABLE publishable_sample (3 parameters)
// 1. INTEGER id
// 2. CHARACTER_STRING text
// 3. STRUCT HtmlTag html
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
	static constexpr bool has_html = has_html_member<T>;
	static_assert( has_html, "type T must have member T::html of a type corresponding to IDL type STRUCT HtmlTag" );


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
	const char* publishableName() { return stringTypeID; }
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
	const auto& get_html() { return t.html; }
	void set_html( decltype(T::html) val) { 
		t.html = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_HtmlTag::compose( composer, t.html );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_html() { return HtmlTag_RefWrapper4Set<decltype(T::html), publishable_sample_WrapperForPublisher>(t.html, *this, GMQ_COLL vector<size_t>(), 2); }

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.id, "id", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.text, "text", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "html" );
		publishable_STRUCT_HtmlTag::compose( composer, t.html );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );


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
	virtual const char* publishableName() { return publishable_sample_WrapperForPublisher<T, ComposerT>::publishableName(); }
};

template<class T, class BufferT>
class publishable_sample_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
	T t;
	static constexpr bool has_id = has_id_member<T>;
	static_assert( has_id, "type T must have member T::id of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_text = has_text_member<T>;
	static_assert( has_text, "type T must have member T::text of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_html = has_html_member<T>;
	static_assert( has_html, "type T must have member T::html of a type corresponding to IDL type STRUCT HtmlTag" );

	static constexpr bool has_void_update_notifier_for_id = has_void_update_notifier_call_for_id<T>;
	static constexpr bool has_update_notifier_for_id = has_update_notifier_call_for_id<T, decltype(T::id)>;
	static constexpr bool has_any_notifier_for_id = has_void_update_notifier_for_id || has_update_notifier_for_id;
	static constexpr bool has_void_update_notifier_for_text = has_void_update_notifier_call_for_text<T>;
	static constexpr bool has_update_notifier_for_text = has_update_notifier_call_for_text<T, decltype(T::text)>;
	static constexpr bool has_any_notifier_for_text = has_void_update_notifier_for_text || has_update_notifier_for_text;
	static constexpr bool has_void_update_notifier_for_html = has_void_update_notifier_call_for_html<T>;
	static constexpr bool has_update_notifier_for_html = has_update_notifier_call_for_html<T, decltype(T::html)>;
	static constexpr bool has_any_notifier_for_html = has_void_update_notifier_for_html || has_update_notifier_for_html;
	static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
	static constexpr bool has_update_notifier = has_update_notifier_call<T>;


public:
	static constexpr uint64_t numTypeID = 11;
	static constexpr const char* stringTypeID = "publishable_sample";

	static constexpr bool reportChanges = false;
	bool changed = false;

	template<class ... ArgsT>
	publishable_sample_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
	const T& getState() { return t; }
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual const char* name() { return stringTypeID; }
	virtual const char* publishableName() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }

	template<typename ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );
		bool changed = false;
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
					if constexpr( has_any_notifier_for_id || reportChanges || has_update_notifier )
					{
						decltype(T::id) oldVal = t.id;
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::id)>( parser, &(t.id) );
						bool currentChanged = oldVal != t.id;
						if ( currentChanged )
						{
							if constexpr ( reportChanges || has_update_notifier )
								changed = true;
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
					if constexpr( has_any_notifier_for_text || reportChanges || has_update_notifier )
					{
						decltype(T::text) oldVal = t.text;
						::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::text)>( parser, &(t.text) );
						bool currentChanged = oldVal != t.text;
						if ( currentChanged )
						{
							if constexpr ( reportChanges || has_update_notifier )
								changed = true;
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
				case 2:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_html )
						{
							decltype(T::html) temp_html;
							publishable_STRUCT_HtmlTag::copy<decltype(T::html)>( t.html, temp_html );
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::html), bool>( parser, t.html );
							if ( changedCurrent )
							{
								if constexpr ( reportChanges || has_update_notifier )
									changed = true;
								if constexpr( has_void_update_notifier_for_html )
									t.notifyUpdated_html();
								if constexpr( has_update_notifier_for_html )
									t.notifyUpdated_html( temp_html );
							}
						}
						else if constexpr( has_void_update_notifier_for_html )
						{
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::html), bool>( parser, t.html );
							if ( changedCurrent )
							{
								if constexpr ( reportChanges || has_update_notifier )
									changed = true;
								t.notifyUpdated_html();
							}
						}
						else
						{
							if constexpr ( reportChanges || has_update_notifier )
								changed = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::html), bool>( parser, t.html );
							else
								publishable_STRUCT_HtmlTag::parse( parser, t.html );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_html )
						{
							decltype(T::html) temp_html;
							publishable_STRUCT_HtmlTag::copy<decltype(T::html)>( t.html, temp_html );
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::html), bool>( parser, t.html, addr, 1 );
							if ( changedCurrent )
							{
								changed = true;
								if constexpr ( reportChanges || has_update_notifier )
									changed = true;
								if constexpr( has_void_update_notifier_for_html )
									t.notifyUpdated_html();
								if constexpr( has_update_notifier_for_html )
									t.notifyUpdated_html( temp_html );
							}
						}
						else if constexpr( has_void_update_notifier_for_html )
						{
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::html), bool>( parser, t.html, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr ( reportChanges || has_update_notifier )
									changed = true;
								t.notifyUpdated_html();
							}
						}
						else if constexpr ( reportChanges || has_update_notifier )
							changed = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::html), bool>( parser, t.html, addr, 1 );
						else
							publishable_STRUCT_HtmlTag::parse( parser, t.html, addr, 1 );
					}
					break;
				}
				default:
					throw std::exception(); // bad format, TODO: ...
			}
			addr.clear();
		}
		if constexpr ( has_update_notifier )
		{
			if ( changed )
				t.notifyUpdated();
		}
	}


	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::id)>( parser, &(t.id), "id" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::text)>( parser, &(t.text), "text" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "html" );
		publishable_STRUCT_HtmlTag::parse( parser, t.html );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseStructEnd( parser );

		if constexpr ( has_full_update_notifier )
			t.notifyFullyUpdated();
	}
	auto get_id() { return t.id; }
	const auto& get_text() { return t.text; }
	const auto& get_html() { return t.html; }
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
	virtual const char* publishableName()
	{
		return publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::publishableName();
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
	static constexpr bool has_html = has_html_member<T>;
	static_assert( has_html, "type T must have member T::html of a type corresponding to IDL type STRUCT HtmlTag" );


public:
	static constexpr uint64_t numTypeID = 11;

	publishable_sample_WrapperForConcentrator() {}
	const char* name() {return "publishable_sample";}
	const char* publishableName() {return "publishable_sample";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.id, "id", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.text, "text", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "html" );
		publishable_STRUCT_HtmlTag::compose( composer, t.html );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );


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
		bool changed = false;
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
				case 2:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_HtmlTag::parse( parser, t.html );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_HtmlTag::parse( parser, t.html, addr, 1 );
					}
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

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "html" );
		publishable_STRUCT_HtmlTag::parse( parser, t.html );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

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

template<class T>
class HtmlTextOrTags_RefWrapper
{
	T& t;


public:
	HtmlTextOrTags_RefWrapper( T& actual ) : t( actual ) {}
	auto get_currentVariant() { return t.currentVariant(); }
	const auto& get_str() { return t.str(); }
	auto get_tags() { return globalmq::marshalling::VectorOfStructRefWrapper<HtmlTag_RefWrapper<typename T::Case_taglists_tags_T::value_type>, typename T::Case_taglists_tags_T>(t.tags()); }
};

template<class T, class RootT>
class HtmlTextOrTags_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;


public:
	HtmlTextOrTags_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_currentVariant() { return t.currentVariant(); }
	void set_currentVariant( typename T::Variants v ) { 
		t.initAs( v ); 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeUnsignedInteger( root.getComposer(), (uint64_t)(t.currentVariant()) );
	}
	const auto& get_str() { return t.str(); }
	void set_str( typename T::Case_text_str_T val) { 
		t.str() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.str() );
	}
	auto get_tags() { return globalmq::marshalling::VectorOfStructRefWrapper<HtmlTag_RefWrapper<typename T::Case_taglists_tags_T::value_type>, typename T::Case_taglists_tags_T>(t.tags()); }
	void set_tags( typename T::Case_taglists_tags_T val) { 
		t.tags() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( root.getComposer() );
		PublishableVectorProcessor::compose<decltype(root.getComposer()), typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( root.getComposer(), t.tags() );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( root.getComposer() );
	}
	auto get4set_tags() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag, RootT, HtmlTag_RefWrapper4Set<typename T::Case_taglists_tags_T::value_type, RootT>>(t.tags(), root, address, 1); }
};

template<class T>
class HtmlTag_RefWrapper
{
	T& t;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_properties = has_properties_member<T>;
	static_assert( has_properties, "type T must have member T::properties of a type corresponding to IDL type VECTOR<STRUCT Property>" );
	static constexpr bool has_tags = has_tags_member<T>;
	static_assert( has_tags, "type T must have member T::tags of a type corresponding to IDL type DISCRIMINATED_UNION HtmlTextOrTags" );


public:
	HtmlTag_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_name() { return t.name; }
	auto get_properties() { return globalmq::marshalling::VectorOfStructRefWrapper<Property_RefWrapper<typename decltype(T::properties)::value_type>, decltype(T::properties)>(t.properties); }
	const auto& get_tags() { return t.tags; }
};

template<class T, class RootT>
class HtmlTag_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_properties = has_properties_member<T>;
	static_assert( has_properties, "type T must have member T::properties of a type corresponding to IDL type VECTOR<STRUCT Property>" );
	static constexpr bool has_tags = has_tags_member<T>;
	static_assert( has_tags, "type T must have member T::tags of a type corresponding to IDL type DISCRIMINATED_UNION HtmlTextOrTags" );


public:
	HtmlTag_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.name );
	}
	auto get_properties() { return globalmq::marshalling::VectorOfStructRefWrapper<Property_RefWrapper<typename decltype(T::properties)::value_type>, decltype(T::properties)>(t.properties); }
	void set_properties( decltype(T::properties) val) { 
		t.properties = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( root.getComposer() );
		PublishableVectorProcessor::compose<decltype(root.getComposer()), decltype(T::properties), publishable_STRUCT_Property>( root.getComposer(), t.properties );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( root.getComposer() );
	}
	auto get4set_properties() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<decltype(T::properties), publishable_STRUCT_Property, RootT, Property_RefWrapper4Set<typename decltype(T::properties)::value_type, RootT>>(t.properties, root, address, 1); }
	const auto& get_tags() { return t.tags; }
	void set_tags( decltype(T::tags) val) { 
		t.tags = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( root.getComposer() );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( root.getComposer(), t.tags );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( root.getComposer() );
	}
	auto get4set_tags() { return HtmlTextOrTags_RefWrapper4Set<decltype(T::tags), RootT>(t.tags, root, address, 2); }
};

template<class T>
class Property_RefWrapper
{
	T& t;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_value = has_value_member<T>;
	static_assert( has_value, "type T must have member T::value of a type corresponding to IDL type CHARACTER_STRING" );


public:
	Property_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_name() { return t.name; }
	const auto& get_value() { return t.value; }
};

template<class T, class RootT>
class Property_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_value = has_value_member<T>;
	static_assert( has_value, "type T must have member T::value of a type corresponding to IDL type CHARACTER_STRING" );


public:
	Property_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.name );
	}
	const auto& get_value() { return t.value; }
	void set_value( decltype(T::value) val) { 
		t.value = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.value );
	}
};


} // namespace basic_test

#endif // _publishable_state_h_8057e425_guard
