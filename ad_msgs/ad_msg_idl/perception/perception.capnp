@0x9be4970c7808281c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::perception");
using Header = import "/ad_msg_idl/std_msgs/header.capnp";
using Geometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct CameraDebugInfo {
    depthType @0 :Int16;
    
    opticalFlowPoints @1 :List(Point2f);
    
    opticalFlowDxy @2 :List(Point2f);
    
}
struct Crosswalk {
    id @0 :UInt64;
    
    laneId @1 :List(UInt64);
    
    confidence @2 :List(Vectorf);
    
    imagePoints @3 :List(Polygon2D);
    
    worldPoints @4 :List(Polygon2D);
    
    width @5 :UInt16;
    
    height @6 :UInt16;
    
    bitMap @7 :List(Bool);

    histInfoVector @8 :List(Header.HistInfo);
    
}
struct RoadMarker {
    id @0 :UInt64;
    confidence @1 :List(Float32);
    type @2 :UInt32;
    imagePointNum @3 :UInt64;
    imagePoints @4 :List(Point2f);
    worldPointNum @5 :UInt64;
    worldPoints @6 :List(Point2f);
    confNum @7 :UInt64;
    keyPointTypesNum @8 :UInt8; 
    keyPointTypes @9 :List(Int8);
    heading @10 :Float32;
}
struct SpeedBump {
    id @0 :UInt64;
    
    imagePoints @1 :List(Point2f);
    
    worldPoints @2 :List(Point2f);
    
    confidence @3 :List(Float32);
    
    histInfoVector @4 :List(Header.HistInfo);
}
struct Polygon2D {
    p2d @0 :List(Point2f);
    
}
struct BirdViewCorners {
    isValid @0 :Bool;
    
    bvCorners @1 :List(Point3f);
    
    cornerConf @2 :List(Float32);
    
}
struct MatchedSensorObjectInfo {
    sensorId @0 :Int16;
    # enum SensorID
    frameTimestampNs @1 :UInt64;
    
    objectId @2 :UInt64;
    
}
struct BlobFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    # enum SensorID
    blobNum @3 :Int16;
    
    blobObjectList @4 :List(Blob);
    
}
struct TrafficSignObject {
    label @0 :Int16;
    ## TrafficSignObject
    # enum ObjectLabel
    typeConfidence @1 :Float32;
    # confidence
    existenceConfidence @2 :Float32;
    
    trackId @3 :Int32;
    # id
    cameraBboxInfo @4 :CameraBBox2DInfo;
    # CameraBBox2DInfo

    histInfoVector @5 :List(Header.HistInfo);
}
struct StopLine {
    imagePoints @0 :List(Point2f);
    
    worldPoints @1 :List(Point2f);
    
    confidence @2 :List(Float32);
    
    id @3 :Int32;

    histInfoVector @4 :List(Header.HistInfo);
    
}
struct RoadAreaFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :Int64;
    
    cameraName @2 :Text;
    
    roadArea @3 :RoadArea;
    
}
struct CameraBlockageInfo {
    blockageType @0 :UInt8;
    # enum blockage_type
    # uint8 UNKNOWN = 0
    # uint8 CLEAR = 1
    # uint8 GLARED = 2
    # uint8 BLOCKED = 3
    # uint8 PARTIAL_BLOCKED = 4
    glareStatus @1 :UInt8;
    # enum glare_status
    # uint8 UNKNOWN = 0
    # uint8 CLEAR = 1
    # uint8 GLARED = 2
    sensorId @2 :Int16;
    # enum SensorID
}
struct FailsafeFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    # enum SensorID
    blockageInfo @3 :CameraBlockageInfo;
    blockageInfoList @4 :List(CameraBlockageInfo);

}
struct TrafficLightObject {
    label @0 :Int16;
    ## TrafficLightObject
    # enum ObjectLabel
    statusLabel @1 :UInt16;
    # enum StatusLabel
    colorLabel @2 :UInt16;
    # status definition:
    # 0 BLACK,
    # 1 GREEN,
    # 2 YELLOW,
    # 3 RED,
    # enum ColorLabel
    typeLabel @3 :UInt16;
    # color definition:
    # NONE = 0x00,
    # GREEN = 0x02,
    # YELLOW = 0x04,
    # RED = 0x08,
    # GREEN_FLASH = 0x10,
    # YELLOW_FLASH = 0x20,
    # BLACK = 0x40,
    # GREEN_NUMBER = 0x80
    # enum TypeLabel
    typeConfidence @4 :Float32;
    # type definition
    # 0 UNDEFINE,
    # 1 NONE,
    # 2 CIRCLE,
    # 3 PEDESTRIAN ,
    # 4 TURN_LEFT,
    # 5 STRAIGHT,
    # 6 TURN_RIGHT,
    # 7 TURN_AROUND,
    # 8 TURN_LEFT_STRAIGHT,
    # 9 TURN_LEFT_AROUND,
    # 10 TURN_RIGHT_STRAIGHT,
    # 11 BIKE，
    # 12 X_ENTER
    # 13 NUMBER
    # 14 ENTER
    # confidence
    existenceConfidence @5 :Float32;
    
    trackId @6 :Int32;
    # id
    cameraBboxInfo @7 :CameraBBox2DInfo;
    # CameraBBox2DInfo
    tlNum @8 :Int16;
    # light num

    histInfoVector @9 :List(Header.HistInfo);
    focus @10 :Int16;
    # light focus
}
struct Vector3d {
    x @0 :Float64;
    
    y @1 :Float64;
    
    z @2 :Float64;
    
}
struct MotionInfo {
    isValid @0 :Bool;
    
