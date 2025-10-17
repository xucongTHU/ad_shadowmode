@0xb8e3302da32f2d64;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::ad_msg_bridge::serialization");

struct CapnMessage {
    data @0 : Text;
    id @1 : UInt32;
}
