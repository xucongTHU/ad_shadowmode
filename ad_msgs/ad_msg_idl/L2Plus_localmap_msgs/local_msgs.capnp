@0xd4e7d211ae1461c6;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::localmap");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";
using SDMap = import "/ad_msg_idl/l2plus_sdmap/sdmap_msgs.capnp";

struct Area {
    id @0 :UInt64;
    areaType @1 :UInt8;
    boundary @2 :List(Geo.Point);
    laneIds @3 :List(UInt64);
    junctionId @4 :UInt64;
    histInfos @5 :List(StdHeader.HistInfo);
}

struct Crosswalk {
    id @0 :UInt64;
    boundary @1 :List(Geo.Point);
    laneIds @2 :List(UInt64);
    junctionId @3 :UInt64;
    histInfos @4 :List(StdHeader.HistInfo);
}

struct CurrentStatus {
    curLaneId @0 :UInt64;
    disToRamp @1 :Float32;
}

struct FusionLineData {
    lineId @0 :UInt64;
    linePositionIndex @1 :UInt8;
    confidence @2 :Float32;
    availability @3 :UInt8;

    c0 @4 :Float32;
    c1 @5 :Float32;
    c2 @6 :Float32;
    c3 @7 :Float32;

    startPoint @8 :Geo.Point;
    endPoint @9 :Geo.Point;

    lineTypes @10 :List(UInt8);
    lineStyles @11 :List(UInt8);
    lineColors @12 :List(UInt8);
    lineWidths @13 :List(Float32);

    typeCutPoints @14 :List(Geo.Point);
    styleCutPoints @15 :List(Geo.Point);
    colorCutPoints @16 :List(Geo.Point);
    widthCutPoints @17 :List(Geo.Point);
}

struct MapHeader {
    timestampNs @0 :UInt64;
    sensorId @1 :UInt8;
    coordType @2 :UInt8;
    origin @3 :Geo.Point;
    originLocalPoints @4 :Geo.Point;
    mapType @5: UInt8;
}

struct FusionLines {
    mapHeader @0 :MapHeader;
    fusionLines @1 :List(FusionLineData);
}

struct SpeedIntervalInfo {
    # 区间限速值，单位m/s
    maxSpeed @0 :Float32;
    minSpeed @1 :Float32;

    # 0: Unknown;
    # 1: Persent;
    # 2: Index;
    # default = 0
    offsetStyle @2  :UInt8;
    startOffset @3  :Float32;
    endOffset @4  :Float32;
}

struct Lane {
    id @0 :UInt64;
    sectionId @1 :UInt64;
    sequenceId @2 :UInt8;
    intersectionId @3 :UInt64;
    laneType @4 :UInt8;
    turnType @5 :UInt32;
    transType @6 :UInt8;

    leftLineId @7 :UInt64;
    rightLineId @8 :UInt64;
    leftLaneId @9 :UInt64;
    rightLaneId @10 :UInt64;
    length @11 :Float32;

    centerPoints @12 :List(Geo.Point);
    laneWidths @13 :List(Float32);
    longitudinalSlopes @14 :List(Float32);
    curvatureRadius @15 :List(Float32);

    maxSpeed @16 :Float32;
    minSpeed @17 :Float32;

    successorLinkIds @18 :List(UInt64);
    predecessorLinkIds @19 :List(UInt64);

    stopLineIds @20 :List(UInt64);
    areaIds @21 :List(UInt64);
    trafficLightIds @22 :List(UInt64);
    trafficSignIds @23 :List(UInt64);
    roadMarkerIds @24 :List(UInt64);
    crossWalkIds @25 :List(UInt64);
    speedBumpIds @26 :List(UInt64);
    centerLocalPoints @27 :List(Geo.Point);
    laneWidthsLocal @28 :List(Float32);
    longitudinalSlopesLocal @29 :List(Float32);
    curvatureRadiusLocal @30 :List(Float32);