    motionStatus @1 :Int16;
    # enum MotionStatus
    center @2 :Vector3d;
    
    centerUncertainty @3 :Matrix3f;
    
    velocity @4 :Vector3f;
    
    velocityUncertainty @5 :Matrix3f;
    
    acceleration @6 :Vector3f;
    
    accelerationUncertainty @7 :Matrix3f;
    
    jerk @8 :Vector3f;
    
    jerkUncertainty @9 :Matrix3f;
    
    velocityHeading @10 :Float32;
    
    velocityHeadingUncertainty @11 :Float32;
    
    velocityHeadingRate @12 :Float32;
    
    velocityHeadingRateUncertainty @13 :Float32;

    relativeVelocity @14 :Vector3f;
}
struct RoadArea {
    image @0 :List(UInt8);
    
    width @1 :UInt16;
    
    height @2 :UInt16;
    
}
struct Point2f {
    x @0 :Float32;
    
    y @1 :Float32;
    
}

struct Point3d {
    x @0 :Float64;

    y @1 :Float64;

    z @2 :Float64;
}

struct GeneralMergeInfo {
    isValid @0 :Bool;
    
    mergeState @1 :Int16;
    # enum SplitMergeState
    mergeId @2 :UInt64;
    
    splitId @3 :UInt64;
    
}
struct RoadSemanticsFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    # enum SensorID
    sensorName @3 :Text;
    # sensor name
    roadSemantics @4 :RoadSemantics;
    
}
struct RoadSemantics {
    trafficLightResults @0 :List(TrafficLightObject);
    
    trafficSignResults @1 :List(TrafficSignObject);
    
    roadMarkerResults @2 :List(RoadMarkerObject);
    
    boardPoleResults @3 :List(BoardPoleObject);
    
    staticObstacleResults @4 :List(Object);
}
struct LMRegion {
    regionType @0 :UInt8;
    
    regionTypeConfidence @1 :Float32;
    #enum region type
    #uint8 REGION_TYPE_NONE=0
    #uint8 DIVERSION_AREA=1
    #uint8 FREESPACE=2
    #uint8 ROAD_HUMP=3
    #uint8 REGION_TYPE_UNKNOWN=3
    trackId @2 :Int16;
    
    regionBoundary @3 :List(Point2f);
    
    junctionPoint @4 :List(Point2f);
    
}
struct BoardPoleObject {
    label @0 :Int16;
    # enum ObjectLabel
    trackId @1 :Int32;
    # id
    typeConfidence @2 :Float32;
    # confidence
    existenceConfidence @3 :Float32;
    
    bbox2dInfo @4 :CameraBBox2DInfo;
    # CameraBBox2DInfo

    histInfoVector @5 :List(Header.HistInfo);
}
struct ObjectFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    # enum SensorID
    perceptionObjectList @3 :List(Object);
    
}

struct Gop {
    sensorId @0 :Int16;
    ## BaseObject
    # enum SensorID
    label @1 :Int16;
    # enum ObjectLabel

    existenceConfidence @2 :Float32;

    # ObjectLabel
    # int16 UNKNOWN=0
    # int16 OBSTACLE_ALL=136
    # int16 OBSTACLE_END=150

    subtype @3 :Int16;

    trackId @4 :Int32;

    currentPredictionTimestampNs @5 :UInt64;

    sizeInfo @6 :SizeInfo;

    polygonBox @7 :PolygonBox;

    directionInfo @8 :DirectionInfo;

    motionInfo @9 :MotionInfo;

    motionStatus @10 :Int16;

    motionUnknownScore @11 :Float32;
    # motion_status enum
    # int16 INVALID_STATUS = -1
    # int16 UNKNOWN = 0
    # int16 STATIC = 1
    # int16 STOPPED = 2
    # int16 MOVING = 3
    # int16 CROSSING = 4
    motionStaticScore @12 :Float32;

    motionStoppedScore @13 :Float32;

    motionMovingScore @14 :Float32;

    motionCrossingScore @15 :Float32;

    cameraBboxInfo @16 :CameraBBox2DInfo;
    # CameraBBox2DInfo
    bboxMotionInfo @17 :Bbox2DMotionInfo;
    # Bbox2DMotionInfo

}

struct GopFrame {
    header @0 :Header.Header;
    frameTimestampNs @1 :UInt64;
    # enum SensorID
    sensorId @2 :Int16;

    perceptionGopList @3 :List(Gop);

}

struct ShadowModeFrame {
    header @0 :Header.Header;
    frameTimestampNs @1 :UInt64;
    sensorId @2 :Int16;

    perceptionTagList @3 :List(Tag);
}

struct Tag {
    sensorId @0 :Int16;
    trackId @1 :Int32;
    label @2 :Int16;
    matchTimestampNsList @3 :List(UInt64);
    shadowModeTypeList @4 :List(Int16);
    tagNameList @5 :List(Int16);
}

struct BaseCameraAttribute {
    isLandmarkValid @0 :Bool;
    
    landmark @1 :List(Point2f);
    
    landmarkScores @2 :List(Float32);
    
    isDirectionValid @3 :Bool;
    
    viewportDirection @4 :Point2f;
    
}
struct DirectionInfo {
    isValid @0 :Bool;
    
    directionYpr @1 :Vector3f;
    
    directionYprUncertainty @2 :Matrix3f;
    
    yaw @3 :Float32;
    
    yawUncertainty @4 :Float32;
    
    yawRate @5 :Float32;
    
    yawRateUncertainty @6 :Float32;
    
}
struct AutomobileLightStatus {
    leftTurn @0 :Int16;
    
