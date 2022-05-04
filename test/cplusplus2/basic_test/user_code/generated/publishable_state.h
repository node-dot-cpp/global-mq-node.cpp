#ifndef _publishable_state_h_38a1e1f6_guard
#define _publishable_state_h_38a1e1f6_guard

#include <marshalling.h>
#include <publishable_impl.h>
#include <globalmq/marshalling2/marshalling2.h>
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
using GmqComposer = globalmq::marshalling2::GmqComposer2<Buffer>;
using GmqParser = globalmq::marshalling2::GmqParser2<Buffer>;
using JsonComposer = globalmq::marshalling2::JsonComposer2<Buffer>;
using JsonParser = globalmq::marshalling2::JsonParser2<Buffer>;
using IComposer2 = globalmq::marshalling2::IComposer2<Buffer>;
using IParser2 = globalmq::marshalling2::IParser2<Buffer>;
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


//===============================================================================
// Dependency ordering
// Property
// HtmlTextOrTags
// HtmlTag
//===============================================================================

namespace structures {

struct Property;
class HtmlTextOrTags;
struct HtmlTag;

struct Property
{
	GMQ_COLL string name;
	GMQ_COLL string value;
	bool operator==(const Property& other) const
	{
		return
			this->name == other.name &&
			this->value == other.value;
	}
	bool operator!=(const Property& other) const
	{
		return !this->operator==(other);
	}
};

class HtmlTextOrTags : public ::globalmq::marshalling::impl::DiscriminatedUnionType
{
public:
	enum Variants { text=21, taglists=22, unknown };
private:
	Variants v = Variants::unknown;
	struct Case_text
	{
		GMQ_COLL string str;
		bool operator==(const Case_text& other) const
		{
			return
				this->str == other.str;
		}
		bool operator!=(const Case_text& other) const
		{
			return !this->operator==(other);
		}
	};

	struct Case_taglists
	{
		GMQ_COLL vector<HtmlTag> tags;
		bool operator==(const Case_taglists& other) const
		{
			return
				this->tags == other.tags;
		}
		bool operator!=(const Case_taglists& other) const
		{
			return !this->operator==(other);
		}
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
				default: break;
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
	HtmlTextOrTags( const HtmlTextOrTags &other ) { implCopyFrom( other ); }
	HtmlTextOrTags& operator = ( const HtmlTextOrTags &other) { implCopyFrom( other ); return *this; }
	HtmlTextOrTags( HtmlTextOrTags&& other) noexcept { implMoveFrom( std::move( other ) ); }
	HtmlTextOrTags& operator = ( HtmlTextOrTags&& other) noexcept { implMoveFrom( std::move( other ) ); return *this; }
	virtual ~HtmlTextOrTags() { implDeinit(); }
	Variants currentVariant() const { return v; }
	void initAs( uint64_t v_ ) {
		implDeinit();
		switch ( v_ ) // init for a new type
		{
			case Variants::text: new ( text_taglists_mem ) Case_text; v = static_cast<Variants>(v_); break;
			case Variants::taglists: new ( text_taglists_mem ) Case_taglists; v = static_cast<Variants>(v_); break;
			default: v = Variants::unknown; break;
		}
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
	
	bool operator==(const HtmlTextOrTags& other) const
	{
		if(this->currentVariant() != other.currentVariant())
			return false;
		switch ( this->currentVariant() )
		{
			case Variants::text:
				return reinterpret_cast<const Case_text*>( text_taglists_mem )->operator==(*reinterpret_cast<const Case_text*>( other.text_taglists_mem ));
			case Variants::taglists:
				return reinterpret_cast<const Case_taglists*>( text_taglists_mem )->operator==(*reinterpret_cast<const Case_taglists*>( other.text_taglists_mem ));
			case Variants::unknown:
				return true;
			default:
				return false;
		}
		return false;
	}
	bool operator!=(const HtmlTextOrTags& other) const
	{
		return !this->operator==(other);
	}
};

struct HtmlTag
{
	GMQ_COLL string name;
	GMQ_COLL vector<Property> properties;
	HtmlTextOrTags tags;
	bool operator==(const HtmlTag& other) const
	{
		return
			this->name == other.name &&
			this->properties == other.properties &&
			this->tags == other.tags;
	}
	bool operator!=(const HtmlTag& other) const
	{
		return !this->operator==(other);
	}
};

namespace scope_test_exchange {
struct MESSAGE_cl_request
{
	int64_t ordinal = 0;
	GMQ_COLL string text_to_server;
	bool operator==(const MESSAGE_cl_request& other) const
	{
		return
			this->ordinal == other.ordinal &&
			this->text_to_server == other.text_to_server;
	}
	bool operator!=(const MESSAGE_cl_request& other) const
	{
		return !this->operator==(other);
	}
};
} // namespace scope_test_exchange

namespace scope_test_exchange {
struct MESSAGE_srv_response
{
	int64_t replied_on = 0;
	int64_t value = 0;
	GMQ_COLL string text_from_server;
	bool operator==(const MESSAGE_srv_response& other) const
	{
		return
			this->replied_on == other.replied_on &&
			this->value == other.value &&
			this->text_from_server == other.text_from_server;
	}
	bool operator!=(const MESSAGE_srv_response& other) const
	{
		return !this->operator==(other);
	}
};
} // namespace scope_test_exchange

struct publishable_sample
{
	int64_t id = 0;
	GMQ_COLL string text;
	HtmlTag html;
	bool operator==(const publishable_sample& other) const
	{
		return
			this->id == other.id &&
			this->text == other.text &&
			this->html == other.html;
	}
	bool operator!=(const publishable_sample& other) const
	{
		return !this->operator==(other);
	}
};


} // namespace structures

//===============================================================================

struct publishable_STRUCT_Property;
class Property_subscriber;
class Property_RefWrapper;
template<class RootT> class Property_RefWrapper4Set;

struct publishable_DISCRIMINATED_UNION_HtmlTextOrTags;
class HtmlTextOrTags_subscriber;
class HtmlTextOrTags_RefWrapper;
template<class RootT> class HtmlTextOrTags_RefWrapper4Set;

struct publishable_STRUCT_HtmlTag;
class HtmlTag_subscriber;
class HtmlTag_RefWrapper;
template<class RootT> class HtmlTag_RefWrapper4Set;


struct publishable_STRUCT_Property
{
	using CppType = structures::Property;
	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t );


	static inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void copy(const CppType& src, CppType& dst);

	static inline bool isSame(const CppType& s1, const CppType& s2);

};

class Property_subscriber
{
public:
	////////////////////////////// begin user override section //////////////////////////////
	virtual void notifyUpdated() {}
	virtual void notifyUpdated_name(GMQ_COLL string oldVal) {}
	virtual void notifyUpdated_value(GMQ_COLL string oldVal) {}
	//////////////////////////////  end user override section  //////////////////////////////

	using ThisType = Property_subscriber;
	using CppType = Property_subscriber;
	using BufferType = GMQueueStatePublisherSubscriberTypeInfo::BufferT;

private:
	GMQ_COLL string name;
	GMQ_COLL string value;


public:
	virtual ~Property_subscriber() {}
	Property_subscriber() {}

	const auto& get_name() const { return this->name; }
	const auto& get_value() const { return this->value; }

	static inline bool isSame(const ThisType& s1, const ThisType& s2);
	static inline bool isSame(const ThisType& s1, const structures::Property& s2);
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const GMQ_COLL unique_ptr<ThisType>& s2) { return isSame(*s1, *s2); }
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const structures::Property& s2) { return isSame(*s1, s2); }
	bool operator==(const ThisType& other) const { return isSame( *this, other ); }
	bool operator!=(const ThisType& other) const { return !this->operator==(other); }
	bool operator==(const structures::Property& other) const { return isSame( *this, other ); }
	bool operator!=(const structures::Property& other) const { return !this->operator==(other); }

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

};

struct publishable_DISCRIMINATED_UNION_HtmlTextOrTags
{
	using CppType = structures::HtmlTextOrTags;
	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t );


	static inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void copy(const CppType& src, CppType& dst);

	static inline bool isSame(const CppType& s1, const CppType& s2);

};

class HtmlTextOrTags_subscriber_base : public ::globalmq::marshalling::impl::DiscriminatedUnionType
{
public:
	enum Variants { text=21, taglists=22, unknown };
private:
	Variants v = Variants::unknown;
	struct Case_text
	{
		GMQ_COLL string str;
		Case_text() {}
		Case_text( const Case_text &other ) = delete;
		Case_text& operator = ( const Case_text &other) = delete;
		Case_text( Case_text&& other) noexcept = default;
		Case_text& operator = ( Case_text&& other) noexcept = default;
	};

	struct Case_taglists
	{
		GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> tags;
		Case_taglists() {}
		Case_taglists( const Case_taglists &other ) = delete;
		Case_taglists& operator = ( const Case_taglists &other) = delete;
		Case_taglists( Case_taglists&& other) noexcept = default;
		Case_taglists& operator = ( Case_taglists&& other) noexcept = default;
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
				default: break;
			}
			v = Variants::unknown;
		}
	}

	void implMoveFrom( HtmlTextOrTags_subscriber_base&& other ) {
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
	HtmlTextOrTags_subscriber_base() {}
	HtmlTextOrTags_subscriber_base( const HtmlTextOrTags_subscriber_base &other ) = delete;
	HtmlTextOrTags_subscriber_base& operator = ( const HtmlTextOrTags_subscriber_base &other) = delete;
	HtmlTextOrTags_subscriber_base( HtmlTextOrTags_subscriber_base&& other) noexcept { implMoveFrom( std::move( other ) ); }
	HtmlTextOrTags_subscriber_base& operator = ( HtmlTextOrTags_subscriber_base&& other) noexcept { implMoveFrom( std::move( other ) ); return *this; }
	virtual ~HtmlTextOrTags_subscriber_base() { implDeinit(); }
	Variants currentVariant() const { return v; }
	void initAs( uint64_t v_ ) {
		implDeinit();
		switch ( v_ ) // init for a new type
		{
			case Variants::text: new ( text_taglists_mem ) Case_text; v = static_cast<Variants>(v_); break;
			case Variants::taglists: new ( text_taglists_mem ) Case_taglists; v = static_cast<Variants>(v_); break;
			default: v = Variants::unknown; break;
		}
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

class HtmlTextOrTags_subscriber : private  HtmlTextOrTags_subscriber_base
{
public:
	////////////////////////////// begin user override section //////////////////////////////
	virtual GMQ_COLL unique_ptr<HtmlTag_subscriber> make_HtmlTag();
	virtual void notifyUpdated() {}
	virtual void notifyUpdated_currentVariant(Variants oldVal) {}
	// IDL CASE text
	virtual void notifyUpdated_str(GMQ_COLL string oldVal) {}
	// IDL CASE taglists
	virtual void notifyUpdated_tags() {}
	virtual void notifyElementUpdated_tags(size_t ix) {}
	virtual void notifyInserted_tags(size_t ix) {}
	virtual void notifyErased_tags(size_t ix, GMQ_COLL unique_ptr<HtmlTag_subscriber> oldVal) {}
	//////////////////////////////  end user override section  //////////////////////////////

	using ThisType = HtmlTextOrTags_subscriber;
	using CppType = HtmlTextOrTags_subscriber;
	using BufferType = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
	using BaseType = HtmlTextOrTags_subscriber_base;
	using HtmlTextOrTags_subscriber_base::Variants;

private:


public:
	virtual ~HtmlTextOrTags_subscriber() {}
	HtmlTextOrTags_subscriber() {}

Variants currentVariant() const { return BaseType::currentVariant(); }
// IDL CASE text
	const auto& get_str() const { return this->str(); }
// IDL CASE taglists
	const auto& get_tags() const { return this->tags(); }

	static inline bool isSame(const ThisType& s1, const ThisType& s2);
	static inline bool isSame(const ThisType& s1, const structures::HtmlTextOrTags& s2);
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const GMQ_COLL unique_ptr<ThisType>& s2) { return isSame(*s1, *s2); }
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const structures::HtmlTextOrTags& s2) { return isSame(*s1, s2); }
	bool operator==(const ThisType& other) const { return isSame( *this, other ); }
	bool operator!=(const ThisType& other) const { return !this->operator==(other); }
	bool operator==(const structures::HtmlTextOrTags& other) const { return isSame( *this, other ); }
	bool operator!=(const structures::HtmlTextOrTags& other) const { return !this->operator==(other); }

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

};

