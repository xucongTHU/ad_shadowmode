@0xc8949faeb18d9a5a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct FusionParkingSlots {
  header @0 :StdMsgsHeader.Header;
  frameTimestampNs @1 :UInt64;
  fusionParkingSlots @2 :List(FusionParkingSlot);
  fusionParkingSlotsOptimize @3 :List(FusionParkingSlot);
}

enum SlotState {
  unavaliable @0;
  avaliable @1;
  unknown @2;
  unreachable @3;
}

struct FusionParkingSlot {
    # parking slot id
    id @0 :UInt64;

    psType @1 :UInt32;

    # parking slot width and length
    width @2 :Float64;
    length @3 :Float64;

    # parking slot center point
    center @4 :StdGeometry.Point;

    # parking slot wide and long direction
    wideDirection @5 :StdGeometry.Vector3;
    longDirection @6 :StdGeometry.Vector3;

    slotState @7 :SlotState;

    limiter @8 :Float32;

    # parking slot acquire method
    sourceType @9 :UInt32;

    # parking slot apaid
    apaid @10 :UInt64;

}