    speedLimitVector @31 :List(SpeedIntervalInfo);

    lateralSlopes @32 :List(Float32);

    headings @33 :List(Float32);

}

struct LineSegment {
    id @0 :UInt64;
    width @1 :Float32;
    color @2 :UInt8;
    type @3 :UInt8;
    style @4 :UInt8;
    confidence @5 :Float32;
    points @6 :List(Geo.Point);
    localPoints @7 :List(Geo.Point);
    histInfos @8 :List(StdHeader.HistInfo);
}

struct LaneLine {
    id @0 :UInt64;
    leftLaneId @1 :UInt64;
    rightLaneId @2 :UInt64;
    lineSegments @3 :List(LineSegment);
    predecessorLanelineId @4 :UInt64;
    successorLanelineId @5 :UInt64;
    curFatherLineId @6 :List(UInt64);  # add bining curbID 
}

struct LaneLink {
    id @0 :UInt64;
    fromLaneId @1 :UInt64;
    toLaneId @2 :UInt64;
    linkType @3 :UInt8;

    refPoints @4 :List(Geo.Point);
    refLocalPoints @5 :List(Geo.Point);
}

struct Pole {
    id @0 :UInt64;
    detectionStatus @1 :UInt8;
    confidence @2 :Float32;
    laneId @3 :UInt64;

    bottomPoint @4 :Geo.Point;
    topPoint @5 :Geo.Point;

    type @6 :UInt8;
    histInfos @7 :List(StdHeader.HistInfo);
}

struct NavStateInfo {
    timestamp @0 :UInt64;

    longitude @1 :Float64;
    latitude @2 :Float64;
    altitude @3 :Float64;

    pose @4 :Geo.Pose;
    poseCov @5 :List(Float64);

    linearSpeed @6 :Geo.Vector3;
    linearAcceleration @7 :Geo.Vector3;

    angularSpeed @8 :Geo.Vector3;
}

struct RoadMarker {
    id @0 :UInt64;
    centroid @1 :Geo.Point;
    quaternion @2 :Geo.Quaternion;
    length @3 :Float32;
    width @4 :Float32;
    height @5 :Float32;
    detectionStatus @6 :UInt8;
    confidence @7 :Float32;
    laneId @8 :UInt64;

    type @9 :UInt32;
    color @10 :UInt8;
    semantic @11 :Text;
    centroidLocal @12 :Geo.Point;
    quaternionLocal @13 :Geo.Quaternion;
    histInfos @14 :List(StdHeader.HistInfo);
}

struct StopLine {
    id @0 :UInt64;
    points @1 :List(Geo.Point);
    laneIds @2 :List(UInt64);
    localPoints @3 :List(Geo.Point);
    histInfos @4 :List(StdHeader.HistInfo);
    junctionId @5 :UInt64;
    nodeID @6 :SDMap.NodeIDType;

}

struct TrafficSignal {
    shape @0 :UInt8;
    status @1  :UInt8;
    attention @2 :UInt8;
}

struct TrafficLight {
    id @0 :UInt64;
    centroid @1 :Geo.Point;
    quaternion @2 :Geo.Quaternion;
    length @3 :Float32;
    width @4 :Float32;
    height @5 :Float32;
    detectionStatus @6  :UInt8;
    confidence @7 :Float32;
    laneId @8 :UInt64;

    countdown @9 :Int16;
    signals @10 :List(TrafficSignal);
    laneIdVector @11 :List(UInt64);

    histInfos @12 :List(StdHeader.HistInfo);
    junctionId @13 :UInt64;
    nodeID @14 :SDMap.NodeIDType;
}

struct TrafficSign {
    id @0 :UInt64;
    centroid @1 :Geo.Point;
    quaternion @2 :Geo.Quaternion;
    length @3 :Float32;
    width @4 :Float32;
    height @5 :Float32;
    detectionStatus @6 :UInt8;
    confidence @7 :Float32;
    laneId @8 :UInt64;

