@0x866b2869b30e1760;

using Cxx = import "/capnp/c++.capnp";
using Gac = import "/ad_msg_idl/ad_gac/gac.capnp";

$Cxx.namespace("gac::rscl");

struct GOP {
    histinfovector @0 :Gac.VectorHistInfo;
    id @1 :UInt32;
    laneid @2 :UInt64;
    type @3 :UInt8;
    position @4 :Gac.Bbox3D;
    headingangle @5 :Float64;
    confidence @6 :Float64;
    trackingperiod @7 :Float64;
    trackingstatus @8 :UInt8;
}

struct FusionGop {
    timestamp @0 :Gac.TimeStamp;
    coordinate @1 :Gac.CoordinateSystem;
    sensorid @2 :Gac.SensorId;
    gopvector @3 :List(GOP);
}