    rightTurn @1 :Int16;
    
    hazard @2 :Int16;
    
    brake @3 :Int16;
    
    leftTurnConfidence @4 :Float32;
    
    rightTurnConfidence @5 :Float32;
    
    hazardConfidence @6 :Float32;
    
    brakeConfidence @7 :Float32;
    
}
struct LMLaneLine {
    closestSegmentLaneLineType @0 :UInt8;
    
    closestSegmentLaneLineColor @1 :UInt8;
    #enum lane line type
    #uint8 LANE_LINE_NONE_TYPE=0
    #uint8 SINGLE_DASHED=1
    #uint8 SINGLE_SOLID=2
    #uint8 LEFT_DASHED_RIGHT_SOLID=3
    #uint8 LEFT_SOLID_RIGHT_DASHED=4
    #uint8 DOUBLE_SOLID=5
    #uint8 DOUBLE_DASHED=6
    #uint8 SAWTOOTH_SOLID=7
    #uint8 FISHBONE_DASHED=8
    #uint8 FISHBONE_SOLID=9
    #uint8 ROAD_EDGE=10
    #uint8 FENCE=11
    #uint8 LANE_LINE_UNKNOWN_TYPE=12
    #uint8 LANELINE_TYPE_ENUM_MAX=13
    closestSegmentLaneLineColorConfidence @2 :Float32;
    #enum lane line color
    #uint8 LANE_LINE_NONE_COLOR=0
    #uint8 LANE_LINE_WHITE=1
    #uint8 LANE_LINE_YELLOE=2
    #uint8 LANE_LINE_UNKNOWN=2
    #uint8 LANE_LINE_COLOR_MAX=3
    polyFitOrder @3 :UInt8;
    # poly fit function (in pixel)
    polyFitCoefficients @4 :List(Float32);
    # coefficient, from high order to low order
    startPoint @5 :Point2f;
    
    endPoint @6 :Point2f;
    
    junctionPoint @7 :List(LMLanePoint2f);
    
    laneLineWidthMeter @8 :Float32;
    
    trackId @9 :Int16;
    
    laneConfidence @10 :Float32;
    # lane confidence
    imagePoints @11 :List(Point2f);
    # points
    worldPoints @12 :List(Point2f);
    
    worldPointsReproj @13 :List(Point2f);
    
}
struct Vectorf {
    vector @0 :List(Float32);
    
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

struct TurnType {
    const turntypeFree :UInt16 = 65535;
    const turntypeTurnLeft :UInt16 = 1;
    const turntypeTurnRight :UInt16 = 2;
    const turntypeLeftAround :UInt16 = 4;
    const turntypeRightAround :UInt16 = 8;
    const turntypeStraight :UInt16 = 16;
}
struct Bbox2DMotionInfo {
    widthVelocity @0 :Float32;
    
    heightVelocity @1 :Float32;
    
    widthHeightRatioVelocity @2 :Float32;
    
    bottomCenterXVelocity @3 :Float32;
    
    bottomCenterYVelocity @4 :Float32;
    
}
struct ModelOutputFeature {
    key @0 :Text;
    
    value @1 :List(Float32);
    
}
struct EnvODDFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    # enum SensorID
    sceneInfo @3 :SceneInfo;
    
    cityLightsInfo @4 :Int8;
    # enum BlobBrightnessState
    # uint8 UNKNOWN = 0
    # uint8 BRIGHT = 1
    # uint8 DARK = 2
    blobNum @5 :Int16;
    
    blobObjectList @6 :List(Blob);
    
}
struct NonAutomobileVehicleLightStatus {
    leftTurn @0 :Int16;
    
    rightTurn @1 :Int16;
    
    brake @2 :Int16;
    
}
struct Point3f {
    x @0 :Float32;
    
    y @1 :Float32;
    
    z @2 :Float32;
    
}
struct LMSpeedLimitSign {
    limitedSpeed @0 :Float32;
    
    trackId @1 :Int16;
    
    bbox2d @2 :Bbox2D;
    
}
struct Polygon3D {
    p3d @0 :List(Point3f);
    
}
struct LidarDangerousZone {
    polygon @0 :List(Point2f);
    
    dangerousLevel @1 :Int16;
    # enum dangerous zone level
    dzoneId @2 :UInt64;
    
    preDzoneIds @3 :List(UInt64);
    
    occluderIds @4 :List(UInt64);
    
    shadowIds @5 :List(UInt64);
    
    height @6 :Float32;
    
    type @7 :Int16;
    # enum DzoneType
}
struct RoadMarkerObject {
    label @0 :Int16;
    # enum ObjectLabel
    trackId @1 :Int32;
    # id
    typeConfidence @2 :Float32;
    # confidence
    existenceConfidence @3 :Float32;
    
    bbox2dInfo @4 :CameraBBox2DInfo;
    # CameraBBox2DInfo

    histInfoVector @5 :List(Header.HistInfo);
}
struct LaneLine {
    id @0 :Int16;
    ## LaneLine
    # enum LaneType
    # int16 HOST_LEFT = 0
    # int16 HOST_RIGHT = 1
    # int16 LEFT_NEIGHBOR_LANE_LEFT = 2
    # int16 LEFT_NEIGHBOR_LANE_RIGHT = 3
    # int16 RIGHT_NEIGHBOR_LANE_LEFT = 4
    # int16 RIGHT_NEIGHBOR_LANE_RIGHT = 5
    # int16 REAR_RIGHT = 6
    # int16 REAR_LEFT = 7
    # int16 ROAD_EDGE_LEFT = 8
    # int16 ROAD_EDGE_RIGHT = 9
    # int16 INVALID = 255
    # laneline id
    trackId @1 :Int32;
    # laneline track id
    trackAge @2 :Int32;
    # laneline tracking age
    distanceToLeftBumper @3 :Float32;
    
