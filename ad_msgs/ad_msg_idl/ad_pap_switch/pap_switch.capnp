@0x8409787de79e38dc;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::pap_switch");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct PAPSwitchReq {
  toParkingMode @0 :Bool;
}

struct PAPSwitchResp {
  inParking @0 :Bool;
}

struct DpStateMachine {
  header @0 :StdHeader.Header;
  runningMode @1 :UInt8;
  apaSWReq @2 :UInt8;
  iviStatusResp @3 :UInt8;
}