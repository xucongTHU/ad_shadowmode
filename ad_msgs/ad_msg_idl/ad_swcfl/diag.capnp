@0xd712055142bf6b22;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::ad_swcfl");
using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

# debug diag msg for hmi 
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
    isInternal @5 :Bool;
}

struct DiagMsg {
    header @0 :StdHeader.Header;

    # platform name
    platform @1:Text;

    # node name
    nodeName @2 :Text;

    # diag info
    infos @3 :List(DiagInfo);
}

# debug diag msg for hmi 