    distanceToRightBumper @4 :Float32;
    # laneline_width [cm]
    lanelineWidth @5 :Float32;
    # laneline_width
    colorId @6 :UInt8;
    # laneline description
    # enum ColorType
    # uint8 NO_COLOR_LINE = 0
    # uint8 OTHER_COLOR = 1
    # uint8 WHITE_COLOR = 2
    # uint8 YELLOW_COLOR = 3
    # uint8 BLUE_COLOR = 4
    # uint8 NUM_COLORTYPES_MAX = 5
    lanelineTypeId @7 :UInt8;
    # enum laneline type
    # uint8 LANE_LINE_NONE_TYPE = 0
    # uint8 SINGLE_DASHED = 1
    # uint8 SINGLE_SOLID = 2
    # uint8 LEFT_DASHED_RIGHT_SOLID = 3
    # uint8 LEFT_SOLID_RIGHT_DASHED = 4
    # uint8 DOUBLE_SOLID = 5
    # uint8 DOUBLE_DASHED = 6
    # uint8 SAWTOOTH_SOLID = 7
    # uint8 FISHBONE_DASHED = 8
    # uint8 FISHBONE_SOLID = 9
    # uint8 ROAD_EDGE = 10
    # uint8 FENCE = 11
    # uint8 SHORT_THICK_DASHED = 12
    # uint8 LANE_LINE_UNKNOWN_TYPE = 13
    # uint8 LANELINE_TYPE_ENUM_MAX = 14
    edgeTypeId @8 :UInt8;
    # enum EdgeType
    # uint8 NO_EDGE_LINE = 0
    # uint8 NOT_EDGE = 1
    # uint8 OTHER_EDGE = 2
    # uint8 CURBSTONE_EDGE = 3
    # uint8 FENCE_EDGE = 4
    # uint8 WALL_EDGE = 5
    # uint8 VIRTUAL_EDGE = 6
    # uint8 NUM_EDGETYPES_MAX = 7
    laneConfidence @9 :Float32;
    # laneline attribute confidence
    colorConfidence @10 :Float32;
    
    lanelineTypeConfidence @11 :Float32;
    
    edgeTypeConfidence @12 :Float32;
    
    lanelineWidthConfidence @13 :Float32;
    
    startPoint @14 :Point2f;
    # bv laneline start/end point
    endPoint @15 :Point2f;
    
    imagePoints @16 :List(Point2f);
    # image points
    worldPointsReproj @17 :List(Point2f);
    # bv sampled points, for HMI
    fittingDegree @18 :Int16;
    # LaneFittingResult
    # PolynomialFittingResult
    # polynomial fitting degree
    polyfitCoefficients @19 :List(Float32);
    # polynomial fitting coefficients
    junctionPoints @20 :List(LMLanePoint2f);
    # junction of different line types
    isLineJunctionValid @21 :Bool;
    
    laneCategory @22 :Int16;
    # tmp fix decision_wrapper compile error in L2 branch, TODO delete this
    lineTypeId @23 :UInt8;
    
    lineTypeConfidence @24 :Float32;
    
    fittingType @25 :Int16;
    
    # laneline_width_left [cm]
    lanelineWidthLeft @26 :Float32;

    # laneline_width_right [cm]
    lanelineWidthRight @27 :Float32;

    histInfoVector @28 :List(Header.HistInfo);
}
struct SizeInfo {
    isValid @0 :Bool;
    
    size @1 :Vector3f;
    
    sizeUncertainty @2 :Matrix3f;
    
}
struct Matrix3f {
    x @0 :Vector3f;
    
    y @1 :Vector3f;
    
    z @2 :Vector3f;
    
}
struct Vector4i {
    x @0 :Int32;
    
    y @1 :Int32;
    
    z @2 :Int32;
    
    w @3 :Int32;
    
}

struct SlotLimiter {
    limiterPoints @0 :List(Point2f);
    confidence @1 :Float32;
    limiterStatus @2 :UInt32;
}

struct SlotParkingLock {
    parkingLockPoint @0 :Point2f;
    confidence @1 :Float32;
    parkingLockStatus @2 :UInt32;
}

struct ParkingSlot {
    id @0 :UInt32;
    confidence @1 :Float32;
    width @2 :Float32;
    length @3 :Float32;
    center @4 :Point2f;
    wideDirection @5 :Point2f;
    longDirection @6 :Point2f;
    isOccupied @7 :Bool;
    type @8 :UInt32;
    slotPoints @9 :List(Geometry.Point);
    slotLimiter @10 :SlotLimiter;
    slotParkingLock @11 :SlotParkingLock;
}

struct ParkingSlotsFrame {
    header @0 :Header.Header;
    frameTimestampNs @1 :UInt64;
    sensorId @2 :Int16;
    sensorName @3 :Text;
    slots @4 :List(ParkingSlot);
}

struct RoadGeometry {
    lanelineResults @0 :List(LaneLine);
    
    stoplineResults @1 :List(StopLine);
    
    freespaceResults @2 :List(Freespace);
    
    crosswalkResults @3 :List(Crosswalk);

    speedBumpResults @4 :List(SpeedBump);

    holisticPathResults @5 :List(HolisticPath);

    centerlineResults @6 :List(CenterLine);

