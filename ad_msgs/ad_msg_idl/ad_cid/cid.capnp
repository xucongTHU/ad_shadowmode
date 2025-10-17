@0xb2ad62d1eb0a19ec;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::cid");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct ArrayObjectInfoS {
    buffer @0 :List(UInt8);
}

struct ArrayStopLineInfoS {
    buffer @0 :List(UInt8);
}

struct ArrayLaneInfoS {
    buffer @0 :List(UInt8);
}

struct ArrayLaneLineInfoS {
    buffer @0 :List(UInt8);
}

struct StructEgoRoadInfoS {
    buffer @0 :List(UInt8);
}

struct ArrayLaneMarkerInfoS {
    buffer @0 :List(UInt8);
}

struct ArrayPedestrianCrossInfoS {
    buffer @0 :List(UInt8);
}

struct ArrayTrafficSignInfoS {
    buffer @0 :List(UInt8);
}

struct ArrayTrafficLightInfoS {
    buffer @0 :List(UInt8);
}