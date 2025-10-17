@0xb475f88b381eb198;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using Base = import "/ad_msg_idl/ad_perception/base.capnp";
using Object = import "/ad_msg_idl/ad_perception/object.capnp";
using TrafficSign = import "/ad_msg_idl/ad_perception/traffic_sign.capnp";

struct LidarDangerousZoneFrame {
    # Header include seq, timestamp, and frame_id
    header @0 :StdHeader.Header;

    frameTimestampNs @1 :UInt64;

    # enum SensorID
    sensorId @2 :Int32;

    height @3 :Float64;

    # enum DangerousZoneType
    type @4 :Int32;

    dzones @5 :List(Base.LidarDangerousZone);
    }
    struct LidarFovFrame {
    # Header include seq, timestamp, and frame_id
    header @0 :StdHeader.Header;

    frameTimestampNs @1 :UInt64;

    # enum SensorID
    sensorId @2 :Int32;

    # fov map size
    xSize @3 :Int32;
    ySize @4 :Int32;

    # fov map grid resolution
    gridResolution @5 :Float32;
    xyPosRange @6 :Base.Point2f;

    # fov map data.
    data @7 :Data;
}

struct ObjectFrame {
    # Header include seq, timestamp, and frame_id
    header @0 :StdHeader.Header;

    frameTimestampNs @1 :UInt64;

    # enum SensorID
    sensorId @2 :Int32;

    perceptionObjectList @3 :List(Object.Object);
}

struct RoadAreaFrame {
    # Header include seq, timestamp, and frame_id
    header @0 :StdHeader.Header;

    frameTimestampNs @1 :Int64;

    cameraName @2 :Text;

    roadArea @3 :Base.RoadArea;
}

struct RoadFrame {
    # Header include seq, timestamp, and frame_id
    header @0 :StdHeader.Header;

    frameTimestampNs @1 :Int64;

    # enum SensorID
    sensorId @2 :Int32;

    roadStruct @3 :Base.RoadStruct;
}

struct TrafficLightSignFrame {
    # Header include seq, timestamp, and frame_id
    header @0 :StdHeader.Header;

    frameTimestampNs @1 :Int64;

    # enum SensorID
    sensorId @2 :Int32;

    # traffic sign
    trafficSign @3 :List(TrafficSign.TrafficSign);

    # traffic light
    trafficLightGroupList @4 :List(Base.TrafficLightGroup);
    cameraNameList @5 :List(Text);
}
