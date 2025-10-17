@0xcdb69596284ec92a;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_pdc");

enum SensorIDEnum {
    sensorRRS @0;
    sensorRRO @1;
    sensorRRM @2;
    sensorRLM @3;
    sensorRLO @4;
    sensorRLS @5;
    sensorFRS @6;
    sensorFRO @7;
    sensorFRM @8;
    sensorFLM @9;
    sensorFLO @10;
    sensorFLS @11;
    sensorIDNum @12;
    sensorIDInvalid @13;
}

enum SdwIDEnum {
    sdwFLC @0;
    sdwFRC @1;
    sdwRLC @2;
    sdwRRC @3;
    sdwIDNum @4;
    sdwIDInvalid @5;
}

struct PdcOutput {
    timestampNs @0 :UInt64;
    sectorDis @1 :List(UInt16);             # element num: sensorIDNum + sdwIDNum
    sourceFlag @2 :List(UInt8);             # element num: sensorIDNum + sdwIDNum
}
