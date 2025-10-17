@0xa6e6c2875c9ac7a1;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using RawData = import "/ad_msg_idl/std_msgs/raw_data.capnp";
using Obstacles = import "/ad_msg_idl/ad_perception/obstacles.capnp";
using ParkingSlots = import "/ad_msg_idl/ad_perception/parking_slots.capnp";
using QuadParkingSlots = import "/ad_msg_idl/ad_perception/quad_parking_slots.capnp";

struct Obstacle2DImage {
  image @0 :RawData.Image;
  obstacles @1 :List(Obstacles.ObstacleRaw);
}

struct Obstacle2DImages {
  obstacleImgs @0 :List(Obstacle2DImage);
}

struct ObstacleImages {
  images @0 :List(RawData.Image);
  obstacles @1 :Obstacles.Obstacles;
}

struct RawIpmImage {
  avmimg @0 :RawData.Image;
  ipmimg @1 :List(RawData.Image);
  slots @2 :ParkingSlots.ParkingSlots;
  quadslots @3 :QuadParkingSlots.QuadParkingSlots;
}
