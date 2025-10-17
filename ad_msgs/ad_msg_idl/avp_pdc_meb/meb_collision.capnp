@0xbbb5b7cdeb2c2736;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_meb");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct Point {
    x @0 :Float32;
    y @1 :Float32;
}

struct Collision {
    distance @0 :Float32;
    obstacleType @1 :UInt8;
    car @2 :Point;
    obj @3 :Point;
}

enum Direction {
    forward @0;
    backward @1;
    forwardLeft @2;
    forwardRight @3;
    backwardLeft @4;
    backwardRight @5;
}

struct Straight {
    upperBound @0 :Float32;
    lowerBound @1 :Float32;
}

struct Turning {
    center @0 :Point;
    furtherBound @1 :Float32;
    closerBound @2 :Float32;
}

struct PathInfo {
    straight @0 :Straight;
    turning @1 :Turning;
}

struct MebCollisionMsg {
    header @0 :StdHeader.Header;
    collision @1 :Collision;
    direction @2 :Direction;
    pathInfo @3 :PathInfo;
}