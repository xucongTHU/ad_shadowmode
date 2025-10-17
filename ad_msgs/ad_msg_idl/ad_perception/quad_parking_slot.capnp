@0xd9f9a20c89d6b223;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using Base = import "/ad_msg_idl/ad_perception/base.capnp";

struct IpmSlotLimiter {
    limiterPoint @0 :Base.Point2f;
    confidence @1 :Float32;
    limiterType @2 :UInt32;
}

struct ApproxBoxPoints {
  point @0 :Base.Point2f;
  borderDist @1 :Float32;
  pointScore @2 :Float32;
  lineLen @3 :Float32;
  lineScore @4 :Float32;
  hasBorderPoint @5 :Bool;
}

struct QuadParkingSlot {
  tl @0 :Base.Point2f;
  tr @1 :Base.Point2f;
  bl @2 :Base.Point2f;
  br @3 :Base.Point2f;
  confidence @4 :Float32;
  label @5 :UInt32;
  filtered @6 :Bool;
  slotType @7 :Int32;
  sTl @8 :Float32;
  sTr @9 :Float32;
  sBl @10 :Float32;
  sBr @11 :Float32;
  dirIn @12 :Base.Point2f;
  dirWidth @13 :Base.Point2f;
  dirLength @14 :Base.Point2f;
  center @15 :Base.Point2f;
  oppModify @16 :Bool;
  isComplete @17 :Bool;
  width @18 :Float32;
  length @19 :Float32;
  isVisited @20 :Bool;
  pTl @21 :ApproxBoxPoints;
  pTr @22 :ApproxBoxPoints;
  pBl @23 :ApproxBoxPoints;
  pBr @24 :ApproxBoxPoints;
  oriTl @25 :Base.Point2f;
  oriTr @26 :Base.Point2f;
  oriBl @27 :Base.Point2f;
  oriBr @28 :Base.Point2f;
}