struct publishable_STRUCT_HtmlTag
{
	using CppType = structures::HtmlTag;
	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t );


	static inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void copy(const CppType& src, CppType& dst);

	static inline bool isSame(const CppType& s1, const CppType& s2);

};

class HtmlTag_subscriber
{
public:
	////////////////////////////// begin user override section //////////////////////////////
	virtual GMQ_COLL unique_ptr<Property_subscriber> make_Property();
	virtual GMQ_COLL unique_ptr<HtmlTextOrTags_subscriber> make_HtmlTextOrTags();
	virtual void notifyUpdated() {}
	virtual void notifyUpdated_name(GMQ_COLL string oldVal) {}
	virtual void notifyUpdated_properties() {}
	virtual void notifyElementUpdated_properties(size_t ix) {}
	virtual void notifyInserted_properties(size_t ix) {}
	virtual void notifyErased_properties(size_t ix, GMQ_COLL unique_ptr<Property_subscriber> oldVal) {}
	virtual void notifyUpdated_tags() {}
	//////////////////////////////  end user override section  //////////////////////////////

	using ThisType = HtmlTag_subscriber;
	using CppType = HtmlTag_subscriber;
	using BufferType = GMQueueStatePublisherSubscriberTypeInfo::BufferT;

private:
	GMQ_COLL string name;
	GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> properties;
	GMQ_COLL unique_ptr<HtmlTextOrTags_subscriber> tags;


public:
	virtual ~HtmlTag_subscriber() {}
	HtmlTag_subscriber() {}

	const auto& get_name() const { return this->name; }
	const auto& get_properties() const { return this->properties; }
	auto& lazy_tags()
	{
		if(!this->tags)
			this->tags = make_HtmlTextOrTags();
		return *(this->tags);
	}
	const auto& get_tags() const { const_cast<ThisType*>(this)->lazy_tags(); return this->tags; }

	static inline bool isSame(const ThisType& s1, const ThisType& s2);
	static inline bool isSame(const ThisType& s1, const structures::HtmlTag& s2);
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const GMQ_COLL unique_ptr<ThisType>& s2) { return isSame(*s1, *s2); }
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const structures::HtmlTag& s2) { return isSame(*s1, s2); }
	bool operator==(const ThisType& other) const { return isSame( *this, other ); }
	bool operator!=(const ThisType& other) const { return !this->operator==(other); }
	bool operator==(const structures::HtmlTag& other) const { return isSame( *this, other ); }
	bool operator!=(const structures::HtmlTag& other) const { return !this->operator==(other); }

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

};

struct publishable_PUBLISHABLE_publishable_sample
{
	using CppType = structures::publishable_sample;
	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t );


	static inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void copy(const CppType& src, CppType& dst);

	static inline bool isSame(const CppType& s1, const CppType& s2);

};

class publishable_sample_subscriber : public globalmq::marshalling::StateSubscriberBase<GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
public:
	////////////////////////////// begin user override section //////////////////////////////
	virtual GMQ_COLL unique_ptr<HtmlTag_subscriber> make_HtmlTag();
	virtual void notifyFullyUpdated() {}
	virtual void notifyUpdated() {}
	virtual void notifyUpdated_id(int64_t oldVal) {}
	virtual void notifyUpdated_text(GMQ_COLL string oldVal) {}
	virtual void notifyUpdated_html() {}
	//////////////////////////////  end user override section  //////////////////////////////

	using ThisType = publishable_sample_subscriber;
	using CppType = publishable_sample_subscriber;
	using BufferType = GMQueueStatePublisherSubscriberTypeInfo::BufferT;

private:
	globalmq::marshalling::SubscriberRegistryBase<BufferType>* const publishableRegistry = nullptr;
	int64_t id = 0;
	GMQ_COLL string text;
	GMQ_COLL unique_ptr<HtmlTag_subscriber> html;


public:
	virtual ~publishable_sample_subscriber() { if(publishableRegistry) publishableRegistry->removeSubscriber(this); }
	publishable_sample_subscriber() {}
	publishable_sample_subscriber(globalmq::marshalling::SubscriberRegistryBase<BufferType>* registry) : publishableRegistry(registry) { if(publishableRegistry) publishableRegistry->addSubscriber(this); }
	void subscribe(const GMQ_COLL string& path) { if(!publishableRegistry) throw std::exception(); publishableRegistry->pathSubscribe(this, path); }

	auto get_id() const { return this->id; }
	const auto& get_text() const { return this->text; }
	auto& lazy_html()
	{
		if(!this->html)
			this->html = make_HtmlTag();
		return *(this->html);
	}
	const auto& get_html() const { const_cast<ThisType*>(this)->lazy_html(); return this->html; }

	static inline bool isSame(const ThisType& s1, const ThisType& s2);
	static inline bool isSame(const ThisType& s1, const structures::publishable_sample& s2);
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const GMQ_COLL unique_ptr<ThisType>& s2) { return isSame(*s1, *s2); }
	static bool isSame(const GMQ_COLL unique_ptr<ThisType>& s1, const structures::publishable_sample& s2) { return isSame(*s1, s2); }
	bool operator==(const ThisType& other) const { return isSame( *this, other ); }
	bool operator!=(const ThisType& other) const { return !this->operator==(other); }
	bool operator==(const structures::publishable_sample& other) const { return isSame( *this, other ); }
	bool operator!=(const structures::publishable_sample& other) const { return !this->operator==(other); }

	//////////////////////////////
	virtual const char* publishableName() override { return "publishable_sample"; }
	virtual uint64_t stateTypeID() override { return 11; }

	// old interface
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferType>& parser ) override { throw std::exception(); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferType>& parser ) override { throw std::exception(); }
	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferType>& parser ) override { throw std::exception(); }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferType>& parser ) override { throw std::exception(); }

	// new interface
	virtual void publishableApplyUpdates( globalmq::marshalling2::ParserBase& parser ) override
	{
		if(auto ptr = dynamic_cast<globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>*>(&parser)) { applyMessageWithUpdates(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>*>(&parser)) { applyMessageWithUpdates(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>*>(&parser)) { applyMessageWithUpdates(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>*>(&parser)) { applyMessageWithUpdates(*ptr); return; }
		throw std::exception();
	}
	virtual void publishableApplyStateSync( globalmq::marshalling2::ParserBase& parser ) override
	{
		if(auto ptr = dynamic_cast<globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>*>(&parser)) { parseStateSyncMessage(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>*>(&parser)) { parseStateSyncMessage(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>*>(&parser)) { parseStateSyncMessage(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>*>(&parser)) { parseStateSyncMessage(*ptr); return; }
		throw std::exception();
	}
	////////////////////////////////

	template<class ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		parser.stateUpdateBegin();
		GMQ_COLL vector<uint64_t> addr;
		while( parser.changeBegin( addr ) )
		{
			GMQ_ASSERT( addr.size() );
			this->parse_continue(parser, addr, 0);
			addr.clear();
			parser.changeEnd();
		}
		parser.stateUpdateEnd();
	}

	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		this->parse_state_sync( parser );
		this->notifyFullyUpdated();
	}

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static void parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { tt.parse_state_sync(parser); }

	inline bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser );
	static bool parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt ) { return tt.parse_notify(parser); }

	inline bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset );
	static bool parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, ThisType& tt, GMQ_COLL vector<uint64_t>& addr, uint64_t offset ) { return tt.parse_continue(parser, addr, offset); }

	inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, ThisType& tt ) { tt.compose(composer); }

	inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer );
	static void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, ThisType& tt ) { tt.compose(composer); }

};

// template parameters are not really used but needed for legacy compatibility
template<class InputBufferT, class ComposerT>
class publishable_sample_concentrator final : private  publishable_sample_subscriber, public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
public:
	using ThisType = publishable_sample_concentrator;
	using CppType = ThisType;

public:
	publishable_sample_concentrator() {}

public:
	//////////////////////////////
	virtual const char* publishableName() override { return publishable_sample_subscriber::publishableName(); }

	// old interface
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<InputBufferT>& parser ) override { throw std::exception(); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<InputBufferT>& parser ) override { throw std::exception(); }
	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<InputBufferT>& parser ) override { throw std::exception(); }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<InputBufferT>& parser ) override { throw std::exception(); }
	virtual void generateStateSyncMessage( ComposerT& composer ) override { throw std::exception(); }

	// new interface
	virtual void publishableApplyUpdates( globalmq::marshalling2::ParserBase& parser ) override { publishable_sample_subscriber::publishableApplyUpdates(parser); }
	virtual void publishableApplyStateSync( globalmq::marshalling2::ParserBase& parser ) override { publishable_sample_subscriber::publishableApplyStateSync(parser); }
	virtual void publishableGenerateStateSync( globalmq::marshalling2::ComposerBase& composer ) override
	{
		if(auto ptr = dynamic_cast<globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>*>(&composer)) { compose(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>*>(&composer)) { compose(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>*>(&composer)) { compose(*ptr); return; }
		if(auto ptr = dynamic_cast<globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>*>(&composer)) { compose(*ptr); return; }
		throw std::exception();
	}
	//////////////////////////////
};

	/* static */
	inline
	void publishable_STRUCT_Property::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.value );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_STRUCT_Property::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.value );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_STRUCT_Property::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.value );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_STRUCT_Property::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.value );

		parser.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_Property::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.value );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_Property::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.value );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_Property::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.value );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_Property::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.value );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_Property::copy(const CppType& src, CppType& dst)
{
	globalmq::marshalling2::StringProcessor::copy( src.name, dst.name );
	globalmq::marshalling2::StringProcessor::copy( src.value, dst.value );
}

/* static */
inline
bool publishable_STRUCT_Property::isSame(const CppType& s1, const CppType& s2)
{
	if( ! globalmq::marshalling2::StringProcessor::isSame( s1.name, s2.name ) ) return false;
	if( ! globalmq::marshalling2::StringProcessor::isSame( s1.value, s2.value ) ) return false;
	return true;
}

