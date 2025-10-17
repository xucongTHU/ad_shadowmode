@0x926b70ea89195a27;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::mrr");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct Mrr2 {
    mrrLongCtrlTargetAccel @0 :Float32;
    adasLongCtrlTypReqMode @1 :UInt8;
    adasLongCtrlTypReqLong @2 :UInt8;
    adasIntlCrusMode @3 :UInt8;
    mrrACCRecpDecel @4 :UInt8;
    mrrLongCtrlTargetAccelJerkMinVa @5 :UInt8;
    mrrifcPriCANAvailableSt @6 :Bool;
    mrrLongCtrTargetAccelJerkMaxnVa @7 :UInt8;
    mrrifcPriCANErrorSt @8 :Bool;
    mrrLongCtrlDecToStopReq @9 :Bool;
    mrrLongCtrlDriveOff @10 :Bool;
    mrrLongCtrlShutdownReq @12 :UInt8;
    mrrACCMinBrakingReq @11 :Bool;
    mrrACCRecpDecelReq @13 :Bool;
    mrrLongCtrlModeForBrkSys @14 :UInt8;
    mrrLongCtrlAccelCtrlReq @15 :Bool;
}

struct Mrr3 {
    mrrAEBTargetDecel @0 :Float32;
    mrrAEBDecelCtrlReq @1 :Bool;
    mrrACCReqQua @2 :Bool;
    mrrAEBLVehHoldReq @3 :Bool;
    mrrABAReq @4 :Bool;
    mrrABALevel @5 :UInt8;
    mrrAWBReq @6 :Bool;
    mrrABPReq @7 :Bool;
    mrrAWBLevel @8 :UInt8;
    mrrActTrqReq @9 :Float32;
    mrrActTrqReqVD @10 :Bool;
    mrrACCUnableActivated @11 :UInt8;
}

struct MrrIfc1 {
    ifcLdwSt @0 :UInt8;
    ifcRdpSt @1 :UInt8;
    ifcRdpActdirection @2 :UInt8;
    ifcLdpActdirection @3 :UInt8;
    ifcTsrSt @4 :UInt8;
    ifcTsrSpdLimType @5 :UInt8;
    ifcLdwActdirection @6 :UInt8;
    ifcTsrAudioWarnEnable @7 :Bool;
    ifcIlcEnableSt @8 :Bool;
    ifcLdsLaneAssistType @9 :UInt8;
    ifcLdpSt @10 :UInt8;
    ifcTSRMode @11 :UInt8;
    ifcHandsOnSt @12 :UInt8;
    ifcHandsOnWarning @13 :UInt8;
    ifcTsrSpdLimit @14 :UInt8;
    ifcTsrSpdLimitUnit @15 :UInt8;
    ifcMRCNumber @16 :UInt8;
    ifcIHbcSt @17 :UInt8;
    ifcIHbcHighBeamReq @18 :Bool;
    ifcTsrEnable @19 :Bool;
    ifcIHbcEnable @20 :Bool;
    ifcTsrVisualWarn @21 :Bool;
    ifcTsrAcousticWarn @22 :Bool;
    ifcILoaEnableSt @23 :Bool;
    ifcMRMNumber @24 :UInt8;
}


struct MrrIfc2 {
    ifcTorqueReq @0 :Float32;
    ifcTorqueReqSt @1 :UInt8;
    adasRemoteDiagCode @2 :UInt8;
}

struct Mrr {
    # Header include seq, timestamp, and frame_id(vehicle_type)
    header @0 :StdHeader.Header;

    mrr2 @1 :Mrr2;
    mrr3 @2 : Mrr3;
    mrrIfc1 @3 : MrrIfc1;
    mrrIfc2 @4 : MrrIfc2;
}

