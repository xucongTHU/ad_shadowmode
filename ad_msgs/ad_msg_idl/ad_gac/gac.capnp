@0xbc4b80c0a4a206b1;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");
using Header = import "/ad_msg_idl/std_msgs/header.capnp";
using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";



struct TimeStamp {

    isValid @0 :Bool;

    sec @1 :UInt32;

    nsec @2 :UInt32;

}







struct Point3d {

    x @0 :Float64;

    y @1 :Float64;

    z @2 :Float64;

}







struct VISLIGHTSPOT {

    lsSpotId @0 :UInt32;

    lsSpotCalssification @1 :UInt8;

    lsSpotLongpos @2 :Float32;

    lsSpotLeftangle @3 :Float32;

    lsSpotRightangle @4 :Float32;

    lsSpotTopangle @5 :Float32;

    lsSpotBottomangle @6 :Float32;

}







struct VectorVISLIGHTSPOT {

    data @0 :List(VISLIGHTSPOT);

}







struct PilotProcessState {

    processIndex @0 :UInt16;

    status @1 :UInt8;

    vendor @2 :UInt8;

}







struct PilotProcessStateVector {

    data @0 :List(PilotProcessState);

}







struct PilotProcessStates {

    pilotProcessStates @0 :PilotProcessStateVector;

}







struct SensorSWCTimeStamp {

    sensorTimestamp @0 :TimeStamp;

    swcTimestamp @1 :TimeStamp;

}







struct CoordinateSystem {

    index @0 :UInt8;

}







struct SensorId {

    index @0 :UInt64;

}







struct HistInfo {

    sensorId @0 :SensorId;

    obstacleId @1 :UInt32;

    rawSensorCaptureTimestamp @2 :TimeStamp;

    sensorPerceptionCaptureTimestamp @3 :TimeStamp;

    sensorPerceptionOutputTimestamp @4 :TimeStamp;

    fusionCaptureTimestamp @5 :TimeStamp;

    fusionOutputTimestamp @6 :TimeStamp;

}







struct VectorHistInfo {

    data @0 :List(HistInfo);

}







struct ObstacleTypeInfo {

    type @0 :UInt16;

    subType @1 :UInt16;

}







struct RoadMarkerTypeInfo {

    type @0 :UInt8;

}







struct TrafficSignTypeInfo {

    type @0 :UInt8;

}







struct TrafficLightSignal {

    type @0 :UInt8;

    status @1 :UInt8;

}







struct Point2f {

    x @0 :Float32;

    y @1 :Float32;

}







struct LaneWidth {

    cutPoint @0 :Point2f;

    width @1 :UInt8;

    widthLeft @2 :UInt8;

    widthRight @3 :UInt8;

}







struct VectorPoint2f {

    data @0 :List(Point2f);

}







struct Point2d {

    x @0 :Float64;

    y @1 :Float64;

}







struct Point3f {

    x @0 :Float32;

    y @1 :Float32;

    z @2 :Float32;

}







struct VectorPoint3f {

    data @0 :List(Point3f);

}







struct VEDAttribute {

    timestamp @0 :SensorSWCTimeStamp;

    egoCarLinearVelocity @1 :Point3d;

    egoCarAngularVelocity @2 :Point3d;

}







struct VectorPoint3d {

    data @0 :List(Point3d);

}







struct Vector2f {

    x @0 :Float32;

    y @1 :Float32;

}







struct Vector2d {

    x @0 :Float64;

    y @1 :Float64;

}







struct Vector3f {

    x @0 :Float32;

    y @1 :Float32;

    z @2 :Float32;

}







struct Vector3d {

    x @0 :Float64;

    y @1 :Float64;

    z @2 :Float64;

}







struct Quaterniond {

    x @0 :Float64;

    y @1 :Float64;

    z @2 :Float64;

    w @3 :Float64;

}







struct Quaternionf {

    qx @0 :Float32;

    qy @1 :Float32;

    qz @2 :Float32;

