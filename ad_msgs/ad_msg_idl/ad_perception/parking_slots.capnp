@0xc8949faeb18d9a5a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using import "/ad_msg_idl/ad_perception/parking_slot.capnp".ParkingSlot;

struct ParkingSlots {
  header @0 :StdMsgsHeader.Header;
  frameTimestampNs @1 :UInt64;
  sensorId @2 :Int32;
  parkingSlotList @3 :List(ParkingSlot);
}
