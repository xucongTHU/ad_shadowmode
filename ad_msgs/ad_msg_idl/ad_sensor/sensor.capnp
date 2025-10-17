@0x9db157816b4a9a38;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::sensor");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

enum SensorQuality {
    invalid @0;   # sensor offline
    bad @1;       # sensor online, but can not use
    downgrade @2; # have issue, but can work
    perfect @3;   # running well, nothing happend
}
struct DualAntennaHeading {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Frame actual timestamp ns (or closest actual time)
    actualTimeNs @1 :UInt64;

    # Sensor quality
    status @2 :SensorQuality;

    # For status debug
    statusInfo @3 :Text;

    # The heading is the angle from True North of the primary antenna to
    # secondary antenna vector in clockwise direction, in degrees (0 to 359.999)
    heading @4 :Float32;
    # heading standard deviation in degrees
    headingStdDev @5 :Float32;

    # pitch angle in degrees (+-90)
    pitch @6 :Float32;
    # pitch standard deviation in degrees
    pitchStdDev @7 :Float32;
}

struct Gnss {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Frame actual timestamp ns (or closest actual time)
    actualTimeNs @1 :UInt64;

    # Sensor quality
    status @2 :SensorQuality;
    # For status debug
    statusInfo @3 :Text;

    # NMEA-0183
    nmeaGga @4 :Text;
    nmeaRmc @5 :Text;

    # Longitude in degrees, ranging from -180 to 180
    longitude @6 :Float64;
    # Latitude in degrees, ranging from -90 to 90
    latitude @7 :Float64;
    # WGS-84 ellipsoid height in meters
    altitude @8 :Float64;

    # East/north/up in meters
    positionStdDev @9 :StdGeometry.Point;
    # East/north/up in meters per second
    linearVelocity @10 :StdGeometry.Vector3;
    # East/north/up in meters per second
    linearVelocityStdDev @11 :StdGeometry.Vector3;

    # Number of satellites in position solution
    numSats @12 :UInt32;
}

struct Imu {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Frame actual timestamp ns (or closest actual time)
    actualTimeNs @1 :UInt64;

    status @2 :SensorQuality;

    # For status debug
    statusInfo @3 :Text;

    # Linear acceleration of the IMU in the Vehicle reference frame
    # x for Forward, y for Left, z for Up
    # in meters per power second (m/s^2)
    accelX @4 :Float32;
    accelY @5 :Float32;
    accelZ @6 :Float32;

    # Angular velocity of the IMU in the Vehicle reference frame
    # x across Forward axes, y across Left axes, z across Up axes
    # in radians per second (rad/s)
    gyroX @7 :Float32;
    gyroY @8 :Float32;
    gyroZ @9 :Float32;
}

struct Ins {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Frame actual timestamp ns (or closest actual time)
    actualTimeNs @1 :UInt64;

    status @2 :SensorQuality;

    # For status debug
    statusInfo @3 :Text;

    # Longitude in degrees, ranging from -180 to 180
    longitude @4 :Float64;
    # Latitude in degrees, ranging from -90 to 90
    latitude @5 :Float64;
    # WGS-84 ellipsoid height in meters
    altitude @6 :Float64;

    # East/north/up std in meters
    positionStdDev @7 :StdGeometry.Vector3;

    # Right-handed rotation from local level around y‑axis, in radius.
    roll @8 :Float64;
    # Right-handed rotation from local level around x‑axis, in radius.
    pitch @9 :Float64;
    # Left-handed rotation around z-axis clockwise from North, in radius.
    yaw @10 :Float64;

    # roll/pitch/yaw std in radius
    attitudeStdDev @11 :StdGeometry.Vector3;

    # Linear velocity of the IMU in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in meters per second
    linearVelocity @12 :StdGeometry.Vector3;

    # East/north/up linear velocity std in meters per second
    linearVelocityStdDev @13 :StdGeometry.Vector3;
}

