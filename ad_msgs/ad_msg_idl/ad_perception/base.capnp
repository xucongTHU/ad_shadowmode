@0xe9df338d6596250a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

# import "ad_msg_idl/std_msgs/header.proto";
using import "/ad_msg_idl/std_msgs/typed_data_list.capnp".Float32List;

struct Point2i {
    x @0 :Int32;
    y @1 :Int32;
}

struct Point2f {
    x @0 :Float32;
    y @1 :Float32;
}
struct Point3f {
    x @0 :Float32;
    y @1 :Float32;
    z @2 :Float32;
}
struct Polygon2D {
    p2d @0 :List(Point2f);
}
struct Polygon3D {
    p3d @0 :List(Point3f);
}

struct PolygonBox {
    isValid @0 :Bool;
    polygonContour @1 :List(Point2f);
    heightLowerSurface @2 :Float32;
    heightUpperSurface @3 :Float32;
}

struct VecPoint2f {
    vector @0 :List(Point2f);
}

struct Vector3d {
    x @0 :Float64;
    y @1 :Float64;
    z @2 :Float64;
}
struct Vector3f {
    x @0 :Float32;
    y @1 :Float32;
    z @2 :Float32;
}
struct Vector4i {
    x @0 :Int32;
    y @1 :Int32;
    z @2 :Int32;
    w @3 :Int32;
}
struct Matrix3f {
    x @0 :Vector3f;
    y @1 :Vector3f;
    z @2 :Vector3f;
}

# --------------------------------------

enum TrafficSignal {
    signalNone @0;
    # #0x00
    signalGreen @1;
    # #0x02
    signalYellow @2;
    # #0x04
    signalRed @3;
    # #0x08
    signalGreenFlash @4;
    # #0x10
    signalYellowFlash @5;
    # #0x20
    signalBlack @6;
    # #0x40
    signalGreenNumber @7;
    # #0x80
}

# --------------------------------------

struct AutomobileLightStatus {
    leftTurn @0 :Int32;
    rightTurn @1 :Int32;
    hazard @2 :Int32;
    brake @3 :Int32;
    leftTurnConfidence @4 :Float32;
    rightTurnConfidence @5 :Float32;
    hazardConfidence @6 :Float32;
    brakeConfidence @7 :Float32;
}

struct BaseCameraAttribute {
    isLandmarkValid @0 :Bool;
    landmark @1 :List(Point2f);
    landmarkScores @2 :List(Float32);

    isDirectionValid @3 :Bool;
    viewportDirection @4 :Point2f;
}

struct Bbox2D {
    initialized @0 :Bool;
    topLeftX @1 :Float32;
    topLeftY @2 :Float32;
    bottomRightX @3 :Float32;
    bottomRightY @4 :Float32;
    confidence @5 :Float32;
}

struct BirdViewCorners {
    isValid @0 :Bool;
    bvCorners @1 :List(Point3f);
    cornerConf @2 :List(Float32);
}

struct Boundary {
    imagePoints @0 :List(Point2f);
    worldPoints @1 :List(Point2f);

    directionX @2 :Float32;
    directionY @3 :Float32;

    confidence @4 :Float32;

    isLine @5 :Bool;
    id @6 :Int32;
}

struct CameraBBox2DInfo {
    rawDetectionBox @0 :Bbox2D;
    trackedBox @1 :Bbox2D;
}

struct PositionInfo {
    isValid @0 :Bool;
    position @1 :Vector3f;
}

struct DepthDetPoint {
    nearestPointX @0 :Float32;
    depthPointX @1 :Float32;
    depthPointY @2 :Float32;
    orientation @3 :Int32;
    # enum DepthFineOriLabel
    label @4 :Int32;
}

struct DirectionInfo {
    isValid @0 :Bool;

    directionYpr @1 :Vector3f;
    directionYprUncertainty @2 :Matrix3f;
    yaw @3 :Float32;
    yawUncertainty @4 :Float32;
    yawRate @5 :Float32;
    yawRateUncertainty @6 :Float32;

