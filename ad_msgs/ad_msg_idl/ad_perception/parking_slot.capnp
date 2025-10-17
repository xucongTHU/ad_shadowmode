@0xa7f64d15bf6d0c57;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using Base = import "/ad_msg_idl/ad_perception/base.capnp";

struct OriginalCornerPoints {
  oriTl @0 :Base.Point2f;
  oriTr @1 :Base.Point2f;
  oriBl @2 :Base.Point2f;
  oriBr @3 :Base.Point2f;
}

struct ParkingSlot {
  confidence @0 :Float32;
  width @1 :Float32;
  length @2 :Float32;
  center @3 :Base.Point2f;
  wideDirection @4 :Base.Point2f;
  longDirection @5 :Base.Point2f;
  isOccupied @6 :Bool;
  type @7 :UInt32;
  oriCornerPoints @8 :OriginalCornerPoints;

  # type def
  enum Type {
    vertical @0;
    parallel @1;
    slant @2;
  }
}