/* static */
inline
bool Property_subscriber::isSame(const ThisType& s1, const ThisType& s2)
{
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_name(), s2.get_name() ) ) return false;
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_value(), s2.get_value() ) ) return false;
	return true;
}
/* static */
inline
bool Property_subscriber::isSame(const ThisType& s1, const structures::Property& s2)
{
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_name(), s2.name ) ) return false;
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_value(), s2.value ) ) return false;
	return true;
}
inline
void Property_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "value" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->value );

	parser.structEnd();
}

	inline
	bool Property_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "value" );
		{
			auto oldVal = this->value;
			this->value = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->value;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_value( oldVal );
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool Property_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->value;
					this->value = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->value;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_value( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void Property_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "value" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->value );

	parser.structEnd();
}

	inline
	bool Property_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "value" );
		{
			auto oldVal = this->value;
			this->value = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->value;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_value( oldVal );
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool Property_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->value;
					this->value = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->value;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_value( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void Property_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "value" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->value );

	parser.structEnd();
}

	inline
	bool Property_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "value" );
		{
			auto oldVal = this->value;
			this->value = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->value;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_value( oldVal );
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool Property_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->value;
					this->value = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->value;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_value( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void Property_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "value" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->value );

	parser.structEnd();
}

	inline
	bool Property_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "value" );
		{
			auto oldVal = this->value;
			this->value = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->value;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_value( oldVal );
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool Property_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->value;
					this->value = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->value;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_value( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	void Property_subscriber::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->value );

		composer.structEnd();
	}

	inline
	void Property_subscriber::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->value );

		composer.structEnd();
	}

	inline
	void Property_subscriber::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->value );

		composer.structEnd();
	}

	inline
	void Property_subscriber::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->value );

		composer.structEnd();
	}

class Property_RefWrapper
{
	using T = structures::Property;
	T& t;

public:
	Property_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_name() const { return t.name; }
	const auto& get_value() const { return t.value; }
};

