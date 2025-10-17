@0xa937f268a243bad8;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using import "/ad_msg_idl/ad_perception/quad_parking_slot.capnp".QuadParkingSlot;
using import "/ad_msg_idl/ad_perception/quad_parking_slot.capnp".IpmSlotLimiter;

struct QuadParkingSlots {
  header @0 :StdMsgsHeader.Header;
  frameTimestampNs @1 :UInt64;
  sensorId @2 :Int32;
  quadParkingSlotList @3 :List(QuadParkingSlot);
  slotLimiterList @4 :List(IpmSlotLimiter);
}
