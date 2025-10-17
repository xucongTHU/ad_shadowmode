@0xc7385f54f3b667cf;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::std_msgs");

struct Point {
    x @0 :Float64;
    y @1 :Float64;
    z @2 :Float64;
}

struct Vector3 {
    x @0 :Float64;
    y @1 :Float64;
    z @2 :Float64;
}

struct Quaternion {
    x @0 :Float64;
    y @1 :Float64;
    z @2 :Float64;
    w @3 :Float64;
}

struct Pose {
    position @0 :Point;
    orientation @1 :Quaternion;
}