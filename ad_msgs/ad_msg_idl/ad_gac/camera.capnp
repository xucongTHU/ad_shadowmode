@0x940087ac1b0147b4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");


struct CameraHeadTimeStruct{
    sec @0:UInt32;
    nsec @1:UInt32;
    expStartS @2:UInt32;
    expStartNs @3:UInt32;
    expEndS @4:UInt32;
    expEndNs @5:UInt32;
    shutter1 @6:UInt32;
    shutter2 @7:UInt32;
}

struct CameraHeaderStruct {
    seq @0:UInt32;
    stamp @1:CameraHeadTimeStruct;
    frameId @2:Text;
}

struct CameraEncodedStruct{
    dataSize @0:UInt32;
    sendTimeHigh @1:UInt32;
    sendTimeLow @2:UInt32;
    frameType @3:UInt32;
    videoFormat @4:Text;
    data @5:Data;
    cameraHeader @6:CameraHeaderStruct;
}

struct CameraDecodedStruct{
    height @0:UInt32;
    width @1:UInt32;
    sendTimeHigh @2:UInt32;
    sendTimeLow @3:UInt32;
    frameType @4:UInt32;
    dataSize @5:UInt32;
    mbufData @6:UInt64;
    cameraHeader @7:CameraHeaderStruct;
}