struct RadarConti {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Frame actual timestamp ns (first can receive time)
    actualTimeNs @1 :UInt64;

    # Sensor fault
    busFault @2 :UInt32;
    # For status debug
    statusInfo @3 :Text;

    # Radar cluster or object array
    # Make empty if sensor unsupported
    clusterList @4 :List(RadarContiCluster);
    objectList @5 :List(RadarContiObject);
}

struct RadarContiCluster {
    # Basic CAN frame receive timestamp ns
    recvTimeNs @0 :UInt64;

    # Radar cluster basic info
    trackId @1 :UInt32;
    distanceLong @2 :Float32; # point of the cluster (in meter)
    distanceLat @3 :Float32;
    velocityLong @4 :Float32; # velocity of the cluster (in m/s)
    velocityLat @5 :Float32;
    rcs @6 :Float32; # radar cross section (in dBm^2)
    dynamicPropEnum @7 :UInt32; # dynamic property of the cluster indicating

    # Radar cluster quality info
    qualityVaild @8 :Bool;
    distanceLongRms @9 :Float32; # standard deviation (in meter)
    distanceLatRms @10 :Float32;
    velocityLongRms @11 :Float32; # m/s
    velocityLatRms @12 :Float32;
    clusterPdh0 @13 :Float32; # false alarm probability of cluster (in 0-1)
    ambigStateEnum @14 :UInt32; # state of doppler (radial velocity) ambiguity solution
    invalidStateEnum @15 :UInt32; # state of cluster validity state
}

struct RadarContiObject {
    # Basic CAN frame receive timestamp ns
    recvTimeNs @0 :UInt64;

    # Radar object basic info
    trackId @1 :UInt32;
    distanceLong @2 :Float32; # center point of the object (in meter)
    distanceLat @3 :Float32;
    velocityLong @4 :Float32; # velocity of the object (in m/s)
    velocityLat @5 :Float32;
    rcs @6 :Float32; # radar cross section (in dBm^2)
    dynamicPropEnum @7 :UInt32; # dynamic property of the object indicating

    # Radar object quality info
    qualityVaild @8 :Bool;
    distanceLongRms @9 :Float32; # standard deviation (in meter)
    distanceLatRms @10 :Float32;
    velocityLongRms @11 :Float32; # m/s
    velocityLatRms @12 :Float32;
    accelLongRms @13 :Float32; # m/s^2
    accelLatRms @14 :Float32;
    orientationRms @15 :Float32; # radius
    probOfExist @16 :Float32;   # 0-1
    measurementStateEnum @17 :UInt32;

    # Radar object extend info
    extendVaild @18 :Bool;
    accelLong @19 :Float32; # acceleration of the object (in m/s^2)
    accelLat @20 :Float32;
    orientation @21 :Float32; # yaw of the object orientation angel (in radius)
    length @22 :Float32;
    width @23 :Float32;
    objectTypeEnum @24 :UInt32;
}

struct RadarEsr {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Frame actual timestamp ns (first can receive time)
    actualTimeNs @1 :UInt64;

    # Sensor fault
    busFault @2 :UInt32;
    # For status debug
    statusInfo @3 :Text;

    # Radar object array
    objectList @4 :List(RadarEsrObject);
}

struct RadarEsrObject {
    # CAN frame receive timestamp ns
    recvTimeNs @0 :UInt64;

    # CAN frame lost flag
    objectVaild @1 :Bool;

    # ESR Track Msg
    trackId @2 :UInt32;
    trackLatRate @3 :Float32;
    trackGroupChanged @4 :Bool;
    trackStatus @5 :UInt32;
    trackAngle @6 :Float32;
    trackRange @7 :Float32;
    trackBridgeObject @8 :Bool;
    trackRollingCount @9 :Bool;
    trackWidth @10 :Float32;
    trackRangeAccel @11 :Float32;
    trackMedRangeMode @12 :UInt32;
    trackRangeRate @13 :Float32;

