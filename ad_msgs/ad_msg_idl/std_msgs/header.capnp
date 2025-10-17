@0xed66b10d1bfb3b2c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::std_msgs");

using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct Time {
    nanoSec @0 :UInt64;
}

struct Header {
  seq @0 :UInt32;
  time @1 :Time;
  frameId @2 :Text;
}

struct MapHeader {
  timestampns @0 :UInt64;
  sensorid @1 :UInt64;
  coordtype @2 :UInt8;
  origin @3 :StdGeometry.Point;
}

struct TimeStamp {
    isValid @0 :Bool;
    sec @1 :UInt32;
    nsec @2 :UInt32;
}

struct HistInfo {
    sensorId @0 :Int16;
    ## according to which type use histinfo
    ## for object, => objectId
    ## for road, => laneId
    obstacleId @1 :UInt32;
    rawSensorCaptureTimestamp @2 :TimeStamp;
    sensorPerceptionCaptureTimestamp @3 :TimeStamp;
    sensorPerceptionOutputTimestamp @4 :TimeStamp;
    fusionCaptureTimestamp @5 :TimeStamp;
    fusionOutputTimestamp @6 :TimeStamp;
}
