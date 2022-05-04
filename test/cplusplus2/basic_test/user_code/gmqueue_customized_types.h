#ifndef GMQUEUE_CUSTOMIZED_TYPES_H
#define GMQUEUE_CUSTOMIZED_TYPES_H

#include <safememory/safe_ptr.h>
#include <marshalling.h>
#include <globalmq/marshalling2/marshalling2.h>
#include <internal_msg.h>

class GMQueueStatePublisherSubscriberTypeInfo
{
public:
	using BufferT = nodecpp::platform::internal_msg::InternalMsg;
	using ParserT = globalmq::marshalling2::JsonParser2<BufferT>;
	using ComposerT = globalmq::marshalling2::JsonComposer2<BufferT>;
//	using ParserT = globalmq::marshalling::GmqParser<BufferT>;
//	using ComposerT = globalmq::marshalling::GmqComposer<BufferT>;
	// template<class T>
	// using OwningPtrT = ::safememory::owning_ptr<T>;
	template<class T>
	using OwningPtrT = ::std::unique_ptr<T>;

	template<class T, class... _Types, 	std::enable_if_t<!std::is_array<T>::value, int> = 0>
	static
	OwningPtrT<T> make_owning(_Types&&... _Args)
	{
		if constexpr ( std::is_same<::safememory::owning_ptr<T>, OwningPtrT<T>>::value )
			return ::safememory::make_owning<T, _Types ...>( ::std::forward<_Types>(_Args)... );
		else if constexpr ( std::is_same<::std::unique_ptr<T>, OwningPtrT<T>>::value )
			return ::std::make_unique<T, _Types ...>( ::std::forward<_Types>(_Args)... );
		else
		{
			static_assert( std::is_same<void, OwningPtrT<T>>::value, "unsupported type" ); // well, compilers don't like just static_assert( false ) here
		}
	}
};


#endif // GMQUEUE_CUSTOMIZED_TYPES_H