    # ESR Track Power
    movableFast @14 :Bool;
    movableSlow @15 :Bool;
    moving @16 :Bool;
    power @17 :Int32;
}

struct PasUssCodeIdInfoType {
    pasUssCodeIdInfoId @0 :UInt8;
    ussCeRight1CodeId @1 :UInt8;
    ussCeLeft1CodeId @2 :UInt8;
    ussCeLeft2CodeId @3 :UInt8;
    ussCeRight2CodeId @4 :UInt8;
    ussDemid1CodeId @5 :UInt8;
    ussDemid2CodeId @6 :UInt8;
}

struct PasOdoInfoType{
    pasOdoInfo6Checksum @0 :UInt8;
    pasOdoInfo6MsgCounter @1 :UInt8;
    odoKappa @2 :Float32;
    odoSha @3 :Float32;
    odoX @4 :Float32;
    odoYawAngle @5 :Float32;
    pasOdoTimeStamp @6 :UInt64;
    pasOdoY @7 :Float32;
}

struct PasDistanceType {
    pasDistanceChecksum @0 :UInt8;
    pasDistanceMsgCounter @1 :UInt8;
    pasRRMidDistance @2 :UInt8;
    pasRLMidDistance @3 :UInt8;
    pasRRDistance @4 :UInt8;
    pasRLDistance @5 :UInt8;
    pasRSLSideDistance @6 :UInt8;
    pasRSRSideDistance @7 :UInt8;
    pasFRDistance @8 :UInt8;
    pasFLDistance @9 :UInt8;
    pasFRMidDistance @10 :UInt8;
    pasFLMidDistance @11 :UInt8;
    pasFSRSideDistance @12 :UInt8;
    pasFSLSideDistance @13 :UInt8;
    pasFLSnsErrFlag @14 :Bool;
    pasFRSnsErrFlag @15 :Bool;
    pasRSRSnsErrFlag @16 :Bool;
    pasRSLSnsErrFlag @17 :Bool;
    pasRLSnsErrFlag @18 :Bool;
    pasRRSnsErrFlag @19 :Bool;
    pasRLMidSnsErrFlag @20 :Bool;
    pasRRMidSnsErrFlag @21 :Bool;
    pasBattVoltErrFlag @22 :Bool;
    pasCtrlModulerrFlag @23 :Bool;
    pasFSRSnsErrFlag @24 :Bool;
    pasFSLSnsErrFlag @25 :Bool;
    pasFLMidSnsErrFlag @26 :Bool;
    pasFRMidSnsErrFlag @27 :Bool;
    pasOverTempErrFlag @28 :Bool;
    pasRRMidSnsCoverErrFlag @29 :Bool;
    pasRLMidSnsCoverErrFlag @30 :Bool;
    pasRRSnsCoverErrFlag @31 :Bool;
    pasRLSnsCoverErrFlag @32 :Bool;
    pasRSLSnsCoverErrFlag @33 :Bool;
    pasRSRSnsCoverErrFlag @34 :Bool;
    pasFRSnsCoverErrFlag @35 :Bool;
    pasFLSnsCoverErrFlag @36 :Bool;
    pasFRMidSnsCoverErrFlag @37 :Bool;
    pasFLMidSnsCoverErrFlag @38 :Bool;
    pasFSLSnsCoverErrFlag @39 :Bool;
    pasFSRSnsCoverErrFlag @40 :Bool;
}

struct MapObjSignal {
    id @0 :UInt8;
    mapObjP1x @1 :Float32;
    mapObjP1y @2 :Float32;
    mapObjP2x @3 :Float32;
    mapObjP2y @4 :Float32;
    mapObjProb @5 :Float32;
    mapObjType @6 :UInt8;
    mapObjHeightBrake @7 :UInt8;
    mapObjHeightWarn @8 :UInt8;
    mapObjHeightProb @9 :Float32;
}

