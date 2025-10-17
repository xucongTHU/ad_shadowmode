@0xe4934bae7c5002a4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::calibration");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
enum SensorId {
    frontWideFovCamera @0;
    frontNarrowFovCamera @1;
    frontSmartCamera @2;
    leftFrontCamera @3;
    rightFrontCamera @4;
    leftRearCamera @5;
    rightRearCamera @6;
    rearCamera @7;
}

enum CheckStatus {
    normal @0;
    warning @1;
    fatal @2;
}

struct CalibrationCheck {
    header @0 :StdHeader.Header;
    measurementTimeNs @1 :UInt64;
    sensor @2 :SensorId;
    status @3 :CheckStatus;
}

struct CtlMsg {
  msg @0 :Int16;
}
struct CtlRequest {
  pid @0 :UInt64;
  name @1 :Text;
}
struct CtlResponse {
  status @0 :UInt64;
}