template<class RootT>
class Property_RefWrapper4Set
{
	using T = structures::Property;
	T& t;
	RootT& root;
	GMQ_COLL vector<uint64_t> address;

public:
	Property_RefWrapper4Set( T& actual, RootT& root_, GMQ_COLL vector<uint64_t>&& address_ )
		: t( actual ), root( root_ ), address( std::move(address_) )
		{ }
	const auto& get_name() const { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		root.getComposer().changeBegin( address, 0 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		globalmq::marshalling2::StringProcessor::compose(root.getComposer(), t.name );
		root.getComposer().changeEnd();
	}
	const auto& get_value() const { return t.value; }
	void set_value( decltype(T::value) val) { 
		t.value = val; 
		root.getComposer().changeBegin( address, 1 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		globalmq::marshalling2::StringProcessor::compose(root.getComposer(), t.value );
		root.getComposer().changeEnd();
	}
};

	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		t.initAs( (typename structures::HtmlTextOrTags::Variants)(caseId) );
		parser.nextElement();
		if ( caseId != structures::HtmlTextOrTags::Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					parser.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					parser.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::parseForStateSyncOrMessageInDepth( parser, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		t.initAs( (typename structures::HtmlTextOrTags::Variants)(caseId) );
		parser.nextElement();
		if ( caseId != structures::HtmlTextOrTags::Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					parser.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					parser.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::parseForStateSyncOrMessageInDepth( parser, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		t.initAs( (typename structures::HtmlTextOrTags::Variants)(caseId) );
		parser.nextElement();
		if ( caseId != structures::HtmlTextOrTags::Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					parser.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					parser.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::parseForStateSyncOrMessageInDepth( parser, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		t.initAs( (typename structures::HtmlTextOrTags::Variants)(caseId) );
		parser.nextElement();
		if ( caseId != structures::HtmlTextOrTags::Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					parser.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					parser.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::parseForStateSyncOrMessageInDepth( parser, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
	}

	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		uint64_t caseId = t.currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != CppType::Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::compose(composer, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		uint64_t caseId = t.currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != CppType::Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::compose(composer, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		uint64_t caseId = t.currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != CppType::Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::compose(composer, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		uint64_t caseId = t.currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != CppType::Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, t.str() );
					break;
				}
				case 22: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::compose(composer, t.tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy(const CppType& src, CppType& dst)
{
	auto srcCaseId = src.currentVariant();
	dst.initAs( srcCaseId );
	switch ( srcCaseId )
	{
	case CppType::Variants::unknown:
		break;
	case CppType::Variants::text:
	{
		globalmq::marshalling2::StringProcessor::copy( src.str(), dst.str() );
		break;
	}
	case CppType::Variants::taglists:
	{
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::copy( src.tags(), dst.tags() );
		break;
	}
	default:
		throw std::exception(); // unexpected
	}
}

/* static */
inline
bool publishable_DISCRIMINATED_UNION_HtmlTextOrTags::isSame(const CppType& s1, const CppType& s2)
{
	if ( s1.currentVariant() != s2.currentVariant() )
		return false;
	switch ( s1.currentVariant() )
	{
	case CppType::Variants::unknown:
		break;
	case CppType::Variants::text:
	{
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.str(), s2.str() ) ) return false;
		break;
	}
	case CppType::Variants::taglists:
	{
		if( ! globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::isSame( s1.tags(), s2.tags() ) ) return false;
		break;
	}
	default:
		throw std::exception(); // unexpected
	}
	return true;
}

/* virtual */
inline
GMQ_COLL unique_ptr<HtmlTag_subscriber> HtmlTextOrTags_subscriber::make_HtmlTag() { return GMQ_COLL unique_ptr<HtmlTag_subscriber>{new HtmlTag_subscriber()}; }
/* static */
inline
bool HtmlTextOrTags_subscriber::isSame(const ThisType& s1, const ThisType& s2)
{
	if ( static_cast<uint64_t>(s1.currentVariant()) != static_cast<uint64_t>(s2.currentVariant()) )
		return false;
	switch ( s1.currentVariant() )
	{
	case Variants::unknown: break;
	case Variants::text: // IDL CASE text
	{
			if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_str(), s2.get_str() ) ) return false;
		break;
	}
	case Variants::taglists: // IDL CASE taglists
	{
			if( ! globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( s1.get_tags(), s2.get_tags() ) ) return false;
		break;
	}
	default:
		return false; // unexpected
	}
	return true;
}
/* static */
inline
bool HtmlTextOrTags_subscriber::isSame(const ThisType& s1, const structures::HtmlTextOrTags& s2)
{
	if ( static_cast<uint64_t>(s1.currentVariant()) != static_cast<uint64_t>(s2.currentVariant()) )
		return false;
	switch ( s1.currentVariant() )
	{
	case Variants::unknown: break;
	case Variants::text: // IDL CASE text
	{
			if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_str(), s2.str() ) ) return false;
		break;
	}
	case Variants::taglists: // IDL CASE taglists
	{
			if( ! globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( s1.get_tags(), s2.tags() ) ) return false;
		break;
	}
	default:
		return false; // unexpected
	}
	return true;
}
inline
void HtmlTextOrTags_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "caseId" );
	uint64_t caseId = parser.parseUnsignedInteger();
	this->initAs( caseId );
	parser.nextElement();
	if ( this->currentVariant() != Variants::unknown )
	{
		parser.namedParamBegin( "caseData" );
		parser.structBegin();
		switch ( this->currentVariant() )
		{
		case Variants::text:
		{
			parser.namedParamBegin( "str" );
			globalmq::marshalling2::StringProcessor::parse( parser, this->str() );
			break;
		}
		case Variants::taglists:
		{
			parser.namedParamBegin( "tags" );
			globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_state_sync( parser, this->tags(), [this]() { return this->make_HtmlTag(); } );
			break;
		}
		default:
			throw std::exception(); // unexpected
		}
		parser.structEnd();
	}

	parser.structEnd();
}

	inline
	bool HtmlTextOrTags_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		this->initAs( caseId );
		parser.nextElement();
		if ( this->currentVariant() != Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( this->currentVariant() )
			{
				case Variants::text:
				{
					parser.namedParamBegin( "str" );
					{
						auto oldVal = this->str();
						this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
						bool currentChanged = oldVal != this->str();
						if ( currentChanged )
						{
							changed = true;
							this->notifyUpdated_str( oldVal );
						}
					}
					break;
				}
				case Variants::taglists:
				{
					parser.namedParamBegin( "tags" );
					{
						GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
						globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
						bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
						if(currentChanged)
						{
							this->tags() = std::move(newVal);
							this->notifyUpdated_tags();
							changed = true;
						}
					}
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTextOrTags_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		if ( addr[offset] == 0 ) // changing current variant
		{
			if ( addr.size() > offset + 1 )
				throw std::exception(); // bad format, TODO: ...
			parser.nextElement();
			parser.leafeBegin();
			uint64_t newVal = parser.parseUnsignedInteger();
			auto oldVal = this->currentVariant();
			bool currentChanged = oldVal != newVal;
			if ( currentChanged )
			{
				this->initAs( newVal );
				changed = true;
				this->notifyUpdated_currentVariant( oldVal );
			}
		}
		else // updating particular members in particular cases
		{
			switch ( this->currentVariant() )
			{
				case Variants::text: // IDL CASE text
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() == offset + 1 )
							{
								parser.nextElement();
								parser.leafeBegin();
								auto oldVal = this->str();
								this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
								bool currentChanged = oldVal != this->str();
								if ( currentChanged )
								{
									changed = true;
									this->notifyUpdated_str( oldVal );
								}
							}
							else // deeper address not supported for this type
								throw std::exception(); // bad format, TODO: ...
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							bool currentChanged = false;
							if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								auto& value = this->tags()[pos];
								currentChanged = HtmlTag_subscriber::parse_continue( parser, *value, addr, offset + 2 );
								if ( currentChanged )
								{
									this->notifyElementUpdated_tags(pos);
								}
							}
							else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								parser.nextElement();
								uint64_t action = parser.parseAction();
								switch ( action )
								{
								case ActionOnVector::remove_at:
								{
									auto oldVal = std::move(this->tags()[pos]);
									this->tags().erase( this->tags().begin() + pos );
									this->notifyErased_tags( pos, std::move(oldVal) );
									currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto& value = this->tags()[pos];
									currentChanged = HtmlTag_subscriber::parse_notify( parser, *value );
									if ( currentChanged )
									{
										this->notifyElementUpdated_tags( pos );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto newVal = this->make_HtmlTag();
									HtmlTag_subscriber::parse_notify(parser, *newVal);
									this->tags().insert( this->tags().begin() + pos, std::move(newVal) );
									this->notifyInserted_tags( pos );
									currentChanged = true;
									break;
								}
								default:
									throw std::exception();
								}
							}
							else if ( addr.size() == offset + 1 ) // replacement of the whole vector
							{
								parser.nextElement();
								parser.leafeBegin();
								GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
								globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
								currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
								if(currentChanged)
									this->tags() = std::move(newVal);

							}
							else // unexpected addresss size
								throw std::exception();

							if ( currentChanged )
							{
								changed = true;
								this->notifyUpdated_tags();
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
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void HtmlTextOrTags_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "caseId" );
	uint64_t caseId = parser.parseUnsignedInteger();
	this->initAs( caseId );
	parser.nextElement();
	if ( this->currentVariant() != Variants::unknown )
	{
		parser.namedParamBegin( "caseData" );
		parser.structBegin();
		switch ( this->currentVariant() )
		{
		case Variants::text:
		{
			parser.namedParamBegin( "str" );
			globalmq::marshalling2::StringProcessor::parse( parser, this->str() );
			break;
		}
		case Variants::taglists:
		{
			parser.namedParamBegin( "tags" );
			globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_state_sync( parser, this->tags(), [this]() { return this->make_HtmlTag(); } );
			break;
		}
		default:
			throw std::exception(); // unexpected
		}
		parser.structEnd();
	}

	parser.structEnd();
}

	inline
	bool HtmlTextOrTags_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		this->initAs( caseId );
		parser.nextElement();
		if ( this->currentVariant() != Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( this->currentVariant() )
			{
				case Variants::text:
				{
					parser.namedParamBegin( "str" );
					{
						auto oldVal = this->str();
						this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
						bool currentChanged = oldVal != this->str();
						if ( currentChanged )
						{
							changed = true;
							this->notifyUpdated_str( oldVal );
						}
					}
					break;
				}
				case Variants::taglists:
				{
					parser.namedParamBegin( "tags" );
					{
						GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
						globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
						bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
						if(currentChanged)
						{
							this->tags() = std::move(newVal);
							this->notifyUpdated_tags();
							changed = true;
						}
					}
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTextOrTags_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		if ( addr[offset] == 0 ) // changing current variant
		{
			if ( addr.size() > offset + 1 )
				throw std::exception(); // bad format, TODO: ...
			parser.nextElement();
			parser.leafeBegin();
			uint64_t newVal = parser.parseUnsignedInteger();
			auto oldVal = this->currentVariant();
			bool currentChanged = oldVal != newVal;
			if ( currentChanged )
			{
				this->initAs( newVal );
				changed = true;
				this->notifyUpdated_currentVariant( oldVal );
			}
		}
		else // updating particular members in particular cases
		{
			switch ( this->currentVariant() )
			{
				case Variants::text: // IDL CASE text
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() == offset + 1 )
							{
								parser.nextElement();
								parser.leafeBegin();
								auto oldVal = this->str();
								this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
								bool currentChanged = oldVal != this->str();
								if ( currentChanged )
								{
									changed = true;
									this->notifyUpdated_str( oldVal );
								}
							}
							else // deeper address not supported for this type
								throw std::exception(); // bad format, TODO: ...
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							bool currentChanged = false;
							if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								auto& value = this->tags()[pos];
								currentChanged = HtmlTag_subscriber::parse_continue( parser, *value, addr, offset + 2 );
								if ( currentChanged )
								{
									this->notifyElementUpdated_tags(pos);
								}
							}
							else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								parser.nextElement();
								uint64_t action = parser.parseAction();
								switch ( action )
								{
								case ActionOnVector::remove_at:
								{
									auto oldVal = std::move(this->tags()[pos]);
									this->tags().erase( this->tags().begin() + pos );
									this->notifyErased_tags( pos, std::move(oldVal) );
									currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto& value = this->tags()[pos];
									currentChanged = HtmlTag_subscriber::parse_notify( parser, *value );
									if ( currentChanged )
									{
										this->notifyElementUpdated_tags( pos );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto newVal = this->make_HtmlTag();
									HtmlTag_subscriber::parse_notify(parser, *newVal);
									this->tags().insert( this->tags().begin() + pos, std::move(newVal) );
									this->notifyInserted_tags( pos );
									currentChanged = true;
									break;
								}
								default:
									throw std::exception();
								}
							}
							else if ( addr.size() == offset + 1 ) // replacement of the whole vector
							{
								parser.nextElement();
								parser.leafeBegin();
								GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
								globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
								currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
								if(currentChanged)
									this->tags() = std::move(newVal);

							}
							else // unexpected addresss size
								throw std::exception();

							if ( currentChanged )
							{
								changed = true;
								this->notifyUpdated_tags();
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
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void HtmlTextOrTags_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "caseId" );
	uint64_t caseId = parser.parseUnsignedInteger();
	this->initAs( caseId );
	parser.nextElement();
	if ( this->currentVariant() != Variants::unknown )
	{
		parser.namedParamBegin( "caseData" );
		parser.structBegin();
		switch ( this->currentVariant() )
		{
		case Variants::text:
		{
			parser.namedParamBegin( "str" );
			globalmq::marshalling2::StringProcessor::parse( parser, this->str() );
			break;
		}
		case Variants::taglists:
		{
			parser.namedParamBegin( "tags" );
			globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_state_sync( parser, this->tags(), [this]() { return this->make_HtmlTag(); } );
			break;
		}
		default:
			throw std::exception(); // unexpected
		}
		parser.structEnd();
	}

	parser.structEnd();
}

	inline
	bool HtmlTextOrTags_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		this->initAs( caseId );
		parser.nextElement();
		if ( this->currentVariant() != Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( this->currentVariant() )
			{
				case Variants::text:
				{
					parser.namedParamBegin( "str" );
					{
						auto oldVal = this->str();
						this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
						bool currentChanged = oldVal != this->str();
						if ( currentChanged )
						{
							changed = true;
							this->notifyUpdated_str( oldVal );
						}
					}
					break;
				}
				case Variants::taglists:
				{
					parser.namedParamBegin( "tags" );
					{
						GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
						globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
						bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
						if(currentChanged)
						{
							this->tags() = std::move(newVal);
							this->notifyUpdated_tags();
							changed = true;
						}
					}
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTextOrTags_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		if ( addr[offset] == 0 ) // changing current variant
		{
			if ( addr.size() > offset + 1 )
				throw std::exception(); // bad format, TODO: ...
			parser.nextElement();
			parser.leafeBegin();
			uint64_t newVal = parser.parseUnsignedInteger();
			auto oldVal = this->currentVariant();
			bool currentChanged = oldVal != newVal;
			if ( currentChanged )
			{
				this->initAs( newVal );
				changed = true;
				this->notifyUpdated_currentVariant( oldVal );
			}
		}
		else // updating particular members in particular cases
		{
			switch ( this->currentVariant() )
			{
				case Variants::text: // IDL CASE text
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() == offset + 1 )
							{
								parser.nextElement();
								parser.leafeBegin();
								auto oldVal = this->str();
								this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
								bool currentChanged = oldVal != this->str();
								if ( currentChanged )
								{
									changed = true;
									this->notifyUpdated_str( oldVal );
								}
							}
							else // deeper address not supported for this type
								throw std::exception(); // bad format, TODO: ...
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							bool currentChanged = false;
							if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								auto& value = this->tags()[pos];
								currentChanged = HtmlTag_subscriber::parse_continue( parser, *value, addr, offset + 2 );
								if ( currentChanged )
								{
									this->notifyElementUpdated_tags(pos);
								}
							}
							else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								parser.nextElement();
								uint64_t action = parser.parseAction();
								switch ( action )
								{
								case ActionOnVector::remove_at:
								{
									auto oldVal = std::move(this->tags()[pos]);
									this->tags().erase( this->tags().begin() + pos );
									this->notifyErased_tags( pos, std::move(oldVal) );
									currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto& value = this->tags()[pos];
									currentChanged = HtmlTag_subscriber::parse_notify( parser, *value );
									if ( currentChanged )
									{
										this->notifyElementUpdated_tags( pos );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto newVal = this->make_HtmlTag();
									HtmlTag_subscriber::parse_notify(parser, *newVal);
									this->tags().insert( this->tags().begin() + pos, std::move(newVal) );
									this->notifyInserted_tags( pos );
									currentChanged = true;
									break;
								}
								default:
									throw std::exception();
								}
							}
							else if ( addr.size() == offset + 1 ) // replacement of the whole vector
							{
								parser.nextElement();
								parser.leafeBegin();
								GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
								globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
								currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
								if(currentChanged)
									this->tags() = std::move(newVal);

							}
							else // unexpected addresss size
								throw std::exception();

							if ( currentChanged )
							{
								changed = true;
								this->notifyUpdated_tags();
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
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void HtmlTextOrTags_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "caseId" );
	uint64_t caseId = parser.parseUnsignedInteger();
	this->initAs( caseId );
	parser.nextElement();
	if ( this->currentVariant() != Variants::unknown )
	{
		parser.namedParamBegin( "caseData" );
		parser.structBegin();
		switch ( this->currentVariant() )
		{
		case Variants::text:
		{
			parser.namedParamBegin( "str" );
			globalmq::marshalling2::StringProcessor::parse( parser, this->str() );
			break;
		}
		case Variants::taglists:
		{
			parser.namedParamBegin( "tags" );
			globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_state_sync( parser, this->tags(), [this]() { return this->make_HtmlTag(); } );
			break;
		}
		default:
			throw std::exception(); // unexpected
		}
		parser.structEnd();
	}

	parser.structEnd();
}

	inline
	bool HtmlTextOrTags_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "caseId" );
		uint64_t caseId = parser.parseUnsignedInteger();
		this->initAs( caseId );
		parser.nextElement();
		if ( this->currentVariant() != Variants::unknown )
		{
			parser.namedParamBegin( "caseData" );
			parser.structBegin();
			switch ( this->currentVariant() )
			{
				case Variants::text:
				{
					parser.namedParamBegin( "str" );
					{
						auto oldVal = this->str();
						this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
						bool currentChanged = oldVal != this->str();
						if ( currentChanged )
						{
							changed = true;
							this->notifyUpdated_str( oldVal );
						}
					}
					break;
				}
				case Variants::taglists:
				{
					parser.namedParamBegin( "tags" );
					{
						GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
						globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
						bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
						if(currentChanged)
						{
							this->tags() = std::move(newVal);
							this->notifyUpdated_tags();
							changed = true;
						}
					}
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			parser.structEnd();
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTextOrTags_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		if ( addr[offset] == 0 ) // changing current variant
		{
			if ( addr.size() > offset + 1 )
				throw std::exception(); // bad format, TODO: ...
			parser.nextElement();
			parser.leafeBegin();
			uint64_t newVal = parser.parseUnsignedInteger();
			auto oldVal = this->currentVariant();
			bool currentChanged = oldVal != newVal;
			if ( currentChanged )
			{
				this->initAs( newVal );
				changed = true;
				this->notifyUpdated_currentVariant( oldVal );
			}
		}
		else // updating particular members in particular cases
		{
			switch ( this->currentVariant() )
			{
				case Variants::text: // IDL CASE text
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() == offset + 1 )
							{
								parser.nextElement();
								parser.leafeBegin();
								auto oldVal = this->str();
								this->str() = globalmq::marshalling2::StringProcessor::parse(parser);
								bool currentChanged = oldVal != this->str();
								if ( currentChanged )
								{
									changed = true;
									this->notifyUpdated_str( oldVal );
								}
							}
							else // deeper address not supported for this type
								throw std::exception(); // bad format, TODO: ...
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							bool currentChanged = false;
							if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								auto& value = this->tags()[pos];
								currentChanged = HtmlTag_subscriber::parse_continue( parser, *value, addr, offset + 2 );
								if ( currentChanged )
								{
									this->notifyElementUpdated_tags(pos);
								}
							}
							else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
							{
								size_t pos = addr[offset + 1];
								if ( pos > this->tags().size() )
									throw std::exception();
								parser.nextElement();
								uint64_t action = parser.parseAction();
								switch ( action )
								{
								case ActionOnVector::remove_at:
								{
									auto oldVal = std::move(this->tags()[pos]);
									this->tags().erase( this->tags().begin() + pos );
									this->notifyErased_tags( pos, std::move(oldVal) );
									currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto& value = this->tags()[pos];
									currentChanged = HtmlTag_subscriber::parse_notify( parser, *value );
									if ( currentChanged )
									{
										this->notifyElementUpdated_tags( pos );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									parser.nextElement();
									parser.leafeBegin();
									auto newVal = this->make_HtmlTag();
									HtmlTag_subscriber::parse_notify(parser, *newVal);
									this->tags().insert( this->tags().begin() + pos, std::move(newVal) );
									this->notifyInserted_tags( pos );
									currentChanged = true;
									break;
								}
								default:
									throw std::exception();
								}
							}
							else if ( addr.size() == offset + 1 ) // replacement of the whole vector
							{
								parser.nextElement();
								parser.leafeBegin();
								GMQ_COLL vector<GMQ_COLL unique_ptr<HtmlTag_subscriber>> newVal;
								globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_HtmlTag(); } );
								currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::isSame( newVal, this->tags() );
								if(currentChanged)
									this->tags() = std::move(newVal);

							}
							else // unexpected addresss size
								throw std::exception();

							if ( currentChanged )
							{
								changed = true;
								this->notifyUpdated_tags();
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
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	void HtmlTextOrTags_subscriber::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		Variants caseId = this->currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case Variants::text: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, this->str() );
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::compose(composer, this->tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

	inline
	void HtmlTextOrTags_subscriber::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		Variants caseId = this->currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case Variants::text: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, this->str() );
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::compose(composer, this->tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

	inline
	void HtmlTextOrTags_subscriber::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		Variants caseId = this->currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case Variants::text: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, this->str() );
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::compose(composer, this->tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

	inline
	void HtmlTextOrTags_subscriber::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		Variants caseId = this->currentVariant();
		composer.namedParamBegin( "caseId" );
		composer.composeUnsignedInteger( caseId );

		composer.nextElement();

		if ( caseId != Variants::unknown )
		{
			composer.namedParamBegin( "caseData" );
			composer.structBegin();
			switch ( caseId )
			{
				case Variants::text: // IDL CASE text
				{
					composer.namedParamBegin( "str" );
					globalmq::marshalling2::StringProcessor::compose(composer, this->str() );
					break;
				}
				case Variants::taglists: // IDL CASE taglists
				{
					composer.namedParamBegin( "tags" );
					globalmq::marshalling2::PtrVectorProcessor2<HtmlTag_subscriber>::compose(composer, this->tags() );
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			composer.structEnd();
		}

		composer.structEnd();
	}

class HtmlTextOrTags_RefWrapper
{
	using T = structures::HtmlTextOrTags;
	T& t;

public:
	HtmlTextOrTags_RefWrapper( T& actual ) : t( actual ) {}
	auto get_currentVariant() { return t.currentVariant(); }
	const auto& get_str() const { return t.str(); }
	auto get_tags() const { return globalmq::marshalling2::VectorOfStructRefWrapper<publishable_STRUCT_HtmlTag, HtmlTag_RefWrapper>(t.tags()); }
};

template<class RootT>
class HtmlTextOrTags_RefWrapper4Set
{
	using T = structures::HtmlTextOrTags;
	T& t;
	RootT& root;
	GMQ_COLL vector<uint64_t> address;

public:
	HtmlTextOrTags_RefWrapper4Set( T& actual, RootT& root_, GMQ_COLL vector<uint64_t>&& address_ )
		: t( actual ), root( root_ ), address( std::move(address_) )
		{ }
	auto get_currentVariant() { return t.currentVariant(); }
	void set_currentVariant( typename T::Variants v ) { 
		t.initAs( v ); 
		root.getComposer().changeBegin( address, 0 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		root.getComposer().composeUnsignedInteger((uint64_t)(t.currentVariant()));
		root.getComposer().changeEnd();
	}
	const auto& get_str() const { return t.str(); }
	void set_str( typename T::Case_text_str_T val) { 
		t.str() = val; 
		root.getComposer().changeBegin( address, 1 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		globalmq::marshalling2::StringProcessor::compose(root.getComposer(), t.str() );
		root.getComposer().changeEnd();
	}
	auto get_tags() const { return globalmq::marshalling2::VectorOfStructRefWrapper<publishable_STRUCT_HtmlTag, HtmlTag_RefWrapper>(t.tags()); }
	void set_tags( typename T::Case_taglists_tags_T val) { 
		t.tags() = val; 
		root.getComposer().changeBegin( address, 1 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_HtmlTag>::compose(root.getComposer(), t.tags() );
		root.getComposer().changeEnd();
	}
	auto get4set_tags() { return globalmq::marshalling2::VectorOfStructRefWrapper4Set<publishable_STRUCT_HtmlTag, RootT, HtmlTag_RefWrapper4Set<RootT>>(t.tags(), root, globalmq::marshalling2::makeAddress(address, 1)); }
};

	/* static */
	inline
	void publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::parseForStateSyncOrMessageInDepth( parser, t.properties );
		parser.nextElement();

		parser.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( parser, t.tags );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::parseForStateSyncOrMessageInDepth( parser, t.properties );
		parser.nextElement();

		parser.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( parser, t.tags );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::parseForStateSyncOrMessageInDepth( parser, t.properties );
		parser.nextElement();

		parser.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( parser, t.tags );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.name );
		parser.nextElement();

		parser.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::parseForStateSyncOrMessageInDepth( parser, t.properties );
		parser.nextElement();

		parser.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( parser, t.tags );

		parser.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_HtmlTag::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::compose(composer, t.properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose(composer, t.tags );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_HtmlTag::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::compose(composer, t.properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose(composer, t.tags );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_HtmlTag::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::compose(composer, t.properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose(composer, t.tags );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_HtmlTag::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::compose(composer, t.properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose(composer, t.tags );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_STRUCT_HtmlTag::copy(const CppType& src, CppType& dst)
{
	globalmq::marshalling2::StringProcessor::copy( src.name, dst.name );
	globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::copy( src.properties, dst.properties );
	publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy( src.tags, dst.tags );
}

/* static */
inline
bool publishable_STRUCT_HtmlTag::isSame(const CppType& s1, const CppType& s2)
{
	if( ! globalmq::marshalling2::StringProcessor::isSame( s1.name, s2.name ) ) return false;
	if( ! globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::isSame( s1.properties, s2.properties ) ) return false;
	if( ! publishable_DISCRIMINATED_UNION_HtmlTextOrTags::isSame( s1.tags, s2.tags ) ) return false;
	return true;
}

/* virtual */
inline
GMQ_COLL unique_ptr<Property_subscriber> HtmlTag_subscriber::make_Property() { return GMQ_COLL unique_ptr<Property_subscriber>{new Property_subscriber()}; }
/* virtual */
inline
GMQ_COLL unique_ptr<HtmlTextOrTags_subscriber> HtmlTag_subscriber::make_HtmlTextOrTags() { return GMQ_COLL unique_ptr<HtmlTextOrTags_subscriber>{new HtmlTextOrTags_subscriber()}; }
/* static */
inline
bool HtmlTag_subscriber::isSame(const ThisType& s1, const ThisType& s2)
{
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_name(), s2.get_name() ) ) return false;
		if( ! globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( s1.get_properties(), s2.get_properties() ) ) return false;
		if( ! HtmlTextOrTags_subscriber::isSame( s1.get_tags(), s2.get_tags() ) ) return false;
	return true;
}
/* static */
inline
bool HtmlTag_subscriber::isSame(const ThisType& s1, const structures::HtmlTag& s2)
{
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_name(), s2.name ) ) return false;
		if( ! globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( s1.get_properties(), s2.properties ) ) return false;
		if( ! HtmlTextOrTags_subscriber::isSame( s1.get_tags(), s2.tags ) ) return false;
	return true;
}
inline
void HtmlTag_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "properties" );
	globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_state_sync( parser, this->properties, [this]() { return this->make_Property(); } );
	parser.nextElement();

	parser.namedParamBegin( "tags" );
	HtmlTextOrTags_subscriber::parse_state_sync( parser, this->lazy_tags() );

	parser.structEnd();
}

	inline
	bool HtmlTag_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "properties" );
		{
			GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
			globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
			bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
			if(currentChanged)
			{
				this->properties = std::move(newVal);
				this->notifyUpdated_properties();
				changed = true;
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "tags" );
		{
			bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_tags();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTag_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				bool currentChanged = false;
				if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					auto& value = this->properties[pos];
					currentChanged = Property_subscriber::parse_continue( parser, *value, addr, offset + 2 );
					if ( currentChanged )
					{
						this->notifyElementUpdated_properties(pos);
					}
				}
				else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					parser.nextElement();
					uint64_t action = parser.parseAction();
					switch ( action )
					{
					case ActionOnVector::remove_at:
					{
						auto oldVal = std::move(this->properties[pos]);
						this->properties.erase( this->properties.begin() + pos );
						this->notifyErased_properties( pos, std::move(oldVal) );
						currentChanged = true;
						break;
					}
					case ActionOnVector::update_at:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto& value = this->properties[pos];
						currentChanged = Property_subscriber::parse_notify( parser, *value );
						if ( currentChanged )
						{
							this->notifyElementUpdated_properties( pos );
						}
						break;
					}
					case ActionOnVector::insert_single_before:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto newVal = this->make_Property();
						Property_subscriber::parse_notify(parser, *newVal);
						this->properties.insert( this->properties.begin() + pos, std::move(newVal) );
						this->notifyInserted_properties( pos );
						currentChanged = true;
						break;
					}
					default:
						throw std::exception();
					}
				}
				else if ( addr.size() == offset + 1 ) // replacement of the whole vector
				{
					parser.nextElement();
					parser.leafeBegin();
					GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
					globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
					currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
					if(currentChanged)
						this->properties = std::move(newVal);

				}
				else // unexpected addresss size
					throw std::exception();

				if ( currentChanged )
				{
					changed = true;
					this->notifyUpdated_properties();
				}
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_continue( parser, this->lazy_tags(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void HtmlTag_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "properties" );
	globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_state_sync( parser, this->properties, [this]() { return this->make_Property(); } );
	parser.nextElement();

	parser.namedParamBegin( "tags" );
	HtmlTextOrTags_subscriber::parse_state_sync( parser, this->lazy_tags() );

	parser.structEnd();
}

	inline
	bool HtmlTag_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "properties" );
		{
			GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
			globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
			bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
			if(currentChanged)
			{
				this->properties = std::move(newVal);
				this->notifyUpdated_properties();
				changed = true;
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "tags" );
		{
			bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_tags();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTag_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				bool currentChanged = false;
				if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					auto& value = this->properties[pos];
					currentChanged = Property_subscriber::parse_continue( parser, *value, addr, offset + 2 );
					if ( currentChanged )
					{
						this->notifyElementUpdated_properties(pos);
					}
				}
				else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					parser.nextElement();
					uint64_t action = parser.parseAction();
					switch ( action )
					{
					case ActionOnVector::remove_at:
					{
						auto oldVal = std::move(this->properties[pos]);
						this->properties.erase( this->properties.begin() + pos );
						this->notifyErased_properties( pos, std::move(oldVal) );
						currentChanged = true;
						break;
					}
					case ActionOnVector::update_at:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto& value = this->properties[pos];
						currentChanged = Property_subscriber::parse_notify( parser, *value );
						if ( currentChanged )
						{
							this->notifyElementUpdated_properties( pos );
						}
						break;
					}
					case ActionOnVector::insert_single_before:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto newVal = this->make_Property();
						Property_subscriber::parse_notify(parser, *newVal);
						this->properties.insert( this->properties.begin() + pos, std::move(newVal) );
						this->notifyInserted_properties( pos );
						currentChanged = true;
						break;
					}
					default:
						throw std::exception();
					}
				}
				else if ( addr.size() == offset + 1 ) // replacement of the whole vector
				{
					parser.nextElement();
					parser.leafeBegin();
					GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
					globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
					currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
					if(currentChanged)
						this->properties = std::move(newVal);

				}
				else // unexpected addresss size
					throw std::exception();

				if ( currentChanged )
				{
					changed = true;
					this->notifyUpdated_properties();
				}
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_continue( parser, this->lazy_tags(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void HtmlTag_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "properties" );
	globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_state_sync( parser, this->properties, [this]() { return this->make_Property(); } );
	parser.nextElement();

	parser.namedParamBegin( "tags" );
	HtmlTextOrTags_subscriber::parse_state_sync( parser, this->lazy_tags() );

	parser.structEnd();
}

	inline
	bool HtmlTag_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "properties" );
		{
			GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
			globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
			bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
			if(currentChanged)
			{
				this->properties = std::move(newVal);
				this->notifyUpdated_properties();
				changed = true;
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "tags" );
		{
			bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_tags();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTag_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				bool currentChanged = false;
				if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					auto& value = this->properties[pos];
					currentChanged = Property_subscriber::parse_continue( parser, *value, addr, offset + 2 );
					if ( currentChanged )
					{
						this->notifyElementUpdated_properties(pos);
					}
				}
				else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					parser.nextElement();
					uint64_t action = parser.parseAction();
					switch ( action )
					{
					case ActionOnVector::remove_at:
					{
						auto oldVal = std::move(this->properties[pos]);
						this->properties.erase( this->properties.begin() + pos );
						this->notifyErased_properties( pos, std::move(oldVal) );
						currentChanged = true;
						break;
					}
					case ActionOnVector::update_at:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto& value = this->properties[pos];
						currentChanged = Property_subscriber::parse_notify( parser, *value );
						if ( currentChanged )
						{
							this->notifyElementUpdated_properties( pos );
						}
						break;
					}
					case ActionOnVector::insert_single_before:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto newVal = this->make_Property();
						Property_subscriber::parse_notify(parser, *newVal);
						this->properties.insert( this->properties.begin() + pos, std::move(newVal) );
						this->notifyInserted_properties( pos );
						currentChanged = true;
						break;
					}
					default:
						throw std::exception();
					}
				}
				else if ( addr.size() == offset + 1 ) // replacement of the whole vector
				{
					parser.nextElement();
					parser.leafeBegin();
					GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
					globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
					currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
					if(currentChanged)
						this->properties = std::move(newVal);

				}
				else // unexpected addresss size
					throw std::exception();

				if ( currentChanged )
				{
					changed = true;
					this->notifyUpdated_properties();
				}
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_continue( parser, this->lazy_tags(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void HtmlTag_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "name" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->name );
	parser.nextElement();

	parser.namedParamBegin( "properties" );
	globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_state_sync( parser, this->properties, [this]() { return this->make_Property(); } );
	parser.nextElement();

	parser.namedParamBegin( "tags" );
	HtmlTextOrTags_subscriber::parse_state_sync( parser, this->lazy_tags() );

	parser.structEnd();
}

	inline
	bool HtmlTag_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "name" );
		{
			auto oldVal = this->name;
			this->name = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->name;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_name( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "properties" );
		{
			GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
			globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
			bool currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
			if(currentChanged)
			{
				this->properties = std::move(newVal);
				this->notifyUpdated_properties();
				changed = true;
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "tags" );
		{
			bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_tags();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool HtmlTag_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->name;
					this->name = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->name;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_name( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				bool currentChanged = false;
				if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					auto& value = this->properties[pos];
					currentChanged = Property_subscriber::parse_continue( parser, *value, addr, offset + 2 );
					if ( currentChanged )
					{
						this->notifyElementUpdated_properties(pos);
					}
				}
				else if ( addr.size() == offset + 2 ) // update of one or more elelments as a whole
				{
					size_t pos = addr[offset + 1];
					if ( pos > this->properties.size() )
						throw std::exception();
					parser.nextElement();
					uint64_t action = parser.parseAction();
					switch ( action )
					{
					case ActionOnVector::remove_at:
					{
						auto oldVal = std::move(this->properties[pos]);
						this->properties.erase( this->properties.begin() + pos );
						this->notifyErased_properties( pos, std::move(oldVal) );
						currentChanged = true;
						break;
					}
					case ActionOnVector::update_at:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto& value = this->properties[pos];
						currentChanged = Property_subscriber::parse_notify( parser, *value );
						if ( currentChanged )
						{
							this->notifyElementUpdated_properties( pos );
						}
						break;
					}
					case ActionOnVector::insert_single_before:
					{
						parser.nextElement();
						parser.leafeBegin();
						auto newVal = this->make_Property();
						Property_subscriber::parse_notify(parser, *newVal);
						this->properties.insert( this->properties.begin() + pos, std::move(newVal) );
						this->notifyInserted_properties( pos );
						currentChanged = true;
						break;
					}
					default:
						throw std::exception();
					}
				}
				else if ( addr.size() == offset + 1 ) // replacement of the whole vector
				{
					parser.nextElement();
					parser.leafeBegin();
					GMQ_COLL vector<GMQ_COLL unique_ptr<Property_subscriber>> newVal;
					globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::parse_notify( parser, newVal, [this]() { return this->make_Property(); } );
					currentChanged = !globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::isSame( newVal, this->properties );
					if(currentChanged)
						this->properties = std::move(newVal);

				}
				else // unexpected addresss size
					throw std::exception();

				if ( currentChanged )
				{
					changed = true;
					this->notifyUpdated_properties();
				}
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_notify( parser, this->lazy_tags() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTextOrTags_subscriber::parse_continue( parser, this->lazy_tags(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_tags();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	void HtmlTag_subscriber::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::compose(composer, this->properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		HtmlTextOrTags_subscriber::compose(composer, this->lazy_tags() );

		composer.structEnd();
	}

	inline
	void HtmlTag_subscriber::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::compose(composer, this->properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		HtmlTextOrTags_subscriber::compose(composer, this->lazy_tags() );

		composer.structEnd();
	}

	inline
	void HtmlTag_subscriber::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::compose(composer, this->properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		HtmlTextOrTags_subscriber::compose(composer, this->lazy_tags() );

		composer.structEnd();
	}

	inline
	void HtmlTag_subscriber::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "name" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->name );

		composer.nextElement();

		composer.namedParamBegin( "properties" );
		globalmq::marshalling2::PtrVectorProcessor2<Property_subscriber>::compose(composer, this->properties );

		composer.nextElement();

		composer.namedParamBegin( "tags" );
		HtmlTextOrTags_subscriber::compose(composer, this->lazy_tags() );

		composer.structEnd();
	}

class HtmlTag_RefWrapper
{
	using T = structures::HtmlTag;
	T& t;

public:
	HtmlTag_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_name() const { return t.name; }
	auto get_properties() const { return globalmq::marshalling2::VectorOfStructRefWrapper<publishable_STRUCT_Property, Property_RefWrapper>(t.properties); }
	const auto& get_tags() const { return t.tags; }
};

template<class RootT>
class HtmlTag_RefWrapper4Set
{
	using T = structures::HtmlTag;
	T& t;
	RootT& root;
	GMQ_COLL vector<uint64_t> address;

public:
	HtmlTag_RefWrapper4Set( T& actual, RootT& root_, GMQ_COLL vector<uint64_t>&& address_ )
		: t( actual ), root( root_ ), address( std::move(address_) )
		{ }
	const auto& get_name() const { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		root.getComposer().changeBegin( address, 0 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		globalmq::marshalling2::StringProcessor::compose(root.getComposer(), t.name );
		root.getComposer().changeEnd();
	}
	auto get_properties() const { return globalmq::marshalling2::VectorOfStructRefWrapper<publishable_STRUCT_Property, Property_RefWrapper>(t.properties); }
	void set_properties( decltype(T::properties) val) { 
		t.properties = val; 
		root.getComposer().changeBegin( address, 1 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		globalmq::marshalling2::PublishableVectorProcessor2<publishable_STRUCT_Property>::compose(root.getComposer(), t.properties );
		root.getComposer().changeEnd();
	}
	auto get4set_properties() { return globalmq::marshalling2::VectorOfStructRefWrapper4Set<publishable_STRUCT_Property, RootT, Property_RefWrapper4Set<RootT>>(t.properties, root, globalmq::marshalling2::makeAddress(address, 1)); }
	const auto& get_tags() const { return t.tags; }
	void set_tags( decltype(T::tags) val) { 
		t.tags = val; 
		root.getComposer().changeBegin( address, 2 );
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose(root.getComposer(), t.tags );
		root.getComposer().changeEnd();
	}
	auto get4set_tags() { return HtmlTextOrTags_RefWrapper4Set<RootT>(t.tags, root, globalmq::marshalling2::makeAddress(address, 2)); }
};

	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.id );
		parser.nextElement();

		parser.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text );
		parser.nextElement();

		parser.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( parser, t.html );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.id );
		parser.nextElement();

		parser.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text );
		parser.nextElement();

		parser.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( parser, t.html );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.id );
		parser.nextElement();

		parser.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text );
		parser.nextElement();

