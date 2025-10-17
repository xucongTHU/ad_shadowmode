@0xe5fceb21692d3b3a;

using RadarMsgDataType=import "/ad_msg_idl/ad_gac/radar_a_cm_data_type.capnp";

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");

struct GeometryPoit {
    x @0:Float64;
    y @1:Float64;
    z @2:Float64;
}

struct GnssInfo {
    header @0:RadarMsgDataType.Header;
    longitude @1:Float64;
    latitude @2:Float64;
    elevation @3:Float64;
    utmPosition @4:GeometryPoit;
    utmZoneNum @5:Int32;
    utmZoneChar @6:UInt8;
    attitude @7:GeometryPoit;
    linearVelocity @8:GeometryPoit;
    sdPosition @9:GeometryPoit; 
    sdVelocity @10:GeometryPoit;
    sdAttitude @11:GeometryPoit;
    cep68 @12:Float64;
    cep95 @13:Float64;
    second @14:Float64;
    satUseNum @15:Int32;
    satInViewNum @16:Int32;
    solutionStatus @17:UInt16;
    positionType @18:UInt16;
    pDop @19:Float32;
    hDop @20:Float32;
    vDop @21:Float32;
    attitudeDual @22:GeometryPoit;
    sdAngleDual @23:GeometryPoit;
    baseLineLengthDual @24:Float64;
    solutionStatusDual @25:Int32;
    positionTypeDual @26:Int32;
    solutionSourceDual @27:Int32;
}

struct InsInfo {
    header @0:RadarMsgDataType.Header;
    longitude @1:Float64;
    latitude @2:Float64;
    elevation @3:Float64;
    utmPosition @4:GeometryPoit;
    utmZoneNum @5:Int32;
    utmZoneChar @6:UInt8;
    attitude @7:GeometryPoit;
    linearVelocity @8:GeometryPoit; 
    sdPosition @9:GeometryPoit; 
    sdVelocity @10:GeometryPoit; 
    sdAttitude @11:GeometryPoit;
    cep68 @12:Float64;
    cep95 @13:Float64;
    second @14:Float64;
    satUseNum @15:Int32;
    satInViewNum @16:Int32;
    solutionStatus @17:UInt16;
    positionType @18:UInt16;
    pDop @19:Float32;
    hDop @20:Float32;
    vDop @21:Float32;
    attitudeDual @22:GeometryPoit;
    sdAngleDual @23:GeometryPoit;
    baseLineLengthDual @24:Float64;
    solutionStatusDual @25:Int32;
    positionTypeDual @26:Int32;
    solutionSourceDual @27:Int32;
    aoc @28:Int32;
    rtkBaseline @29:Int32;
    angularVelocity @30:GeometryPoit;
    acceleration @31:GeometryPoit;
}

struct ImuInfo {
    header @0:RadarMsgDataType.Header;
    angularVelocity @1:GeometryPoit;
    acceleration @2:GeometryPoit;
    imuStatus @3:UInt16;
    temperature @4:Float32;
}



