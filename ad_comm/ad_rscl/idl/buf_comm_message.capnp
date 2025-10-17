@0x887b4bc6d39d621e;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

using import "/ad_msg_idl/std_msgs/header.capnp".Time;

struct BufNewMessageNotification {
    key @0 :Text;
    token @1 :UInt32;
    revision @2 :UInt64;
    time @3 :Time;
}