struct ApaSlotinfo {
    apaSlotId @0 :UInt8;
    apaSlotId1 @1 :UInt8;
    apaSlotStatus @2 :UInt8;
    apaSlotType @3 :UInt8;
    apaSlotDepethReferance @4 :UInt8;
    apaSlotObj1x @5 :Float32;
    apaSlotObj1y @6 :Float32;
    apaSlotObj1type @7 :UInt8;
    apaSlotObj1Alpha @8 :Float32;
    apaSlotObj2x @9 :Float32;
    apaSlotObj2y @10 :Float32;
    apaSlotObj2Type @11 :UInt8;
    apaSlotObj2Alpha @12 :Float32;
    apaSlotLength @13 :UInt16;
    apaSlotDepth @14 :UInt16;
}

struct PASMapPsolObjInfoType {
    pasMapPsolObjInfoId @0 :UInt8;
    mapPsolObjP1X @1 :Float32;
    mapPsolObjP1Y @2 :Float32;
    mapPsolObjP2X @3 :Float32;
    mapPsolObjP2Y @4 :Float32;
    mapPsolObjHeightStatus @5 :UInt8;
    }

struct PasUssInfoType{
    pasUssInfoId @0 :UInt8;
    ussDe1 @1 :UInt16;
    ussDe1Timestamp @2 :UInt64;
    ussCeLeft1 @3 :UInt16;
    ussCeRight1 @4 :UInt16;
    ussCeLeft1Timestamp @5 :UInt64;
    ussCeRight1Timestamp @6 :UInt64;
    ussDe2 @7 :UInt16;
    ussCeLeft2 @8 :UInt16;
    ussCeRight2 @9 :UInt16;
    }

struct PasUssTraceIdInfoType{
    pasUssTraceIdInfoId @0 :UInt8;
    ussCeLeft1TraceId @1 :UInt16;
    ussCeRight1TraceId @2 :UInt16;
    ussCeLeft2TraceId @3 :UInt16;
    ussCeRight2TraceId @4 :UInt16;
    ussDe1TraceId @5 :UInt16;
    ussDe2TraceId @6 :UInt16;
    }

