@0x926b70ea89195a27;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::chassis_acan");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct Eps2 {
    epsAssistPraSwitchToLka @0 :UInt8;
    epsHandsOffDetSt @1 :UInt8;
    epsHandsOffDetStVd @2 :UInt8;
    epsLkaCtrlAbortFeedback @3 :UInt8;
    epsLkaCtrlDlvdValue @4 :Float32;
    epsLkaCtrlDlvdValueVd @5 :UInt8;
    epsLkaCtrlSt @6 :UInt8;
    epsPasAbortFeedback @7 :UInt8;
    epsPasEpasFailed @8 :UInt8;
    epsPasReqFeedback @9 :UInt8;
}

struct Bcs10A {
    bcsCddapVehHoldActive @0 :UInt8;
    bcsLsM12ComfortAvl @1 :UInt8;
    bcsLsM12EmergencyAvl @2 :UInt8;
    bcsLsM23Avl @3 :UInt8;
    bcsLsmPosLongaCapbl @4 :UInt8;
    bcsLsmSubMtLevelEcho @5 :UInt8;
    bcsLsmSubMtLongEcho @6 :UInt8;
    bcsLsmSubMtReqAvailable @7 :UInt8;
    bcsLsmSubMtReqEcho @8 :UInt8;
    bcsRlWheelRotatedDirection @9 :UInt8;
    bcsRlWheelRotatedDirectionApa @10 :UInt8;
    bcsRlWheelRotatedDirectionVd @11 :UInt8;
    bcsRlWheelSpd @12 :Float32;
    bcsRlWheelSpdVd @13 :UInt8;
    bcsRrWheelRotatedDirection @14 :UInt8;
    bcsRrWheelRotatedDirectionApa @15 :UInt8;
    bcsRrWheelRotatedDirectionVd @16 :UInt8;
    bcsRrWheelSpd @17 :Float32;
    bcsRrWheelSpdVd @18 :UInt8;
}

struct Vcu21A {
    vcuEmsAccPedalActPst @0 :Float32;
    vcuEmsAccPedalActPstVd @1 :UInt8;
    vcuEmsBrkPedalSt @2 :UInt8;
    vcuEmsBrkPedalStVd @3 :UInt8;
    vcuEmsEngSpd @4 :UInt16;
    vcuEmsEngSpdVd @5 :UInt8;
}

struct Vcu13A {
    vcuAcCmaxSpdLimit @0 :UInt8;
    vcuAccLimitOperatingSt @1 :UInt8;
    vcuAccMaxRecpDecel @2 :Float32;
    vcuAccMaxRecpDecelVd @3 :UInt8;
    vcuActVehWheelTorq @4 :UInt16;
    vcuApaAvailable @5 :UInt8;
    vcuApaGearIntervern @6 :UInt8;
    vcuApaResp @7 :UInt8;
    vcuApaWheelTorqDriverReq @8 :UInt16;
    vcuLateralAssistBtnInfo @9 :UInt8;
    vcuLateralAssistBtnInfoVd @10 :UInt8;
}

struct Ebb2A {
    ebbBrkFxCpb @0 :UInt8;
    ebbBrkLightReq @1 :UInt8;
    ebbBrkPedPst @2 :Float32;
    ebbBrkPedPstVd @3 :UInt8;
    ebbBrkPedalApplied @4 :UInt8;
    ebbBrkPedalAppliedSt @5 :UInt8;
}

struct GwBcm2A {
    bcmAtwsSt @0 :UInt8;
    bcmAutoHighBeamCtrlSt @1 :UInt8;
    bcmBeamSwitchSt @2 :UInt8;
    bcmBonnetAjarSt @3 :UInt8;
    bcmBrakeLightSwitchSt @4 :UInt8;
    bcmDriverDoorAjarSt @5 :UInt8;
    bcmDriverDoorLockSt @6 :UInt8;
    bcmFrontFogLampSt @7 :UInt8;
    bcmFrontWasherSt @8 :UInt8;
    bcmFrontWiperSt @9 :UInt8;
    bcmHighBeamSt @10 :UInt8;
    bcmKeySt @11 :UInt8;
    bcmLeftTurnLampFaultSt @12 :UInt8;
    bcmLeftTurnLampSt @13 :UInt8;
    bcmLowBeamSt @14 :UInt8;
    bcmMainLampSwSt @15 :UInt8;
    bcmParkingLampSt @16 :UInt8;
    bcmPsngrDoorAjarSt @17 :UInt8;
    bcmPsngrDoorLockSt @18 :UInt8;
    bcmRearFogLampSt @19 :UInt8;
    bcmReverseSwitchSt @20 :UInt8;
    bcmReverseSwitchStVd @21 :UInt8;
    bcmRightTurnLampFaultSt @22 :UInt8;
    bcmRightTurnLampSt @23 :UInt8;
    bcmRlDoorAjarSt @24 :UInt8;
    bcmRrDoorAjarSt @25 :UInt8;
    bcmTrunkAjarSt @26 :UInt8;
    bcmTurnLightSwitchSt @27 :UInt8;
    bcmVehReverseSt @28 :UInt8;
}