    qw @3 :Float32;

}







struct Pose3f {

    translation @0 :Point3f;

    rotation @1 :Quaternionf;

}







struct Pose3d {

    translation @0 :Point3d;

    rotation @1 :Quaterniond;

}







struct Matrix3f {

    x @0 :Vector3f;

    y @1 :Vector3f;

    z @2 :Vector3f;

}







struct Matrix2f {

    x @0 :Vector2f;

    y @1 :Vector2f;

}







struct PathPoint {

    timestamp @0 :TimeStamp;

    position @1 :Point2f;

    headingAngle @2 :Float32;

    velocity @3 :Vector2f;

}







struct VectorPathPoint {

    data @0 :List(PathPoint);

}







struct ImageKeyPoint {

    point @0 :Point2d;

    xVar @1 :Float32;

    yVar @2 :Float32;

    isVisable @3 :Bool;

}







struct VectorImageKeyPoint {

    data @0 :List(ImageKeyPoint);

}







struct ImageBbox2D {

    topLeftX @0 :Float32;

    topLeftY @1 :Float32;

    bottomRightX @2 :Float32;

    bottomRightY @3 :Float32;

}







struct BirdViewBbox2D {

    centerPosition @0 :Point2f;

    heading @1 :Float32;

    length @2 :Float32;

    width @3 :Float32;

}







struct Bbox3D {

    pose @0 :Pose3f;

    width @1 :Float32;

    height @2 :Float32;

    length @3 :Float32;

}







struct IntervalInfo {

    dataId @0 :UInt64;

    dataValue @1 :Float64;

    dataType @2 :UInt8;

    offsetStyle @3 :UInt8;

    startOffset @4 :Float64;

    endOffset @5 :Float64;

}







struct VectorIntervalInfo {

    data @0 :List(IntervalInfo);

}







struct Point3dAttribute {

    point @0 :Point3d;

    heading @1 :Float64;

    lateralSlope @2 :Float64;

    longitudinalSlope @3 :Float64;

    curvatureRadius @4 :Float64;

}







struct VectorPoint3dAttribute {

    data @0 :List(Point3dAttribute);

}







struct BoundaryIntervalInfo {

    type @0 :UInt8;

    style @1 :UInt8;

    color @2 :UInt8;

    width @3 :UInt16;

    length @4 :UInt32;

    confidence @5 :UInt8;

    offsetStyle @6 :UInt8;

    startOffset @7 :Float64;

    endOffset @8 :Float64;

}







struct VectorBoundaryIntervalInfo {

    data @0 :List(BoundaryIntervalInfo);

}







struct VectorBirdViewBbox2D {

    data @0 :List(BirdViewBbox2D);

}







struct SegmentInfo {

    startPointIndex @0 :UInt32;

    endPointIndex @1 :UInt32;

    type @2 :UInt32;

}







struct VectorSegmentInfo {

    data @0 :List(SegmentInfo);

}







struct FreespaceInterior {

    interiorPolygon @0 :VectorPoint3d;

    segments @1 :VectorSegmentInfo;

}







struct VectorFreespaceInterior {

    data @0 :List(FreespaceInterior);

}







struct FreespaceExterior {

    exteriorPolygon @0 :VectorPoint3f;

    segments @1 :VectorSegmentInfo;

    interiorVector @2 :VectorFreespaceInterior;

}







struct VectorFreespaceExterior {

    data @0 :List(FreespaceExterior);

}







struct GeneralFreeSpace {

    histInfoVector @0 :VectorHistInfo;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    freespaceVector @3 :VectorFreespaceExterior;

}







struct MapCoordinate {

    type @0 :UInt8;

    zoneId @1 :UInt8;

    southFlag @2 :UInt8;

}







struct LocalLocalization {

    timestamp @0 :TimeStamp;

    outputTimestamp @1 :TimeStamp;

    parent @2 :CoordinateSystem;

    child @3 :CoordinateSystem;