    surface @7 :Int32;
    surfaceScore @8 :Float32;
}

struct Freespace {
    id @0 :Int32;

    imagePoints @1 :List(Point2f);
    worldPoints @2 :List(Point2f);

    width @3 :UInt32;
    height @4 :UInt32;

    bitMap @5 :List(Bool);

    confidence @6 :List(Float32);
}
struct LaneLine {
    imagePoints @0 :List(Point2f);
    worldPoints @1 :List(Point2f);
    worldPointsReproj @2 :List(Point2f);
    laneConfidence @3 :Float32;
    colorConfidence @4 :Float32;
    lineConfidence @5 :Float32;
    edgeConfidence @6 :Float32;
    fittingOnImgPlane @7 :Bool;
    # enum LaneType
    id @8 :Int32;
    trackId @9 :Int32;
    trackAge @10 :Int32;
    distanceToLeftBumper @11 :Float32;
    distanceToRightBumper @12 :Float32;
    # enum LaneCategory
    laneCategory @13 :Int32;
    # enum ColorType
    colorId @14 :Int32;
    # enum LineType
    lineId @15 :Int32;
    # enum EdgeType
    edgeId @16 :Int32;
}

struct LidarDangerousZone {
    polygon @0 :List(Point2f);

    # enum dangerous zone level
    dangerousLevel @1 :Int32;

    dzoneId @2 :UInt64;

    preDzoneIds @3 :List(UInt64);
    occluderIds @4 :List(UInt64);
    shadowIds @5 :List(UInt64);
    height @6 :Float32;

    # enum DzoneType
    type @7 :Int32;
}

struct LMLaneLine {
    enum LaneLineType {
        laneLineNoneType @0;
        singleDashed @1;
        singleSolid @2;
        leftDashedRightSolid @3;
        leftSolidRightDashed @4;
        doubleSolid @5;
        doubleDashed @6;
        sawtoothSolid @7;
        fishboneDashed @8;
        fishboneSolid @9;
        roadEdge @10;
        fence @11;
        laneLineUnknownType @12;
        lanelineTypeEnumMax @13;
    }
    closestSegmentLaneLineType @0 :LaneLineType;
    enum LaneLineColor {
        laneLineNoneColor @0;
        laneLineWhite @1;
        laneLineYelloe @2;
        laneLineUnknown @3;
        laneLineColorMax @4;
    }

    closestSegmentLaneLineColor @1 :LaneLineColor;

    closestSegmentLaneLineColorConfidence @2 :Float32;

    # poly fit function (in pixel)
    polyFitOrder @3 :UInt32;
    # coefficient, from high order to low order
    polyFitCoefficients @4 :List(Float32);

    startPoint @5 :Point2f;
    endPoint @6 :Point2f;
    junctionPoint @7 :List(LMLanePoint2f);

    laneLineWidthMeter @8 :Float32;
    trackId @9 :Int32;
}

struct LMLanePoint2f {
    x @0 :Float32;
    y @1 :Float32;
    nextSegmentLaneLineType @2 :UInt32;
    nextSegmentLaneLineColor @3 :UInt32;
    nextSegmentLaneLineColorConfidence @4 :UInt32;
}

struct LMLaneSpeedLimitSign {
    limitedSpeed @0 :Float32;
    trackId @1 :Int32;
    bbox2d @2 :Bbox2D;
}

enum TurnType {
    turntypeFree @0;     # 0xffff
    turntypeTurnLeft @1;    # 0x0001
    turntypeTurnRight @2;   # 0x0002
    turntypeLeftAround @3;  # 0x0004
    turntypeRightAround @4; # 0x0008
    turntypeStraight @5;    # 0x0010
}

struct LMLaneTurnTypeSign {
    turnType @0 :TurnType;

    trackId @1 :Int32;

    bbox2d @2 :Bbox2D;
}

