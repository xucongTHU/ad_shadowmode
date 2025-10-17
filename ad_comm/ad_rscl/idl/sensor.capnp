@0x8961e2e3b7325e12;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";

struct CameraEncodedStruct{
    header @0 :Header.Header;
    sendTime @1:UInt64;
    frameType @2:UInt8;
    height @3 :UInt32;
    width @4 :UInt32;
    videoFormat @5:UInt8;
    dataSize @6:UInt32;
    data @7:Data;
}
