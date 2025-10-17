@0xfde9986434ba3d1d;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::navigation");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct LocationPos {
    longitude @0 :Float64;
    latitude @1 :Float64;
}

struct TencentRoute {
    # list of links from navigation, routes may be empty
    linkIds @0 :List(UInt64);
    routes @1 :List(LocationPos);
}

struct RouteGuidance {
    header @0 :StdHeader.Header;
    intersectionPos @1 :RoutePos;
    matchResult @2 :Bool;
    nextIntersectionType @3 :UInt16;
    nextSpecIntersectionType @4 :UInt16;
    newRemainDistance @5 :UInt16;
    pointId @6 :Text;
    nextIntersectionRemainDistance @7 :UInt16;
    totalRemainDistance @8 :UInt16;
    segmentLength @9 :UInt16;
    hintType @10 :UInt8;
    enterPosA @11 :RoutePos;
    leavePosB @12 :RoutePos;
    actionLength @13 :UInt16;
    tunnelLength @14 :UInt16;
    hasCloseTurn @15 :Bool;
    closeIntersectionType @16 :UInt16;
    closeIntersectionLength @17 :UInt16;
    closeIntersectionPos @18 :RoutePos;
    nextSegmentLength @19 :UInt16;
    limitSpeedKmph @20 :UInt16;
    highwayEntranceInfo @21 :EntExitInfo;
    highwayExitInfo @22 :EntExitInfo;
    intersectionLaneInfo @23 :LaneInfo;
    curSegmentIndex @24 :UInt16;
}

struct EntExitInfo {
    id @0 :UInt16;
    routePos @1 :RoutePos;
    nameInfo @2 :Text;
}

struct LaneInfo {
    header @0 :StdHeader.Header;
    position @1 :RoutePos;
    distanceToRouteEnd @2 :UInt16;
    # from left to right
    lanesData @3 :List(LaneData);
    recommendLanes @4 :Text;
    laneNumLeft @5 :UInt8;
    laneNumRight @6 :UInt8;
    isValidLaneInfo @7 :Bool;
}

struct RoutePos {
    coorStart @0 :Int32;
    longitude @1 :Float64;
    latitude @2 :Float64;
    altitude @3 :Float32;
}

struct LaneData {
    laneFlag @0 :Text;
    laneArrow @1 :Text;
    laneProperty @2 :Text;
}

struct TrafficLight  {
    header @0 :StdHeader.Header;
    multiLightInfo @1 :List(MultiLightBubbleInfo);
}

struct MultiLightBubbleInfo {
    point @0 :GeoCoordinateZ;
    lightInfo @1 :List(OneLight);
}

struct OneLight {
    lightType @0 :UInt8;
    turnArrowType @1  :UInt8;
    remainTime @2  :UInt8;
    totalTime @3  :UInt8;
}

struct GeoCoordinateZ {
    longitude @0 :Float64;
    latitude @1 :Float64;
    altitude @2 :Float32;
}

struct SpeedLimit {
    header @0 :StdHeader.Header;
    limitSpeedKmph @1  :UInt8;
    speedLimit @2  :UInt8;
    averageSpeedKmph @3  :UInt8;
    remainLength @4 :UInt16;
}

struct NaviSegment {
    intersectionType @0 :UInt16;
    segmentLength @1  :UInt16;
}

struct NaviSegmentList {
    header @0 :StdHeader.Header;
    segmentList @1  :List(NaviSegment);
}

struct TTSInfo{
    header @0 :StdHeader.Header;
    ttsType @1  :UInt16;
    ttsSubType @2  :UInt16;
    ttsVoiceText @3 :Text;
}