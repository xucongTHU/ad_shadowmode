@0xb88d98fa51b9f3fa;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::control");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct ControlDebug {
    header @0 :StdMsgsHeader.Header;

    mpcTrajectoryIndex @1 :Float64;
    mpcUtmx @2 :Float64;
    mpcUtmy @3 :Float64;
    mpcAngleOffset @4 :Float64;
    mpcDeltaU @5 :Float64;
    mpcDeltaULimit @6 :Float64;
    mpcCalculateTimeMs @7 :Float64;
    frontWheelLastTime @8 :Float64;
    steerLimit @9 :Float64;
    sigmaCmd @10 :Float64;
    trajectoryStartIndexLat @11 :Float64;
    trajectoryStartIndexLatDelay @12 :Float64;
    trajectoryStartIndexLatPreview @13 :Float64;
    tgtPosX @14 :Float64;
    tgtPosY @15 :Float64;
    latPidOut @16 :Float64;
    latTotalErr @17 :Float64;
    tgtFrontRearAngle @18 :Float64;
    latPidMax @19 :Float64;
    latPidMin @20 :Float64;
    latPidI @21 :Float64;
    imax @22 :Float64;
    imin @23 :Float64;

    posTimeIndex @24 :Float64;
    velTimeIndex @25 :Float64;
    accTimeIndex @26 :Float64;
    velI @27 :Float64;
    accFb @28 :Float64;
    accFf @29 :Float64;
    gravityCompen @30 :Float64;
    velFf @31 :Float64;
    velFb @32 :Float64;
    velCmd @33 :Float64;
    accCmd @34 :Float64;
    accCompenCmdBfFilter @35 :Float64;
    accCompenCmdAfFilter @36 :Float64;
    accErr @37 :Float64;
    accI @38 :Float64;

    steerAngleCmd @39 :Float64;
    throttleCmd @40 :Float64;
    brakeCmd @41 :Float64;
    gearCmd @42 :UInt32;
    turnSignalCmd @43 :Int32;

    headingError @44 :Float64;
    headingErrorDelay @45 :Float64;
    headingErrorPreview @46 :Float64;
    latDistanceError @47 :Float64;
    latDistanceErrorDelay @48 :Float64;
    latDistanceErrorPreview @49 :Float64;
    latYawRateError @50 :Float64;
    latYawRateErrorDelay @51 :Float64;
    latYawRateErrorPreview @52 :Float64;
    lonDistanceError @53 :Float64;
    velError @54 :Float64;
    stopDisErr @55 :Float64;
    stopPointIndex @56 :UInt32;
    exceedStopPointFlag @57 :UInt32;

    utmPosX @58 :Float64;
    utmPosY @59 :Float64;
    utmVelX @60 :Float64;
    utmVelY @61 :Float64;
    utmAccX @62 :Float64;
    utmAccY @63 :Float64;
    utmRoll @64 :Float64;
    utmPitch @65 :Float64;
    utmPitchFilter @66 :Float64;
    utmHeading @67 :Float64;

    vehicleVelX @68 :Float64;
    vehicleAccX @69 :Float64;
    vehicleAccY @70 :Float64;
    vehicleAngularYawRate @71 :Float64;
    vehicleSteerAngle @72 :Float64;
    vehicleThrottle @73 :Float64;
    vehicleBrake @74 :Float64;
    vehicleGear @75 :UInt32;
    vehicleTurnLight @76 :UInt32;

    vehicleAutoEnable @77 :UInt32;
    vehicleAccEnable @78 :UInt32;
    vehicleBrakeEnable @79 :UInt32;
    vehicleSteerEnable @80 :UInt32;
    vehicleFlDoorState @81 :UInt32;
    vehicleFrDoorState @82 :UInt32;
    vehicleRlDoorState @83 :UInt32;
    vehicleRrDoorState @84 :UInt32;
    vehicleTrunkDoorState @85 :UInt32;

    trajectoryLength @86 :UInt32;
    trajRealstateUpdate @87 :UInt32;

    vehicleCurrentState @88 :UInt32;

    tgtHeading @89 :Float64;
    tgtHeadingDelay @90 :Float64;
    tgtHeadingPreview @91 :Float64;

    deviationHeading @92 :Float64;
    adaptiveHeading @93 :Float64;
    originHeading @94 :Float64;

    maxCurvature @95 :Float64;
    weightYawRateErr @96 :Float64;

    lqrLatError @97 :Float64;
    lqrLatErrorRate @98 :Float64;
    lqrHeadingError @99 :Float64;
    lqrHeadingErrorRate @100 :Float64;
    lqrRefHeading @101 :Float64;
    lqrHeading @102 :Float64;
    lqrRefCurvature @103 :Float64;
    lqrSteerAngle @104 :Float64;
    lqrSteerAngleFeedforward @105 :Float64;
    lqrSteerAngleFeedback @106 :Float64;

    controlTextboxString @107 :Text;
}
