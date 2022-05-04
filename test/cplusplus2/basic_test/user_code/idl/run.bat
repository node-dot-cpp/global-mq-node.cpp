
set P1="--parser=globalmq::marshalling2::GmqParser2<nodecpp::platform::internal_msg::InternalMsg>"
set P2="--parser=globalmq::marshalling2::GmqParser2<globalmq::marshalling::Buffer>"
set P3="--parser=globalmq::marshalling2::JsonParser2<nodecpp::platform::internal_msg::InternalMsg>"
set P4="--parser=globalmq::marshalling2::JsonParser2<globalmq::marshalling::Buffer>"
set C1="--composer=globalmq::marshalling2::GmqComposer2<nodecpp::platform::internal_msg::InternalMsg>"
set C2="--composer=globalmq::marshalling2::GmqComposer2<globalmq::marshalling::Buffer>"
set C3="--composer=globalmq::marshalling2::JsonComposer2<nodecpp::platform::internal_msg::InternalMsg>"
set C4="--composer=globalmq::marshalling2::JsonComposer2<globalmq::marshalling::Buffer>"
idl_compiler.exe publishable_state.idl ../generated/publishable_state.h --cplusplus -p=Nodecpp -c=GMQueueStatePublisherSubscriberTypeInfo %P1% %P2% %P3% %P4% %C1% %C2% %C3% %C4% -m=basic_test
pause