		parser.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( parser, t.html );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.id );
		parser.nextElement();

		parser.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text );
		parser.nextElement();

		parser.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::parseForStateSyncOrMessageInDepth( parser, t.html );

		parser.structEnd();
	}

	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::compose(composer, t.html );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::compose(composer, t.html );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::compose(composer, t.html );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		publishable_STRUCT_HtmlTag::compose(composer, t.html );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_PUBLISHABLE_publishable_sample::copy(const CppType& src, CppType& dst)
{
	globalmq::marshalling2::Int64Processor::copy( src.id, dst.id );
	globalmq::marshalling2::StringProcessor::copy( src.text, dst.text );
	publishable_STRUCT_HtmlTag::copy( src.html, dst.html );
}

/* static */
inline
bool publishable_PUBLISHABLE_publishable_sample::isSame(const CppType& s1, const CppType& s2)
{
	if( ! globalmq::marshalling2::Int64Processor::isSame( s1.id, s2.id ) ) return false;
	if( ! globalmq::marshalling2::StringProcessor::isSame( s1.text, s2.text ) ) return false;
	if( ! publishable_STRUCT_HtmlTag::isSame( s1.html, s2.html ) ) return false;
	return true;
}

/* virtual */
inline
GMQ_COLL unique_ptr<HtmlTag_subscriber> publishable_sample_subscriber::make_HtmlTag() { return GMQ_COLL unique_ptr<HtmlTag_subscriber>{new HtmlTag_subscriber()}; }
/* static */
inline
bool publishable_sample_subscriber::isSame(const ThisType& s1, const ThisType& s2)
{
		if( ! globalmq::marshalling2::Int64Processor::isSame( s1.get_id(), s2.get_id() ) ) return false;
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_text(), s2.get_text() ) ) return false;
		if( ! HtmlTag_subscriber::isSame( s1.get_html(), s2.get_html() ) ) return false;
	return true;
}
/* static */
inline
bool publishable_sample_subscriber::isSame(const ThisType& s1, const structures::publishable_sample& s2)
{
		if( ! globalmq::marshalling2::Int64Processor::isSame( s1.get_id(), s2.id ) ) return false;
		if( ! globalmq::marshalling2::StringProcessor::isSame( s1.get_text(), s2.text ) ) return false;
		if( ! HtmlTag_subscriber::isSame( s1.get_html(), s2.html ) ) return false;
	return true;
}
inline
void publishable_sample_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "id" );
	globalmq::marshalling2::Int64Processor::parse( parser, this->id );
	parser.nextElement();

	parser.namedParamBegin( "text" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->text );
	parser.nextElement();

	parser.namedParamBegin( "html" );
	HtmlTag_subscriber::parse_state_sync( parser, this->lazy_html() );

	parser.structEnd();
}

	inline
	bool publishable_sample_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "id" );
		{
			auto oldVal = this->id;
			this->id = globalmq::marshalling2::Int64Processor::parse(parser);
			bool currentChanged = oldVal != this->id;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_id( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "text" );
		{
			auto oldVal = this->text;
			this->text = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->text;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_text( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "html" );
		{
			bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_html();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool publishable_sample_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->id;
					this->id = globalmq::marshalling2::Int64Processor::parse(parser);
					bool currentChanged = oldVal != this->id;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_id( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->text;
					this->text = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->text;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_text( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTag_subscriber::parse_continue( parser, this->lazy_html(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void publishable_sample_subscriber::parse_state_sync( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "id" );
	globalmq::marshalling2::Int64Processor::parse( parser, this->id );
	parser.nextElement();

	parser.namedParamBegin( "text" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->text );
	parser.nextElement();

	parser.namedParamBegin( "html" );
	HtmlTag_subscriber::parse_state_sync( parser, this->lazy_html() );

	parser.structEnd();
}

	inline
	bool publishable_sample_subscriber::parse_notify( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "id" );
		{
			auto oldVal = this->id;
			this->id = globalmq::marshalling2::Int64Processor::parse(parser);
			bool currentChanged = oldVal != this->id;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_id( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "text" );
		{
			auto oldVal = this->text;
			this->text = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->text;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_text( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "html" );
		{
			bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_html();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool publishable_sample_subscriber::parse_continue( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->id;
					this->id = globalmq::marshalling2::Int64Processor::parse(parser);
					bool currentChanged = oldVal != this->id;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_id( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->text;
					this->text = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->text;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_text( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTag_subscriber::parse_continue( parser, this->lazy_html(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void publishable_sample_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "id" );
	globalmq::marshalling2::Int64Processor::parse( parser, this->id );
	parser.nextElement();

	parser.namedParamBegin( "text" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->text );
	parser.nextElement();

	parser.namedParamBegin( "html" );
	HtmlTag_subscriber::parse_state_sync( parser, this->lazy_html() );

	parser.structEnd();
}

	inline
	bool publishable_sample_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "id" );
		{
			auto oldVal = this->id;
			this->id = globalmq::marshalling2::Int64Processor::parse(parser);
			bool currentChanged = oldVal != this->id;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_id( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "text" );
		{
			auto oldVal = this->text;
			this->text = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->text;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_text( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "html" );
		{
			bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_html();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool publishable_sample_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->id;
					this->id = globalmq::marshalling2::Int64Processor::parse(parser);
					bool currentChanged = oldVal != this->id;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_id( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->text;
					this->text = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->text;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_text( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTag_subscriber::parse_continue( parser, this->lazy_html(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

inline
void publishable_sample_subscriber::parse_state_sync( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
{
	parser.structBegin();

	parser.namedParamBegin( "id" );
	globalmq::marshalling2::Int64Processor::parse( parser, this->id );
	parser.nextElement();

	parser.namedParamBegin( "text" );
	globalmq::marshalling2::StringProcessor::parse( parser, this->text );
	parser.nextElement();

	parser.namedParamBegin( "html" );
	HtmlTag_subscriber::parse_state_sync( parser, this->lazy_html() );

	parser.structEnd();
}

	inline
	bool publishable_sample_subscriber::parse_notify( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser )
	{
		bool changed = false;
		parser.structBegin();
		parser.namedParamBegin( "id" );
		{
			auto oldVal = this->id;
			this->id = globalmq::marshalling2::Int64Processor::parse(parser);
			bool currentChanged = oldVal != this->id;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_id( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "text" );
		{
			auto oldVal = this->text;
			this->text = globalmq::marshalling2::StringProcessor::parse(parser);
			bool currentChanged = oldVal != this->text;
			if ( currentChanged )
			{
				changed = true;
				this->notifyUpdated_text( oldVal );
			}
		}
		parser.nextElement();
		parser.namedParamBegin( "html" );
		{
			bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
			if ( changedCurrent )
			{
					changed = true;
				this->notifyUpdated_html();
			}
		}

		parser.structEnd();
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	bool publishable_sample_subscriber::parse_continue( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, GMQ_COLL vector<uint64_t>& addr, uint64_t offset )
	{
		bool changed = false;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->id;
					this->id = globalmq::marshalling2::Int64Processor::parse(parser);
					bool currentChanged = oldVal != this->id;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_id( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 )
				{
					parser.nextElement();
					parser.leafeBegin();
					auto oldVal = this->text;
					this->text = globalmq::marshalling2::StringProcessor::parse(parser);
					bool currentChanged = oldVal != this->text;
					if ( currentChanged )
					{
						changed = true;
						this->notifyUpdated_text( oldVal );
					}
				}
				else // deeper address not supported for this type
					throw std::exception(); // bad format, TODO: ...
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					parser.nextElement();
					parser.leafeBegin();
					bool changedCurrent = HtmlTag_subscriber::parse_notify( parser, this->lazy_html() );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				else // let child continue parsing
				{
					bool changedCurrent = HtmlTag_subscriber::parse_continue( parser, this->lazy_html(), addr, offset + 1 );
					if ( changedCurrent )
					{
						changed = true;
						this->notifyUpdated_html();
					}
				}
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if ( changed )
			this->notifyUpdated();
		return changed;
	}

	inline
	void publishable_sample_subscriber::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, this->id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		HtmlTag_subscriber::compose(composer, this->lazy_html() );

		composer.structEnd();
	}

	inline
	void publishable_sample_subscriber::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, this->id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		HtmlTag_subscriber::compose(composer, this->lazy_html() );

		composer.structEnd();
	}

	inline
	void publishable_sample_subscriber::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, this->id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		HtmlTag_subscriber::compose(composer, this->lazy_html() );

		composer.structEnd();
	}

	inline
	void publishable_sample_subscriber::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer )
	{
		composer.structBegin();

		composer.namedParamBegin( "id" );
		globalmq::marshalling2::Int64Processor::compose(composer, this->id );

		composer.nextElement();

		composer.namedParamBegin( "text" );
		globalmq::marshalling2::StringProcessor::compose(composer, this->text );

		composer.nextElement();

		composer.namedParamBegin( "html" );
		HtmlTag_subscriber::compose(composer, this->lazy_html() );

		composer.structEnd();
	}


//===============================================================================

namespace scope_test_exchange {

using cl_request = ::globalmq::marshalling::impl::MessageName<2>;
using srv_response = ::globalmq::marshalling::impl::MessageName<3>;

struct publishable_MESSAGE_cl_request
{
	using CppType = structures::scope_test_exchange::MESSAGE_cl_request;
	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t );


	static inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

};

	/* static */
	inline
	void publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.ordinal );
		parser.nextElement();

		parser.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_to_server );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.ordinal );
		parser.nextElement();

		parser.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_to_server );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.ordinal );
		parser.nextElement();

		parser.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_to_server );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.ordinal );
		parser.nextElement();

		parser.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_to_server );

		parser.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_cl_request::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.ordinal );

		composer.nextElement();

		composer.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_to_server );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_cl_request::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.ordinal );

		composer.nextElement();

		composer.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_to_server );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_cl_request::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.ordinal );

		composer.nextElement();

		composer.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_to_server );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_cl_request::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "ordinal" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.ordinal );

		composer.nextElement();

		composer.namedParamBegin( "text_to_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_to_server );

		composer.structEnd();
	}

struct publishable_MESSAGE_srv_response
{
	using CppType = structures::scope_test_exchange::MESSAGE_srv_response;
	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t );

	static inline void parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t );


	static inline void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t );

	static inline void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t );

};

	/* static */
	inline
	void publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.replied_on );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.value );
		parser.nextElement();

		parser.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_from_server );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.replied_on );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.value );
		parser.nextElement();

		parser.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_from_server );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.replied_on );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.value );
		parser.nextElement();

		parser.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_from_server );

		parser.structEnd();
	}
	/* static */
	inline
	void publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, CppType& t )
	{
		parser.structBegin();

		parser.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.replied_on );
		parser.nextElement();

		parser.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::parseForStateSyncOrMessageInDepth( parser, t.value );
		parser.nextElement();

		parser.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::parseForStateSyncOrMessageInDepth( parser, t.text_from_server );

		parser.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_srv_response::compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.replied_on );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.value );

		composer.nextElement();

		composer.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_from_server );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_srv_response::compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.replied_on );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.value );

		composer.nextElement();

		composer.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_from_server );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_srv_response::compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.replied_on );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.value );

		composer.nextElement();

		composer.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_from_server );

		composer.structEnd();
	}

	/* static */
	inline
	void publishable_MESSAGE_srv_response::compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, const CppType& t )
	{
		composer.structBegin();

		composer.namedParamBegin( "replied_on" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.replied_on );

		composer.nextElement();

		composer.namedParamBegin( "value" );
		globalmq::marshalling2::Int64Processor::compose(composer, t.value );

		composer.nextElement();

		composer.namedParamBegin( "text_from_server" );
		globalmq::marshalling2::StringProcessor::compose(composer, t.text_from_server );

		composer.structEnd();
	}

