@0xf01770bbbaff6c07;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::mcu");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct PAPStatusOut{
    header @0 :StdHeader.Header;

    papStatus @1 : UInt8;
    papChangestate @2 :UInt8;
    papStatusWarning @3:UInt8;
}

struct PAPParkingReq{
     header @0 :StdHeader.Header;
     parkingReq @1:UInt8;
     parkingStatus @2 :UInt8;

}

struct EMstate{
    header @0 :StdHeader.Header;
    emSwcState @1: UInt8;
}

struct UdpRaw {
  header @0 :StdHeader.Header;
  dataSize @1: UInt32;
  udpData @2: Data;
}

struct LowCtrl {
  header @0 :StdHeader.Header;
  sysErrorSt @1: UInt8;
  driverTakeOver @2: UInt8;
  ePSControlStatus @3: UInt8;
  eSCControlStatus @4: UInt8;
  modeEnable @5: UInt8;
  socCount @6: UInt8;
  gearReqIn @7: UInt8;
}
struct ParkingStatus {
  header @0 :StdHeader.Header;
  fromMcuParkingSts @1: UInt8;
  errorCode @2: UInt8;
  reserve1 @3: UInt8;
  reserve2 @4: UInt8;
}