struct LMPerceptionRoad {
    # send to LocalMap/SceneMapping module

    laneLines @0 :List(LMLaneLine);
    laneTurntypeSigns @1 :List(LMLaneTurnTypeSign);
    laneSpeedLimitSigns @2 :List(LMLaneSpeedLimitSign);
    speedLimitSigns @3 :List(LMSpeedLimitSign);
    regions @4 :List(LMRegion);
}

struct LMRegion {
    enum RegionType {
    regionTypeNone @0;
    diversionArea @1;
    freespace @2;
    roadHump @3;
    regionTypeUnknown @4;
    }
    regionType @0 :RegionType;
    regionTypeConfidence @1 :Float32;

    trackId @2 :Int32;
    regionBoundary @3 :List(Point2f);
    junctionPoint @4 :List(Point2f);
}

struct LMSpeedLimitSign {
    limitedSpeed @0 :Float32;
    trackId @1 :Int32;
    bbox2d @2 :Bbox2D;
}

struct MatchedSensorObjectInfo {
    # enum SensorID
    sensorId @0 :Int32;

    frameTimestampNs @1 :UInt64;
    objectId @2 :UInt64;
}

struct ModelOutputFeature {
    key @0 :Text;
    value @1 :Float32List;
}

struct MotionInfo {
    isValid @0 :Bool;

    # enum MotionStatus
    motionStatus @1 :Int32;

    center @2 :Vector3d;
    centerUncertainty @3 :Matrix3f;
    velocity @4 :Vector3d;
    velocityUncertainty @5 :Matrix3f;
    acceleration @6 :Vector3d;
    accelerationUncertainty @7 :Matrix3f;
    jerk @8 :Vector3f;
    jerkUncertainty @9 :Matrix3f;
    velocityHeading @10 :Float32;
    velocityHeadingUncertainty @11 :Float32;
    velocityHeadingRate @12 :Float32;
    velocityHeadingRateUncertainty @13 :Float32;
}

struct NonAutomobileVehicleLightStatus {
    leftTurn @0 :Int32;
    rightTurn @1 :Int32;
    brake @2 :Int32;
}

struct PedestrianIntention {
    # enum Intention
    crossing @0 :Int32;
    # enum AttentionBool
    attentionToEgoCar @1 :Int32;
}

struct RoadArea {
    image @0 :Data;
    width @1 :UInt32;
    height @2 :UInt32;
}

struct RoadStruct {
    lanelineResults @0 :List(LaneLine);
    stoplineResults @1 :List(StopLine);
    boundaryResults @2 :List(Boundary);
    freespaceResults @3 :List(Freespace);
}

struct SizeInfo {
    isValid @0 :Bool;
    size @1 :Vector3f;
    sizeUncertainty @2 :Matrix3f;
}

struct StopLine {
    imagePoints @0 :List(Point2f);
    worldPoints @1 :List(Point2f);
    confidence @2 :List(Float32);
    id @3 :Int32;
}

struct TLImage2DInfo {
    cameraName @0 :Text;
    detectedBbox @1 :Bbox2D;
    isProjectionValid @2 :Bool;
    projectedCenterPoint @3 :Point2f;
}

struct TrafficLight {
    # general
    timestampNs @0 :UInt64;

    id @1 :Text;
    groupId @2 :Text;

    # TrafficSignal
    colorLabel @3 :TrafficSignal;

    # TurnType
    turnType @4 :TurnType;

    duringTime @5 :Float32;

    # image info
    image2dInfo @6 :List(TLImage2DInfo);

    # geometry
    worldGeometry @7 :List(Vector3f);
    localGeometry @8 :Vector3f;
}

struct TrafficLightGroup {
    trafficLights @0 :List(TrafficLight);

    groupId @1 :Text;

    # TrafficSignal
    colorLabel @2 :TrafficSignal;

    # TurnType
    turnType @3 :TurnType;

    duringTime @4 :Float32;
}