    type @9 :UInt32;
    laneIdVector @10 :List(UInt64);

    histInfos @11 :List(StdHeader.HistInfo);
    junctionId @12 :UInt64;
}

struct Section {
    id @0 :UInt64;
    sectionType @1 :UInt8;
    sectionClass @2 :UInt8;
    intersectionId @3 :UInt64;
    length @4 :Float32;

    # sorted from right to left
    laneIds @5 :List(UInt64);

    # road edge and laneline
    # There may be multiple left and right borders. For example, it is composed of lane lines and curb edges
    leftBoundaryIds @6 :List(UInt64);
    rightBoundaryIds @7 :List(UInt64);
    successorLinkIds @8 :List(UInt64);
    predecessorLinkIds @9  :List(UInt64);

    isRoutingSection @10 :Bool;

    # check the quality of this section
    quality @11 :UInt8;
}

struct SectionLink {
    id @0 :UInt64;
    fromSectionId @1 :UInt64;
    toSectionId @2 :UInt64;
    linkType @3 :UInt8;
}


struct SpeedBump {
    id @0 :UInt64;
    points @1 :List(Geo.Point);
    laneIds @2 :List(UInt64);
    localPoints @3 :List(Geo.Point);
    junctionId @4 :UInt64;
    histInfos @5 :List(StdHeader.HistInfo);
}

struct ConnectionData {
    type @0 :UInt8;
    incomeID @1 :UInt64;
    outgoingID @2 :UInt64;
    incomeNode @3 :Geo.Point;
    outgoingNode @4 :Geo.Point;
    connectedIDs @5 :List(UInt64);
}

struct Junction {
    id @0 :UInt64;
    type @1 :UInt8;
    laneConnections @2 :List(ConnectionData);
    sectionConnections @3 :List(ConnectionData);
    geometry @4 :List(Geo.Point);
}

struct RoadStruct {
    mapHeader @0 :MapHeader;
    curStatus @1 :CurrentStatus;

    # semantic map data
    lines @2 :List(LaneLine);
    stopLines @3 :List(StopLine);
    areas @4 :List(Area);
    trafficLights @5 :List(TrafficLight);
    trafficSigns @6 :List(TrafficSign);
    roadMarkers @7 :List(RoadMarker);
    crossWalks @8 :List(Crosswalk);
    poles @9 :List(Pole);

    #route map data
    lanes @10 :List(Lane);
    laneLinks @11 :List(LaneLink);
    sections @12 :List(Section);
    sectionLinks @13 :List(SectionLink);

    speedBumps @14 :List(SpeedBump);
    junctions @15 :List(Junction);

    mapQuality @16 :UInt8;
}

struct SLAMRoadStruct {
    mapHeader @0 :MapHeader;
    curStatus @1 :CurrentStatus;

    # semantic map data
    lines @2 :List(LaneLine);
    stopLines @3 :List(StopLine);
    areas @4 :List(Area);
    trafficLights @5 :List(TrafficLight);
    trafficSigns @6 :List(TrafficSign);
    roadMarkers @7 :List(RoadMarker);
    crossWalks @8 :List(Crosswalk);
    poles @9 :List(Pole);

    #route map data
    lanes @10 :List(Lane);
    laneLinks @11 :List(LaneLink);
    sections @12 :List(Section);
    sectionLinks @13 :List(SectionLink);

    speedBumps @14 :List(SpeedBump);
    junctions @15 :List(Junction);

    mapQuality @16 :UInt8;
}

struct RoadStructBin {
    mapHeader @0 :MapHeader;
    curStatus @1 :CurrentStatus;
    # Data buffer in string, include semantic map data and route map data
    data @2 :Text;
}

struct LocationPos {
    longitude @0 :Float64;
    latitude @1 :Float64;
}

struct TencentRoute {
    linkIds @0 :List(UInt64);
    routes @1 :List(LocationPos);
}
