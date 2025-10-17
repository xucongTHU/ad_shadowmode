@0xe7c138c86fba0d48;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_fusion_map");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct TrajectoryPoint {
    x @0 : Float32;
    y @1 : Float32;
    theta @2 : Float32;
}

struct Trajectory {
    trajectoryPoints @0 :List(TrajectoryPoint);
}

struct TrajectoryArray {
    header @0 :StdHeader.Header;
    trajectoryList @1 :List(Trajectory);
}