template<class ... HandlersT >
void implHandleMessage( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, HandlersT ... handlers )
{
	parser.structBegin();
	parser.namedParamBegin("msgid");
	uint64_t msgID = parser.parseUnsignedInteger();
	parser.nextElement();
	parser.namedParamBegin("msgbody");
	bool ok = false;

	switch ( msgID )
	{
		case cl_request::id: ok = ::globalmq::marshalling::impl::implHandleMessage<cl_request>( parser, handlers... ); break;
		case srv_response::id: ok = ::globalmq::marshalling::impl::implHandleMessage<srv_response>( parser, handlers... ); break;
		default: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;
	}

	/*if (!ok) return;
	parser.structEnd();*/
}

template<class ... HandlersT >
void handleMessage( globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, HandlersT ... handlers )
{
	implHandleMessage( parser, handlers... );
}

template<class ... HandlersT >
void implHandleMessage( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, HandlersT ... handlers )
{
	parser.structBegin();
	parser.namedParamBegin("msgid");
	uint64_t msgID = parser.parseUnsignedInteger();
	parser.nextElement();
	parser.namedParamBegin("msgbody");
	bool ok = false;

	switch ( msgID )
	{
		case cl_request::id: ok = ::globalmq::marshalling::impl::implHandleMessage<cl_request>( parser, handlers... ); break;
		case srv_response::id: ok = ::globalmq::marshalling::impl::implHandleMessage<srv_response>( parser, handlers... ); break;
		default: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;
	}

	/*if (!ok) return;
	parser.structEnd();*/
}