    accuracyLevel @4 :UInt32;

    localPose @5 :Pose3d;

    egoCarAngularVelocity @6 :Vector3d;

    egoCarLinearVelocity @7 :Vector3d;

    egoCarLinearAcceleration @8 :Vector3d;

    egoCarToGroundRotation @9 :Quaterniond;

    localTranslationCovariance @10 :Vector3d;

    localRotationCovariance @11 :Vector3d;

}







struct Point3I {

    x @0 :Int32;

    y @1 :Int32;

    z @2 :Int32;

}







struct MatchingResult {

    linkId @0 :UInt64;

    laneId @1 :UInt64;

    longitudinalOffStart @2 :Int32;

    longitudinalOffEnd @3 :Int32;

    accuracy @4 :Float32;

    relativeHeading @5 :Int32;

    lateralOffLeft @6 :Int32;

    lateralOffRight @7 :Int32;

    lateralOffMiddle @8 :Int32;

    probobility @9 :UInt8;

}







struct VectorMatchingResult {

    data @0 :List(MatchingResult);

}







struct GlobalLocalization {

    timestamp @0 :TimeStamp;

    outputTimestamp @1 :TimeStamp;

    parent @2 :MapCoordinate;

    child @3 :MapCoordinate;

    position @4 :Point3I;

    rotation @5 :Quaterniond;

    locCovariance @6 :Vector3d;

    poseCovariance @7 :Vector3d;

    status @8 :UInt8;

    vehicleSpeed @9 :Int16;

    heading @10 :Int32;

    actived @11 :Bool;

    matchingResults @12 :VectorMatchingResult;

}







struct PredictPath {

    confidence @0 :Float32;

    pathPointVector @1 :List(PathPoint);

}







struct VectorPredictPath {

    data @0 :List(PredictPath);

}







struct GeneralObstacle {

    histInfoVector @0 :List(HistInfo);

    obstacleId @1 :UInt32;

    obstacleTypeInfo @2 :ObstacleTypeInfo;

    confidence @3 :Float32;

    existenceConfidence @4 :Float32;

    classConfidence @5 :Float32;

    centerPosition @6 :Point2f;

    absoluteVelocity @7 :Vector2f;

    relativeVelocity @8 :Vector2f;

    absoluteAccelaration @9 :Vector2f;

    headingAngle @10 :Float32;

    headingAngularVelocity @11 :Float32;

    movingStatus @12 :UInt8;

    bbox3dWidth @13 :Float32;

    bbox3dHeight @14 :Float32;

    bbox3dLength @15 :Float32;

    centerPositionCov @16 :Matrix2f;

    velocityCov @17 :Matrix2f;

    accelarationCov @18 :Matrix2f;

    sizeCov @19 :Matrix3f;

    headingAngleStd @20 :Float32;

    trackingPeriod @21 :Float32;

    isMovable @22 :Bool;

    isHeadingToEgoCar @23 :Bool;

    isEverHeadingToEgoCar @24 :Bool;

    lampStatus @25 :UInt8;

    trackingStatus @26 :UInt8;

    cutInStatus @27 :UInt8;

    cutOutStatus @28 :UInt8;

    predictPathVector @29 :List(PredictPath);

    polygon @30 :List(Point3f);

}







struct VectorGeneralObstacle {

    data @0 :List(GeneralObstacle);

}







struct FusionObstacle {

    timestamp @0 :TimeStamp;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    obstacleNum @3 :UInt32;

    generalObstacleVector @4 :List(GeneralObstacle);

}







struct VisionObstacleUint {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt32;

    type @2 :ObstacleTypeInfo;

    confidence @3 :Float32;

    movingStatus @4 :UInt8;

    trackingPeriod @5 :Float32;

    trackingStatus @6 :UInt8;

    size @7 :Vector3f;

    longitudinalPosition @8 :Float32;

    lateralPosition @9 :Float32;

    longitudinalVelocity @10 :Float32;

