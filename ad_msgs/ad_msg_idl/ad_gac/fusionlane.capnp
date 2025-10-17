@0x876065df4087cf4f;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl::fusionlane");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct TimeStamp {
    isvalid @0 :Bool;
    sec @1 :UInt32;
    nsec @2 :UInt32;
}

struct CoordinateSystem {
    index @0 :UInt8;
}

struct SensorId {
    index @0 :UInt64;
}

struct HistInfo {
    sensorid @0 :SensorId;
    obstacleid @1 :UInt32;
    rawsensorcapturetimestamp @2 :TimeStamp;
    sensorperceptioncapturetimestamp @3 :TimeStamp;
    sensorperceptionoutputtimestamp @4 :TimeStamp;
    fusioncapturetimestamp @5 :TimeStamp;
    fusionoutputtimestamp @6 :TimeStamp;
}

struct LaneType {
    cutpoint @0 :StdGeometry.Point;
    type @1 :UInt8;
}

struct LaneColor {
    cutpoint @0 :StdGeometry.Point;
    color @1 :UInt8;
}

struct LaneWidth {
    cutpoint @0 :StdGeometry.Point;
    width @1 :UInt8;
    widthleft @2 :UInt8;
    widthright @3 :UInt8;
}

struct LaneBoundary {
    histinfo @0 :List(HistInfo);
    laneid @1 :UInt32;
    lanepositionindex @2 :UInt8;
    confidence @3 :Float64;
    availability @4 :UInt8;
    c0 @5 :Float64;
    c1 @6 :Float64;
    c2 @7 :Float64;
    c3 @8 :Float64;
    rangestart @9 :Float64;
    rangeend @10 :Float64;
    lanetypevector @11 :List(LaneType);
    lanecolorvector @12 :List(LaneColor);
    lanewidthvector @13 :List(LaneWidth);
}

struct FusionLane {
    timestamp @0 :TimeStamp;
    coordinate @1 :CoordinateSystem;
    sensorid @2 :SensorId;
    laneboundaryvector @3 :List(LaneBoundary);
}

