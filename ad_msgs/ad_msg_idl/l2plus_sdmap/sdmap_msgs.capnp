@0xe8990901729f50ee;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::sdmap");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";
using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct LaneTrafficInfo {
    isBus @0 :Bool;
    laneType @1 :UInt32;
    arrow @2 :UInt32;
}

struct LaneInfoMeta {
    laneNum @0 :UInt8;
    laneInfos @1 :List(LaneTrafficInfo);
}

struct LinkIDType {
    tileID @0 :UInt32;
    linkID @1 :UInt32;
    rawID @2 :UInt64;
}

struct LinkCurvature {
    offset @0 :UInt32;
    curvature @1 :Int32;
}

struct Link {
    linkID @0 :LinkIDType;
    laneNumSum @1 :UInt8;
    laneNumS2E @2 :UInt8;
    laneNumE2S @3 :UInt8;
    linkDirection @4 :UInt8;
    speedLimitS2E @5 :UInt8;
    speedLimitE2S @6 :UInt8;
    speedClassType @7 :UInt8;
    startAngle @8 :Int16;
    endAngle @9 :Int16;
    roadKindTypes @10 :List(UInt8);
    geometry @11 :List(Geo.Point);
    linkCurvatures @12 :List(LinkCurvature);
    guidanceInfos @13 :List(LaneInfoMeta);
    nextLinkIDs @14 :List(LinkIDType);
    previousLinkIDs @15 :List(LinkIDType);
    roadClassType @16 :Int8;
    constructionType @17 :UInt8;
    linkLength @18 :Float32;
    pointSpeedLimit @19 :List(PointSpeedInfo);
    mainActionAmap @20 :UInt8;
    assistantActionAmap @21 :UInt8;
}

struct PointSpeedInfo {
    pointSpeedType @0 :UInt8;
    speedLimit @1 :UInt8;
    pointPosition @2 :Geo.Point;
    limitDirection @3 :UInt8;
}

struct NodeIDType {
    tileID @0 :UInt32;
    nodeID @1 :UInt32;
}

struct LinkNode {
    nodeID @0 :NodeIDType;
    position @1 :Geo.Point;
    crossFlag @2 :UInt8;
    trafficLightFlag @3 :Bool;
    enterLinkIDs @4 :List(LinkIDType);
    outLinkIDs @5 :List(LinkIDType);
}

struct LocalizationInfo {
    timeStamp @0 :UInt64;
    position @1 :Geo.Point;
}

struct SDMap {
    mapHeader @0 :Header.MapHeader;
    links @1 :List(Link);
    startLinkNodes @2 :List(LinkNode);
    endLinkNodes @3 :List(LinkNode);
    navPathInfo @4 :NavPathInfo;
    locInfo @5 :LocalizationInfo;
}

struct NavPathInfo {
    linkRawIDs @0 :List(UInt64);
    coordinates @1 :List(Geo.Point);
    drivingDirections @2 :List(UInt8);
}