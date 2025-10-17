@0x85ecda5ceee481f8;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_planning");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct AEBRequest {
    header @0 :StdHeader.Header;
    enabled @1 :Bool;
    acc @2 :Float64;
}

struct AEBDebugInfo {
    header @0 :StdHeader.Header;
    aebFlag @1 :Bool;
    aebMode @2 :AebMode;
    aebOdFactor @3 :AebOdFactor;
    aebUssFactor @4 :AebUssFactor;
    aebVisualization @5 :AebVisualization;
}

struct AebMode {
    aebUss @0 :Bool;
    aebOd @1 :Bool;
}

struct AebOdFactor {
    collision @0 :Bool;
    collisionIndex @1 :UInt8;
    timeEnable @2 :Bool;
    sEnable @3 :Bool;
    timeBrake @4 :Float64;
    timeEmergency @5 :Float64;
    timeBuff @6 :Float64;
    timeSafe @7 :Float64;
    stopS @8 :Float64;
    emergencyBrakeDistance @9 :Float64;
    predictionSumDist @10 :Float64;
}

struct AebUssFactor {
    collision @0 :Bool;
    ussLable @1 : UInt8;
    ussDistance @2 :UssDistance;
}

struct UssDistance {
    frontMin @0 :Float64;
    rearMin @1 :Float64;
    leftMin @2 :Float64;
    rightMin @3 :Float64;
    frontLeftLeft @4 :Float64;
    frontLeftMid @5 :Float64;
    frontRightMid @6 :Float64;
    frontRightRight @7 :Float64;
    rearLeftLeft @8 :Float64;
    rearLeftMid @9 :Float64;
    rearRightMid @10 :Float64;
    rearRightRight @11 :Float64;
    leftFront @12 :Float64;
    leftRear @13 :Float64;
    rightFront @14 :Float64;
    rightRear @15 :Float64;
}

struct AebVisualization {
    emergState @0 :Bool;
    vehicleMsgs @1 :VehicleMsgs;
    obsCubMsgs @2 :ObsCubMsgs;
    obsCylMsgs @3 :ObsCylMsgs;
    egoPose @4: EgoPose;
    egoPoseExtend @5: EgoPoseExtend;
    egoPredictionPose @6: EgoPredictionPose;
    egoPredictionPoseExtend @7: EgoPredictionPoseExtend;
    mainState @8: Int8;
}

struct VehicleMsgs {
    length @0 :Float64;
    width @1 :Float64;
    lengthExtend @2 :Float64;
    widthExtend @3 :Float64;
}

struct ObsCubMsgs {
    collision @0 :Bool;
    x @1 :Float64;
    y @2 :Float64;
    theta @3 :Float64;
    length @4 :Float64;
    width @5 :Float64;
    id @6 :Float64;
}

struct ObsCylMsgs {
    collision @0 :Bool;
    x @1 :Float64;
    y @2 :Float64;
    radius @3 :Float64;
    id @4 :Float64;
}

struct EgoPose {
    geoCenter @0 :GeoCenter;
    wheelBaseCenter @1 :WheelBaseCenter;
}

struct EgoPoseExtend {
    geoCenter @0 :GeoCenter;
    wheelBaseCenter @1 :WheelBaseCenter;
}

struct EgoPredictionPose {
    geoCenter @0 :GeoCenter;
    wheelBaseCenter @1 :WheelBaseCenter;
}

struct EgoPredictionPoseExtend {
    geoCenter @0 :GeoCenter;
    wheelBaseCenter @1 :WheelBaseCenter;
}

struct GeoCenter {
    x @0 :Float64;
    y @1 :Float64;
    theta @2 :Float64;
}

struct WheelBaseCenter {
    x @0 :Float64;
    y @1 :Float64;
    theta @2 :Float64;
}