    lateralVelocity @11 :Float32;

    longitudinalAcceleration @12 :Float32;

    lateralAcceleration @13 :Float32;

    headingAngle @14 :Float32;

    centerPositionCov @15 :Matrix2f;

    velocityCov @16 :Matrix2f;

}







struct VectorVisionObstacleUint {

    data @0 :List(VisionObstacleUint);

}







struct VisionObstacle {

    timestamp @0 :TimeStamp;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    obstacleNum @3 :UInt32;

    visionObstacleVector @4 :VectorVisionObstacleUint;

}







struct VectorFloat {

    data @0 :List(Float32);

}







struct InternalBBox2D {

    topLeft @0 :Point2f;

    bottomRight @1 :Point2f;

    confidence @2 :Float32;

    isLeftSideTruncated @3 :Bool;

    isRightSideTruncated @4 :Bool;

    isTopSideTruncated @5 :Bool;

    isBottomSideTruncated @6 :Bool;

}







struct CameraBBox2DInfo {

    rawDetectionBox @0 :InternalBBox2D;

    trackedBox @1 :InternalBBox2D;

}







struct ModelOutputFeature {

    key @0 :Text;

    value @1 :VectorFloat;

}







struct BaseCameraAttribute {

    isLandmarkValid @0 :Bool;

    landmark @1 :VectorPoint2f;

    landmarkScores @2 :VectorFloat;

    isDirectionValid @3 :Bool;

    viewportDirection @4 :Vector2f;

}







struct AutomobileLightStatus {

    leftTurn @0 :Int32;

    rightTurn @1 :Int32;

    hazard @2 :Int32;

    brake @3 :Int32;

}







struct NonAutomobileVehicleLightStatus {

    leftTurn @0 :Int32;

    rightTurn @1 :Int32;

    brake @2 :Int32;

}







struct DepthDetPoint {

    nearestPointX @0 :Float32;

    depthPointX @1 :Float32;

    depthPointY @2 :Float32;

    orientation @3 :Int32;

    label @4 :Int32;

}




struct DerivedType {
    const cameraNonVehicle :UInt8 = 0;
    const cameraPedestrian :UInt8 = 1;
    const cameraVehicle :UInt8 = 2;
    const cameraStaticObstacle :UInt8 = 3;
    const lidarObject :UInt8 = 4;
    const radarObject :UInt8 = 5;
    const fusionObject :UInt8 = 6;
    const cameraTrafficlight :UInt8 = 7;
    const cameraTrafficsign :UInt8 = 8;

    type @0 :UInt8;

}







struct BirdViewCorners {

    bvCorners @0 :VectorPoint3f;

    cornerConf @1 :VectorFloat;

}







struct VectorModelOutputFeature {

    data @0 :List(ModelOutputFeature);

}







struct InternalObject {

    timeStamp @0 :TimeStamp;

    type @1 :ObstacleTypeInfo;

    typeConfidence @2 :Float32;

    existenceConfidence @3 :Float32;

    confidence @4 :Float32;

    sensorId @5 :SensorId;

    trackId @6 :UInt32;

    trackAgeS @7 :UInt32;

    derivedType @8 :DerivedType;

    position @9 :Point3f;

    positionUncertainty @10 :Matrix3f;

    relativeVelocity @11 :Point3f;

    relativeVelocityUncertainty @12 :Matrix3f;

    acceleration @13 :Point3f;

    accelerationUncertainty @14 :Point3f;

    absoluteVelocity @15 :Point3f;

    absoluteVelocityUncertainty @16 :Point3f;

    coutourPosition @17 :Point2f;

    coutourPoints @18 :VectorPoint2f;

    heightLowerSurface @19 :Float32;

    heightUpperSurface @20 :Float32;

    shape @21 :Vector3f;

    shapeUncertainty @22 :Matrix3f;

    heading @23 :Float32;

    headingUncertainty @24 :Float32;

    maintenanceStatus @25 :UInt8;

