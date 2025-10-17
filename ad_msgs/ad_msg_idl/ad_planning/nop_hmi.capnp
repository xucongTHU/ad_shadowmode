@0x8e2085ba631b1e83;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::planning");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct NopHmiLcInfo {
    enum LcType {
        lcTypeNone @0;
        lcTypeILC @1;
        lcTypeMLC @2;
        lcTypeALC @3;
    }
    lcType @0 :LcType;

    enum MlcType {
        mlcTypeNone @0;
        mlcTypeRoadToRamp @1;
        mlcTypeRampToRoad @2;
        mlcTypeRoadToRoad @3;
        mlcTypeRampToRamp @4;
    }
    mlcType @1 :MlcType;

    enum LcStage {
        lcStageNone @0;
        lcStageWait @1;
        lcStageProcess @2;
        lcStageBack @3;
        lcStageDone @4;
    }
    lcStage @2 :LcStage;

    enum LcDirection {
        lcDirectionNone @0;
        lcDirectionLeft @1;
        lcDirectionRight @2;
    }
    lcDirection @3 :LcDirection;

    targetLaneId @4 :UInt64;
}

struct NopHmiCipvInfo {
    hasCipv @0 :Bool;
    trackId @1 :Int32;
}

struct NopHmiRoadInfo {
    velLimit @0 :Float32;
}

struct NopHmiDodgeInfo {
    truckId @0 :List(Int32);
    objectId @1 :List(Int32);
}

struct VelHmiInfo {
    selectedTimeHeadway @0 :Int32;
    selectedSpeed @1 :UInt32;
    vehicleState @2 :Int32;
}

struct NopHmiPanelInfo {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    lcInfo @1 :NopHmiLcInfo;
    cipvInfo @2 :NopHmiCipvInfo;
    roadInfo @3 :NopHmiRoadInfo;
    dodgeInfo @4 :NopHmiDodgeInfo;
    vehicleInfo @5 :VelHmiInfo;
}

# fcwmode:
# 0x0:OFF mode
# 0x1:On mode
# 0x2:Active mode
# 0x3:Failure mode
#
# aebMode：
# 0x0:OFF mode
# 0x1:On mode
# 0x2:Active mode
# 0x3:Failure mode
#
# lkaMode:
# 0x00:FAIL
# 0x01:OFF
# 0x02:STANDBY
# 0x03:ENABLED
# 0x04:INTERVENTION_LEFT
# 0x05:INTERVENTION_RIGHT
# 0x06:COMPLETED
# 0x07:ABORTED
# 0x08:SUPPRESSED_LEFT
# 0x09:SUPPRESSED_RIGHT
#
# elkMode:
# 0x00:FAIL
# 0x01:OFF
# 0x02:STANDBY
# 0x03:ENABLED
# 0x04:INTERVENTION_LEFT
# 0x05:INTERVENTION_RIGHT
# 0x06:COMPLETED
# 0x07:ABORTED
# 0x08:SUPPRESSED_LEFT
# 0x09:SUPPRESSED_RIGHT
#
# ldwMode:
# 0x00:FAIL
# 0x01:OFF
# 0x02:STANDBY
# 0x03:ENABLED
# 0x04:LEFT_WARNING
# 0x05:RIGHT_WARNING
#
# shadowMode:
# bit0:  0:BrakeOpen              1:shadowMode
# bit1:  0:AEB not active         1: aeb active
# bit2:  0:EBA not active         1: EBA active
# bit3:  0:AEB/FCW/DW not active  1: AWB/FCW/DW active

struct ASStateMachine {
    header @0 :StdHeader.Header;
    fcwMode @1 :UInt8;
    aebMode @2 :UInt8;
    lkaMode @3 :UInt8;
    elkMode @4 :UInt8;
    ldwMode @5 :UInt8;
    shadowMode @6 :UInt8;
    lookIndex @7 : UInt32;
    fctbMode @8 :UInt8;
    rctbMode @9 :UInt8;
    esaMode @10 :UInt8;
    aesMode @11 :UInt8;
}