    roadMarkerResults @7 :List(RoadMarker);
    
}
struct LidarDangerousZoneFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    # enum SensorID
    height @3 :Float64;
    
    type @4 :Int16;
    # enum DangerousZoneType
    dzones @5 :List(LidarDangerousZone);
    
}
struct TrafficLight {
    timestampNs @0 :UInt64;
    # general
    id @1 :Text;
    
    groupId @2 :Text;
    
    colorLabel @3 :UInt16;
    # TrafficSignal
    turnType @4 :UInt16;
    # TurnType
    duringTime @5 :Float32;
    
    image2dInfo @6 :List(TLImage2DInfo);
    # image info
    worldGeometry @7 :List(Vector3f);
    # geometry
    localGeometry @8 :Vector3f;
    
}
struct RoadStruct {
    lanelineResults @0 :List(LaneLine);
    
    stoplineResults @1 :List(StopLine);
    
    boundaryResults @2 :List(Boundary);
    
    freespaceResults @3 :List(Freespace);
    
    trafficLightResults @4 :List(TrafficLightObject);
    
    trafficSignResults @5 :List(TrafficSignObject);
    
}
struct Bbox2D {
    initialized @0 :Bool;
    
    topLeftX @1 :Float32;
    
    topLeftY @2 :Float32;
    
    bottomRightX @3 :Float32;
    
    bottomRightY @4 :Float32;
    
    confidence @5 :Float32;
    
    isLeftSideTruncated @6 :Bool;
    
    isRightSideTruncated @7 :Bool;
    
    isTopSideTruncated @8 :Bool;
    
    isBottomSideTruncated @9 :Bool;
    
}

struct Internal2DBBox {
    sensorId @0 :Int16;

    topLeftX @1 :Float32;
    
    topLeftY @2 :Float32;
    
    bottomRightX @3 :Float32;
    
    bottomRightY @4 :Float32;
    
    isLeftSideTruncated @5 :Bool;
    
    isRightSideTruncated @6 :Bool;
    
    isTopSideTruncated @7 :Bool;
    
    isBottomSideTruncated @8 :Bool;
}

struct SceneInfo {
    sceneBrightnessInfo @0 :UInt8;
    # enum SceneBrightness
    # uint8 UNKNOWN = 0
    # uint8 DAYTIME = 1
    # uint8 NIGHT_BRIGHT = 2
    # uint8 NIGHT_DARK = 3
    sceneWeatherInfo @1 :UInt8;
    # enum WeatherSceneInfo
    # uint8 UNKNOWN = 0
    # uint8 OVERCAST = 1
    # uint8 SUNNY = 2
    # uint8 RAINY = 3
    # uint8 FOGGY = 4
    # uint8 SNOWY = 5
    # uint8 SANDY = 6
    # uint8 OTHERS = 7
    sceneEnvironmentInfo @2 :UInt8;
    # enum SceneEnvironmentInfo
    # uint8 UNKNOWN = 0
    # uint8 OPEN_ROAD = 1
    # uint8 CLOSED_ROAD = 2
    # uint8 TUNNEL = 3
}
struct AutomobileDoorStatus {
    # DEFAULT:-2
    # UNKNOWN:-1
    # NEGATIVE：0
    # POSITIVE：1
    bboxValidity @0 :Int8;
    # DEFAULT:-2
    # UNKNOWN:-1
    # CLOSED：0
    # OPEN：1
    leftDoorStatus @1 :Int8;
    # DEFAULT:-2
    # UNKNOWN:-1
    # CLOSED：0
    # OPEN：1
    rightDoorStatus @2 :Int8;
    # DEFAULT:-2
    # UNKNOWN: -1
    # CLOSED:0
    # OPEN_UP_OR_DOWN:1
    # OPEN_LEFT:2
    # OPEN_RIGHT:3
    # OPEN_LEFT_AND_RIGHT:4
    rearDoorStatus @3 :Int8;
}

struct Object {
    sensorId @0 :Int16;
    ## BaseObject
    # enum SensorID
    label @1 :Int16;
    # enum ObjectLabel
    typeConfidence @2 :Float32;
    
    existenceConfidence @3 :Float32;
    
    generalObjectProbilibty @4 :Float32;
    
    # ObjectLabel
    # int16 UNKNOWN=0
    # int16 PEDESTRIAN=1
    # int16 VEHICLE=2
    # int16 VEHICLE_CAR=3
    # int16 VEHICLE_SUV=4
    # int16 VEHICLE_VAN=5
    # int16 VEHICLE_TRUCK=6
    # int16 VEHICLE_PICKUP_TRUCK=7
    # int16 VEHICLE_BUS=8
    # int16 VEHICLE_TAXI=9
    # int16 VEHICLE_EMERGENCY=10
    # int16 VEHICLE_SCHOOL_BUS=11
    # int16 VEHICLE_OTHERS=12
    # int16 VEHICLE_END=13
    # int16 BIKE=14
    # int16 NO_PERSON_VEHICLE=15
    # int16 BIKE_BICYCLE=16
    # int16 BIKE_BIKEBIG=17
    # int16 BIKE_BIKESMALL=18
    # int16 BIKE_END=19
    # int16 TRAFFICLIGHT_ALL=20
    # int16 TRAFFICLIGHT_END=64
    # int16 TRAFFICSIGN_ALL=65
    # int16 TRAFFICSIGN_END=135
    # int16 OBSTACLE_ALL=136
    # int16 OBSTACLE_END=150
    # int16 ROADMARKER_ALL=151
    # int16 ROADMARKER_END=162

    subtype @5 :Int16;
    subTypeProbs @6 :List(Float32);
    
    trackId @7 :Int32;
    
    trackAgeTimeSecond @8 :Float32;
    