    occlusionStatus @26 :UInt8;

    birdviewCorners @27 :BirdViewCorners;

    feature @28 :VectorModelOutputFeature;

    cameraBboxInfo @29 :CameraBBox2DInfo;

    baseCameraAttribute @30 :BaseCameraAttribute;

    automobileLightStatus @31 :AutomobileLightStatus;

    nonAutomobileLightStatus @32 :NonAutomobileVehicleLightStatus;

    isWithPed @33 :Bool;

    upperBodyDirection @34 :Point2f;

    lowerBodyDirection @35 :Point2f;

    faceDirection @36 :Point2f;

    isOnBike @37 :Bool;

    depthPoint @38 :DepthDetPoint;

    trackRange @39 :Float32;

    trackRangeRate @40 :Float32;

    trackRangeAccelRate @41 :Float32;

    trackRangeLatRate @42 :Float32;

    trackLatRate @43 :Float32;

    histInfoVector @44 :VectorHistInfo;

    trackingPeriod @45 :Float32;

    isMovable @46 :Bool;

    isHeadingToEgoCar @47 :Bool;

    isEverHeadingToEgoCar @48 :Bool;

    lampStatus @49 :UInt8;

    trackingStatus @50 :UInt8;

    motionStatus @51 :Int8;

    laneAssignments @52 :Int8;

}







struct VectorInternalObject {

    data @0 :List(InternalObject);

}







struct VectorSensorSWCTimeStamp {

    data @0 :List(SensorSWCTimeStamp);

}







struct TimeStampGap {

    type @0 :UInt32;

    sensorTimestampVector @1 :VectorSensorSWCTimeStamp;

}







struct VectorVEDAttribute {

    data @0 :List(VEDAttribute);

}







struct LightSpot {

    timestamp @0 :TimeStamp;

    sensorId @1 :SensorId;

    coordinate @2 :CoordinateSystem;

    lsDayTimeStatus @3 :UInt8;

    lsTunnelAreaDetection @4 :UInt8;

    lsSpotsNumber @5 :UInt32;

    lightSpotVector @6 :VectorVISLIGHTSPOT;

}







struct VisionOdd {

    timestamp @0 :TimeStamp;

    sensorId @1 :SensorId;

    weatherInfo @2 :UInt8;

    visibleDistance @3 :UInt8;

    environmentLightStatus @4 :UInt8;

    rainfallInfo @5 :UInt8;

    tollgateStatus @6 :UInt8;

    roadCondition @7 :UInt8;

}







struct CameraTSLRObject {

    keypoints @0 :VectorImageKeyPoint;

    type @1 :UInt8;

    classConfidence @2 :Float32;

    bbox2d @3 :ImageBbox2D;

}







struct VectorCameraTSLRObject {

    data @0 :List(CameraTSLRObject);

}







struct CameraTSLR {

    timestamp @0 :TimeStamp;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    objects @3 :VectorCameraTSLRObject;

}







struct LaneType {

    cutPoint @0 :Point2f;

    type @1 :UInt8;

}







struct VectorLaneType {

    data @0 :List(LaneType);

}







struct LaneColor {

    cutPoint @0 :Point2f;

    color @1 :UInt8;

}







struct VectorLaneColor {

    data @0 :List(LaneColor);

}







struct InternalLanePoint2f {

    x @0 :Float32;

    y @1 :Float32;

    nextSegmentLaneLineType @2 :UInt8;

    nextSegmentLaneLineColor @3 :UInt8;

    nextSegmentLaneLineWidth @4 :LaneWidth;

    nextSegmentLaneLineColorConfidence @5 :Float32;

    nextSegmentLaneLineTypeConfidence @6 :Float32;

    nextSegmentLaneLineWidthConfidence @7 :Float32;

    pointType @8 :UInt8;

}







struct VectorLaneWidth {

    data @0 :List(LaneWidth);

}







struct LaneBoundary {

    histInfo @0 :VectorHistInfo;

    laneId @1 :UInt32;

