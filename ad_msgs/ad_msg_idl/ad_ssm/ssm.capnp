@0xa2d5b7c3699de675;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::ssm");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";

enum SystemMode {
    modStartup @0;
    modFactory @1;
    modNormal @2;
    modUpdate @3;
    modVerify @4;
    modCalibrate @5;
    modStandby @6;
    modReserved @7;
}

enum NodeID {
    nodeTopSM @0;
    nodeADP @1;
    nodeOTA @2;
    nodeCalibrate @3;
    nodeSentry @4;
    nodeReserved @5;
}

enum VehicleMode {
    modUnknown @0;
    modDriving @1;
    modParking @2;
    modSentry @3;
}

struct CommMessage {
    header @0 :Header.Header;
    node @1 :NodeID;
    data @2 :SystemMode;
}

struct SysStateMessage {
    systemMode @0 :SystemMode;
    vehicleMode @1 :VehicleMode;
}
