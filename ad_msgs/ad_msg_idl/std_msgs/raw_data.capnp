@0xb5bf7b96a18199da;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::std_msgs");

using import "/ad_msg_idl/std_msgs/header.capnp".Header;

enum MemoryType {
  host @0;
  cambriconVgu @1;
  horizonIon @2;
}

struct Image {
    header @0 :Header;
    oriHeight @1 :UInt32;
    oriWidth @2 :UInt32;
    height @3 :UInt32;
    width @4 :UInt32;
    encoding @5 :Text;
    step @6 :UInt32;
    data @7 :Data;
    phyAddr @8 :UInt64;
    virAddr @9 :UInt64;
    memtype @10 :MemoryType;
}

struct ImageVGU {
    header @0 :Header;
    height @1 :UInt32;
    width @2 :UInt32;
    encoding @3 :Text;
    addr @4 :UInt64;
}

struct PymFrame {
    header @0 :Header;
    dataSize @1 :UInt32;
    oriHeight @2 :UInt32;
    oriWidth @3 :UInt32;
    sliceNumber @4 :UInt32;
    data @5 :Data;
}

struct PointXYZIR {
    x @0 :Float32;
    y @1 :Float32;
    z @2 :Float32;
    intensity @3 :UInt32;
    ring @4 :UInt32;
}

struct PointCloud {
    header @0 :Header;
    stampMs @1 :UInt64;
    isDense @2 :Bool;
    # float32 * 4 + uint16 = 18 bytes
    step @3 :UInt32;
    pointCount @4 :UInt32;

    # Only possible to have x,y,z,intensity,ring
    pointBuf @5 :Data;
    width @6 :UInt32;
    height @7 :UInt32;
}

struct CameraEncodedStruct{
    header @0 :Header;
    sendTime @1:UInt64;
    frameType @2:UInt8;
    height @3 :UInt32;
    width @4 :UInt32;
    videoFormat @5:UInt8;
    dataSize @6:UInt32;
    data @7:Data;
}