    lanePositionIndex @2 :UInt8;

    confidence @3 :Float32;

    availability @4 :UInt8;

    c0 @5 :Float32;

    c1 @6 :Float32;

    c2 @7 :Float32;

    c3 @8 :Float32;

    rangeStart @9 :Float32;

    rangeEnd @10 :Float32;

    laneTypeVector @11 :VectorLaneType;

    laneColorVector @12 :VectorLaneColor;

    laneWidthVector @13 :VectorLaneWidth;

}







struct VectorLaneBoundary {

    data @0 :List(LaneBoundary);

}







struct FusionLane {

    timestamp @0 :TimeStamp;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    laneBoundaryVector @3 :VectorLaneBoundary;

}







struct VectorUInt64 {

    data @0 :List(UInt64);

}







struct Link {

    linkId @0 :UInt64;

    linkType @1 :UInt8;

    linkAttribute @2 :UInt8;

    length @3 :Float64;

    intersectionId @4 :UInt64;

    laneIds @5 :VectorUInt64;

    leftBoundaryIds @6 :VectorUInt64;

    rightBoundaryIds @7 :VectorUInt64;

    precedingLinkIds @8 :VectorUInt64;

    succeedingLinkIds @9 :VectorUInt64;

    linkClass @10 :UInt8;

}







struct Lane {

    laneId @0 :UInt64;

    linkId @1 :UInt64;

    laneType @2 :UInt8;

    turnType @3 :UInt8;

    transType @4 :UInt8;

    sequence @5 :UInt8;

    intersectionId @6 :UInt64;

    leftLaneId @7 :UInt64;

    rightLaneId @8 :UInt64;

    leftBoundaryId @9 :UInt64;

    rightBoundaryId @10 :UInt64;

    precedingLaneIds @11 :VectorUInt64;

    succeedingLaneIds @12 :VectorUInt64;

    length @13 :Float64;

    widthVector @14 :VectorIntervalInfo;

    speedLimitVector @15 :VectorIntervalInfo;

    objectIds @16 :VectorIntervalInfo;

    centerLine @17 :VectorPoint3dAttribute;

}







struct VectorLane {

    data @0 :List(Lane);

}







struct Boundary {

    boundaryId @0 :UInt64;

    linkId @1 :UInt64;

    leftLaneId @2 :UInt64;

    rightLaneId @3 :UInt64;

    precedingBoundaryIds @4 :VectorUInt64;

    succeedingBoundaryIds @5 :VectorUInt64;

    segments @6 :VectorBoundaryIntervalInfo;

    centerLine @7 :VectorPoint3d;

}







struct VectorBoundary {

    data @0 :List(Boundary);

}







struct Stopline {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt64;

    laneId @2 :VectorUInt64;

    stopLine @3 :VectorPoint3d;

}







struct VectorStopline {

    data @0 :List(Stopline);

}







struct Marking {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt64;

    laneId @2 :UInt64;

    position @3 :Point3f;

    bbox3d @4 :Bbox3D;

    type @5 :UInt8;

    color @6 :UInt8;

    semanticInfo @7 :Text;

    confidence @8 :Float32;

    laneVectorIndex @9 :Int32;

    detectionStatus @10 :UInt8;

}







struct VectorMarking {

    data @0 :List(Marking);

}







struct Sign {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt64;

    laneId @2 :UInt64;

    type @3 :UInt32;

    position @4 :Point3f;

    laneVectorIndex @5 :Int32;

    detectionStatus @6 :UInt8;

}







struct VectorSign {

    data @0 :List(Sign);

}







struct Signal {

    type @0 :UInt8;

    status @1 :UInt8;

}







struct VectorSignal {

    data @0 :List(Signal);

}







struct SafeIsland {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt64;

    laneIdVector @2 :VectorUInt64;

    bvBbox2d @3 :BirdViewBbox2D;

}







struct VectorSafeIsland {

    data @0 :List(SafeIsland);

}







