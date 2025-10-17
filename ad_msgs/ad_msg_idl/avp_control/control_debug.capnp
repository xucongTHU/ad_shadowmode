@0xb88d98fa51b9f3fa;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_control");

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

    adrcTdVel @97 :Float64;
    adrcTdVelDot @98 :Float64;
    adrcObsEsoZ1 @99 :Float64;
    adrcObsEsoZ2 @100 :Float64;
    adrcObsEsoZ3 @101 :Float64;
    adrcComputerU1 @102 :Float64;
    adrcComputerU2 @103 :Float64;
    adrcComputerU3 @104 :Float64;
    adrcComputerU @105 :Float64;
    adrcFhan @106 :Float64;

    trajCurMax @107 :Float64;
    trajCurMin @108 :Float64;
    trajCurRateMid @109 :Float64;
    trajCurRateHigh @110 :Float64;
    trajCurRateHigher @111 :Float64;
    trajCurRateFlag @112 :Float64;
    trajCurSteerAngleMinIndex @113 :Float64;
    trajCurSteerAngleMaxIndex @114 :Float64;
    trajCurDeltaSteerAngleIndex @115 :Float64;

    parkingPointA @116 :Float64;
    parkingPointB @117 :Float64;
    parkingPointC @118 :Float64;
    parkingPointD @119 :Float64;
    parkingPointParallelA @120 :Float64;
    parkingPointParallelB @121 :Float64;
    intersectionPointLineTraj @122 :Float64;

    speedObserver @123 :Float64;
    slope @124 :Float64;
    slopeCompensate @125 :Float64;
    padeXi @126 :Float64;
    distancePreview @127 :Float64;
    motorSpeed2Vel @128 :Float64;
    latWheelAngleFf @129 :Float64;
    latWheelAngleFb @130 :Float64;
    latOutput @131 :Float64;

    standbyLog1 @132 :Float64;
    standbyLog2 @133 :Float64;

    turnSteerDecision @134 :UInt32;
    turnSteerDecisionLast @135 :UInt32;
    trajSumDistance @136 :Float64;
    latHeadingErrFb @137 :Float64;
    latDistErrFb @138 :Float64;
    parkStopDist @139 :Float64;
    torqueCmd @140 :Float64;
}
