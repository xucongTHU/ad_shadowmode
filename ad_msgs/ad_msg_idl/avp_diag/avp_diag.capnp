@0x8a8aaa7a49237bee;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_diag");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_diag");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct DiagException {
  timestampNs @0 :UInt64;
  code @1 :UInt32;
  message @2 :Text;
}

struct DiagExceptionList {
  header @0 :StdMsgsHeader.Header;
  exceptions @1 :List(DiagException);
}

struct DiagVisualizerInfo {
  header @0 :StdMsgsHeader.Header;
  parkingCount @1 :UInt32;
  parkingCostTime @2 :UInt64;
  latDistanceError @3 :Float64;
  lonDistanceError @4 :Float64;
}
