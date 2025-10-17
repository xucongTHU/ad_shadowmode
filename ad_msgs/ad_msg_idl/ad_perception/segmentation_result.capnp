@0xbdbbd6f6848f833c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using RawData = import "/ad_msg_idl/std_msgs/raw_data.capnp";
using Base = import "/ad_msg_idl/ad_perception/base.capnp";

struct SegmentImages {
  images @0 :List(RawData.Image);
}

struct OdomSegmentImages {
  segmentImgs @0 :SegmentImages;
  selfCarPose @1: Base.Point3f;
  selfCarAngle @2: Base.Point3f;
}