    lastMeasureUpdateTimestampNs @9 :UInt64;
    
    currentPredictionTimestampNs @10 :UInt64;
    
    sizeInfo @11 :SizeInfo;
    
    polygonBox @12 :PolygonBox;
    
    directionInfo @13 :DirectionInfo;
    
    bvCorners @14 :BirdViewCorners;
    
    motionInfo @15 :MotionInfo;
    
    motionStatus @16 :Int16;
    
    motionUnknownScore @17 :Float32;
    # motion_status enum
    # int16 INVALID_STATUS = -1
    # int16 UNKNOWN = 0
    # int16 STATIC = 1
    # int16 STOPPED = 2
    # int16 MOVING = 3
    # int16 CROSSING = 4
    motionStaticScore @18 :Float32;
    
    motionStoppedScore @19 :Float32;
    
    motionMovingScore @20 :Float32;
    
    motionCrossingScore @21 :Float32;
    
    maintenanceStatus @22 :Int16;
    # enum MaintenanceStatus
    occlusionStatus @23 :Int16;
    # enum OcclusionStatus
    sensorSource @24 :Int16;
    # enum SensorSource
    generalMergeInfo @25 :GeneralMergeInfo;
    
    feature @26 :List(ModelOutputFeature);
    # ModelOutputFeatures
    cameraBboxInfo @27 :CameraBBox2DInfo;
    # CameraBBox2DInfo
    bboxMotionInfo @28 :Bbox2DMotionInfo;
    # Bbox2DMotionInfo
    enum DerivedType {
    # Derived types
    # only for visualization
    ## Shared: NonAutomobileVehicle Pedestrian Automobile
        cameraNonVehicle @0;
        cameraPedestrian @1;
        cameraVehicle @2;
        cameraStaticObstacle @3;
        lidarObject @4;
        radarObject @5;
        fusionObject @6;
        cameraTrafficlight @7;
        cameraTrafficsign @8;
    }
    derivedtype @29 :DerivedType;
    attribute @30 :BaseCameraAttribute;
    
    automobileLightStatus @31 :AutomobileLightStatus;
    ## Shared: Automobile FusionObject
    isRightEdgeDetected @32 :Bool;
    ## Static Obstacle Specific
    rightEdgeLine @33 :Vector4i;
    
    nonAutomobileVehicleLightStatus @34 :NonAutomobileVehicleLightStatus;
    ## NonAutomobileVehicle Specific
    # enum NonAutomobileVehicleLightStatus
    isWithPed @35 :Bool;
    
    umbrella @36 :Int8;
    ## Pedestrian Specific
    # action enum AttrBool
    call @37 :Int8;
    
    phone @38 :Int8;
    
    wave @39 :Int8;
    
    standing @40 :Int8;
    
    upperBodyDirection @41 :Point2f;
    # direction
    lowerBodyDirection @42 :Point2f;
    
    faceDirection @43 :Point2f;
    
    intention @44 :PedestrianIntention;
    # intention
    isOnBike @45 :Bool;
    
    depthPoint @46 :DepthDetPoint;
    ## Automobile Specific
    odRdInfo @47 :ObjectLaneAssignments;
    ## Object LaneAssignments
    debugInfo @48 :CameraDebugInfo;
    ## CameraObject Specific
    pointCloudTimestampNs @49 :UInt64;
    ## LidarObject Specific
    trackRange @50 :Float64;
    ## RadarObject Specific
    trackRangeRate @51 :Float64;
    
    trackRangeAccelRate @52 :Float64;
    
    trackLatRate @53 :Float64;
    
    matchedSensorObjectInfo @54 :List(MatchedSensorObjectInfo);
    ## FusionObject Specific
    colorLabel @55 :UInt16;
    ## TrafficLight Specific, only for visualization

    ## Door status for Automobile vehicles
    automobileDoorStatus @56 :AutomobileDoorStatus;

    ## record history object time msg
    histInfo @57 :List(Header.HistInfo);

    ## track status
    trackStatus @58 :UInt8;

    sensorIdBox @59 :List(Internal2DBBox);

    #still status，value为UNKNOWN = -1, STATIC = 0, DYNAMIC = 1
    motionState @60 :Int8;

    #CarDoorState value为UNKNOWN = -1, CLOSE = 0, LEFTOPEN = 1, RIGHTOPEN = 2,OPEN = 3
    carDoorState  @61 :Int8;
}

struct FSLinePt {
  v @0 :Int16;
  label @1 :UInt8;
  isValid @2: Bool;
}

struct FSLineResult {
  fsLinePts @0 :List(FSLinePt);
  timestampNs @1 :UInt64;
  isValid @2 :Bool;
}

struct FSLineFrame {
  header @0 :Header.Header;
  fisheyeFsLineResults @1 :List(FSLineResult);  # front->left->rear->right
}

struct Freespace {
    id @0 :Int32;
    
    imagePoints @1 :List(Point2f);
    
    worldPoints @2 :List(Point2f);
    
    cameraOpticalCenter @3 :Point2f;
    
    angleResults @4 :List(Float32);
    
    rangeResults @5 :List(Float32);
    
    confidence @6 :List(Float32);

    col @7 :Int32;

    row @8 :Int32;

    resolution @9 :Float32;

    # bitMap.size() = row * col
    occupancyGrid @10 :List(UInt8);

    width @11 :UInt16;

    height @12 :UInt16;

    bitMap @13 :List(Bool);

    histInfoVector @14 :List(Header.HistInfo);
}
struct PedestrianIntention {
    crossing @0 :Int16;
    # enum Intention
    attentionToEgoCar @1 :Int16;
    # enum AttentionBool
}
struct DepthDetPoint {
    nearestPointX @0 :Float32;
    