struct Bcs9A {
    bcsFlWheelRotatedDirection @0 :UInt8;
    bcsFlWheelRotatedDirectionApa @1 :UInt8;
    bcsFlWheelRotatedDirectionVd @2 :UInt8;
    bcsFlWheelSpd @3 :Float32;
    bcsFlWheelSpdVd @4 :UInt8;
    bcsFrWheelRotatedDirection @5 :UInt8;
    bcsFrWheelRotatedDirectionApa @6 :UInt8;
    bcsFrWheelRotatedDirectionVd @7 :UInt8;
    bcsFrWheelSpd @8 :Float32;
    bcsFrWheelSpdVd @9 :UInt8;
}

struct Vcu9A {
    vcuBrkPedPst @0 :Float32;
    vcuBrkPedPstVd @1 :UInt8;
    vcuBrkVcuuPress @2 :UInt16;
    vcuBrkVcuuPressVd @3 :UInt8;
    vcuCrntGearLvl @4 :UInt8;
    vcuCrntGearLvlVd @5 :UInt8;
    vcuRecpTorqFa @6 :UInt16;
    vcuRecpTorqFavd @7 :UInt8;
}

struct Vcu14A {
    emsAccButtInfo @0 :UInt8;
    emsGasPedalActPstforMrr @1 :Float32;
    emsGasPedalActPstforMrrvd @2 :UInt8;
    vcuAccDriverOrvd @3 :UInt8;
    vcuAccElecEcFail @4 :UInt8;
    vcuAccTmcFail @5 :UInt8;
    vcuVehWheelTorqMax @6 :UInt16;
}

struct Bcs11A {
    bcsActVehLaltrlAccel @0 :Float32;
    bcsActVehLaltrlAccelVd @1 :UInt8;
    bcsActVehLongAccel @2 :Float32;
    bcsActVehLongAccelVd @3 :UInt8;
    bcsTcsOff @4 :UInt8;
    bcsVdcOff @5 :UInt8;
    bcsVehLaltrlAccelOffset @6 :Float32;
    bcsVehLongAccelOffset @7 :Float32;
}

struct Eps1 {
    epsFunctionStyleSt @0 :UInt8;
    epsSteeringAngle @1 :Float32;
    epsSteeringAngleSpd @2 :UInt16;
    epsSteeringAngleSpdVd @3 :UInt8;
    epsSteeringAngleVd @4 :UInt8;
    epsSteeringModeSt @5 :UInt8;
    epsStrngWhlTorq @6 :Float32;
    epsStrngWhlTorqVd @7 :UInt8;
    epsWarnLamp @8 :UInt8;
}

struct Bcs6A {
    bcsYawRate @0 :Float32;
    bcsYawRateOffset @1 :Float32;
    bcsYawRateSt @2 :UInt8;
}

struct ChassisAcan {
    # Header include seq, timestamp, and frame_id(vehicle_type)
    header @0 :StdHeader.Header;

    eps2 @1 :Eps2;
    bcs10A @2 : Bcs10A;
    vcu21A @3 : Vcu21A;
    vcu13A @4 : Vcu13A;
    ebb2A @5 : Ebb2A;
    gwBcm2A @6 : GwBcm2A;
    bcs9A @7 : Bcs9A;
    vcu9A @8 : Vcu9A;
    vcu14A @9 : Vcu14A;
    bcs11A @10 : Bcs11A;
    eps1 @11 : Eps1;
    bcs6A @12 : Bcs6A;
}