template<class ... HandlersT >
void handleMessage( globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser, HandlersT ... handlers )
{
	implHandleMessage( parser, handlers... );
}

template<class ... HandlersT >
void implHandleMessage( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, HandlersT ... handlers )
{
	parser.structBegin();
	parser.namedParamBegin("msgid");
	uint64_t msgID = parser.parseUnsignedInteger();
	parser.nextElement();
	parser.namedParamBegin("msgbody");
	bool ok = false;

	switch ( msgID )
	{
		case cl_request::id: ok = ::globalmq::marshalling::impl::implHandleMessage<cl_request>( parser, handlers... ); break;
		case srv_response::id: ok = ::globalmq::marshalling::impl::implHandleMessage<srv_response>( parser, handlers... ); break;
		default: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;
	}

	/*if (!ok) return;
	parser.structEnd();*/
}

template<class ... HandlersT >
void handleMessage( globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser, HandlersT ... handlers )
{
	implHandleMessage( parser, handlers... );
}

template<class ... HandlersT >
void implHandleMessage( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, HandlersT ... handlers )
{
	parser.structBegin();
	parser.namedParamBegin("msgid");
	uint64_t msgID = parser.parseUnsignedInteger();
	parser.nextElement();
	parser.namedParamBegin("msgbody");
	bool ok = false;

	switch ( msgID )
	{
		case cl_request::id: ok = ::globalmq::marshalling::impl::implHandleMessage<cl_request>( parser, handlers... ); break;
		case srv_response::id: ok = ::globalmq::marshalling::impl::implHandleMessage<srv_response>( parser, handlers... ); break;
		default: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;
	}

	/*if (!ok) return;
	parser.structEnd();*/
}

