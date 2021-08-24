#ifndef NODECPP_GMQUEUE_CUSTOMIZED_TYPES_H
#define NODECPP_GMQUEUE_CUSTOMIZED_TYPES_H

#include <safememory/safe_ptr.h>
#include <marshalling.h>
#include <internal_msg.h>

class GMQueueStatePublisherSubscriberTypeInfo
{
public:
	using BufferT = nodecpp::platform::internal_msg::InternalMsg;
	using ParserT = globalmq::marshalling::JsonParser<BufferT>;
	using ComposerT = globalmq::marshalling::JsonComposer<BufferT>;
//	using ParserT = globalmq::marshalling::GmqParser<BufferT>;
//	using ComposerT = globalmq::marshalling::GmqComposer<BufferT>;
	// template<class T>
	// using OwningPtrT = ::safememory::owning_ptr<T>;
	template<class T>
	using OwningPtrT = ::std::unique_ptr<T>;
};


#endif // NODECPP_GMQUEUE_CUSTOMIZED_TYPES_H