struct UssArray {
    header @0 :StdHeader.Header;
    # CodeID of Secondary echo distance
    pasUssCodeIdInfoArray @1:List(PasUssCodeIdInfoType);
    pasOdoInfo @2 :PasOdoInfoType;
    # Distance from sensors
    pasDistance @3 :PasDistanceType;
    # obstacle point with the center of the rear axle of the vehicle
    mapObjSignalArray @4 :List(MapObjSignal);
    mapObjectsSignals1MsgCounter @5 :UInt8;
    mapObjectsSignals1Timestamp @6 :UInt64;
    mapObjectsSignals5MsgCounter @7 :UInt8;
    pslStatus @8 :UInt8;
    pasStatus @9 :UInt8;
    mapObjectsSignals4MsgCounter @10 :UInt8;
    mapObjectsSignals4Timestamp @11 :UInt64;
    mapObjectsSignals3MsgCounter @12 :UInt8;
    mapObjectsSignals3Timestamp @13 :UInt64;
    mapObjectsSignals2MsgCounter @14 :UInt8;
    # parking Spaces detect
    apaSlotInfoArray @15 :List(ApaSlotinfo);
    pasApaSlotInfo1MsgCounter @16 :UInt8;
    slotInfo1Timestamp @17 :UInt64;
    pasApaSlotInfo2MsgCounter @18 :UInt8;
    slotInfo2Timestamp @19 :UInt64;
    pasApaSlotInfo3MsgCounter @20 :UInt8;
    slotInfo3Timestamp @21 :UInt64;
    pasApaSlotInfo4MsgCounter @22 :UInt8;
    slotInfo4Timestamp @23 :UInt64;
    pasMapPsolObjInfoArray @24 :List(PASMapPsolObjInfoType);
    pasMapPsolObjInfo7MsgCounter @25 :UInt8;
    pasMapPsolObjInfo1MsgCounter @26 :UInt8;
    pasMapPsolObjInfo3MsgCounter @27 :UInt8;
    pasMapPsolObjInfo2MsgCounter @28 :UInt8;
    pasMapPsolObjInfo4MsgCounter @29 :UInt8;
    pasMapPsolObjInfo5MsgCounter @30 :UInt8;
    pasMapPsolObjInfo6MsgCounter @31 :UInt8;
    pasMapPsolObjInfo9MsgCounter @32 :UInt8;
    pasMapPsolObjInfo10MsgCounter @33 :UInt8;
    pasMapPsolObjInfo8MsgCounter @34 :UInt8;
    pasUssInfoArray @35 :List(PasUssInfoType);
    pasUssInfo1MsgCounter @36 :UInt8;
    pasUssInfo4MsgCounter @37 :UInt8;
    pasUssInfo5MsgCounter @38 :UInt8;
    pasUssInfo2MsgCounter @39 :UInt8;
    pasUssInfo3MsgCounter @40 :UInt8;
    pasUssInfo6MsgCounter @41 :UInt8;
    # ID of echo distance
    pasUssTraceIdInfoArray @42 :List(PasUssTraceIdInfoType);
    pasUssTraceIDInfo2p1MsgCounter @43 :UInt8;
    pasUssTraceIDInfo2p2MsgCounter @44 :UInt8;
    pasUssTraceIDInfo2p3MsgCounter @45 :UInt8;
    pasUssTraceIDInfo2p4MsgCounter @46 :UInt8;
    pasUssTraceIDInfo1MsgCounter @47 :UInt8;
    pasUssTraceIDInfo2MsgCounter @48 :UInt8;
    pasUssTraceIDInfo2p5MsgCounter @49 :UInt8;
    pasUssCodeIDInfo1MsgCounter @50 :UInt8;
}

struct Range {
    # uss sensor install pos
    id @0 :Text;
    status @1 :UInt32;
    # Basic CAN frame receive timestamp ns
    recvTimeNs @2 :UInt64;
    # In Longe Range Unit : 0 : first wave ; 1 : second wave
    # In Short Range Unit : 0 : Direct distance ; 1: Indirect distance
    rangeArray @3 :List(UInt32);
}

struct RangeArray {
    enum Type {
        longRangeUnit @0;
        shortRangeUnit @1;
    }
    header @0 :StdHeader.Header;
    type @1 :Type;
    rangeUnitArray @2 :List(Range);
}

# Huawei sensor data

struct AdsfiImageData {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Img params
    width @1 :UInt32;
    height @2 :UInt32;
    dataSize @3 :UInt32;

    # buffer type
    # HAF_BUFFER_CPU = 0
    # HAF_BUFFER_DVPP = 1
    # HAF_BUFFER_AICORE = 2
    # HAF_BUFFER_MBUF = 3
    bufferType @4 :UInt8;

    # image type
    # HAF_IMAGE_RGB_UINT8 = 0,
    # HAF_IMAGE_BGR_UINT8 = 1,
    # HAF_IMAGE_YUV420SP_NV12_UINT8 = 2,
    # HAF_IMAGE_GRAY_UINT8 = 3,
    # HAF_IMAGE_RGB_FLOAT32 = 4,
    # HAF_IMAGE_BGR_FLOAT32 = 5,
    # HAF_IMAGE_YUV420SP_NV12_FLOAT32 = 6,
    # HAF_IMAGE_GRAY_FLOAT32 = 7,
    # HAF_IMAGE_YUV420P_I420_UINT8 = 8,
    # HAF_IMAGE_YUV420SP_NV21_UINT8 = 9,
    # HAF_IMAGE_YUV422SP_UINT8 = 10,
    # HAF_IMAGE_YVU422SP_UINT8 = 11,
    # HAF_IMAGE_RGBA_UINT8 = 12,
    # HAF_IMAGE_BGRA_UINT8 = 13,
    # HAF_IMAGE_ARGB_UINT8 = 14,
    # HAF_IMAGE_ABGR_UINT8 = 15,
    imageType @5 :UInt8;