template<class ... HandlersT >
void handleMessage( globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser, HandlersT ... handlers )
{
	implHandleMessage( parser, handlers... );
}

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args );

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args );

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args );

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args );

//**********************************************************************
// MESSAGE "cl_request" Targets: GMQ (2 parameters)
//  1. INTEGER ordinal (REQUIRED)
//  2. CHARACTER_STRING text_to_server (REQUIRED)
//**********************************************************************

template<typename ... Args>
void MESSAGE_cl_request_compose(globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<ordinal_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<text_to_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("ordinal");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "ordinal", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_to_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_to_server", arg_2_type::nameAndTypeID, args...);
	composer.structEnd();
}

template<typename ... Args>
void MESSAGE_cl_request_compose(globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<ordinal_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<text_to_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("ordinal");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "ordinal", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_to_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_to_server", arg_2_type::nameAndTypeID, args...);
	composer.structEnd();
}

template<typename ... Args>
void MESSAGE_cl_request_compose(globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<ordinal_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<text_to_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("ordinal");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "ordinal", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_to_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_to_server", arg_2_type::nameAndTypeID, args...);
	composer.structEnd();
}

template<typename ... Args>
void MESSAGE_cl_request_compose(globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<ordinal_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<text_to_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("ordinal");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "ordinal", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_to_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_to_server", arg_2_type::nameAndTypeID, args...);
	composer.structEnd();
}

inline
structures::scope_test_exchange::MESSAGE_cl_request MESSAGE_cl_request_parse(globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_cl_request;
	T t;
	publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

inline
structures::scope_test_exchange::MESSAGE_cl_request MESSAGE_cl_request_parse(globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_cl_request;
	T t;
	publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

inline
structures::scope_test_exchange::MESSAGE_cl_request MESSAGE_cl_request_parse(globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_cl_request;
	T t;
	publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

inline
structures::scope_test_exchange::MESSAGE_cl_request MESSAGE_cl_request_parse(globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_cl_request;
	T t;
	publishable_MESSAGE_cl_request::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

//**********************************************************************
// MESSAGE "srv_response" Targets: GMQ (3 parameters)
//  1. INTEGER replied_on (REQUIRED)
//  2. INTEGER value (REQUIRED)
//  3. CHARACTER_STRING text_from_server (REQUIRED)
//**********************************************************************

template<typename ... Args>
void MESSAGE_srv_response_compose(globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<replied_on_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<value_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_3_type = globalmq::marshalling2::NamedParameterWithProcessor<text_from_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("replied_on");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "replied_on", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("value");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "value", arg_2_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_from_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_from_server", arg_3_type::nameAndTypeID, args...);
	composer.structEnd();
}

template<typename ... Args>
void MESSAGE_srv_response_compose(globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<replied_on_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<value_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_3_type = globalmq::marshalling2::NamedParameterWithProcessor<text_from_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("replied_on");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "replied_on", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("value");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "value", arg_2_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_from_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_from_server", arg_3_type::nameAndTypeID, args...);
	composer.structEnd();
}

template<typename ... Args>
void MESSAGE_srv_response_compose(globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<replied_on_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<value_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_3_type = globalmq::marshalling2::NamedParameterWithProcessor<text_from_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("replied_on");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "replied_on", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("value");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "value", arg_2_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_from_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_from_server", arg_3_type::nameAndTypeID, args...);
	composer.structEnd();
}

template<typename ... Args>
void MESSAGE_srv_response_compose(globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args)
{
	using arg_1_type = globalmq::marshalling2::NamedParameterWithProcessor<replied_on_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_2_type = globalmq::marshalling2::NamedParameterWithProcessor<value_Type::Name, globalmq::marshalling2::Int64Processor>;
	using arg_3_type = globalmq::marshalling2::NamedParameterWithProcessor<text_from_server_Type::Name, globalmq::marshalling2::StringProcessor>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	composer.structBegin();
	composer.namedParamBegin("replied_on");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "replied_on", arg_1_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("value");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "value", arg_2_type::nameAndTypeID, args...);

	composer.nextElement();

	composer.namedParamBegin("text_from_server");
	::globalmq::marshalling2::composeParam2<globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "text_from_server", arg_3_type::nameAndTypeID, args...);
	composer.structEnd();
}

inline
structures::scope_test_exchange::MESSAGE_srv_response MESSAGE_srv_response_parse(globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_srv_response;
	T t;
	publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

inline
structures::scope_test_exchange::MESSAGE_srv_response MESSAGE_srv_response_parse(globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_srv_response;
	T t;
	publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

inline
structures::scope_test_exchange::MESSAGE_srv_response MESSAGE_srv_response_parse(globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_srv_response;
	T t;
	publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

inline
structures::scope_test_exchange::MESSAGE_srv_response MESSAGE_srv_response_parse(globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>& parser)
{
	using T = structures::scope_test_exchange::MESSAGE_srv_response;
	T t;
	publishable_MESSAGE_srv_response::parseForStateSyncOrMessageInDepth(parser, t);
	return t;
}

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	composer.structBegin();
	composer.namedParamBegin("msgid");
	composer.composeUnsignedInteger(msgID::id);

	composer.nextElement();

	composer.namedParamBegin("msgbody");
	if constexpr ( msgID::id == cl_request::id )
		MESSAGE_cl_request_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == srv_response::id )
		MESSAGE_srv_response_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.structEnd();
}

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	composer.structBegin();
	composer.namedParamBegin("msgid");
	composer.composeUnsignedInteger(msgID::id);

	composer.nextElement();

	composer.namedParamBegin("msgbody");
	if constexpr ( msgID::id == cl_request::id )
		MESSAGE_cl_request_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == srv_response::id )
		MESSAGE_srv_response_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.structEnd();
}

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	composer.structBegin();
	composer.namedParamBegin("msgid");
	composer.composeUnsignedInteger(msgID::id);

	composer.nextElement();

	composer.namedParamBegin("msgbody");
	if constexpr ( msgID::id == cl_request::id )
		MESSAGE_cl_request_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == srv_response::id )
		MESSAGE_srv_response_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.structEnd();
}

template<typename msgID, typename ... Args>
void composeMessage( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	composer.structBegin();
	composer.namedParamBegin("msgid");
	composer.composeUnsignedInteger(msgID::id);

	composer.nextElement();

	composer.namedParamBegin("msgbody");
	if constexpr ( msgID::id == cl_request::id )
		MESSAGE_cl_request_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == srv_response::id )
		MESSAGE_srv_response_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.structEnd();
}

} // namespace scope_test_exchange 

//**********************************************************************
// PUBLISHABLE publishable_sample (3 parameters)
// 1. INTEGER id
// 2. CHARACTER_STRING text
// 3. STRUCT HtmlTag html
//**********************************************************************

template<class ComposerT>
class publishable_sample_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	using BufferT = typename ComposerT::BufferType;
	using RegistryT = globalmq::marshalling::PublisherRegistryBase<ComposerT>;
	using T = structures::publishable_sample;
	RegistryT* publishableRegistry = nullptr;
	BufferT buffer;
	ComposerT composer;
	T t;

public:
	static constexpr uint64_t numTypeID = 11;
	static constexpr const char* stringTypeID = "publishable_sample";

	publishable_sample_WrapperForPublisher() : composer( buffer ) {}
	publishable_sample_WrapperForPublisher( T arg ) : t( std::move( arg ) ), composer( buffer ) {}
	publishable_sample_WrapperForPublisher( RegistryT* registry ) : publishableRegistry(registry), composer( buffer ) { if(publishableRegistry) { publishableRegistry->addPublisher(this); } }
	virtual ~publishable_sample_WrapperForPublisher() { if(publishableRegistry) { publishableRegistry->removePublisher(this); } }
	ComposerT& getComposer() { return composer; }
	virtual void startTick( BufferT&& buff ) override { buffer = std::move( buff ); composer.reset(); composer.stateUpdateBegin(); }
	virtual BufferT&& endTick() override { composer.stateUpdateEnd(); return std::move( buffer ); }
	virtual const char* publishableName() override { return stringTypeID; }
	virtual uint64_t stateTypeID() override { return numTypeID; }
	virtual void generateStateSyncMessage(ComposerT& composer) override { compose(composer); }
	auto get_id() const { return t.id; }
	void set_id( decltype(T::id) val) { 
		t.id = val; 
		composer.changeBegin( GMQ_COLL vector<uint64_t>(), 0 );
		composer.nextElement();
		composer.leafeBegin();
		globalmq::marshalling2::Int64Processor::compose(composer, t.id );
		composer.changeEnd();
	}
	const auto& get_text() const { return t.text; }
	void set_text( decltype(T::text) val) { 
		t.text = val; 
		composer.changeBegin( GMQ_COLL vector<uint64_t>(), 1 );
		composer.nextElement();
		composer.leafeBegin();
		globalmq::marshalling2::StringProcessor::compose(composer, t.text );
		composer.changeEnd();
	}
	const auto& get_html() const { return t.html; }
	void set_html( decltype(T::html) val) { 
		t.html = val; 
		composer.changeBegin( GMQ_COLL vector<uint64_t>(), 2 );
		composer.nextElement();
		composer.leafeBegin();
		publishable_STRUCT_HtmlTag::compose(composer, t.html );
		composer.changeEnd();
	}
	auto get4set_html() { return HtmlTag_RefWrapper4Set<publishable_sample_WrapperForPublisher>(t.html, *this, globalmq::marshalling2::makeAddress(GMQ_COLL vector<uint64_t>(), 2)); }

	void compose( globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer ) { publishable_PUBLISHABLE_publishable_sample::compose( composer, t ); }
	void compose( globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>& composer ) { publishable_PUBLISHABLE_publishable_sample::compose( composer, t ); }
	void compose( globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>& composer ) { publishable_PUBLISHABLE_publishable_sample::compose( composer, t ); }
	void compose( globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>& composer ) { publishable_PUBLISHABLE_publishable_sample::compose( composer, t ); }
	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser ) { publishable_PUBLISHABLE_publishable_sample::parseForStateSyncOrMessageInDepth(parser, t); }
	const T& debugOnlyGetState() { return t; }
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
				return new publishable_sample_concentrator<InputBufferT, ComposerT>();
			default:
				return nullptr;
		}
	}
};

//===============================================================================


} // namespace basic_test

#endif // _publishable_state_h_38a1e1f6_guard
