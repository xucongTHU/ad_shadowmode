@0x80d0a8a682d1129e;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using Base = import "/ad_msg_idl/ad_perception/base.capnp";

struct UssRange {
  distanceId @0 :Text;
  recvTimeNs @1 :UInt64;
  distance @2 :UInt32;
}