struct PedestrianCrossing {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt64;

    laneIdVector @2 :VectorUInt64;

    bvBbox2d @3 :BirdViewBbox2D;

}







struct VectorPedestrianCrossing {

    data @0 :List(PedestrianCrossing);

}







struct VectorLink {

    data @0 :List(Link);

}







struct SpeedLimitSign {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt8;

    confidence @2 :Float32;

    trackingStatus @3 :UInt8;

    type @4 :UInt32;

    centerPosition @5 :Point3f;

}







struct VectorSpeedLimitSign {

    data @0 :List(SpeedLimitSign);

}







struct FusionTrafficSign {

    timestamp @0 :TimeStamp;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    trafficSignNum @3 :UInt32;

    signVector @4 :VectorSpeedLimitSign;

}







struct VectorTrafficLightSignal {

    data @0 :List(TrafficLightSignal);

}







struct Light {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt64;

    laneId @2 :UInt64;

    isDetectedByCamera @3 :Bool;

    isGotOnlyByCamera @4 :Bool;

    countDownSecond @5 :Int32;

    confidence @6 :Float32;

    bbox3d @7 :Bbox3D;

    laneIdVector @8 :VectorUInt64;

    signalVector @9 :VectorSignal;

}







struct VectorLight {

    data @0 :List(Light);

}







struct BumpLike {

    histInfoVector @0 :VectorHistInfo;

    id @1 :UInt64;

    laneIdVector @2 :VectorUInt64;

    type @3 :UInt8;

    bumplikeBbox @4 :VectorLaneBoundary;

}







struct VectorBumpLike {

    data @0 :List(BumpLike);

}







struct VectorInternalLanePoint2f {

    data @0 :List(InternalLanePoint2f);

}







struct InternalLaneLine {

    laneIndex @0 :Int32;

    trackId @1 :Int32;

    trackAge @2 :Int32;

    distanceToLeftBumper @3 :Float32;

    distanceToRightBumper @4 :Float32;

    lanelineWidth @5 :LaneWidth;

    color @6 :UInt8;

    laneType @7 :UInt8;

    confidence @8 :Float32;

    colorConfidence @9 :Float32;

    typeConfidence @10 :Float32;

    widthConfidence @11 :Float32;

    startPoint @12 :Point2f;

    endPoint @13 :Point2f;

    imagePoints @14 :VectorPoint2f;

    bvPoints @15 :VectorPoint2f;

    fittingType @16 :Int32;

    fittingOrder @17 :Int32;

    fittingCoefficients @18 :VectorFloat;

    cutPointVector @19 :VectorInternalLanePoint2f;

    isLineJunctionValid @20 :Bool;

}







struct VectorInternalLaneLine {

    data @0 :List(InternalLaneLine);

}







struct InternalStopLine {

    imagePoints @0 :VectorPoint2f;

    birdviewPoints @1 :VectorPoint2f;

    confidence @2 :Float32;

    id @3 :UInt32;

}







struct VectorInternalStopLine {

    data @0 :List(InternalStopLine);

}







struct InternalBoundary {

    imagePoints @0 :VectorPoint2f;

    birdviewPoints @1 :VectorPoint2f;

    directionX @2 :Float32;

    directionY @3 :Float32;

    confidence @4 :Float32;

    isLine @5 :Bool;

    id @6 :UInt32;

}







struct VectorInternalBoundary {

    data @0 :List(InternalBoundary);

}







struct InternalTSR {

    label @0 :TrafficSignTypeInfo;

    bbox2d @1 :ImageBbox2D;

    id @2 :UInt32;

}







struct InternalTLR {

    label @0 :TrafficLightSignal;

    bbox2d @1 :ImageBbox2D;

    id @2 :UInt32;

}







struct VectorInternalTLR {

    data @0 :List(InternalTLR);

}







struct InternalRoadMarker {

    label @0 :RoadMarkerTypeInfo;

    bbox2d @1 :ImageBbox2D;

