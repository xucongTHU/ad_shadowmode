@0x9c440bde6cee6429;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct DmaShmNewMessageNotification {
    header @0 :StdHeader.Header;
    index @1 :UInt64;
    signature @2 :UInt64;
}