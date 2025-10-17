@0x8779d8df55dff5d6;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl::cid");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";


struct CidCan {
    buffer @0 :List(UInt8);
}

