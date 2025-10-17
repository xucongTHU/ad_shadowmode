@0xedf2ced67e01e6b4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::sensor");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct CameraRawDataEval {
    cameraName @0 :Text;
    cameraType @1 :Text;
    frameFrequency @2 :Float32;
    delayNs @3 :UInt64;
    timeGap @4 :UInt64;
}

struct InsRawDataEval {
    header @0 :StdHeader.Header;
    gnssParseDelayUs @1 :Float64;
    gnssTransDelayUs @2 :Float64;
    insParseDelayUs @3 :Float64;
    insTransDelayUs @4 :Float64;
    imuParseDelayUs @5 :Float64;
    imuTransDelayUs @6 :Float64;
    insTimeGapMs @7 :Float64;
}

struct LidarRawDataEval {
    lidarName @0 :Text;
    lidarType @1 :Text;
    frameFrequency @2 :Float32;
    delayNs @3 :Int32;
}

struct RadarCanStatusEval {
    radarName @0 :Text;
    majorRelease @1 :Int32;
    minorRelease @2 :Int32;
    patchLevel @3 :Int32;
    extendedRange @4 :Int32;
    countryCode @5 :Int32;
    voltageError @6 :Int32;
    persistentError @7 :Int32;
    temperatureError @8 :Int32;
    outputType @9 :Int32;
    objectQuality @10 :Int32;
    objectExtension @11 :Int32;
    motionRxState @12 :Int32;
}

struct SensorEval {
    header @0 :StdHeader.Header;
    lidarEvalList @1 :List(LidarRawDataEval);
    cameraEvalList @2 :List(CameraRawDataEval);
    radarEvalList @3 :List(RadarCanStatusEval);
}

struct SensorEvalRawItem {
    sensorName @0 :Text;
    evalType @1 :Text;
    group @2 :Text;
    dataList @3 :List(Data);
}

struct SensorEvalRaw {
    pid @0 :Text;
    contents @1 :List(SensorEvalRawItem);
}
