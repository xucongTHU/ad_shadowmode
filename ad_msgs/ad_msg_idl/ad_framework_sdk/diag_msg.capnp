@0xfc3a267c48f8ce49;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::ad_framework_sdk");
using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";


struct DiagInfo {
    name @0 :Text;
    enum Level {
        info @0;
        warning @1;
        error @2;
        fatal @3;
    }
    level @1 :Level;
    active @2 :Bool;
    timestamp @3 :UInt64;
    msg @4 :Text;
}

struct DiagMsg {
    header @0 :StdHeader.Header;

    # node name
    nodeName @1 :Text;

    # diag info
    infos @2 :List(DiagInfo);
}