    seq @6 :UInt32;

    # buffer address
    rawData @7 :UInt64;
    mbufData @8 :UInt64;
}

struct AdsfiImageDataV2 {
    # Img params
    width @0 :UInt32;
    height @1 :UInt32;
    dataSize @2 :UInt32;

    # buffer type
    # HAF_BUFFER_CPU = 0
    # HAF_BUFFER_DVPP = 1
    # HAF_BUFFER_AICORE = 2
    # HAF_BUFFER_MBUF = 3
    bufferType @3 :UInt8;

    # image type
    # HAF_IMAGE_RGB_UINT8 = 0,
    # HAF_IMAGE_BGR_UINT8 = 1,
    # HAF_IMAGE_YUV420SP_NV12_UINT8 = 2,
    # HAF_IMAGE_GRAY_UINT8 = 3,
    # HAF_IMAGE_RGB_FLOAT32 = 4,
    # HAF_IMAGE_BGR_FLOAT32 = 5,
    # HAF_IMAGE_YUV420SP_NV12_FLOAT32 = 6,
    # HAF_IMAGE_GRAY_FLOAT32 = 7,
    # HAF_IMAGE_YUV420P_I420_UINT8 = 8,
    # HAF_IMAGE_YUV420SP_NV21_UINT8 = 9,
    # HAF_IMAGE_YUV422SP_UINT8 = 10,
    # HAF_IMAGE_YVU422SP_UINT8 = 11,
    # HAF_IMAGE_RGBA_UINT8 = 12,
    # HAF_IMAGE_BGRA_UINT8 = 13,
    # HAF_IMAGE_ARGB_UINT8 = 14,
    # HAF_IMAGE_ABGR_UINT8 = 15,
    imageType @4 :UInt8;

    seq @5 :UInt32;
    rawData @6 :List(UInt8);
}

struct AdsfiImageFrame {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Img
    data @1 :AdsfiImageData;
}

struct AdsfiImageFrameV2 {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # Img
    data @1 :AdsfiImageDataV2;
}

struct AdsfiLidarPointCloud {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # point cloud params
    isBigEndian @1 :UInt32;
    height @2 :UInt32;
    width @3 :UInt32;
    pointStep @4 :UInt32;
    rowStep @5 :UInt32;
    isDense @6 :UInt32;

    # point cloud data
    data @7 :Data;
}

enum INSStatus {
    invalid @0;
    valid @1;
}

struct InsToNavi {
    header @0 :StdHeader.Header;

    insStatus @1 :INSStatus;
    
    # WGS-84, precision to 10^-6
    longitude @2 :Float64;
    latitude @3 :Float64;
    
    
    # velocity unit is m/s 
    forwardVel @4 :Float64;
    rightVel @5 :Float64;
    upVel @6 :Float64;
    
    # unit: degree, clockwise, north as 0 degree, [0, 360)
    heading @7 :Float64;
    
    # number of used satellites
    satelliteCount @8 :UInt16;
    
    hdop @9 :Float32;
    # utcTime
    utcTime @10 :Float64;
    # ms
    timestamp @11 :UInt64;

    # gsv data
    gsv @12 :Text;
}

struct PacketFrame {
    recvTimeNs @0 :UInt64;
    frameHeader @1 :Data;
    frameData @2 :Data;
}

struct RawData {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;
    sensorName @1 :Text;
    sensorType @2 :Text;
    packetSize @3 :UInt32;
    packetList @4 :List(PacketFrame);
}

struct CameraRpcMsg {
    topicList @0 :List(Text);
    status @1 : Bool;
}