    id @2 :UInt32;

}







struct VectorInternalRoadMarker {

    data @0 :List(InternalRoadMarker);

}







struct InternalRegion {

    regionType @0 :UInt8;

    regionTypeConfidence @1 :Float32;

    trackId @2 :UInt32;

    regionBoundary @3 :VectorPoint2f;

    junctionPointVector @4 :VectorPoint2f;

}







struct VectorInternalRegion {

    data @0 :List(InternalRegion);

}







struct InternalCrosswalk {

    imagePoints @0 :VectorPoint2f;

    birdviewPoints @1 :VectorPoint2f;

    length @2 :Float32;

    width @3 :Float32;

    confidence @4 :Float32;

    id @5 :UInt32;

}







struct VectorInternalTSR {

    data @0 :List(InternalTSR);

}







struct InternalTSLR {

    histInfo @0 :HistInfo;

    timestamp @1 :TimeStamp;

    coordinate @2 :CoordinateSystem;

    sensorId @3 :SensorId;

    tsrVector @4 :VectorInternalTSR;

    tlrVector @5 :VectorInternalTLR;

    roadMarkerVector @6 :VectorInternalRoadMarker;

}







struct VEDCheck {

    type @0 :UInt32;

    vedAttributeVector @1 :VectorVEDAttribute;

}







struct VectorInternalCrosswalk {

    data @0 :List(InternalCrosswalk);

}







struct InternalRoadStructure {

    histInfo @0 :HistInfo;

    timestamp @1 :TimeStamp;

    coordinate @2 :CoordinateSystem;

    lanelineVector @3 :VectorInternalLaneLine;

    boundaryVector @4 :VectorInternalBoundary;

    stoplineVector @5 :VectorInternalStopLine;

    crosswalkVector @6 :VectorInternalCrosswalk;

    tsrVector @7 :VectorInternalTSR;

    tlrVector @8 :VectorInternalTLR;

    roadMarkerVector @9 :VectorInternalRoadMarker;

    regionVector @10 :VectorInternalRegion;

}







struct FusionMap {

    timestamp @0 :TimeStamp;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    linkVector @3 :VectorLink;

    laneVector @4 :VectorLane;

    boundaryVector @5 :VectorBoundary;

    stoplineVector @6 :VectorStopline;

    markingVector @7 :VectorMarking;

    signVector @8 :VectorSign;

    lightVector @9 :VectorLight;

    safeIslandVector @10 :VectorSafeIsland;

    pedestrianCrossingVector @11 :VectorPedestrianCrossing;

    bumplikeVector @12 :VectorBumpLike;

}







struct InternalPerceptionObject {

    timestamp @0 :TimeStamp;

    coordinate @1 :CoordinateSystem;

    sensorId @2 :SensorId;

    obstacleNum @3 :UInt32;

    internalObjectVector @4 :VectorInternalObject;

}







struct AppStatus {

    status @0 :List(UInt8);

    timestamp @1 :TimeStamp;

    nodeId @2 :UInt8;

}




struct  CameraHeader {
    sec @0 :UInt32;
    nSec @1 :UInt32;
    expStartS @2 :UInt32;
    expStartNs @3 :UInt32;
    expEndS @4 :UInt32;
    expEndNs @5 :UInt32;
    shutter1 @6 :UInt32;
    shutter2 @7 :UInt32; 
}

struct CameraHeaderMbuf {
    seq @0 :UInt32;
    stamp @1 :CameraHeader;
    frameId @2 :Text;
}

struct CameraEncodeMbuf {
    sendTimeHigh @0 :UInt32;
    sendTimeLow @1 :UInt32;
    frameType @2 :UInt32;
    videoFrame @3 :Text;
    dataSize @4 :UInt32;
    adapterId @5 :UInt32;
    cameraHeader @6 :CameraHeaderMbuf;
}

struct DiagReport{
    appId @0 :UInt32;
    alarmID @1 :List(UInt32);
}