    depthPointX @1 :Float32;
    
    depthPointY @2 :Float32;
    
    landmarkFrontPoint @3 :Point2f;
    
    landmarkRearPoint @4 :Point2f;
    
    orientation @5 :Int32;
    
    label @6 :Int16;
    # enum DepthFineOriLabel
}
struct TLImage2DInfo {
    cameraName @0 :Text;
    
    detectedBbox @1 :Bbox2D;
    
    isProjectionValid @2 :Bool;
    
    projectedCenterPoint @3 :Point2f;
    
}
struct TrafficLightSignFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :Int64;
    
    sensorId @2 :Int16;
    # enum SensorID
    trafficSign @3 :List(TrafficSign);
    # traffic sign
    trafficLightGroupList @4 :List(TrafficLightGroup);
    # traffic light
    cameraNameList @5 :List(Text);
    
}
struct Blob {
    objectTrackId @0 :UInt64;
    ## Blob
    # BlobObjectGeneral
    obejctPairedId @1 :UInt64;
    
    objectCyclesTraced @2 :UInt16;
    
    detBbox2d @3 :Bbox2D;
    # BlobObjectKinematicWorld
    posX @4 :Float32;
    
    posY @5 :Float32;
    
    posZ @6 :Float32;
    
    distance @7 :Float32;
    
    leftAngle @8 :Float32;
    
    rightAngle @9 :Float32;
    
    topAngle @10 :Float32;
    
    bottomAngle @11 :Float32;
    
    classConfidence @12 :Float32;
    # BlobObjectAttribute
    blobType @13 :UInt8;
    # enum BlobType
    #uint8 UNKNOWN=0
    #uint8 HEADLIGHT=1
    #uint8 TAILLIGHT=2
    #uint8 STREETLAMP=3
    #uint8 TOPLIGHT=4
    #uint8 FOGLIGHT=5
    #uint8 BREAKLIGHT=6
    blobStatus @14 :UInt8;
    # BlobStatus
    #uint8 UNKNOWN=0
    #uint8 STATIC=1
    #uint8 DYNAMIC=2
    oncomingVehicleProbability @15 :UInt8;
    
    preceedingVehicleProbability @16 :UInt8;
    
    objectId @17 :UInt64;
    # TODO delete, for compile errors
    objectIdPaired @18 :UInt64;
    
    intensity @19 :UInt16;
    
    luminousFlux @20 :Float32;
    
    objectType @21 :Int16;
    
    probabilityVehicleOncoming @22 :UInt8;
    
    probabilityVehiclePreceeding @23 :UInt8;
    
}
struct RoadFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :Int64;
    
    sensorId @2 :Int16;
    # enum SensorID
    sensorName @3 :Text;
    # sensor name
    roadStruct @4 :RoadStruct;
    
}
struct TrafficSignal {

    const signalNone :UInt8 = 0;
    const signalGreen :UInt8 = 2;
    const signalYellow :UInt8 = 4;
    const signalRed :UInt8 = 8;
    const signalGreenFlash :UInt8 = 16;
    const signalYellowFlash :UInt8 = 32;
    const signalBlack :UInt8 = 64;
    const signalGreenNumber :UInt8 = 128;
}
struct ObjectLaneAssignments {
    laneAssignments @0 :List(Int16);
    ## ObjectLaneInfo
}
struct RoadGeometryFrame {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    # enum SensorID
    sensorName @3 :Text;
    # sensor name
    roadGeometry @4 :RoadGeometry;
    
}
struct VecPoint2f {
    vector @0 :List(Point2f);
    
}
struct Vector3f {
    x @0 :Float32;
    
    y @1 :Float32;
    
    z @2 :Float32;
    
}
struct TrafficLightGroup {
    trafficLights @0 :List(TrafficLight);
    
    groupId @1 :Text;
    
    colorLabel @2 :UInt8;
    # TrafficSignal
    turnType @3 :UInt16;
    # TurnType
    duringTime @4 :Float32;
    
}
struct LMLaneTurnTypeSign {
    turnType @0 :UInt16;
    
    trackId @1 :Int16;
    # enum turn type
    #uint16 TURN_TYPE_NONE=0                      #0x00
    #uint8 FORWARD=1                              #0x01
    #uint8 TURN_LEFT=2                            #0x02
    #uint8 TURN_RIGHT=4                           #0x04
    #uint8 TURN_AROUND=8                          #0x08
    #uint8 TURN_TYPE_UNKNOWN=16                   #0x10
    bbox2d @2 :Bbox2D;
    
}
struct TrafficSign {
    sensorId @0 :Int16;
    ## BaseObject
    # enum SensorID
    label @1 :Int16;
    # enum ObjectLabel
    typeConfidence @2 :Float32;
    
    existenceConfidence @3 :Float32;
    
    subType @4 :Int16;
    # enum ObjectLabel
    subTypeProbs @5 :List(Float32);
    
    trackId @6 :Int32;
    
    trackAgeTimeSecond @7 :Float32;
    
    lastMeasureUpdateTimestampNs @8 :UInt64;
    
    currentPredictionTimestampNs @9 :UInt64;
    
    sizeInfo @10 :SizeInfo;
    
    polygonBox @11 :PolygonBox;
    
    directionInfo @12 :DirectionInfo;
    
    motionInfo @13 :MotionInfo;
    
    motionStatus @14 :Int16;
    
    motionStaticScore @15 :Float32;
    
    maintenanceStatus @16 :Int16;
    # enum MaintenanceStatus
    occlusionStatus @17 :Int16;
    # enum OcclusionStatus
    feature @18 :List(ModelOutputFeature);
    # ModelOutputFeatures
    cameraBboxInfo @19 :CameraBBox2DInfo;
    ## TrafficSign Specific
    # CameraBBox2DInfo
}
struct PolygonBox {
    isValid @0 :Bool;
    
    polygonContour @1 :List(Point2f);
    
    heightLowerSurface @2 :Float32;
    
    heightUpperSurface @3 :Float32;
    
    # only used in lidar_gop
    polygonContour3d @4 :List(Point3d);
}
struct LMLaneSpeedLimitSign {
    limitedSpeed @0 :Float32;
    
    trackId @1 :Int16;
    
    bbox2d @2 :Bbox2D;
    
}
struct LMLanePoint2f {
    x @0 :Float32;
    
    y @1 :Float32;
    
    nextSegmentLaneLineType @2 :UInt8;
    
    nextSegmentLaneLineColor @3 :UInt8;
    
    nextSegmentLaneLineWidth @4 :Float32;
    
    nextSegmentLaneLineTypeConfidence @5 :Float32;
    
    nextSegmentLaneLineColorConfidence @6 :Float32;
    
    nextSegmentLaneLineWidthConfidence @7 :Float32;
    
    pointType @8 :UInt8;

    nextSegmentLaneLineWidthLeft @9 :Float32;

    nextSegmentLaneLineWidthRight @10 :Float32;
}
struct CameraBBox2DInfo {
    rawDetectionBox @0 :Bbox2D;
    
    trackedBox @1 :Bbox2D;
    
}
struct LMPerceptionRoad {
    header @0 :Header.Header;
    # send to LocalMap/SceneMapping module
    frameTimestampNs @1 :UInt64;
    
    sensorId @2 :Int16;
    
    sensorName @3 :Text;
    # sensor name
    laneLines @4 :List(LMLaneLine);
    
    laneTurntypeSigns @5 :List(LMLaneTurnTypeSign);
    
    laneSpeedLimitSigns @6 :List(LMLaneSpeedLimitSign);
    
    speedLimitSigns @7 :List(LMSpeedLimitSign);
    
    regions @8 :List(LMRegion);
    
}

struct UniADPlanningDebug {
    textboxString @0 :Text;
}

struct HolisticPath {
    confidence @0 :Float32;
    # confidence

    # bv path start/end point
    startPoint @1 :Point2f;
    endPoint @2 :Point2f;

    # bv points
    pointsBv @3 :List(Point2f);

    # bv sampled points
    sampledPointsBv @4 :List(Point2f);

    # image points, for HMI
    imagePoints @5 :List(Point2f);

    # NOT use for now
    # PolynomialFittingResult
    # polynomial fitting degree
    fittingDegree @6 :Int16;

    # polynomial fitting coefficients
    polyfitCoefficients @7 :List(Float32);

    # path track id
    trackId @8 :Int32;

    # path track age
    trackAge @9 :Int32;
}


struct CenterLine {
    confidence @0 :Float32;

    # bv points
    pointsBv @1 :List(Point2f);

    # bv points type
    pointsBvType @2 :List(UInt16);
    # enum BlobType
    #STRAIGHT         = 1 << 0
    #TURN_LEFT        = 1 << 1
    #TURN_RIGHT       = 1 << 2
    #TURN_LEFT_AROUND = 1 << 3
    #TURN_RIGHT_AROUND= 1 << 4
    #TURN_LEFT_AREA   = 1 << 5
    #UNKNOWN          = 1 << 6
    
    # path track id
    trackId @3 :Int32;

    # path track age
    trackAge @4 :Int32;
}

struct PartitionInfo {
    # partition number
    numPartition @0 :UInt32;

    # partition ranges, size = numPartition + 1
    ranges @1 :List(Float32);

    # partition steps, size = numPartition
    steps @2 :List(Float32);

    # voxels number in ranges, size = numPartition
    numVoxelsPerRange @3 :List(UInt32);
}

struct OccGrid {
    confidence @0 :Float32;
    x @1 :Float32;
    y @2 :Float32;
    minHeight @3 :Float32;
    maxHeight @4 :Float32;
    gridSizeX @5 :Float32;
    gridSizeY @6 :Float32;

    # 0: UNKNOWN, 1: UNKNOWN_MOVABLE,
    # 2: UNKNOWN_UNMOVABLE, 3: PEDESTRIAN,
    # 4: BICYCLE, 5: VEHICLE, 6: FREESPACE
    label @7 :UInt8;
}

struct OccupancyData {
    # partition infos of x, y, z axises
    xPartitionInfo @0 :PartitionInfo;
    yPartitionInfo @1 :PartitionInfo;
    zPartitionInfo @2 :PartitionInfo;

    # number in x-axis
    length @3 :UInt32;

    # number in y-axis
    width @4 :UInt32;

    # occupancy voxels info
    idxLowList @5 :List(UInt8);
    idxHighList @6 :List(UInt8);
    occTypeList @7 :List(UInt8);
    # enum OccTypeLabel
    # UInt8 UNKNOWN = 0
    # UInt8 GROUND = 1
    # UInt8 OTHERS = 2
}

struct OccupancyInfo {
    header @0 :Header.Header;
    # Header include seq, timestamp, and frame_id
    frameTimestampNs @1 :UInt64;

    sensorId @2 :Int16;
    # enum SensorID

    #cherry
    gridsNum @3 :UInt64;
    occGrids @4 :List(OccGrid);
   
    # occupancy data j6m
    occupancyData @5 :OccupancyData;
}
