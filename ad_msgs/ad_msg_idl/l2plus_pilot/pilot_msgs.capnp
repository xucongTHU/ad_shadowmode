@0xdb2949a3444f92be;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::pilot");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct AccelCorrVehDynT {
    corrAccel @0 :Float32;
    corrAccelVar @1 :Float32;
}

struct AccObjectUsageStatusT {
    interest @0 :Bool;
    useForControl @1 :Bool;
    useForAlert @2 :Bool;
    lostReason @3 :Bool;
}

struct AccOoiGenAttributesT {
    uiCutInOutProbability @0 :UInt8;
    eDynamicProperty @1 :UInt8;
    fLifeTime @2 :Float32;
    uiLifeCycles @3 :UInt16;
    eMaintenanceState @4 :UInt8;
    uiObjectId @5 :Int8;
    eUsageState @6 :UInt8;
}

struct AccOoiGenKinematicsT {
    fDistX @0 :Float32;
    fDistY @1 :Float32;
    fVrelX @2 :Float32;
    fVrelY @3 :Float32;
    fArelX @4 :Float32;
    fArelY @5 :Float32;
    fVabsX @6 :Float32;
    fVabsY @7 :Float32;
    fAabsX @8 :Float32;
    fAabsY @9 :Float32;
}

struct AccOoiNextLongT {
    kinematic @0 :AccOoiGenKinematicsT;
    attributes @1 :AccOoiGenAttributesT;
    eRelObjLossReason @2 :UInt8;
}

struct AccOoiT {
    kinematic @0 :AccOoiGenKinematicsT;
    attributes @1 :AccOoiGenAttributesT;
}

struct AccOutputStatusT {
    alert @0 :Bool;
    inhibited @1 :Bool;
    inhibitionReason @2 :UInt8;
    allowInit @3 :Bool;
}

struct AccSituationClassifierT {
    situation @0 :UInt8;
    criticality @1 :UInt8;
}

struct AlnAlgoParametersT {
    fElevMonMax @0 :Float32;
    fAzimMonMax @1 :Float32;
    fElevEolMax @2 :Float32;
    fAzimEolMax @3 :Float32;
    fElevMonMaxDiff @4 :Float32;
    fAzimMonMaxDiff @5 :Float32;
    bElevMonEnable @6 :Bool;
    bAzimMonEnable @7 :Bool;
    bEolChannelCalibEnable @8 :Bool;
    bOnlineChannelCalibEnable @9 :Bool;
    bOnlineAngleDevCalibEnable @10 :Bool;
    uDummy @11 :List(UInt8);
}

struct AssessedObjListT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    headerAssessedObjList @2 :HeaderAssessedObjListT;
    objList @3 :List(FctPubObjectT);
}

struct AttributesT {
    eDynamicProperty @0 :UInt8;
    uiStoppedConfidence @1 :UInt8;
    eAbsMovingState @2 :UInt8;
    eClassification @3 :UInt8;
    uiClassConfidence @4 :UInt8;
}

struct BswSAlgoParametersT {
    uiVersionNumber @0 :UInt32;
    fct @1 :FctAlgoParametersT;
    em @2 :EnvmAlgoParametersT;
    aln @3 :AlnAlgoParametersT;
}

struct BswSEnvmCtrlDataT {
    uVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    hwSample @2 :UInt16;
    envmOpMode @3 :UInt8;
    rspCycleViolation @4 :Bool;
    envmCycleViolation @5 :Bool;
    uiTimeStampUs @6 :UInt32;
    uDummy81 @7 :UInt8;
    uDummy82 @8 :UInt8;
    uDummy83 @9 :UInt8;
}

struct BusDebugMiOs {
    debugMiOs @0 :List(BusMio);
}

struct BusDelayTime {
    lccraEgofrontdelaytimeSec @0 :Float32;
    lccraEgoreardelaytimeSec @1 :Float32;
    lccraLeftfrontdelaytimeSec @2 :Float32;
    lccraLeftreardelaytimeSec @3 :Float32;
    lccraRightfrontdelaytimeSec @4 :Float32;
    lccraRightreardelaytimeSec @5 :Float32;
    lccraNextleftfrontdelaytimeSec @6 :Float32;
    lccraNextleftreardelaytimeSec @7 :Float32;
    lccraNextrightfrontdelaytimeSec @8 :Float32;
    lccraNextrightreardelaytimeSec @9 :Float32;
}

struct BusMio {
    id @0 :Int16;
    position @1 :List(Float32);
    velocity @2 :List(Float32);
    acceleration @3 :List(Float32);
    length @4 :Float32;
    width @5 :Float32;
    rangeMagnitude @6 :Float32;
    velocityMagnitude @7 :Float32;
    accelerationMagnitude @8 :Float32;
    ttc @9 :Float32;
    timeGap @10 :Float32;
}

struct BusObject {
    numTgtObj @0 :UInt8;
    objects @1 :List(BusObjPara);
}

struct BusObjPara {
    id @0 :Int16;
    position @1 :List(Float32);
    velocity @2 :List(Float32);
    acceleration @3 :List(Float32);
    length @4 :Float32;
    width @5 :Float32;
    headingAngle @6 :Float32;
    classification @7 :UInt8;
    sensorsource @8 :UInt32;
    existence @9 :Float32;
}

struct BusWeight {
    lccraEgofrontweightNu @0 :UInt8;
    lccraEgorearweightNu @1 :UInt8;
    lccraLeftfrontweightNu @2 :UInt8;
    lccraLeftrearweightNu @3 :UInt8;
    lccraRightfrontweightNu @4 :UInt8;
    lccraRightrearweightNu @5 :UInt8;
    lccraNextleftfrontweightNu @6 :UInt8;
    lccraNextleftrearweightNu @7 :UInt8;
    lccraNextrightfrontweightNu @8 :UInt8;
    lccraNextrightrearweightNu @9 :UInt8;
}

struct BWarnLevelT {
    bPreWarnActive @0 :Bool;
    bAcuteWarnActive @1 :Bool;
    bDoorLockingActive @2 :Bool;
}

struct CamLaneT {
    type @0 :UInt8;
    quality @1 :UInt8;
    position @2 :Float32;
    curvature @3 :Float32;
    curvatureDer @4 :Float32;
    widthMarking @5 :Float32;
    headingAngle @6 :Float32;
    viewRangeStart @7 :Float32;
    viewRangeEnd @8 :Float32;
    lineCrossing @9 :UInt8;
    lineMarkColor @10 :UInt8;
    canMsgEnd @11 :UInt8;
}

struct CamObjAttributes {
    eStatusBrakeLight @0 :UInt8;
    uiBrakeLightConfidence @1 :UInt8;
    eStatusTurnIndicator @2 :UInt8;
    uiTurnIndicatorConfidence @3 :UInt8;
    eAssociatedLane @4 :UInt8;
    uiAssociatedLaneConfidence @5 :UInt8;
    eLaneChangeManeuver @6 :UInt8;
    percentageOwnDrivingLane @7 :UInt8;
    percentageSideDrivingLaneLeft @8 :UInt8;
    percentageSideDrivingLaneRight @9 :UInt8;
    eMainteanceState @10 :UInt8;
    percentageProbOfExist @11 :UInt8;
    eMotionState @12 :UInt8;
    eObjectOcclusion @13 :UInt8;
    objectLifeCycles @14 :UInt32;
}

struct CamObjClassification {
    probabilityOfClassification @0 :UInt8;
    objectType @1 :UInt8;
}

struct CamObjectList {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    aObject @2 :List(CamObject);
    sLaneLines @3 :List(CamLaneT);
}

struct CamObject {
    objectId @0 :UInt8;
    kinematic @1 :CamObjKinematics;
    geometry @2 :CamObjGeometry;
    classification @3 :CamObjClassification;
    attributes @4 :CamObjAttributes;
}

struct CamObjGeometry {
    fHeight @0 :Float32;
    fOffsetToGround @1 :Float32;
    fLength @2 :Float32;
    fWidth @3 :Float32;
}

struct CamObjKinematics {
    fDistX @0 :Float32;
    fDistXStd @1 :Float32;
    fDistY @2 :Float32;
    fDistYStd @3 :Float32;
    fVx @4 :Float32;
    fVxStd @5 :Float32;
    fVy @6 :Float32;
    fVyStd @7 :Float32;
    fAx @8 :Float32;
    fAxStd @9 :Float32;
    fAy @10 :Float32;
    fAyStd @11 :Float32;
    fOrientation @12 :Float32;
    eRefpointLocation @13 :UInt8;
}

struct CcAccelerationControlDataT {
    maximumAccelerationLateralLimited @0 :Int16;
    maximumCommandedAcceleration @1 :Int16;
    minimumCommandedAcceleration @2 :Int16;
    maximumRequestedAcceleration @3 :Int16;
    minimumRequestedAcceleration @4 :Int16;
    maximumRequestedAccelerationFiltered @5 :Int16;
    minimumRequestedAccelerationFiltered @6 :Int16;
    maximumRequestedAccelerationLastCycle @7 :Int16;
    minimumRequestedAccelerationLastCycle @8 :Int16;
    minAllowedAccel @9 :Int16;
    maxAllowedAccel @10 :Int16;
    maxCustomAllowedAccel @11 :Int16;
    minCustomAllowedAccel @12 :Int16;
    accelStatus @13 :UInt8;
    accelGradientLimits @14 :CcAccelGradientLimitsT;
}

struct CcAccelGradientLimitsT {
    maxNegGrad @0 :Int32;
    maxPosGrad @1 :Int32;
}

struct CcControlDataT {
    limAcceleration @0 :Int16;
    vlcAcceleration @1 :Int16;
    vlcAccelerationPPart @2 :Int16;
    vlcAccelerationIPart @3 :Int16;
    maximumAccelerationLimit @4 :Int16;
    minimumAccelerationLimit @5 :Int16;
    accelerationRequestGrad @6 :Int32;
    decelLimOverrideActive @7 :UInt8;
    vlcAvlcToVlcTransition @8 :UInt8;
    decelLimActiveCounter @9 :UInt16;
    vlcAccelFilterTime @10 :UInt16;
}

struct CparEbaParametersT {
    ebaCodingParamGen @0 :UInt32;
    ebaCodingParamFmod @1 :UInt32;
    ebaCodingParamValid @2 :Bool;
}

struct CpCustOutPredTrajT {
    fCurveC0 @0 :Float32;
    fCurveC1 @1 :Float32;
    fAngle @2 :Float32;
    ucPredictState @3 :UInt8;
}

struct CtaDebug {
    header @0 :StdHeader.Header;
    ctaInput @1 :CtaInputT;
    ctaParam @2 :CtaParamT;
    ctaOutput @3 :CtaOutProT;
    ctaDebug @4 :CtaDebugT;
}

struct CtaDebugT {
    uiVersionNumber @0 :UInt32;
    fctaDebugInfo @1 :CtafctaDebugInfoT;
    rctaDebugInfo @2 :CtarctaDebugInfoT;
    ctaObjectList @3 :List(CtaObjectInfoGlobalT);
    ctObjectList @4 :List(CtObjectInfoGlobalT);
}

struct CtaEgoVehicleInReqT {
    fegoVelocityMps @0 :Float32;
    fYawRateRadps @1 :Float32;
    fSelfSteeringRad @2 :Float32;
    uGearNu @3 :UInt8;
}

struct CtaemsrrObjectInReq {
    fDistXMet @0 :Float32;
    fDistXStdMet @1 :Float32;
    fDistYMet @2 :Float32;
    fDistYStdMet @3 :Float32;
    fVrelXMps @4 :Float32;
    fVrelYMps @5 :Float32;
    fArelXMpss @6 :Float32;
    fArelYMpss @7 :Float32;
    fVabsXMps @8 :Float32;
    fVabsYMps @9 :Float32;
    fWidthLeftMet @10 :Float32;
    fWidthRightMet @11 :Float32;
    fLengthFrontMet @12 :Float32;
    fLengthRearMet @13 :Float32;
    fAbsOrientationRad @14 :Float32;
    fRelHeadingRad @15 :Float32;
    fRelHeadingStdRad @16 :Float32;
    uiLifeCyclesNu @17 :UInt16;
    uiMaintenanceStateNu @18 :UInt8;
    uClassificationNu @19 :UInt32;
    fFirstDetectXMet @20 :Float32;
    fFirstDetectYMet @21 :Float32;
    fProbabilityOfExistencePer @22 :Float32;
    uiHighestAssocProbPer @23 :UInt8;
    uiMeasuredTargetFrequencyNu @24 :UInt8;
    bObjStable @25 :Bool;
    fRcs @26 :Float32;
    fMirrorProbPer @27 :Float32;
}

struct CtafctaAlgoParamT {
    ctaKfTtcThresholdS @0 :Float32;
    ctaKfTtcThresholdL2S @1 :Float32;
    ctaKfTtcThresholdL3S @2 :Float32;
    ctaKfXMinBreakthroughMet @3 :Float32;
    ctaKfXMinBreakthroughL2Met @4 :Float32;
    ctaKfXMinBreakthroughL3Met @5 :Float32;
    ctaKfXMaxBreakthroughMet @6 :Float32;
    ctaKfVEgoMaxMps @7 :Float32;
    ctaKfVEgoMinMps @8 :Float32;
    ctaKfVTargetMinMps @9 :Float32;
    ctaKfVTargetMaxMps @10 :Float32;
    ctaKfMaxHeadingAngleDeg @11 :Float32;
    ctaKfMinHeadingAngleDeg @12 :Float32;
    ctaKfTargetRangeMaxMet @13 :Float32;
    ctaKfTargetRangeMaxL2Met @14 :Float32;
    ctaKfTargetRangeMaxL3Met @15 :Float32;
    ctaKfBreakthroughMarginMet @16 :Float32;
    ctaKfTtcThresholdMarginS @17 :Float32;
    ctaKbActiveNu @18 :Bool;
}

struct CtafctaDebugInfoT {
    fTtcThresholdS @0 :Float32;
    fXMinBreakthroughMet @1 :Float32;
    fXMaxBreakthroughMet @2 :Float32;
    fMaxObjRangeMet @3 :Float32;
    fMaxHeadingAngleDeg @4 :Float32;
    fMinHeadingAngleDeg @5 :Float32;
    fVegoMinMps @6 :Float32;
    fVegoMaxMps @7 :Float32;
    fVTargetMin @8 :Float32;
    fVTargetMax @9 :Float32;
    fCriticalTtcS @10 :Float32;
    fCriticalObjDistYMet @11 :Float32;
    fCriticalObjDistYLastCycleMet @12 :Float32;
    uCriticalObjIdNu @13 :UInt8;
    uCriticalObjIdLastCycleNu @14 :UInt8;
    iCriticalObjIdNu @15 :Int32;
    iCriticalObjIdLastCycleNu @16 :Int32;
    uInterruptCycleCountNu @17 :UInt8;
    bWarningInterrupt @18 :Bool;
    bFctaWarnActive @19 :Bool;
    fctaVaObjectListGlobal @20 :List(CtafctaDebugObjGlobalT);
    fctaVaObjectListGlobalUpdate @21 :List(CtafctaDebugObjGlobalT);
    fctaStateMachine @22 :UInt8;
    ctaFTtcThresholdS @23 :List(Float32);
    ctaFXMinBreakthroughMet @24 :List(Float32);
    ctaFXMaxBreakthroughMet @25 :List(Float32);
    ctaFMaxObjRangeMet @26 :List(Float32);
    ctaBFctaWarnActive @27 :List(Bool);
}

struct CtafctaDebugObjGlobalT {
    fBtHitHystTimerS @0 :Float32;
    uBreakthroughHitConfi @1 :UInt8;
    bBreakthroughHit @2 :Bool;
    bWarning @3 :Bool;
    bWarningLastCycle @4 :Bool;
    bObjectInRange @5 :Bool;
    bTtcBelowThresh @6 :Bool;
    bBtHitHystActive @7 :Bool;
    fRearTrackProb @8 :Float32;
    uRearMirrorCnt @9 :UInt8;
    bRelevant @10 :Bool;
    bMirror @11 :Bool;
    bObjectFromSide @12 :Bool;
    bValidApproachAngle @13 :Bool;
    bObjectAtEdgeFoV @14 :Bool;
    bShortWarning @15 :Bool;
    bUpdatedRecently @16 :Bool;
    bQuality @17 :Bool;
    bRearMirrorObject @18 :Bool;
    bObjMovementValid @19 :Bool;
    ctaFBtHitHystTimerS @20 :List(Float32);
    ctaUBreakthroughHitConfi @21 :List(UInt8);
    ctaBBreakthroughHit @22 :List(Bool);
    ctaBWarning @23 :List(Bool);
    ctaBWarningLastCycle @24 :List(Bool);
    ctaBObjectInRange @25 :List(Bool);
    ctaBTtcBelowThresh @26 :List(Bool);
    ctaBBtHitHystActive @27 :List(Bool);
}

struct CtaFusionObjectInReqT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :CtaSignalHeaderT;
    uNumOfUsedObjects @2 :UInt8;
    aObjects @3 :List(CtaFusionObjectT);
}

struct CtaFusionObjectT {
    fDistXMet @0 :Float32;
    fDistXStdMet @1 :Float32;
    fDistYMet @2 :Float32;
    fDistYStdMet @3 :Float32;
    fVrelXMps @4 :Float32;
    fVrelYMps @5 :Float32;
    fArelXMpss @6 :Float32;
    fArelYMpss @7 :Float32;
    fVabsXMps @8 :Float32;
    fVabsYMps @9 :Float32;
    fWidthLeftMet @10 :Float32;
    fWidthRightMet @11 :Float32;
    fLengthFrontMet @12 :Float32;
    fLengthRearMet @13 :Float32;
    fAbsOrientationRad @14 :Float32;
    fRelHeadingRad @15 :Float32;
    fRelHeadingStdRad @16 :Float32;
    uiLifeCyclesNu @17 :UInt16;
    uiMaintenanceStateNu @18 :UInt8;
    uClassificationNu @19 :UInt32;
    fFirstDetectXMet @20 :Float32;
    fFirstDetectYMet @21 :Float32;
    fProbabilityOfExistencePer @22 :Float32;
    uiHighestAssocProbPer @23 :UInt8;
    uiMeasuredTargetFrequencyNu @24 :UInt8;
    bObjStable @25 :Bool;
    fRcs @26 :Float32;
    fMirrorProbPer @27 :Float32;
    uNumOfUsedObjects @28 :UInt8;
    uiIdNu @29 :UInt16;
    iRawFusionIdNu @30 :Int32;
    bRightSide @31 :Bool;
}

struct CtaInputT {
    ctaLeftFrontEmsrrObjList @0 :List(CtaemsrrObjectInReq);
    ctaRightFrontEmsrrObjList @1 :List(CtaemsrrObjectInReq);
    ctaLeftRearEmsrrObjList @2 :List(CtaemsrrObjectInReq);
    ctaRightRearEmsrrObjList @3 :List(CtaemsrrObjectInReq);
    fCycleTimeS @4 :Float32;
    bFctaFunctionActive @5 :Bool;
    bFctaFunctionOutputActive @6 :Bool;
    bRctaFunctionActive @7 :Bool;
    bRctaFunctionOutputActive @8 :Bool;
    bFctaFailure @9 :Bool;
    bRctaFailure @10 :Bool;
    ctaemFusionObjList @11 :CtaFusionObjectInReqT;
    egoVehicleInfo @12 :CtaEgoVehicleInReqT;
    ctaRoadInformation @13 :CtaRoadInReqT;
    ctaNsNvramNu @14 :CtanvramT;
}

struct CtanvramT {
    ctaNbFctaPowerOffSwitchStateNu @0 :Bool;
    ctaNbRctaPowerOffSwitchStateNu @1 :Bool;
}

struct CtaObjectBorderT {
    fXMinMet @0 :Float32;
    fXMaxMet @1 :Float32;
    fYMinMet @2 :Float32;
    fYMaxMet @3 :Float32;
}

struct CtaObjectInfoGlobalT {
    objectBorder @0 :CtaObjectBorderT;
    objectMovementBorder @1 :CtaObjectBorderT;
    objectRotated @2 :CtaObjectRotatedT;
    fTtcS @3 :Float32;
    fTtcAccelS @4 :Float32;
    fTtcFilteredS @5 :Float32;
    fTtcRadialS @6 :Float32;
    fFctaVabs @7 :Float32;
    fRctaVabs @8 :Float32;
    fFctaxLastCycleMet @9 :Float32;
    fFctayLastCycleMet @10 :Float32;
    fRctaxLastCycleMet @11 :Float32;
    fRctayLastCycleMet @12 :Float32;
    fFctaVxPosBasedMps @13 :Float32;
    fFctaVyPosBasedMps @14 :Float32;
    fRctaVxPosBasedMps @15 :Float32;
    fRctaVyPosBasedMps @16 :Float32;
    fCycleTimeSumS @17 :Float32;
    fUpdateRateNu @18 :Float32;
    fAssocProbFilteredNu @19 :Float32;
    fFctaxMovementMet @20 :Float32;
    fFctayMovementMet @21 :Float32;
    fRctaxMovementMet @22 :Float32;
    fRctayMovementMet @23 :Float32;
    fAngleDeg @24 :Float32;
    fObjWidthMaxMet @25 :Float32;
    fObjLengthMaxMet @26 :Float32;
}

struct CtaObjectRotatedT {
    fDistX @0 :Float32;
    fDistY @1 :Float32;
    fWidth @2 :Float32;
    fLength @3 :Float32;
}

struct CtaOutProT {
    uiVersionNumber @0 :UInt32;
    ctaNsNvramNu @1 :CtanvramT;
    fRctAfTtcS @2 :Float32;
    uRctaCriticalObjIdNu @3 :UInt8;
    iRctaCriticalObjIdNu @4 :Int32;
    bRctaWarning @5 :Bool;
    bRctaWarningL2 @6 :Bool;
    bRctaWarningL3 @7 :Bool;
    bFctaWarning @8 :Bool;
    fFctAfTtcS @9 :Float32;
    uFctaCriticalObjIdNu @10 :UInt8;
    iFctaCriticalObjIdNu @11 :Int32;
    uRctaCriticalObjTypeNu @12 :UInt8;
    uRctaSystemFaultStateNu @13 :UInt8;
    bRctaWarningLeftL1 @14 :Bool;
    bRctaWarningLeftL2 @15 :Bool;
    bRctaWarningRightL1 @16 :Bool;
    bRctaWarningRightL2 @17 :Bool;
    bFctaWarningLeftL1 @18 :Bool;
    bFctaWarningLeftL2 @19 :Bool;
    bFctaWarningRightL1 @20 :Bool;
    bFctaWarningRightL2 @21 :Bool;
    uFctaCriticalObjTypeNu @22 :UInt8;
    uFctaSystemFaultStateNu @23 :UInt8;
}

struct CtaParamT {
    ctaKsVehicleParameterNu @0 :CtaVehicleParamT;
    ctaKsRctaAlgoParameterNu @1 :CtarctaAlgoParamT;
    ctaKsFctaAlgoParameterNu @2 :CtafctaAlgoParamT;
}

struct CtarctaAlgoParamT {
    ctaKfTtcThresholdS @0 :Float32;
    ctaKfTtcThresholdL2S @1 :Float32;
    ctaKfTtcThresholdL3S @2 :Float32;
    ctaKfTtcThresholdMarginS @3 :Float32;
    ctaKfXMinBreakthroughMet @4 :Float32;
    ctaKfXMinBreakthroughL2Met @5 :Float32;
    ctaKfXMinBreakthroughL3Met @6 :Float32;
    ctaKfXMaxBreakthroughMet @7 :Float32;
    ctaKfVEgoMaxMps @8 :Float32;
    ctaKfVEgoMinMps @9 :Float32;
    ctaKfVTargetMinMps @10 :Float32;
    ctaKfMaxHeadingAngleDeg @11 :Float32;
    ctaKfMinHeadingAngleDeg @12 :Float32;
    ctaKfTargetRangeMaxMet @13 :Float32;
    ctaKfTargetRangeMaxL2Met @14 :Float32;
    ctaKfTargetRangeMaxL3Met @15 :Float32;
    ctaKbActiveNu @16 :Bool;
    ctaKsSteeringAngleCutOffNu @17 :CtarctaSteeringAngleCutOffParamT;
}

struct CtarctaDebugInfoT {
    fTtcThresholdS @0 :List(Float32);
    fXMinBreakthroughMet @1 :List(Float32);
    fXMaxBreakthroughMet @2 :List(Float32);
    fMaxObjRangeMet @3 :List(Float32);
    fMaxHeadingAngleDeg @4 :Float32;
    fMinHeadingAngleDeg @5 :Float32;
    fCriticalTtcS @6 :Float32;
    fCriticalObjDistYMet @7 :Float32;
    fCriticalObjDistYLastCycleMet @8 :Float32;
    uCriticalObjIdNu @9 :UInt8;
    uCriticalObjIdLastCycleNu @10 :UInt8;
    iCriticalObjIdNu @11 :Int32;
    iCriticalObjIdLastCycleNu @12 :Int32;
    uInterruptCycleCountNu @13 :UInt8;
    bWarningInterrupt @14 :Bool;
    bRctaWarnActive @15 :List(Bool);
    rctaVaObjectListGlobal @16 :List(CtarctaDebugObjGlobalT);
    rctaVaObjectListGlobalUpdate @17 :List(CtarctaDebugObjGlobalT);
    rctaStateMachine @18 :UInt8;
}

struct CtarctaDebugObjGlobalT {
    fBtHitHystTimerS @0 :List(Float32);
    uBreakthroughHitConfi @1 :List(UInt8);
    bBreakthroughHit @2 :List(Bool);
    bWarning @3 :List(Bool);
    bWarningLastCycle @4 :List(Bool);
    bObjectInRange @5 :List(Bool);
    bTtcBelowThresh @6 :List(Bool);
    bBtHitHystActive @7 :List(Bool);
    bRelevant @8 :Bool;
    bMirror @9 :Bool;
    bObjectFromSide @10 :Bool;
    bValidApproachAngle @11 :Bool;
    bShortTtc @12 :Bool;
    bUpdatedRecently @13 :Bool;
    bQuality @14 :Bool;
}

struct CtarctaSteeringAngleCutOffParamT {
    fSteerAngleCutOffMinDeg @0 :Float32;
    fSteerAngleCutOffMidDeg @1 :Float32;
    fSteerAngleCutOffMaxDeg @2 :Float32;
    fXMinBreakthroughSwaMidMet @3 :Float32;
    fXMinBreakthroughSwaMaxMet @4 :Float32;
    bEnableSteeringAngleCutOffNu @5 :Bool;
}

struct CtaRoadInReqT {
    fCurveRadiusMet @0 :Float32;
}

struct CtaSensorMountingT {
    ctaKfLatPosMet @0 :Float32;
    ctaKfLongPosMet @1 :Float32;
}

struct CtaSignalHeaderT {
    uiTimeStamp @0 :UInt32;
    uiMeasurementCounter @1 :UInt16;
    uiCycleCounter @2 :UInt16;
    eSigStatus @3 :UInt8;
}

struct CtaVehicleParamT {
    ctaKfWheelBaseMet @0 :Float32;
    ctaKfVehicleWidthMet @1 :Float32;
    ctaKfVehicleLengthMet @2 :Float32;
    ctaKfOverhangFrontMet @3 :Float32;
    ctaKsLeftFrontSensorMounting @4 :CtaSensorMountingT;
    ctaKsRightFrontSensorMounting @5 :CtaSensorMountingT;
    ctaKsLeftRearSensorMountingNu @6 :CtaSensorMountingT;
    ctaKsRightRearSensorMountingNu @7 :CtaSensorMountingT;
}

struct CtObjectInfoGlobalT {
    fFctaxBreakthroughMet @0 :Float32;
    fRctaxBreakthroughMet @1 :Float32;
    fFctaxBreakthroughFilteredMet @2 :Float32;
    fRctaxBreakthroughFilteredMet @3 :Float32;
    fFctaxBreakthroughStdMet @4 :Float32;
    fRctaxBreakthroughStdMet @5 :Float32;
    fFctattcS @6 :Float32;
    fRctattcS @7 :Float32;
    fFctattcFilteredS @8 :Float32;
    fRctattcFilteredS @9 :Float32;
    fFctaDistToCrossingLineMet @10 :Float32;
    fRctaDistToCrossingLineMet @11 :Float32;
    fDistToCrossingLineFilteredMet @12 :Float32;
    fFctaRearTrackProbPer @13 :Float32;
    fRctaRearTrackProbPer @14 :Float32;
    bFctaRearTrackNu @15 :Bool;
    bRctaRearTrackNu @16 :Bool;
}

struct CurveVehDynT {
    curve @0 :Float32;
    c1 @1 :Float32;
    gradient @2 :Float32;
    varC0 @3 :Float32;
    varC1 @4 :Float32;
    quality @5 :Float32;
    crvError @6 :Float32;
    crvConf @7 :UInt8;
}

struct CustEmInDataT {
    customData @0 :UInt8;
}

struct CustomOutputT {
    avlcObjectEffective @0 :Bool;
    extCcCancelRq @1 :Bool;
    extCcCancelWarnRq @2 :Bool;
    extAccInhibitionRqOut @3 :UInt8;
    requestedLongAccelRaw @4 :Int16;
    requestedLongPosAccelGrad @5 :Int16;
    requestedLongNegAccelGrad @6 :Int16;
    requestedLongAccelHyst @7 :Int16;
    softStopRequest @8 :Bool;
    maxOperatingSpeed @9 :Int16;
    minOperatingSpeed @10 :Int16;
    accHeadwayLeverSetting @11 :UInt8;
    accAccelEnable @12 :Bool;
    accDisplayOut @13 :UInt16;
    cruiseCtrlMode @14 :UInt8;
    accInternalState @15 :UInt8;
}

struct DrvIntCurveVehDynT {
    curve @0 :Float32;
    variance @1 :Float32;
    gradient @2 :Float32;
}

struct EbaPreBrkAccelParNvT {
    valid @0 :UInt8;
    accelL1 @1 :List(EbaPreBrkAccelTabNvT);
    accelL2 @2 :List(EbaPreBrkAccelTabNvT);
}

struct EbaPreBrkAccelTabNvT {
    velo @0 :Float32;
    accel @1 :Float32;
}

struct EbaPreselT {
    eEbaInhibitionMask @0 :UInt16;
    ucEbaMovingObjQuality @1 :UInt8;
    eEbaHypCat @2 :UInt8;
    bCrossingPedEbaPresel @3 :Bool;
}

struct EcaMtCyclEnvmodeT {
    uiVersionNumber @0 :UInt32;
    eCycleMode @1 :UInt8;
    fEcaMtCycleTime @2 :Float32;
}

struct EgoVehicleInfoT {
    sSigHeader @0 :LbsSignalHeaderT;
    fegoVelocityMps @1 :Float32;
    fVaregoVelocityMps @2 :Float32;
    fegoAccelerationMps2 @3 :Float32;
    fVaregoAccelerationMps2 @4 :Float32;
    fYawRateRadps @5 :Float32;
    fLatAccelMps2 @6 :Float32;
    fSelfSteeringRad @7 :Float32;
}

struct EmCemAlgoParametersT {
    eCemParCodState @0 :UInt8;
    eCemParCldState @1 :UInt8;
    eCemParModeSelection @2 :UInt8;
}

struct EmFusionGenAttributesT {
    eDynamicPropertyNu @0 :UInt8;
    uiDynConfidencePer @1 :UInt8;
    eClassificationNu @2 :UInt32;
    uiClassConfidencePer @3 :UInt8;
}

struct EmFusionGenGeneralsT {
    fLifeTimeS @0 :Float32;
    uiLifeCyclesNu @1 :UInt16;
    uiMaintenanceStateNu @2 :UInt8;
    uiIdNu @3 :UInt16;
    iRawFusionIdNu @4 :Int32;
}

struct EmFusionGenGeometryT {
    fWidthMet @0 :Float32;
    fWidthStdMet @1 :Float32;
    fWidthLeftMet @2 :Float32;
    fWidthRightMet @3 :Float32;
    fLengthMet @4 :Float32;
    fLengthStdMet @5 :Float32;
    fLengthFrontMet @6 :Float32;
    fLengthRearMet @7 :Float32;
    fOrientationRad @8 :Float32;
    fOrientationStdRad @9 :Float32;
}

struct EmFusionGenKinematicsT {
    fDistXMet @0 :Float32;
    fDistXStdMet @1 :Float32;
    fDistYMet @2 :Float32;
    fDistYStdMet @3 :Float32;
    fVrelXMps @4 :Float32;
    fVrelXStdMps @5 :Float32;
    fVrelYMps @6 :Float32;
    fVrelYStdMps @7 :Float32;
    fArelXMpss @8 :Float32;
    fArelXStdMpss @9 :Float32;
    fArelYMpss @10 :Float32;
    fArelYStdMpss @11 :Float32;
    fVabsXMps @12 :Float32;
    fVabsXStdMps @13 :Float32;
    fVabsYMps @14 :Float32;
    fVabsYStdMps @15 :Float32;
    fAabsXMpss @16 :Float32;
    fAabsXStdMpss @17 :Float32;
    fAabsYMpss @18 :Float32;
    fAabsYStdMpss @19 :Float32;
}

struct EmFusionGenObjectT {
    sKinematic @0 :EmFusionGenKinematicsT;
    sGeometry @1 :EmFusionGenGeometryT;
    sGeneral @2 :EmFusionGenGeneralsT;
    sAttribute @3 :EmFusionGenAttributesT;
    sQualifier @4 :EmFusionGenQualifiersT;
}

struct EmFusionGenQualifiersT {
    fProbabilityOfExistencePer @0 :Float32;
    uiMeasuredTargetFrequencyNu @1 :UInt8;
    bObjStable @2 :Bool;
}

struct EmGenObjListSt {
    uiVersionNumber @0 :UInt32;
    uNumOfUsedObjects @1 :UInt8;
    sSigHeader @2 :LbsSignalHeaderT;
    aObject @3 :List(LbsGenObjectSt);
}

struct EmPedAlgoParametersT {
    ePedArtDummy @0 :UInt8;
}

struct EmRoadT {
    fCurveRadiusMet @0 :Float32;
    fDrivenCurveRadiusMet @1 :Float32;
    fC0FusedEgo1pm @2 :Float32;
    fC1FusedEgo1pm2 @3 :Float32;
    fYawAngleFusedEgoRad @4 :Float32;
    fConfEgoPer @5 :Float32;
    fC0Fused1pm @6 :Float32;
    fC1Fused1pm2 @7 :Float32;
    fYOffsetFusedMet @8 :Float32;
    fYawAngleFusedRad @9 :Float32;
    fConfCurvaturePer @10 :Float32;
    fConfYOffsetPer @11 :Float32;
    fYOffsetFusedOppBorderMet @12 :Float32;
    fConfYOppOffsetPer @13 :Float32;
    fConfAdjacentLanesPer @14 :Float32;
    fConfOppositeLanesPer @15 :Float32;
    fLaneWidthMet @16 :Float32;
    iNumOfAdjacentLanesNu @17 :Int8;
    iNumOfOppositeLanesNu @18 :Int8;
    borderEstmGridDataFC01pm @19 :Float32;
    borderEstmGridDataFC11pm2 @20 :Float32;
    borderEstmGridDataFYawAngleRad @21 :Float32;
    borderEstmGridDataFYDistMet @22 :Float32;
    borderEstmGridDataFMaxXMet @23 :Float32;
    borderEstmGridDataFConfPer @24 :Float32;
    roadType @25 :LbsRoadTypeT;
}

struct EmSrrGenAttributesT {
    eDynamicPropertyNu @0 :UInt8;
    uiDynConfidencePer @1 :UInt8;
    eClassificationNu @2 :UInt32;
    uiClassConfidencePer @3 :UInt8;
}

struct EmSrrGenGeneralsT {
    fLifeTimeS @0 :Float32;
    uiLifeCyclesNu @1 :UInt16;
    uiMaintenanceStateNu @2 :UInt8;
    uiIdNu @3 :UInt16;
}

struct EmSrrGenGeometryT {
    fWidthMet @0 :Float32;
    fWidthStdMet @1 :Float32;
    fWidthLeftMet @2 :Float32;
    fWidthRightMet @3 :Float32;
    fLengthMet @4 :Float32;
    fLengthStdMet @5 :Float32;
    fLengthFrontMet @6 :Float32;
    fLengthRearMet @7 :Float32;
    fOrientationRad @8 :Float32;
    fOrientationStdRad @9 :Float32;
}

struct EmSrrGenKinematicsT {
    fDistXMet @0 :Float32;
    fDistXStdMet @1 :Float32;
    fDistYMet @2 :Float32;
    fDistYStdMet @3 :Float32;
    fVrelXMps @4 :Float32;
    fVrelXStdMps @5 :Float32;
    fVrelYMps @6 :Float32;
    fVrelYStdMps @7 :Float32;
    fArelXMpss @8 :Float32;
    fArelXStdMpss @9 :Float32;
    fArelYMpss @10 :Float32;
    fArelYStdMpss @11 :Float32;
    fVabsXMps @12 :Float32;
    fVabsXStdMps @13 :Float32;
    fVabsYMps @14 :Float32;
    fVabsYStdMps @15 :Float32;
    fAabsXMpss @16 :Float32;
    fAabsXStdMpss @17 :Float32;
    fAabsYMpss @18 :Float32;
    fAabsYStdMpss @19 :Float32;
}

struct EmsrrGenObjectArrayT {
    sSigHeader @0 :EnvmSignalHeaderT;
    uiNumOfObjects @1 :UInt8;
    aObjects @2 :List(EmSrrGenObjectT);
    sSensorPosition @3 :SrrPosSt;
}

struct EmSrrGenObjectT {
    objId @0 :UInt8;
    sKinematic @1 :EmSrrGenKinematicsT;
    sGeometry @2 :EmSrrGenGeometryT;
    sGeneral @3 :EmSrrGenGeneralsT;
    sAttribute @4 :EmSrrGenAttributesT;
    sQualifier @5 :EmSrrGenQualifiersT;
}

struct EmsrrGenObjListT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    aFrontLeftObjects @2 :EmsrrGenObjectArrayT;
    aFrontRightObjects @3 :EmsrrGenObjectArrayT;
    aRearLeftObjects @4 :EmsrrGenObjectArrayT;
    aRearRightObjects @5 :EmsrrGenObjectArrayT;
}

struct EmSrrGenQualifiersT {
    fProbabilityOfExistencePer @0 :Float32;
    uiMeasuredTargetFrequencyNu @1 :UInt8;
    bObjStable @2 :Bool;
}

struct EmsrrObjListSt {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :LbsSignalHeaderT;
    aObject @2 :List(LbsSrrObjectSt);
}

struct EmTArsObject {
    kinematic @0 :EnvmTCrKinEnvmatic;
    geometry @1 :EnvmTCrGeometry;
    motionAttributes @2 :EnvmTCrMotionAttributes;
    attributes @3 :EnvmTCrAttributes;
    sensorSpecific @4 :EnvmTCrSensorSpecific;
    legacy @5 :EnvmTCrLegacy;
}

struct EmTGenObjGenerals {
    fLifeTime @0 :Float32;
    uiLifeCycles @1 :UInt16;
    eMaintenanceState @2 :UInt8;
    uiId @3 :UInt8;
    eSplitMergeState @4 :UInt8;
    uiMergeId @5 :UInt8;
    uiSplitId @6 :UInt8;
}

struct EmTGenObjGeometry {
    aShapePointCoordinates @0 :List(EnvmTGenObjShapePointCoord);
}

struct EmTGenObjListHeader {
    iNumOfUsedObjects @0 :Int8;
    iSortedObjectList @1 :List(Int8);
    eObjListSource @2 :UInt16;
}

struct EnvmAdasHighlightIdT {
    uiFcwHighlightIdNu @0 :Int32;
    uiFcwWarningLevelEnum @1 :UInt8;
    uiAebHighlightIdNu @2 :Int32;
    uiAebWarningLevelEnum @3 :UInt8;
    uiAccHighlightIdNu @4 :Int32;
    uiAccWarningLevelEnum @5 :UInt8;
    uiFctaLeftHighlightIdNu @6 :Int32;
    uiFctaLeftWarningLevelEnum @7 :UInt8;
    uiFctaRightHighlightIdNu @8 :Int32;
    uiFctaRightWarningLevelEnum @9 :UInt8;
    uiRctaLeftHighlightIdNu @10 :Int32;
    uiRctaLeftWarningLevelEnum @11 :UInt8;
    uiRctaRightHighlightIdNu @12 :Int32;
    uiRctaRightWarningLevelEnum @13 :UInt8;
    uiLcaLeftHighlightIdNu @14 :Int32;
    uiLcaLeftWarningLevelEnum @15 :UInt8;
    uiLcaRightHighlightIdNu @16 :Int32;
    uiLcaRightWarningLevelEnum @17 :UInt8;
    uiDowLeftHighlightIdNu @18 :Int32;
    uiDowLeftWarningLevelEnum @19 :UInt8;
    uiDowRightHighlightIdNu @20 :Int32;
    uiDowRightWarningLevelEnum @21 :UInt8;
    uiRcwHighlightIdNu @22 :Int32;
    uiRcwWarningLevelEnum @23 :UInt8;
    uiAlcHighlightIdNu @24 :Int32;
    uiAlcWarningLevelEnum @25 :UInt8;
}

struct EnvmAlgoParametersT {
    cem @0 :EmCemAlgoParametersT;
    lat @1 :EnvmLatAlgoParametersT;
    ped @2 :EmPedAlgoParametersT;
}

struct EnvmDebug {
    header @0 :StdHeader.Header;
    # ENVM input
    # BSW_s_EnvmCtrlData_t Ctrl
    # AssessedObjList_t FctObjectList
    vehicleDynData @1 :VedVehDynT;
    # VED_VehPar_t VehicleStatData
    # VED_VehSig_t VehSig
    # EnvmNvmIn_t NvmIn
    # CustEMInData_t CustIn
    # Envm_t_CameraObjInputData CamObjInput
    # BSW_s_AlgoParameters_t AlgoParameters
    # EMCallBackHdlr_t EnvmCallBackHdlr
    # ExtSRRObjList_array_t ExtSRRObjList
    camObject @2 :CamObjectList;
    extObjList @3 :ExtObjectListT;
    # ENVM output & VLCSEN input
    envmGenObjectList @4 :EnvmTGenObjectList;
    envmTechObjectList @5 :EnvmTCrObjectList;
    objSyncEgoDynamic @6 :VedVehDynT;
    tgtSyncEgoDynamic @7 :VedVehDynT;
    pFusionObjectList @8 :EnvmFusionObjectListT;
    adasHighlightId @9 :EnvmAdasHighlightIdT;
    # VLCSEN input
    # VLCCtrlData_t SenCtrlData
    egoDynRaw @10 :VedVehDynT;
    dfvLongOut @11 :VlcDfv2SenInfoT;
    camLaneData @12 :TCamLaneInputData;
    # VLCSEN output
    vlcAccOoiData @13 :VlcSenAccOoiT;
    pubFctObj @14 :AssessedObjListT;
    accDisplayObj @15 :VlcAccObjectT;
    accOutput @16 :VlcAccOutputDataT;
    overtakeAssistInfo @17 :OvertakeAssistInfo;
    pSrrGenObjList @18 :EmsrrGenObjListT;
    predictedTrajectory @19 :CpCustOutPredTrajT;
}

struct EnvmFusionObjectListT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    uNumOfUsedObjects @2 :UInt8;
    aObjects @3 :List(EmFusionGenObjectT);
}

struct EnvmLatAlgoParametersT {
    bUseExternalYawRateLat @0 :Bool;
    bUseExternalAxLat @1 :Bool;
    uYawRateObjLat @2 :UInt8;
    uAccelXObjLat @3 :UInt8;
}

struct EnvmNvmInT {
    trafficOrientation @0 :UInt8;
    nvmInState @1 :EnvmNvmStateT;
}

struct EnvmNvmStateT {
    stateTrafficOrientation @0 :UInt32;
}

struct EnvmSignalHeaderT {
    uiTimeStamp @0 :UInt32;
    uiMeasurementCounter @1 :UInt16;
    uiCycleCounter @2 :UInt16;
    eSigStatus @3 :UInt8;
}

struct EnvmTCameraObjInputData {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
}

struct EnvmTCrAttributes {
    eClassification @0 :UInt8;
    uiClassConfidence @1 :UInt8;
    ui8Dummy @2 :UInt8;
    uiReferenceToTrace @3 :UInt8;
}

struct EnvmTCrGeometry {
    fWidth @0 :Float32;
    fLength @1 :Float32;
    fOrientation @2 :Float32;
    fOrientationStd @3 :Float32;
}

struct EnvmTCrKinEnvmatic {
    fMaxAccelY @0 :Float32;
}

struct EnvmTCrLegacy {
    fAngle @0 :Float32;
    fLastTargetDistY @1 :Float32;
    fLastTargetDistX @2 :Float32;
}

struct EnvmTCrMotionAttributes {
    eDynamicProperty @0 :UInt8;
    uiStoppedConfidence @1 :UInt8;
    eAbsMovingState @2 :UInt8;
    eDynamicSubProperty @3 :UInt8;
}

struct EnvmTCrObjectList {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    aObject @2 :List(EmTArsObject);
}

struct EnvmTCrSensorSpecific {
    fRcs @0 :Float32;
    ucMeasuredSources @1 :UInt8;
    eObjRelationsClass @2 :UInt8;
    eFovOverlapFar @3 :UInt8;
    bNearObjInBeam @4 :Bool;
}

struct EnvmTGenObjAttributes {
    eDynamicProperty @0 :UInt8;
    uiDynConfidence @1 :UInt8;
    eClassification @2 :UInt8;
    uiClassConfidence @3 :UInt8;
    eObjectOcclusion @4 :UInt8;
}

struct EnvmTGenObjectList {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    headerObjList @2 :EmTGenObjListHeader;
    aObject @3 :List(EnvmTGenObject);
}

struct EnvmTGenObject {
    kinematic @0 :EnvmTGenObjKinEnvmatics;
    geometry @1 :EmTGenObjGeometry;
    attributes @2 :EnvmTGenObjAttributes;
    general @3 :EmTGenObjGenerals;
    qualifiers @4 :EnvmTGenObjQualifiers;
}

struct EnvmTGenObjKinEnvmatics {
    fDistX @0 :Float32;
    fDistXStd @1 :Float32;
    fDistY @2 :Float32;
    fDistYStd @3 :Float32;
    fVrelX @4 :Float32;
    fVrelXStd @5 :Float32;
    fVrelY @6 :Float32;
    fVrelYStd @7 :Float32;
    fArelX @8 :Float32;
    fArelXStd @9 :Float32;
    fArelY @10 :Float32;
    fArelYStd @11 :Float32;
    fVabsX @12 :Float32;
    fVabsXStd @13 :Float32;
    fVabsY @14 :Float32;
    fVabsYStd @15 :Float32;
    fAabsX @16 :Float32;
    fAabsXStd @17 :Float32;
    fAabsY @18 :Float32;
    fAabsYStd @19 :Float32;
}

struct EnvmTGenObjQualifiers {
    uiProbabilityOfExistence @0 :UInt8;
    uiAccObjQuality @1 :UInt8;
    uiEbaObjQuality @2 :UInt8;
    eEbaHypCat @3 :UInt8;
    eEbaInhibitionMask @4 :UInt16;
}

struct EnvmTGenObjShapePointCoord {
    fPosX @0 :Float32;
    fPosY @1 :Float32;
    uiPosXStd @2 :UInt16;
    uiPosYStd @3 :UInt16;
}

struct ExtObjectListT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    uiTimeStamp @2 :UInt32;
    uiCycleCounter @3 :UInt16;
    eSigStatus @4 :UInt8;
    headerObjList @5 :HeaderObjListT;
    objects @6 :List(ExtObjectsT);
}

struct ExtObjectsT {
    objectId @0 :Int32;
    kinematic @1 :KinematicT;
    geometry @2 :GeometryT;
    attributes @3 :AttributesT;
    general @4 :GeneralT;
    qualifiers @5 :QualifiersT;
    sensorSpecific @6 :SensorSpecificT;
    ebaPresel @7 :EbaPreselT;
    legacy @8 :LegacyObjT;
}

struct ExtSrrObjListArrayT {
    uiVersionNumber @0 :UInt32;
    iNumofSrRs @1 :UInt32;
    sRrObjects @2 :List(ExtSrrObjListT);
}

struct ExtSrrObjListT {
    sSigHeader @0 :EnvmSignalHeaderT;
    sSrrPos @1 :SrrPosSt;
    uiNumOfObjects @2 :UInt8;
    sRrObject @3 :List(SrrObjSt);
    sWarning @4 :SrrWarningSt;
}

struct FctAccAlgoParametersT {
    cameraFusionPreselBits @0 :UInt8;
    naviFusionPreselBits @1 :UInt8;
}

struct FctAlgoParametersT {
    eba @0 :FctEbaAlgoParametersT;
    acc @1 :FctAccAlgoParametersT;
    general @2 :FctGeneralAlgoParT;
    blockage @3 :FctBlockageAlgoParametersT;
}

struct FctBlockageAlgoParametersT {
    f32ShutoffRange @0 :List(Float32);
    f32ShutonRange @1 :List(Float32);
    f32ShutoffTimeTimeout @2 :Float32;
    f32ShutoffWayTimeout @3 :Float32;
    uiShutoffNoOfObjLosses @4 :UInt8;
    bBlockageActive @5 :Bool;
    bUseRoadbeam @6 :List(Bool);
    ui8RoadbeamSpeedMin @7 :List(UInt8);
    ui8RoadVisibilityMin @8 :List(UInt8);
    ui8RoadQualityMin @9 :List(UInt8);
    ui8UseFarNearScanForBlck @10 :UInt8;
    f32BlockageHiTempThreshFactor @11 :Float32;
    i8BlockageHiTempThresh @12 :Int8;
    i8BlockageLoTempThresh @13 :Int8;
    ui8BlockageSpeedThreshold @14 :UInt8;
    ui8BlockageWiperThresh @15 :UInt8;
    bBlockageUseTemperature @16 :Bool;
    bBlockageUseWiper @17 :Bool;
}

struct FctEbaAlgoParametersT {
    codingValid @0 :Bool;
    codingBits @1 :UInt32;
    preBrkParAccelTab @2 :EbaPreBrkAccelParNvT;
}

struct FctGeneralAlgoParT {
    fnSwitchBits @0 :UInt8;
}

struct FctPubObjectT {
    laneInformation @0 :LaneInformationT;
    objOfInterest @1 :ObjOfInterestT;
    legacy @2 :LegacyAolT;
}

struct FuzzyRuleInputT {
    fvRelDistanceClose @0 :Int32;
    fvSpeedRelSlower @1 :Int32;
    fvSpeedRelFaster @2 :Int32;
    fvVOwnStillstand @3 :Int32;
    fvAObjBraking @4 :Int32;
    fvSpeedRelSameSpeed @5 :Int32;
    fvAObjLowaccel @6 :Int32;
    fvDistanceMinErrorToFar @7 :Int32;
    fvVOwnUrban @8 :Int32;
    fvSpeedRelMuchSlower @9 :Int32;
    fvDistanceSetErrorNear @10 :Int32;
    fvSoftnessSoft @11 :Int32;
    fvSpeedRelLittleslower @12 :Int32;
    fvAObjAccelerating @13 :Int32;
    fvAObjRolling @14 :Int32;
    fvVObjStandstill @15 :Int32;
    fvDistanceMinErrorVeryshort @16 :Int32;
    fvRelDistanceStillOk @17 :Int32;
    fvDistanceMinErrorShort @18 :Int32;
    fvVOwnFast @19 :Int32;
    fvSpeedRelLittlefaster @20 :Int32;
    fvDistanceMinErrorBelow @21 :Int32;
    fvVOwnSlow @22 :Int32;
    fvRelDistanceFar @23 :Int32;
    fvRelDistanceOk @24 :Int32;
    fvAObjHardBraking @25 :Int32;
    fvDistanceMinErrorNear @26 :Int32;
    fvDistanceSetErrorClose @27 :Int32;
    fvDistanceSetErrorNotCloseEnough @28 :Int32;
    fvSoftnessVeryDynamic @29 :Int32;
    fvDistanceToClose @30 :Int32;
    fvVObjFast @31 :Int32;
    fvDistanceSetErrorToClose @32 :Int32;
    fvAObjSoftBraking @33 :Int32;
    fvDistanceSetErrorToFar @34 :Int32;
    fvVObjUrban @35 :Int32;
    fvSpeedRelReallySlower @36 :Int32;
    fvDistanceSetErrorMoreThanRequested @37 :Int32;
}

struct FuzzySignalInputT {
    fuzzyRelDistance @0 :Int32;
    fuzzyAObj @1 :Int32;
    fuzzyDistanceSetError @2 :Int32;
    fuzzyDistanceMinError @3 :Int32;
    fuzzySoftness @4 :Int32;
    fuzzyVOwn @5 :Int32;
    fuzzyVObj @6 :Int32;
    fuzzyDistance @7 :Int32;
    fuzzySpeedRel @8 :Int32;
}

struct GeneralT {
    fLifeTime @0 :Float32;
    fTimeStamp @1 :UInt32;
    eObjMaintenanceState @2 :UInt8;
    cObjMergingId @3 :UInt8;
}

struct GeometryT {
    fWidth @0 :Float32;
    fLength @1 :Float32;
    fOrientation @2 :Float32;
    fOrientationStd @3 :Float32;
    fOrientationValid @4 :Float32;
}

struct HbaAccelCorrVehDynT {
    corrAccel @0 :Float32;
}

struct HbaBlockageDataT {
    eBlockageState @0 :UInt32;
    fTimeElapsedS @1 :Float32;
    fDistTraveledM @2 :Float32;
    fBlockageTimeS @3 :Float32;
}

struct HbaBusInputSignalsGlareFreeAreaT {
    fCutOffAngleLeftMet @0 :Float32;
    fCutOffAngleRightMet @1 :Float32;
    fCutOffAngleUpperMet @2 :Float32;
    fCutOffAngleLowerMet @3 :Float32;
}

struct HbaBusInputSignalsT {
    sSigHeader @0 :SignalHeaderT;
    uiHbaRequestNu @1 :UInt8;
    uiHighBeamIndicatorLampNu @2 :UInt8;
    uiSensitiveModeNu @3 :UInt8;
    uiheadLightStateNu @4 :UInt8;
    flightDistanceMet @5 :Float32;
    glareFreeAreas @6 :List(HbaBusInputSignalsGlareFreeAreaT);
    uiNumGlareFreeAreasNu @7 :UInt32;
}

struct HbaCameraDataT {
    fTimeS @0 :Float32;
    fTimeFwdS @1 :Float32;
    fDistM @2 :Float32;
    fDistFwdM @3 :Float32;
    fRadiusM @4 :Float32;
    fRadiusAbsM @5 :Float32;
}

struct HbaCityDataT {
    eCityState @0 :UInt32;
    fDistInCityM @1 :Float32;
    fTimeInCityS @2 :Float32;
    fDistOutCityM @3 :Float32;
    fTimeOutCityS @4 :Float32;
}

struct HbaCurveVehDynT {
    curve @0 :Float32;
}

struct HbaDebug {
    gSHbaFuncData @0 :HbaFuncDataT;
    hbaInput @1 :HbaInReqT;
}

struct HbaDecisionDataT {
    uiHbaTurnoffReason @0 :UInt32;
    sKeepOffData @1 :HbaKeepOffDataT;
    sSpeedData @2 :HbaSpeedDataT;
    sCityData @3 :HbaCityDataT;
    sBlockageData @4 :HbaBlockageDataT;
    sWeather @5 :HbaWeatherDataT;
}

struct HbaDecisionOutT {
    eCityState @0 :UInt32;
    eBlockageState @1 :UInt32;
    eWeatherState @2 :UInt32;
    eBrightnessState @3 :UInt32;
    eTrafficStyle @4 :UInt32;
    eJunctionState @5 :UInt32;
    eHighWayState @6 :UInt32;
    eTunnelState @7 :UInt32;
    eHighReflectorDensityState @8 :UInt32;
}

struct HbaFuncDataT {
    uiTimeStamp @0 :UInt32;
    sCamData @1 :HbaCameraDataT;
    sDecisionData @2 :HbaDecisionDataT;
    sDecisionOut @3 :HbaDecisionOutT;
    sMatrixData @4 :HbaMatrixDataT;
}

struct HbaInReqT {
    sSigHeader @0 :EnvmSignalHeaderT;
    sLightObjList @1 :LsdLightObjectListT;
    sEnvironment @2 :LsdEnvironmentDataT;
    sEgoVehDyn @3 :HbaVedVehDynT;
    sEgoVehSignal @4 :HbaVedVehSigT;
    sBusInputs @5 :HbaBusInputSignalsT;
    sGenObjList @6 :EnvmTGenObjectList;
}

struct HbaKeepOffDataT {
    uiReason @0 :UInt32;
    sSmallRadius @1 :HbaKeepOffSmallRadiusT;
    sMaybeCity @2 :HbaKeepOffMaybeCityT;
    sMinTimeHbOff @3 :HbaKeepOffMinTimeHbOffT;
    sTurnOnDelay @4 :HbaKeepOffTurnOnDelayT;
    sPrecVehLost @5 :HbaKeepOffPrecedingVehLostT;
    sPotVeh @6 :HbaKeepOffPotVehT;
}

struct HbaKeepOffMaybeCityT {
    fDelayTimerOncoming @0 :Float32;
    fDelayTimerPreceding @1 :Float32;
    fDelayTimerGlob @2 :Float32;
    uiCount @3 :UInt32;
}

struct HbaKeepOffMinTimeHbOffT {
    fHbOffTimerS @0 :Float32;
}

struct HbaKeepOffPotVehT {
    fDelayTimerOnc @0 :Float32;
    fDelayTimerPrec @1 :Float32;
    fDelayTimerGlob @2 :Float32;
    cnt @3 :Float32;
}

struct HbaKeepOffPrecedingVehLostT {
    fDistance2coverM @0 :Float32;
    fCurveRadiusMarkRad @1 :Float32;
}

struct HbaKeepOffSmallRadiusT {
    fDelayTimerS @0 :Float32;
}

struct HbaKeepOffTurnOnDelayT {
    fTurnOnTimerOncS @0 :Float32;
    fTurnOnTimerPrecS @1 :Float32;
    fTurnOnTimerTruckLightS @2 :Float32;
    fTurnOnTimerPrecSlowOvertakerS @3 :Float32;
}

struct HbaLateralT {
    yawRate @0 :HbaYawRateVehDynT;
    curve @1 :HbaCurveVehDynT;
}

struct HbaLongitudinalT {
    veloCorr @0 :HbaVeloCorrVehDynT;
    accelCorr @1 :HbaAccelCorrVehDynT;
}

struct HbaMatrixDataT {
    sObjData @0 :List(HbaObjectDataInternalT);
    uiNumRelObj @1 :UInt8;
    uiNumEmObj @2 :UInt8;
    uiNumLightObj @3 :UInt8;
    sRelVeh @4 :HbaRelevantVehicleT;
    sMatrixOut @5 :HbaMatrixT;
}

struct HbaMatrixT {
    eHighBeamState @0 :UInt32;
    uiTurnOffReason @1 :UInt16;
}

struct HbaMotionStateVehDynT {
    motState @0 :UInt8;
}

struct HbaObjectDataInternalT {
    uiLightObjId @0 :Int8;
    uiEmObjId @1 :Int8;
    bIsRelOncoming @2 :Bool;
    bIsRelPreceding @3 :Bool;
    lightObj @4 :LsdLightObjectT;
    eSource @5 :UInt32;
}

struct HbaOutputT {
    eHighBeamState @0 :UInt32;
    uiTurnOffReason @1 :UInt16;
}

struct HbaRelevantVehicleT {
    bPrecDetected @0 :Bool;
    bOnCDetected @1 :Bool;
    bEmObjDetected @2 :Bool;
    fTurnOnDelayAllS @3 :Float32;
    fTurnOnDelayOncS @4 :Float32;
    fTurnOnDelayPreS @5 :Float32;
    fTurnOffDelayOncS @6 :Float32;
    fTurnOffDelayPreS @7 :Float32;
}

struct HbaSpeedDataT {
    uiDisableHbaBySpeed @0 :UInt8;
    uiDisableHbaByHighSpeed @1 :UInt8;
    eSpeedState @2 :UInt32;
    fActivatePenaltyTimer @3 :Float32;
}

struct HbaVedVehDynT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    longitudinal @2 :HbaLongitudinalT;
    lateral @3 :HbaLateralT;
    motionState @4 :HbaMotionStateVehDynT;
}

struct HbaVedVehSigT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    vehSigAdd @2 :HbaVehSigAddT;
}

struct HbaVehSigAddT {
    wiperState @0 :UInt8;
    wiperStage @1 :UInt8;
    fogLampFront @2 :Bool;
    fogLampRear @3 :Bool;
    speedoSpeed @4 :Float32;
}

struct HbaVeloCorrVehDynT {
    corrVelo @0 :Float32;
}

struct HbaWeatherDataT {
    fWiperStageTime @0 :Float32;
    bWiperStageUsed @1 :Bool;
}

struct HbaYawRateVehDynT {
    yawRate @0 :Float32;
}

struct HeaderAssessedObjListT {
    iNumOfUsedObjects @0 :Int8;
    iPadding @1 :UInt8;
    aiOoiList @2 :List(Int8);
    eRelObjLossReason @3 :UInt8;
}

struct HeaderObjListT {
    iNumOfUsedObjects @0 :Int8;
    iDistXSortedObjectList @1 :List(Int8);
}

struct HeadOutCustHbaT {
    fHbaDecel @0 :Float32;
    uiHbaLevel @1 :UInt8;
    uiHbaFactor @2 :Float32;
    uActiveHyp @3 :UInt8;
    eFctChan @4 :UInt8;
}

struct HeadOutCustThreatLvlT {
    threatLvl @0 :UInt8;
}

struct HypoIntfCustomT {
    fTtsEmergency @0 :Float32;
    fTtsEmergencyStd @1 :Float32;
}

struct HypoIntfDegrPerfT {
    fMaxDistAln @0 :Float32;
    fMaxDistHrz @1 :Float32;
    fMaxDistVis @2 :Float32;
    fMaxDist @3 :Float32;
}

struct HypoIntfDegrSftyT {
    fMaxDistAln @0 :Float32;
    fMaxDistHrz @1 :Float32;
    fMaxDistVis @2 :Float32;
    fMaxDist @3 :Float32;
}

struct HypoIntfDegrT {
    safety @0 :HypoIntfDegrSftyT;
    performance @1 :HypoIntfDegrPerfT;
}

struct HypoIntfHeaderT {
    uiNumOfHypotheses @0 :UInt8;
}

struct HypothesisIntfT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    header @2 :HypoIntfHeaderT;
    degradation @3 :HypoIntfDegrT;
    hypothesis @4 :List(HypothesisT);
    sTcuSysInfo @5 :TcuSysInfoT;
}

struct HypothesisT {
    uiObjectRef @0 :Int8;
    eType @1 :UInt8;
    uiObjectProbability @2 :UInt8;
    eEbaObjectClass @3 :UInt8;
    eEbaDynProp @4 :UInt8;
    eEbaInhibitionMask @5 :UInt16;
    uiHypothesisProbability @6 :UInt8;
    fHypothesisLifetime @7 :Float32;
    fTtc @8 :Float32;
    fTtcStd @9 :Float32;
    fTtc2 @10 :Float32;
    fTtc3 @11 :Float32;
    fTtc4 @12 :Float32;
    fTtbPre @13 :Float32;
    fTtbPreStd @14 :Float32;
    fTtbAcute @15 :Float32;
    fTtbAcuteStd @16 :Float32;
    fTtsPre @17 :Float32;
    fTtsPreStd @18 :Float32;
    fTtsAcute @19 :Float32;
    fTtsAcuteStd @20 :Float32;
    fLongNecAccel @21 :Float32;
    fLatNecAccel @22 :Float32;
    fDistX @23 :Float32;
    fDistXStd @24 :Float32;
    fVrelX @25 :Float32;
    fVrelXStd @26 :Float32;
    fArelX @27 :Float32;
    fArelXStd @28 :Float32;
    fDistY @29 :Float32;
    fDistYStd @30 :Float32;
    fVrelY @31 :Float32;
    fVrelYStd @32 :Float32;
    fArelY @33 :Float32;
    fArelYStd @34 :Float32;
    fClosingVelocity @35 :Float32;
    fClosingVelocityStd @36 :Float32;
    custom @37 :HypoIntfCustomT;
}

struct IsaInfo {
    isaState @0 :Bool;
    isaSpeedValidity @1 :UInt8;
    isaSpeed @2 :UInt8;
    previousSetSpeed @3 :UInt8;
    previousIsaSpeed @4 :UInt8;
}

struct KinCtrlDynInputT {
    longiInitializationAccel @0 :Int16;
    steerAngle @1 :Float32;
    steerSpeed @2 :Float32;
    standStillDetected @3 :Bool;
    dcStatusInformation @4 :UInt8;
    driverOverrideAccelPedal @5 :Bool;
    driverOverrideDecelPedal @6 :Bool;
    driverBraking @7 :Bool;
    dasAccelRequestLimited @8 :Bool;
    dasDecelRequestLimited @9 :Bool;
    longiShutoffAcknowledged @10 :Bool;
    accEnable @11 :Bool;
    accInhibit @12 :Bool;
    accReset @13 :Bool;
    parkBrkEng @14 :Bool;
    brkSw @15 :Bool;
    doorStateFr @16 :Bool;
    doorStateFl @17 :Bool;
    doorStateRr @18 :Bool;
    doorStateRl @19 :Bool;
    hoodState @20 :Bool;
    trunkState @21 :Bool;
    seatbeltState @22 :UInt8;
    countryCode @23 :UInt8;
}

struct KinDisplayInputT {
    speedUnit @0 :UInt8;
    speedometerSpeed @1 :UInt16;
}

struct KinDriverOutputT {
    driveOffRequest @0 :Bool;
    driveOffPossible @1 :Bool;
    failureState @2 :UInt8;
    operationalMode @3 :UInt8;
    ldmDriveMode @4 :UInt8;
}

struct KinematicT {
    fDistX @0 :Float32;
    fDistXStd @1 :Float32;
    fDistY @2 :Float32;
    fDistYStd @3 :Float32;
    fVrelX @4 :Float32;
    fVrelXStd @5 :Float32;
    fVrelY @6 :Float32;
    fVrelYStd @7 :Float32;
    fVabsX @8 :Float32;
    fVabsXStd @9 :Float32;
    fVabsY @10 :Float32;
    fVabsYStd @11 :Float32;
    fArelX @12 :Float32;
    fArelXStd @13 :Float32;
    fArelY @14 :Float32;
    fArelYStd @15 :Float32;
    fAabsX @16 :Float32;
    fAabsXStd @17 :Float32;
    fAabsY @18 :Float32;
    fAabsYStd @19 :Float32;
}

struct KinFctInfoT {
    objectDetected @0 :Bool;
    fcaAlert @1 :Bool;
    fcaStatus @2 :UInt8;
    dmAlertLevel @3 :UInt8;
    dmStatus @4 :UInt8;
    headwayControlAlert @5 :Bool;
    requestedDistance @6 :UInt8;
    objInterestDistance @7 :UInt8;
    headwaySetting @8 :UInt8;
    desiredSpeed @9 :UInt8;
    recommendedSpeed @10 :UInt8;
    speedTarget @11 :UInt8;
}

struct KinOutputT {
    minRequestedLongAcceleration @0 :Int16;
    maxRequestedLongAcceleration @1 :Int16;
    dasStatus @2 :UInt8;
    dasAccelLimitationActive @3 :Bool;
    dasFailureInformation @4 :UInt8;
    brakePreFill @5 :Bool;
    standStillRequest @6 :Bool;
}

struct LaneInformationT {
    eAssociatedLane @0 :UInt8;
    eFuncAssociatedLane @1 :UInt8;
    uiCutInProbability @2 :UInt8;
    uiCutOutProbability @3 :UInt8;
}

struct LatAccelVehDynT {
    latAccel @0 :Float32;
    variance @1 :Float32;
}

struct LateralT {
    yawRate @0 :YawRateVehDynT;
    offCompStWheelAngle @1 :Float32;
    curve @2 :CurveVehDynT;
    drvIntCurve @3 :DrvIntCurveVehDynT;
    accel @4 :LatAccelVehDynT;
    slipAngle @5 :SideSlipVehDynT;
}

struct LbsBsdCalculateT {
    uiVersionNumNu @0 :UInt32;
    bBsdWarnActiveLeft @1 :Bool;
    bBsdWarnActiveRight @2 :Bool;
    uBsdWarnActiveLeftId @3 :UInt8;
    uBsdWarnActiveRightId @4 :UInt8;
}

struct LbsbsdDebugT {
    uiVersionNumber @0 :UInt32;
    bsdObjInfo @1 :List(LbsbsdInfoDebugT);
    bsdGlobalsZones @2 :LbsBsdGlobalBsdZoneSt;
    bsdGlobals @3 :LbsBsdGlobalsT;
    bsdZoneObjParList @4 :List(LbsBsdZoneObjPar);
    lbsBsdWarnDecideListDebug @5 :List(LbsBsdWarnDecideDebugT);
}

struct LbsBsdGlobalBsdZoneSt {
    bsdZoneParameterLeft @0 :LbsBsdZoneParameterT;
    bsdZoneParameterRight @1 :LbsBsdZoneParameterT;
}

struct LbsBsdGlobalsT {
    scenarioObserver @0 :LbsBsdScenarioObserverT;
    fVxThreshMovStatMps @1 :Float32;
    fAngleFrontSectorDeg @2 :Float32;
    fBsdZoneXminStaticMet @3 :Float32;
    fBsdZoneXminMet @4 :Float32;
    fBsdZoneXmaxMet @5 :Float32;
    fBsdZoneYminMet @6 :Float32;
    fBsdZoneYmaxMet @7 :Float32;
    fBsdZoneXminWithHystMet @8 :Float32;
    fBsdZoneXmaxWithHystMet @9 :Float32;
    fBsdZoneYminWithHystMet @10 :Float32;
    fBsdZoneYmaxWithHystMet @11 :Float32;
    fMinAssocProbFrontNu @12 :Float32;
    fMinAssocProbSideRearNu @13 :Float32;
    fMinXmovedMet @14 :Float32;
    fBsdWarnActiveLeftDistXMet @15 :Float32;
    fBsdWarnActiveRightDistXMet @16 :Float32;
    bBsdWarnActive @17 :Bool;
    bBsdWarnActiveLeft @18 :Bool;
    bBsdWarnActiveRight @19 :Bool;
    bBsdWarnActiveLeftLastCycle @20 :Bool;
    bBsdWarnActiveRightLastCycle @21 :Bool;
    bBsdWarnActiveLeftIdNu @22 :UInt8;
    bBsdWarnActiveRightIdNu @23 :UInt8;
    bInnerSensorDriven @24 :Bool;
    bInnerSensorSteering @25 :Bool;
    bMultiObjectSuppression @26 :Bool;
    uActivationObjectCounterLeft @27 :UInt8;
    uActivationObjectCounterLeftId @28 :List(UInt8);
    uActivationObjectCounterLeftIdLastCycle @29 :List(UInt8);
    bFirstAppearObjStartDurationLeft @30 :Bool;
    bBsdEnterStandbyformultiObjLeft @31 :Bool;
    uActivationObjectCounterRight @32 :UInt8;
    uActivationObjectCounterRightId @33 :List(UInt8);
    uActivationObjectCounterRightIdLastCycle @34 :List(UInt8);
    bFirstAppearObjStartDurationRight @35 :Bool;
    bBsdEnterStandbyformultiObjRight @36 :Bool;
}

struct LbsbsdInfoDebugT {
    fSoTDelayTimeS @0 :Float32;
    fRearConfNu @1 :Float32;
    fBsdZoneObjXminMet @2 :Float32;
    ubAppearanceNu @3 :UInt8;
    ubHitsInFrontNu @4 :UInt8;
    ubHitsInSideNu @5 :UInt8;
    ubHitsInRearNu @6 :UInt8;
    ubGrdHitCounterNu @7 :UInt8;
    ubBehindGrdCounterNu @8 :UInt8;
    ubClassNu @9 :UInt8;
    ubOwnLaneCounterNu @10 :UInt8;
    bInBsdZone @11 :Bool;
    bInSotZone @12 :Bool;
    bInSotZonePrevious @13 :Bool;
    bObjectAndZoneOverlap @14 :Bool;
    bBsdRelevant @15 :Bool;
    bBsdWarning @16 :Bool;
    bUpdatedRecently @17 :Bool;
    bUpdatedRecentlyWeak @18 :Bool;
    bLivedLongEnough @19 :Bool;
    bQualityEnough @20 :Bool;
    bObjectOnOwnlane @21 :Bool;
    bCreateBehindGrd @22 :Bool;
    bObjectBehindGrd @23 :Bool;
    bSoTDelayActive @24 :Bool;
    bShortWarn @25 :Bool;
    bIsSoT @26 :Bool;
    bFastSoT @27 :Bool;
    bPlausibility @28 :Bool;
    bPossibleWrappedObj @29 :Bool;
    eObjDirection @30 :UInt8;
}

struct LbsBsdObjInfoT {
    fBsdZoneXMinMet @0 :Float32;
}

struct LbsBsdScenarioObserverT {
    uNumberSoTObjsNu @0 :UInt8;
    uNumberSoTObjsLastCycleNu @1 :UInt8;
}

struct LbsBsdWarnDecideDebugT {
    bsdWarnDecideUBsdWarning @0 :UInt8;
    bsdWarnDecideObjStable @1 :Bool;
    bsdWarnDecideLcaMirrorObject @2 :Bool;
    bsdWarnDecideLcaMirrorFrontObject @3 :Bool;
    bsdWarnDecideInBsdZone @4 :Bool;
    bsdWarnDecideBBsdRelevant @5 :Bool;
    bsdWarnDecideBLivedLongEnough @6 :Bool;
    bsdWarnDecideBObjectOnOwnlane @7 :Bool;
    bsdWarnDecideBObjectBehindGrd @8 :Bool;
    bsdWarnDecideBPlausibility @9 :Bool;
    bsdWarnDecideBUpdatedRecently @10 :Bool;
    bsdWarnDecideUbGrdHitCounterNu @11 :UInt8;
    bsdWarnDecideBQualityEnough @12 :Bool;
    bsdWarnDecideBSoTDelayActive @13 :Bool;
    bsdWarnDecideBShortWarn @14 :Bool;
    bsdWarnDecideBFastSoT @15 :Bool;
    bsdWarnDecideBObjectAndZoneOverlap @16 :Bool;
}

struct LbsBsdZoneObjPar {
    fZoneXminMet @0 :Float32;
    fZoneXmaxMet @1 :Float32;
    fZoneYminMet @2 :Float32;
    fZoneYmaxMet @3 :Float32;
}

struct LbsBsdZoneParameterT {
    fBsdZoneXminMet @0 :Float32;
    fBsdZoneXmaxMet @1 :Float32;
    fBsdZoneYminMet @2 :Float32;
    fBsdZoneYmaxMet @3 :Float32;
    fBsdZoneXminWithHystMet @4 :Float32;
    fBsdZoneXmaxWithHystMet @5 :Float32;
    fBsdZoneYminWithHystMet @6 :Float32;
    fBsdZoneYmaxWithHystMet @7 :Float32;
}

struct LbsCalculateT {
    roadRelation @0 :List(LbsSrrObjRoadRelationT);
    lastObjIdList @1 :List(UInt16);
    lbsObjHistoryList @2 :List(LbsSrrObjHistoryT);
    lbsRunState @3 :LbsRunStateT;
    lbsObjInfoList @4 :List(LbsObjInfoSt);
    lbssiCalc @5 :LbsSiCalculateT;
    siObjInfoList @6 :List(LbsSiObjInfoT);
    lbsbsdCalc @7 :LbsBsdCalculateT;
    bsdObjInfoList @8 :List(LbsBsdObjInfoT);
    lbslcaCalc @9 :LbsLcaCalculateT;
    lcaObjInfoList @10 :List(LbsLcaObjInfoT);
    oseObjInfoList @11 :List(LbsOseObjInfoT);
    lbsWarnLastCycle @12 :LbsWarningLastCycleT;
}

struct LbsCanOutput {
    bDowPowerModeDone @0 :Bool;
    uDowRightActivation @1 :UInt8;
    uDowLeftActivation @2 :UInt8;
    uDowState @3 :UInt8;
    uDowSystemFaultStatus @4 :UInt8;
    uDowObjIndex @5 :UInt8;
    siDowLeftHighlightIdNu @6 :Int32;
    siDowRightHighlightIdNu @7 :Int32;
    uRcWwarningReq @8 :UInt8;
    uRcwWarnState @9 :UInt8;
    uRcwSystemFaultStatus @10 :UInt8;
    bRcwState @11 :Bool;
    uRcwObjIndex @12 :UInt8;
    siRcwHighlightIdNu @13 :Int32;
    uLcabsdOnOffSet @14 :UInt8;
    uLcaSystemFaultStatus @15 :UInt8;
    uLcaLeftWarnSt @16 :UInt8;
    uLcaRightWarnSt @17 :UInt8;
    uLcabsdObjIndex @18 :UInt8;
    siLcaLeftHighlightIdNu @19 :Int32;
    siLcaRightHighlightIdNu @20 :Int32;
}

struct LbsDebug {
    header @0 :StdHeader.Header;
    pubMsgLbsReqPorts @1 :LbsInReqSt;
    pubMsgLbsOutPorts @2 :LbsOutProT;
    pubMsgLbsReqDebugs @3 :LbsDebugT;
}

struct LbsDebugT {
    uiVersionNumber @0 :UInt32;
    bsdDebug @1 :LbsbsdDebugT;
    lcaDebug @2 :LbslcaDebugT;
    oseDebug @3 :LbsoseDebugT;
    rcwDebug @4 :LbsrcwDebugT;
    lbsCalcuDebug @5 :LbsCalculateT;
}

struct LbsFunStateT {
    bBsdWarning @0 :Bool;
    bBsdWarningLeft @1 :Bool;
    bBsdWarningRight @2 :Bool;
    bLcaWarning @3 :Bool;
    bLcaWarningLeft @4 :Bool;
    bLcaWarningRight @5 :Bool;
    bOseWarning @6 :BWarnLevelT;
    bOseWarningLeft @7 :BWarnLevelT;
    bOseWarningRight @8 :BWarnLevelT;
    uRcwWarning @9 :UInt8;
    fTtcS @10 :Float32;
}

struct LbsGenObjAttributesT {
    eDynamicPropertyNu @0 :UInt8;
    uiDynConfidencePer @1 :UInt8;
    eClassificationNu @2 :UInt32;
    uiClassConfidencePer @3 :UInt8;
}

struct LbsGenObjectSt {
    kinemactic @0 :LbsGenObjKinematicsT;
    geometry @1 :LbsGenObjGeometryT;
    general @2 :LbsGenObjGeneralsT;
    attributes @3 :LbsGenObjAttributesT;
    bRightSensor @4 :Bool;
}

struct LbsGenObjGeneralsT {
    fLifeTimeS @0 :Float32;
    uiLifeCyclesNu @1 :UInt16;
    uiMaintenanceStateNu @2 :UInt8;
    uiIdNu @3 :UInt16;
    iRawFusionIdNu @4 :Int32;
}

struct LbsGenObjGeometryT {
    fWidthMet @0 :Float32;
    fWidthStdMet @1 :Float32;
    fWidthLeftMet @2 :Float32;
    fWidthRightMet @3 :Float32;
    fLengthMet @4 :Float32;
    fLengthStdMet @5 :Float32;
    fLengthFrontMet @6 :Float32;
    fLengthRearMet @7 :Float32;
    fAbsOrientationRad @8 :Float32;
    fAbsOrientationStdRad @9 :Float32;
}

struct LbsGenObjKinematicsT {
    fDistXMet @0 :Float32;
    fDistXStdMet @1 :Float32;
    fDistYMet @2 :Float32;
    fDistYStdMet @3 :Float32;
    fVrelXMps @4 :Float32;
    fVrelXStdMps @5 :Float32;
    fVrelYMps @6 :Float32;
    fVrelYStdMps @7 :Float32;
    fArelXMpss @8 :Float32;
    fArelXStdMpss @9 :Float32;
    fArelYMpss @10 :Float32;
    fArelYStdMpss @11 :Float32;
    fVabsXMps @12 :Float32;
    fVabsXStdMps @13 :Float32;
    fVabsYMps @14 :Float32;
    fVabsYStdMps @15 :Float32;
    fAabsXMpss @16 :Float32;
    fAabsXStdMpss @17 :Float32;
    fAabsYMpss @18 :Float32;
    fAabsYStdMpss @19 :Float32;
}

struct LbsGlobalsT {
    parameterObjSel @0 :ParameterObjSelT;
    lastCycleStates @1 :LbsLastCycleStatesT;
    fSensorOffsetToRearMet @2 :Float32;
    fSensorOffetToSideMet @3 :Float32;
    fMaxSpeedOverGroundMps @4 :Float32;
    bInnerSensorDriven @5 :Bool;
    bInnerSensorSteering @6 :Bool;
    bFctObjOutput @7 :Bool;
    bBsdFunctionOutput @8 :Bool;
    bLcaFunctionOutput @9 :Bool;
    bRpcsFunctionOutput @10 :Bool;
    bOseFunctionOutput @11 :List(Bool);
}

struct LbsInReqSt {
    genObjList @0 :EmGenObjListSt;
    srrObjList @1 :EmsrrObjListSt;
    egoVehInfo @2 :EgoVehicleInfoT;
    road @3 :EmRoadT;
    lbsSystemParam @4 :LbsSystemParamT;
    lbsNsNvramNu @5 :LbsnvramT;
    lbsSupportInfo @6 :LbsSupportInfoInputT;
}

struct LbsLastCycleStatesT {
    eRoadType @0 :UInt8;
    bBsdFunctionActive @1 :Bool;
    bLcaFunctionActive @2 :Bool;
    bOseFunctionAcitve @3 :Bool;
    bEgoSpeedConditionBsd @4 :Bool;
    bEgoSpeedConditionLca @5 :Bool;
    bEgoSpeedConditionRpcs @6 :Bool;
    bEgoSpeedConditionOse @7 :Bool;
}

struct LbsLcaCalculateT {
    uiVersionNumNu @0 :UInt32;
    bLcaWarnActive @1 :Bool;
    uLcaWarningIdNu @2 :UInt8;
    fXObjectWarningMet @3 :Float32;
    fCriticalTtcS @4 :Float32;
    fFmObjRate @5 :Float32;
}

struct LbsLcaConfigT {
    fTtcThreshVrelLowS @0 :Float32;
    fTtcThreshVrelMidS @1 :Float32;
    fTtcThreshVrelHighS @2 :Float32;
    fTtcHysteresisS @3 :Float32;
    fLcaRangeMaxMet @4 :Float32;
    fLcaCurveRadMaxMet @5 :Float32;
    uLcaWarningDurationCfg @6 :UInt8;
}

struct LbslcaDebugT {
    uiVersionNumber @0 :UInt32;
    uLcaWarningIdNu @1 :UInt32;
    lcaWarnInfo @2 :LbsLcaWarnInfoT;
    lcaConfig @3 :LbsLcaConfigT;
    lcaFrontMirror @4 :LbsLcaFrontMirrorT;
    fLcaRange @5 :Float32;
    uCntLcaPathBlockedLeft @6 :UInt8;
    uCntLcaPathBlockedRight @7 :UInt8;
    bLcaPathBlockedLeft @8 :Bool;
    bLcaPathBlockedRight @9 :Bool;
    lcaObjOutputList @10 :List(LbslcaObjInfoDebugT);
    lbsLcaWarnDecideListDebug @11 :List(LbsLcaWarnDecideDebugT);
}

struct LbsLcaFrontMirrorT {
    fFmObjRate @0 :Float32;
    lcaVfVxThreshAddMps @1 :Float32;
    lcaVfVxThreshOwnLaneMinMps @2 :Float32;
    lcaVfVxThreshOwnLaneMaxMps @3 :Float32;
    lcaVfVxThreshAdjLaneMinMps @4 :Float32;
    lcaVfVxThreshAdjLaneMaxMps @5 :Float32;
    fRcsStableObjOwnLane @6 :Float32;
    fRcsStableObjAdjLane @7 :Float32;
    uClosetStableObjIdOwnLane @8 :UInt8;
    uClosetStableObjIdAdjLane @9 :UInt8;
    uNofFmObjects @10 :UInt8;
}

struct LbslcaObjInfoDebugT {
    fTtcThreshold @0 :Float32;
    fBehindGrdProbPer @1 :Float32;
    uFrontMirrorCnt @2 :UInt8;
    bUpdateRecently @3 :Bool;
    bInLcaRange @4 :Bool;
    bLcaMirrorObject @5 :Bool;
    bLcaMirrorFrontObject @6 :Bool;
    bLcaObjPathInvalid @7 :Bool;
    bLcaQuality @8 :Bool;
    bLcaLaneConditions @9 :Bool;
    bLcaRelevant @10 :Bool;
    bLcaWarningConditions @11 :Bool;
    bLcaWarning @12 :Bool;
}

struct LbsLcaObjInfoT {
    fTtcThreshold @0 :Float32;
    fBehindGrdProbPer @1 :Float32;
    uFrontMirrorCnt @2 :UInt8;
    bUpdateRecently @3 :Bool;
    bInLcaRange @4 :Bool;
    bLcaMirrorObject @5 :Bool;
    bLcaMirrorFrontObject @6 :Bool;
    bLcaObjPathInvalid @7 :Bool;
    bLcaQuality @8 :Bool;
    bLcaLaneConditions @9 :Bool;
    bLcaRelevant @10 :Bool;
    bLcaWarningConditions @11 :Bool;
    bLcaWarning @12 :Bool;
}

struct LbsLcaWarnDecideDebugT {
    lcaWarnDecideBObjStable @0 :Bool;
    lcaWarnDecideBLcaQuality @1 :Bool;
    lcaWarnDecideBLcaRelevant @2 :Bool;
    lcaWarnDecideBLcaMirrorObject @3 :Bool;
    lcaWarnDecideBLcaMirrorFrontObject @4 :Bool;
    lcaWarnDecideBLcaLaneConditions @5 :Bool;
    lcaWarnDecideBLcaObjPathInvalid @6 :Bool;
    lcaWarnDecideFBehindGrdProbPer @7 :Float32;
    lcaWarnDecideBLcaWarnActive @8 :Bool;
    lcaWarnDecideBLcaWarningConditions @9 :Bool;
    lcaWarnDecideFTtcS @10 :Float32;
    lcaWarnDecideFTtcThreshold @11 :Float32;
    lcaWarnDecideFDistXMet @12 :Float32;
    lcaWarnDecideFTtcAccelMps2 @13 :Float32;
    lcaWarnDecideFVrelXMps @14 :Float32;
    lcaWarnDecideBInLcaRange @15 :Bool;
    lcaWarnDecideBUpdateRecently @16 :Bool;
    lcaWarnDecideUiHighestAssocProbPer @17 :UInt8;
    lcaWarnDecideBLcaWarnActiveRight @18 :Bool;
    lcaWarnDecideBLcaWarnActiveLeft @19 :Bool;
}

struct LbsLcaWarnInfoT {
    fCriticalTtcS @0 :Float32;
    fXObjectWarningMet @1 :Float32;
    uLcaWarningIdNu @2 :UInt8;
    bLcaWarnActive @3 :Bool;
    bLcaWarningLastCycle @4 :Bool;
    bLcaWarnActiveLeft @5 :Bool;
    bLcaWarnActiveRight @6 :Bool;
}

struct LbsnvramT {
    lbsNbBsdPowerOffSwitchStateNu @0 :Bool;
    lbsNbLcaPowerOffSwitchStateNu @1 :Bool;
    lbsNbDowPowerOffSwitchStateNu @2 :Bool;
    lbsNbRcwPowerOffSwitchStateNu @3 :Bool;
}

struct LbsObjBordersT {
    fXminMet @0 :Float32;
    fXmaxMet @1 :Float32;
    fYminMet @2 :Float32;
    fYmaxMet @3 :Float32;
}

struct LbsObjectSelectT {
    bObjectFastEnough @0 :Bool;
    bBreakthroughHit @1 :Bool;
    bObjInRange @2 :Bool;
}

struct LbsObjInfoSt {
    objBorders @0 :LbsObjBordersT;
    objMovementBorders @1 :LbsObjBordersT;
    objSel @2 :LbsObjectSelectT;
    fTtcS @3 :Float32;
    fTtcAccelMps2 @4 :Float32;
    fTtcFilteredS @5 :Float32;
    fTtcRadialS @6 :Float32;
    fVabsMpss @7 :Float32;
    fRangeRadial @8 :Float32;
    fXLastCycleMet @9 :Float32;
    fYLastCycleMet @10 :Float32;
    fVxPosBased @11 :Float32;
    fVyPosBased @12 :Float32;
    fSpeedFilteredMpss @13 :Float32;
    fCycletimeSumS @14 :Float32;
    fUpdateRateNu @15 :Float32;
    fXMovementMet @16 :Float32;
    fYMovementMet @17 :Float32;
    fAngleDeg @18 :Float32;
    fAssocProbFiltered @19 :Float32;
    fObjWidthMax @20 :Float32;
    fObjLengthMax @21 :Float32;
    uUniqueId @22 :UInt16;
    uLastMergedObjId @23 :UInt8;
    bLowTtcAtStart @24 :Bool;
    bCreateAdjStableObj @25 :Bool;
    firstStableDynProp @26 :UInt8;
}

struct LbsoseDebugT {
    uiVersionNumNu @0 :UInt32;
    oseObjInfoArray @1 :List(LbsoseObjInfoArrayDebugT);
    oseWarnDecideDebug @2 :List(LbsOseWarnDecideDebugT);
}

struct LbsoseObjInfoArrayDebugT {
    infoLevel @0 :List(LbsoseObjInfoLevelDebugT);
    fYBreakthrough @1 :List(Float32);
    fYBreakthroughStd @2 :List(Float32);
    fTtcS @3 :List(Float32);
    fTtcFilteredS @4 :List(Float32);
    fDistToCrossingLineMet @5 :List(Float32);
    fSideTrackProb @6 :Float32;
    bRelevant @7 :Bool;
    bMirror @8 :Bool;
    bSideTrack @9 :Bool;
    bObjectFromRear @10 :Bool;
    bValidApproachAngle @11 :Bool;
    bObjectAtEdgeFoV @12 :Bool;
    bShortTtc @13 :Bool;
    bUpdatedRecently @14 :Bool;
    fQuality @15 :Float32;
    fEstWidth @16 :LbsoseWidthEstimDebugT;
}

struct LbsoseObjInfoLevelDebugT {
    fBtHitHystTimer @0 :Float32;
    uBreakthroughHitConfi @1 :List(UInt8);
    bBreakthroughHit @2 :List(Bool);
    bWarning @3 :Bool;
    bWarningLastCycle @4 :Bool;
    bObjectInRange @5 :Bool;
    bTtcBelowThresh @6 :List(Bool);
    bBtHitHystActive @7 :Bool;
}

struct LbsOseObjInfoLevelOutProT {
    fBtHitHystTimer @0 :Float32;
    uBreakthroughHitConfi @1 :List(UInt8);
    bBreakthroughHit @2 :List(Bool);
    bWarning @3 :Bool;
    bWarningLastCycle @4 :Bool;
    bObjectInRange @5 :Bool;
    bTtcBelowThresh @6 :List(Bool);
    bBtHitHystActive @7 :Bool;
}

struct LbsOseObjInfoT {
    infoLevel @0 :List(LbsOseObjInfoLevelOutProT);
    fYBreakthrough @1 :List(Float32);
    fYBreakthroughStd @2 :List(Float32);
    fTtcS @3 :List(Float32);
    fTtcFilteredS @4 :List(Float32);
    fDistToCrossingLineMet @5 :List(Float32);
    fSideTrackProb @6 :Float32;
    bRelevant @7 :Bool;
    bMirror @8 :Bool;
    bSideTrack @9 :Bool;
    bObjectFromRear @10 :Bool;
    bValidApproachAngle @11 :Bool;
    bObjectAtEdgeFoV @12 :Bool;
    bShortTtc @13 :Bool;
    bUpdatedRecently @14 :Bool;
    fQuality @15 :Float32;
    fEstWidth @16 :LbsOseWidthEstimOutProT;
}

struct LbsOseWarnDecideDebugT {
    objid @0 :UInt8;
    oseWarnDecideInfoLevel @1 :List(LbsOseWarnDecideInfoLevelT);
    lcaWarnDecideBRelevant @2 :Bool;
    lcaWarnDecideNonBMirror @3 :Bool;
    lcaWarnDecideNonBShortTtc @4 :Bool;
    lcaWarnDecideNonBSideTrack @5 :Bool;
    lcaWarnDecideBValidApproachAngle @6 :Bool;
    lcaWarnDecideBUpdatedRecently @7 :Bool;
    lcaWarnDecideFQuality @8 :Float32;
    lcaWarnDecideFRcs @9 :Float32;
    lcaWarnDecideFTtcS @10 :List(Float32);
    lcaWarnDecideFTtcFilteredS @11 :List(Float32);
}

struct LbsOseWarnDecideInfoLevelT {
    lcaWarnDecideBWarning @0 :Bool;
    lcaWarnDecideBObjectInRange @1 :Bool;
    lcaWarnDecideUBreakthroughHitConfi @2 :List(UInt8);
    lcaWarnDecideBTtcBelowThresh @3 :List(Bool);
    lcaWarnDecideBBtHitHystActive @4 :Bool;
    lcaWarnDecideFTtcThresholdS @5 :Float32;
}

struct LbsoseWidthEstimDebugT {
    uCounters @0 :List(UInt16);
    fValueMet @1 :Float32;
}

struct LbsOseWidthEstimOutProT {
    uCounters @0 :List(UInt16);
    fValueMet @1 :Float32;
}

struct LbsOutProT {
    lbsFunState @0 :LbsFunStateT;
    lbsGlobals @1 :LbsGlobalsT;
    lbsNsNvramNu @2 :LbsnvramT;
    lbscanOutputs @3 :LbsCanOutput;
}

struct LbsrcwCorridorObserverT {
    lbSfXDist @0 :Float32;
    lbSfCorridorOccupancy @1 :Float32;
    lbSfXMin @2 :Float32;
    lbSfXMax @3 :Float32;
    lbSfInCorridorTime @4 :Float32;
    lbSuObjId @5 :UInt8;
}

struct LbsrcwDebugT {
    lbsDebugRcWstatemachine @0 :UInt8;
    lbsDebugRcwDebugSubConditions @1 :LbsrcwStatusSubcondition;
    lbsDebugRcwWarningInfo @2 :LbsrcwWarningInfoT;
    lbsDebugRcwObjInfo @3 :List(LbsrcwInfoT);
    lbsDebugRcwObjInfoLastcycle @4 :List(LbsrcwInfoT);
    lbsDebugRcwCorridorObjs @5 :List(LbsrcwCorridorObserverT);
    lbsrcwWarnDecideDebug @6 :List(LbsRcwWarnDecideDebugT);
}

struct LbsrcwInfoT {
    uObjId @0 :UInt8;
    lbSfTtcThreshold @1 :Float32;
    lbSfCorridorOverlap @2 :Float32;
    lbSfCorridorOccupancy @3 :Float32;
    lbSfObjectOccupancy @4 :Float32;
    lbSfCorridorOccThreshold @5 :Float32;
    lbSfInCOrridorTime @6 :Float32;
    lbSfYBreakThrough @7 :Float32;
    lbSfHeadingFiltered @8 :Float32;
    lbSuCorridorHitCnt @9 :UInt8;
    lbSuMultiPathCnt @10 :UInt8;
    lbSbRcwQuality @11 :Bool;
    lbSbUpdateRecently @12 :Bool;
    lbSbRcwRelevant @13 :Bool;
    lbSbInRcwCorridor @14 :Bool;
    lbSbHeadingAngleInRange @15 :Bool;
    lbSbObjCorridorBlocked @16 :Bool;
    lbSbMultiPathObj @17 :Bool;
    lbSbRcwWarningConditions @18 :Bool;
    lbSbRcwWarning @19 :Bool;
    lbSbOppositeOverlap @20 :Bool;
}

struct LbsrcwStatusSubcondition {
    lbSdebugActiveBRcwWarningActive @0 :Bool;
    lbSdebugActiveBCorridorBlocked @1 :Bool;
    lbSdebugPassiveVelocityOutOfRange @2 :Bool;
    lbSdebugPassiveLatAcceloutofRange @3 :Bool;
    lbSdebugPassiveLeftTurnLight @4 :Bool;
    lbSdebugPassiveRightTurnLight @5 :Bool;
    lbSdebugPassiveGearPosition @6 :Bool;
    lbSdebugPassiveBlockingtimeActive @7 :Bool;
    lbSdebugHmiOffBRcwHmiOpen @8 :Bool;
    lbSdebugFailureBRcwFailureCondition @9 :Bool;
}

struct LbsRcwWarnDecideDebugT {
    rcwWarnDecideBRcwWarningConditions @0 :UInt8;
    rcwWarnDecideBEnableRcwWarning @1 :Bool;
    rcwWarnDecideFTtcRelevant @2 :Float32;
    rcwWarnDecideFTtcThreshold @3 :Float32;
    rcwWarnDecideFAxObj @4 :Float32;
    rcwWarnDecideBUpdateRecently @5 :Bool;
    rcwWarnDecideBObjCorridorBlocked @6 :Bool;
    rcwWarnDecideFCorridorOccupancy @7 :Float32;
    rcwWarnDecideFCorridorOccThreshold @8 :Float32;
    rcwWarnDecideFVrelToTrajMps @9 :Float32;
    rcwWarnDecideFWarnDecisionVrel2TrajMaxThs @10 :Bool;
    rcwWarnDecideFAssocProbFiltered @11 :Float32;
    rcwWarnDecideFWarnDecisionAssocProbMin @12 :Float32;
    rcwWarnDecideBTtCthresholdjudgecondition @13 :Bool;
    rcwWarnDecideBfAxObjBelowTtcAdjust @14 :Bool;
    rcwWarnDecideFTtcThresholdAx @15 :Float32;
}

struct LbsrcwWarningInfoT {
    lbSfTtc @0 :Float32;
    lbSfXObjectWarning @1 :Float32;
    lbSuRcwWarningId @2 :UInt8;
    lbSbRcwWarningActive @3 :Bool;
    lbSbRcwWarningActiveLastCycle @4 :Bool;
}

struct LbsRoadTypeT {
    fRoadTypeConf @0 :Float32;
    uiRoadType @1 :UInt8;
}

struct LbsRunStateT {
    bLbsFunctionActionLastCycle @0 :Bool;
}

struct LbsSiCalculateT {
    uiVersionNumNu @0 :UInt32;
}

struct LbsSignalHeaderT {
    uiTimeStampMs @0 :UInt32;
    uiMeasurementCounterNu @1 :UInt16;
    uiCycleCounterNu @2 :UInt16;
    eSigStatusNu @3 :UInt8;
    aReserve @4 :UInt8;
}

struct LbsSiObjInfoT {
    eAssociatedLane @0 :UInt8;
    fVrelToTrajMps @1 :Float32;
    fDistToTrajMet @2 :Float32;
    fTraceBracketLeftMet @3 :Float32;
    fTraceBracketRightMet @4 :Float32;
    fObjBracketOverlapMet @5 :Float32;
}

struct LbsSrrObjectSt {
    qualifiers @0 :LbsSrrObjQualifiersT;
    sensorSpecific @1 :LbsSrrObjSensorSpecificT;
}

struct LbsSrrObjHistoryT {
    fFirstDetectXMet @0 :Float32;
    fFirstDetectYMet @1 :Float32;
    fMaxRangeMet @2 :Float32;
}

struct LbsSrrObjQualifiersT {
    fProbabilityOfExistencePer @0 :Float32;
    uiHighestAssocProbPer @1 :UInt8;
    uiMeasuredTargetFrequencyNu @2 :UInt8;
    bObjStable @3 :Bool;
}

struct LbsSrrObjRoadRelationT {
    fDist2CourseMet @0 :Float32;
    fDist2BorderMet @1 :Float32;
    bDist2BorderValid @2 :Bool;
}

struct LbsSrrObjSensorSpecificT {
    fRcs @0 :Float32;
    fMirrorProbPer @1 :Float32;
}

struct LbsSupportInfoInputT {
    lbsrcwFailure @0 :Bool;
    lbsLeftTurnLightOpen @1 :Bool;
    lbsRightTurnLightOpen @2 :Bool;
    lbsGearInReverse @3 :Bool;
    lbsbsdFailure @4 :Bool;
    lcaFailure @5 :Bool;
    gearInReverseAndParking @6 :Bool;
    vehicleSpdDisplayValid @7 :Bool;
    actGearValid @8 :Bool;
    uTurnLightReqSt @9 :UInt8;
    cdcs11VoiceMode @10 :UInt8;
    dowFailure @11 :Bool;
    bDoorFr @12 :Bool;
    bDoorFl @13 :Bool;
    bDoorRr @14 :Bool;
    bDoorRl @15 :Bool;
    bDowPowerMode3Mins @16 :Bool;
}

struct LbsSystemParamT {
    bBsdFunctionActive @0 :Bool;
    bBsdFunctionOutputActive @1 :Bool;
    bLcaFunctionActive @2 :Bool;
    bLcaFunctionOutputActive @3 :Bool;
    bOseFunctionActive @4 :Bool;
    bOseFunctionOutputActive @5 :Bool;
    bRcwFunctionActive @6 :Bool;
    bRcwFunctionOutputActive @7 :Bool;
    fCycletimeS @8 :Float32;
}

struct LbsWarningLastCycleT {
    uiVersionNumNu @0 :UInt32;
    bBsdWarningLastCycle @1 :Bool;
    bBsdWarningLeftLastCycle @2 :Bool;
    bBsdWarningRightLastCycle @3 :Bool;
    bLcaWarningLastCycle @4 :Bool;
    bOseWarningLastCycle @5 :List(Bool);
}

struct LcfFusionObjectsArrayT {
    uNumTgtObjNu @0 :UInt8;
    sFusionObjects @1 :List(LcfFusionObjectsT);
}

struct LcfFusionObjectsT {
    iIdNu @0 :Int32;
    fPositionMet @1 :List(Float32);
    fVelocityMps @2 :List(Float32);
    fAccelerationMps2 @3 :List(Float32);
    fLengthMet @4 :Float32;
    fWidthMet @5 :Float32;
    fHeadingAngleRad @6 :Float32;
    uClassificationNu @7 :UInt8;
    uSensorsourceNu @8 :UInt32;
    fExistencePerc @9 :Float32;
}

struct LcfSenDebug {
    header @0 :StdHeader.Header;
    sLbpInput @1 :SLbpInputT;
    sLbpOutput @2 :SLbpOutputT;
    sLbpDebug @3 :SLbpDebugT;
    odprInReq @4 :OdprInReqT;
    odprOutPro @5 :OdprOutProT;
    odprDebug @6 :OdprDebugT;
    sLdpsaInput @7 :SLdpsaInputT;
    sLdpsaOutput @8 :SLdpsaOutputT;
    sLdpsaDebug @9 :SLdpsaDebugT;
    sLdwsaInReq @10 :SLdwsaInReqT;
    sLdwsaOutPro @11 :SLdwsaOutProT;
    sLdwsaDebug @12 :SLdwsaDebugT;
    sTjasaInReq @13 :STjasaInReqT;
    sTjasaOutPro @14 :STjasaOutProT;
    sTjasaDebug @15 :STjasaDebugT;
    gLcfOpsInput @16 :SLcfopsInReqT;
    gLcfOpsOutput @17 :SLcfopsOutProT;
    gLcfOpsDebug @18 :SLcfopsDebugT;
    gLcfLccraInput @19 :SLccraInReqT;
    gLcfLccraOutput @20 :SLccraOutProT;
    gLcfLccraDebug @21 :SLccraDebugT;
    gLcfAlpInput @22 :SAlpInReqSt;
    gLcfAlpOutput @23 :SAlpOutputSt;
}

struct LcftjatctOutT {
    tjatctDeltaFCmd @0 :Float32;
    tjatctSumCtrlCrv @1 :Float32;
    lgcEnableCtrlNu @2 :Bool;
    cdcCtrlErrDistY @3 :Float32;
    cdcCtrlErrHeading @4 :Float32;
    yawRate @5 :Float32;
    dataLog0 @6 :Float32;
    dataLog1 @7 :Float32;
    dataLog2 @8 :Float32;
    dataLog3 @9 :Float32;
    dataLog4 @10 :Float32;
    dataLog5 @11 :Float32;
    dataLog6 @12 :Float32;
    dataLog7 @13 :Float32;
    dataLog8 @14 :Float32;
    dataLog9 @15 :Float32;
}

struct LcfVehDebug {
    header @0 :StdHeader.Header;
    tcOutput @1 :LcftjatctOutT;
    tpData @2 :LcfVehTp;
    tcDebug @3 :LcfVehTcDebugT;
    sHodOutPro @4 :SHodOutputT;
    sHodDebug @5 :SHodDebugT;
}

struct LcfVehTcDebugT {
    uiVersionNumNu @0 :UInt32;
    cdcBtfQualifier @1 :UInt8;
    cdcCtrlErrDistY @2 :Float32;
    cdcCtrlErrHeading @3 :Float32;
    cdcEnaCntrlByTgq @4 :Bool;
    cdcEnaFreezeByTgq @5 :Bool;
    cdcEnaResetByTgq @6 :Bool;
    cdcEnaWatchdog @7 :Bool;
    cdcEstCurHeading @8 :Float32;
    cdcEstDeltaTheta @9 :Float32;
    cdcFltDeltaTheta @10 :Float32;
    cdcFltErrDistYTpl @11 :Float32;
    cdcHldCtrlErrDistY @12 :Float32;
    cdcHldCtrlErrHeading @13 :Float32;
    cdcPreErrCtrlHeading @14 :Float32;
    cdcRawBtfQualifier @15 :UInt8;
    cdcRawCtrlErrDistY @16 :Float32;
    cdcRawCtrlErrHeading @17 :Float32;
    cdcRawDeltaTheta @18 :Float32;
    cdcRawErrDistYTpl @19 :Float32;
    cdcRawFltDeltaTheta @20 :Float32;
    clmCrvBySteerAngle @21 :Float32;
    clmEnaDegrReq @22 :Bool;
    clmEnaGrdDeltaFCmd @23 :Bool;
    clmEnaGrdFbcDc @24 :Bool;
    clmEnaGrdFfcCrv @25 :Bool;
    clmEnaSatDeltaFCmd @26 :Bool;
    clmEnaSatFbcDc @27 :Bool;
    clmEnaSatFfcCrv @28 :Bool;
    clmGrdDeltaFCmd @29 :Float32;
    clmGrdFbcDc @30 :Float32;
    clmGrdFfcCrv @31 :Float32;
    clmRawEnaDegrReq @32 :Bool;
    clmRawGrdDeltaFCmd @33 :Float32;
    clmRawGrdFbcDc @34 :Float32;
    clmRawGrdFfcCrv @35 :Float32;
    clmSatDeltaFCmd @36 :Float32;
    clmSatFbcDc @37 :Float32;
    clmSatFfcCrv @38 :Float32;
    clmThdDeltaFCmdGrd @39 :Float32;
    clmThdDeltaFCmdSat @40 :Float32;
    clmThdFbcDcGrd @41 :Float32;
    clmThdFbcDcSat @42 :Float32;
    clmThdFfcCrvGrd @43 :Float32;
    clmThdFfcCrvSat @44 :Float32;
    devBtfFfcQualifierPar @45 :UInt8;
    devBtfFfcQualifierRte @46 :UInt8;
    devCoeffDeltaGainFfc @47 :Float32;
    devCrvTestSignal @48 :Float32;
    devDeltaFTestSignal @49 :Float32;
    devDlySetDeltaF2DotPar @50 :Float32;
    devDlySetDeltaF2DotRte @51 :Float32;
    devDlySetDeltaFDotPar @52 :Float32;
    devDlySetDeltaFDotRte @53 :Float32;
    devDlySetDeltaFPar @54 :Float32;
    devDlySetDeltaFRte @55 :Float32;
    devEnaCntrlByTgq @56 :Bool;
    devEnaCrvGen @57 :UInt8;
    devEnaDeltaFGen @58 :UInt8;
    devEnaFreezeByTgq @59 :Bool;
    devEnaResetByTgq @60 :Bool;
    devHldReqDeltaFRte @61 :Float32;
    devReqDeltaFRte @62 :Float32;
    devRstCrvGen @63 :Bool;
    devRstDeltaFGen @64 :Bool;
    devSetDeltaF3DotPar @65 :Float32;
    devSetDeltaF3DotRte @66 :Float32;
    devSetDeltaFPar @67 :Float32;
    devSetDeltaFRte @68 :Float32;
    devTimeCrvGen @69 :Float32;
    devTimeDeltaFGen @70 :Float32;
    dteCoeffA0TranferFcn @71 :Float32;
    dteCoeffA1TranferFcn @72 :Float32;
    dteCoeffB0TranferFcn @73 :Float32;
    dteCoeffB1TranferFcn @74 :Float32;
    dteCoeffB2TranferFcn @75 :Float32;
    dteCoeffDenS0LaDmc @76 :Float32;
    dteCoeffDenS1LaDmc @77 :Float32;
    dteCoeffDenS2LaDmc @78 :Float32;
    dteCoeffDenS3LaDmc @79 :Float32;
    dteCoeffNumS0LaDmc @80 :Float32;
    dteCoeffNumS1LaDmc @81 :Float32;
    dteDelta2DotForCrv @82 :Float32;
    dteDelta2DotLaDmc @83 :Float32;
    dteDelta2DotVdyFcn @84 :Float32;
    dteDelta3DotLaDmc @85 :Float32;
    dteDeltaByVdyFcn @86 :Float32;
    dteDeltaDotForCrv @87 :Float32;
    dteDeltaDotLaDmc @88 :Float32;
    dteDeltaF2DotPar @89 :Float32;
    dteDeltaF2DotRte @90 :Float32;
    dteDeltaF3DotPar @91 :Float32;
    dteDeltaF3DotRte @92 :Float32;
    dteDeltaFDotPar @93 :Float32;
    dteDeltaFDotRte @94 :Float32;
    dteDeltaFPar @95 :Float32;
    dteDeltaFRte @96 :Float32;
    dteDeltaVdyFcn @97 :Float32;
    dteDlyCurSteerAngle @98 :Float32;
    dteDlyDeltaDotVdyFcn @99 :Float32;
    dteDlyDeltaVdyFcn @100 :Float32;
    dteDlySetCrvDotLaDmc @101 :Float32;
    dteDlySetCrvLaDmc @102 :Float32;
    dteDlySetDelta2DotLaDmc @103 :Float32;
    dteDlySetDeltaDotLaDmc @104 :Float32;
    dteDlySetDeltaLaDmc @105 :Float32;
    dteEnaCtrlByTgq @106 :Bool;
    dteEnaFreezeByTgq @107 :Bool;
    dteEnaResetByTgq @108 :Bool;
    dteEstCrvByBnkAgl @109 :Float32;
    dteFltDlyCurSteerAngle @110 :Float32;
    dteHldReqCrvByBnkAgl @111 :Float32;
    dteHldReqCrvByDstrb @112 :Float32;
    dteHldReqDeltaByBnkAgl @113 :Float32;
    dteHldReqDeltaByDstrb @114 :Float32;
    dteKappaAngleLaDmc @115 :Float32;
    dteLmtEstCrvByBnkAgl @116 :Float32;
    dteLmtReqCrvByBnkAgl @117 :Float32;
    dteLmtReqCrvByDstrb @118 :Float32;
    dteLmtReqDeltaByBnkAgl @119 :Float32;
    dteLmtReqDeltaByDstrb @120 :Float32;
    dteLmtVehVelX @121 :Float32;
    dteMaxCrvByBnkAgl @122 :Float32;
    dteMaxDeltaByBnkAgl @123 :Float32;
    dteMaxReqCrvByDstrb @124 :Float32;
    dteMaxReqDeltaByDstrb @125 :Float32;
    dteNdlySetCrvLaDmc @126 :Float32;
    dteNdlySetDeltaLaDmc @127 :Float32;
    dtePsi2DotVdyFcn @128 :Float32;
    dtePsi3DotVdyFcn @129 :Float32;
    dteRawCrvLaDmc @130 :Float32;
    dteRawDeltaDotLaDmc @131 :Float32;
    dteRawDeltaFDotPar @132 :Float32;
    dteRawDeltaFDotRte @133 :Float32;
    dteRawFltEstCrvByBnkAgl @134 :Float32;
    dteRawLmtEstCrvByBnkAgl @135 :Float32;
    dteRawReqCrvByBnkAgl @136 :Float32;
    dteRawReqCrvByDstrb @137 :Float32;
    dteRawReqDeltaByBnkAgl @138 :Float32;
    dteRawReqDeltaByDstrb @139 :Float32;
    dteReqCrvByBnkAgl @140 :Float32;
    dteReqCrvByDstrb @141 :Float32;
    dteReqDeltaByBnkAgl @142 :Float32;
    dteReqDeltaByDstrb @143 :Float32;
    dteResCrvDenLaDmc @144 :Float32;
    dteResDeltaDenLaDmc @145 :Float32;
    dteResDeltaDenPar @146 :Float32;
    dteResDeltaDenRte @147 :Float32;
    dteResDeltaDenVdyFcn @148 :Float32;
    dteSetCrv2DotLaDmc @149 :Float32;
    dteSetCrvGainLaDmc @150 :Float32;
    dteSetCrvLaDmc @151 :Float32;
    dteSetDelta3DotLaDmc @152 :Float32;
    dteSetDeltaGainLaDmc @153 :Float32;
    dteSetDeltaLaDmc @154 :Float32;
    estAngleCurSteer @155 :Float32;
    estAngleLaDmcSteer @156 :Float32;
    estAnglePObsDTheta @157 :Float32;
    estAnglePObsDThetaFreeze @158 :Float32;
    estAnglePObsDThetaLmt0 @159 :Float32;
    estAnglePObsDThetaLmt0Raw @160 :Float32;
    estAnglePObsDThetaLmt1 @161 :Float32;
    estAnglePObsDThetaLmt2 @162 :Float32;
    estAnglePObsDThetaSat @163 :Float32;
    estAnglePObsDThetaSel @164 :Float32;
    estAnglePObsDThetaThd @165 :Float32;
    estAnglePObsDThetaThd0 @166 :Float32;
    estAngleVdySteer @167 :Float32;
    estBetaDotPobs @168 :Float32;
    estBetaDotSObs @169 :Float32;
    estBetaSObs @170 :Float32;
    estBtfQualifierByBeta @171 :UInt16;
    estBtfQualifierByEna @172 :UInt16;
    estBtfQualifierByHdr @173 :UInt16;
    estCoeffA11StateSpace @174 :Float32;
    estCoeffA12StateSpace @175 :Float32;
    estCoeffA21StateSpace @176 :Float32;
    estCoeffA22StateSpace @177 :Float32;
    estCoeffB11StateSpace @178 :Float32;
    estCoeffB21StateSpace @179 :Float32;
    estCoeffL11Pobs @180 :Float32;
    estCoeffL11Sobs @181 :Float32;
    estCoeffL12Pobs @182 :Float32;
    estCoeffL13Pobs @183 :Float32;
    estCoeffL21Pobs @184 :Float32;
    estCoeffL21Sobs @185 :Float32;
    estCoeffL22Pobs @186 :Float32;
    estCoeffL23Pobs @187 :Float32;
    estCoeffL31Pobs @188 :Float32;
    estCoeffL32Pobs @189 :Float32;
    estCoeffL33Pobs @190 :Float32;
    estCoeffL41Pobs @191 :Float32;
    estCoeffL42Pobs @192 :Float32;
    estCoeffL43Pobs @193 :Float32;
    estCrvPiObsCrvFlt @194 :Float32;
    estCrvPlObsIn @195 :Float32;
    estCurSteerAngle @196 :Float32;
    estDThetaDotPobs @197 :Float32;
    estDyDotPobs @198 :Float32;
    estDeltaYPlObsIn @199 :Float32;
    estDistFromCgToGud @200 :Float32;
    estDistPObsDy @201 :Float32;
    estDistPObsDyFreeze @202 :Float32;
    estDistPObsDyGrdnt @203 :Float32;
    estDistPObsDyGrdntThd @204 :Float32;
    estDistPObsDySat @205 :Float32;
    estDistPObsDySel @206 :Float32;
    estDistPObsDyThd @207 :Float32;
    estDistPobsDyGrdntRaw @208 :Float32;
    estDistYDevByGrdntLmt1 @209 :Float32;
    estDistYDevStep @210 :Float32;
    estDistYDevTrajFromCur @211 :Float32;
    estDlyCurSteerAngle @212 :Float32;
    estEnaActvtGrdntLmt1 @213 :Bool;
    estEnaActvtGrdntLmt2 @214 :Bool;
    estEnaBetaSatSObs @215 :Bool;
    estEnaByMeanHdr @216 :Bool;
    estEnaByMulHdrPerc @217 :Bool;
    estEnaCntrlByTgq @218 :Bool;
    estEnaFreezeByTgq @219 :Bool;
    estEnaLmt2ByDistY @220 :Bool;
    estEnaLmtByDistY @221 :Bool;
    estEnaPObsDThetaLmt0 @222 :Bool;
    estEnaPObsDThetaLmt1 @223 :Bool;
    estEnaPObsDThetaLmt2 @224 :Bool;
    estEnaPObsDThetaRst1 @225 :Bool;
    estEnaPObsDThetaRst2 @226 :Bool;
    estEnaPObsDThetaSat @227 :Bool;
    estEnaPObsDyGrdnt @228 :Bool;
    estEnaPObsDyGrdntRaw @229 :Bool;
    estEnaPObsDySat @230 :Bool;
    estEnaResetByTgq @231 :Bool;
    estErrVehYawRate @232 :Float32;
    estEstBetaPobs @233 :Float32;
    estEstBetaSObs @234 :Float32;
    estEstDThetaPobs @235 :Float32;
    estEstDyPobs @236 :Float32;
    estEstPsiDotPobs @237 :Float32;
    estEstPsiDotSObs @238 :Float32;
    estFacDThetaWghtHdrSel @239 :Float32;
    estFacDyWghtHdrSel @240 :Float32;
    estFltDThetaDotPObs @241 :Float32;
    estFltDyDotPObs @242 :Float32;
    estHdrPercByDy @243 :Float32;
    estHdrPercByTheta @244 :Float32;
    estHldBetaSObs @245 :Float32;
    estLmtBetaSObs @246 :Float32;
    estLmtHdrPercByDy @247 :Float32;
    estLmtHdrPercByTheta @248 :Float32;
    estLmtVehVelX @249 :Float32;
    estMeanHdrPerc @250 :Float32;
    estModeSelParHdr @251 :UInt8;
    estMulHdrPerc @252 :Float32;
    estPsi2DotPobs @253 :Float32;
    estPsi2DotSObs @254 :Float32;
    estRatioSteerGear @255 :Float32;
    estRawBetaSObs @256 :Float32;
    estRawEstBetaPobs @257 :Float32;
    estRawEstDThetaPobs @258 :Float32;
    estRawEstDyPobs @259 :Float32;
    estRawEstPsiDotPobs @260 :Float32;
    estRawFltDThetaDotPObs @261 :Float32;
    estRawFltDyDotPObs @262 :Float32;
    estRawHdrPercByDy @263 :Float32;
    estRawHdrPercByTheta @264 :Float32;
    estThdBetaSatSObs @265 :Float32;
    estThdMeanHdrSel @266 :Float32;
    estThdMulHdrSel @267 :Float32;
    ffcHldReqFfcCrv @268 :Float32;
    ffcReqFfcCrv @269 :Float32;
    lgcActiveLgcParamSetNu @270 :UInt8;
    lgcCacIntResetNu @271 :Bool;
    lgcCacPt1ResetNu @272 :Bool;
    lgcCdcCmdRad @273 :Float32;
    lgcCmpn2DotLaDmcCas @274 :Float32;
    lgcCmpn2DotLaDmcCdc @275 :Float32;
    lgcCmpn2DotLaDmcLdc @276 :Float32;
    lgcCmpnDotLaDmcCas @277 :Float32;
    lgcCmpnDotLaDmcCdc @278 :Float32;
    lgcCmpnDotLaDmcLdc @279 :Float32;
    lgcCmpnLaDmcCas @280 :Float32;
    lgcCmpnLaDmcCdc @281 :Float32;
    lgcCmpnLaDmcLdc @282 :Float32;
    lgcCoeffDGainCac @283 :Float32;
    lgcCoeffDGainLcCac @284 :Float32;
    lgcCoeffDGainLcLdc @285 :Float32;
    lgcCoeffDGainLdc @286 :Float32;
    lgcCoeffDGainOfCac @287 :Float32;
    lgcCoeffDGainOfLdc @288 :Float32;
    lgcCoeffDGainSfCac @289 :Float32;
    lgcCoeffDGainSfLdc @290 :Float32;
    lgcCoeffIGainCac @291 :Float32;
    lgcCoeffIGainLcCac @292 :Float32;
    lgcCoeffIGainLcLdc @293 :Float32;
    lgcCoeffIGainLdc @294 :Float32;
    lgcCoeffIGainOfCac @295 :Float32;
    lgcCoeffIGainOfLdc @296 :Float32;
    lgcCoeffIGainSfCac @297 :Float32;
    lgcCoeffIGainSfLdc @298 :Float32;
    lgcCoeffMainPGainCac @299 :Float32;
    lgcCoeffMainPGainLdc @300 :Float32;
    lgcCoeffNumS0LaDmc @301 :Float32;
    lgcCoeffNumS1LaDmc @302 :Float32;
    lgcCoeffPGainByCrvCac @303 :Float32;
    lgcCoeffPGainByCrvLdc @304 :Float32;
    lgcCoeffPGainCac @305 :Float32;
    lgcCoeffPGainLcCac @306 :Float32;
    lgcCoeffPGainLcLdc @307 :Float32;
    lgcCoeffPGainLdc @308 :Float32;
    lgcCoeffPGainOfCac @309 :Float32;
    lgcCoeffPGainOfLdc @310 :Float32;
    lgcCoeffPGainSfCac @311 :Float32;
    lgcCoeffPGainSfLdc @312 :Float32;
    lgcCoeffPt1GainCac @313 :Float32;
    lgcCoeffPt1GainLcCac @314 :Float32;
    lgcCoeffPt1GainLcLdc @315 :Float32;
    lgcCoeffPt1GainLdc @316 :Float32;
    lgcCoeffPt1GainOfCac @317 :Float32;
    lgcCoeffPt1GainOfLdc @318 :Float32;
    lgcCoeffPt1GainSfCac @319 :Float32;
    lgcCoeffPt1GainSfLdc @320 :Float32;
    lgcCoeffPole1LaDmc @321 :Float32;
    lgcCoeffPole2LaDmc @322 :Float32;
    lgcCrvReqBac1pm @323 :Float32;
    lgcCrvReqDte1pm @324 :Float32;
    lgcCrvReqFfcFrz1pm @325 :Float32;
    lgcCrvReqFfcGrdLimT11pm @326 :Float32;
    lgcCrvReqFfcGrdLimT21pm @327 :Float32;
    lgcCrvReqFfcGrdLim1pm @328 :Float32;
    lgcCtrlCrv1pm @329 :Float32;
    lgcCtrlCrvDe1pm @330 :Float32;
    lgcCtrlErrHeadAglCrtdRad @331 :Float32;
    lgcCtrlErrMainPGain @332 :Float32;
    lgcCtrlErrMainPGainCas @333 :Float32;
    lgcCtrlErrMainPGainCdc @334 :Float32;
    lgcDeltaByBnkAglCompDeg @335 :Float32;
    lgcDeltaFbacDeg @336 :Float32;
    lgcDeltaFCmdCdc @337 :Float32;
    lgcDeltaFCmdDcDeg @338 :Float32;
    lgcDeltaFCmdFfcDeg @339 :Float32;
    lgcDeltaFCmdUnlimitedDeg @340 :Float32;
    lgcDeltaFCmdDeg @341 :Float32;
    lgcDeltaFCmdRad @342 :Float32;
    lgcDeltaFdGainCas @343 :Float32;
    lgcDeltaFdGainCdc @344 :Float32;
    lgcDeltaFdGainLdc @345 :Float32;
    lgcDeltaFiGainCas @346 :Float32;
    lgcDeltaFiGainCdc @347 :Float32;
    lgcDeltaFiGainLdc @348 :Float32;
    lgcDeltaFpGainCas @349 :Float32;
    lgcDeltaFpGainCdc @350 :Float32;
    lgcDeltaFpGainLdc @351 :Float32;
    lgcDeltaFpt1GainCas @352 :Float32;
    lgcDeltaFpt1GainCdc @353 :Float32;
    lgcDeltaFpt1GainLdc @354 :Float32;
    lgcEnaActObjFollow @355 :UInt8;
    lgcEnaActSafetyFcn @356 :UInt8;
    lgcEnaCntrlByTgq @357 :Bool;
    lgcEnaFreezeByTgq @358 :Bool;
    lgcEnaModeChangeCas @359 :Bool;
    lgcEnaModeChangeCdc @360 :Bool;
    lgcEnaModeChangeDtct @361 :Bool;
    lgcEnaPGainGrdLmtCas @362 :Bool;
    lgcEnaPGainGrdLmtCdc @363 :Bool;
    lgcEnaPGainGrdLmtLdc @364 :Bool;
    lgcEnaPGainGrdSignCas @365 :Int8;
    lgcEnaPGainGrdSignCdc @366 :Int8;
    lgcEnaPGainGrdSignLdc @367 :Float32;
    lgcEnaPGainGrdThdCas @368 :Bool;
    lgcEnaPGainGrdThdCdc @369 :Bool;
    lgcEnaPGainGrdThdLdc @370 :Bool;
    lgcEnaResetByTgq @371 :Bool;
    lgcEnaRstByDistY @372 :Bool;
    lgcEnaRstByStandStill @373 :Bool;
    lgcEnaRstByTrq @374 :Bool;
    lgcEnaRstIntCac @375 :Bool;
    lgcEnaRstIntLdc @376 :Bool;
    lgcEnaRstPt1Cac @377 :Bool;
    lgcEnaRstPt1Ldc @378 :Bool;
    lgcEnableCtrlNu @379 :Bool;
    lgcErrCourse2DotCas @380 :Float32;
    lgcErrCourse2DotCdc @381 :Float32;
    lgcErrCourseDotCas @382 :Float32;
    lgcErrCourseDotCdc @383 :Float32;
    lgcErrCtrlCourseCas @384 :Float32;
    lgcErrCtrlCourseCdc @385 :Float32;
    lgcErrCtrlDistY @386 :Float32;
    lgcErrDistY2Dot @387 :Float32;
    lgcErrDistYDot @388 :Float32;
    lgcFfCrv1pm @389 :Float32;
    lgcFltErrCourseCas @390 :Float32;
    lgcFltErrCourseCdc @391 :Float32;
    lgcFltErrCourseDotCas @392 :Float32;
    lgcFltErrCourseDotCdc @393 :Float32;
    lgcFltPt1YErrMet @394 :Float32;
    lgcFltRawErrDistYDot @395 :Float32;
    lgcHldReqDeltaF @396 :Float32;
    lgcHoldNu @397 :Bool;
    lgcLdcAloneICmdRad @398 :Float32;
    lgcLdcCmdRad @399 :Float32;
    lgcLdcIntResetNu @400 :Bool;
    lgcLdcPt1ResetNu @401 :Bool;
    lgcLmtCoeffPGainCas @402 :Float32;
    lgcLmtCoeffPGainCdc @403 :Float32;
    lgcLmtCoeffPGainLdc @404 :Float32;
    lgcLmtReqDeltaF @405 :Float32;
    lgcLmtSelReqDeltaF @406 :Float32;
    lgcMaxReqDeltaF @407 :Float32;
    lgcRawCmpn2DotLaDmcCas @408 :Float32;
    lgcRawCmpn2DotLaDmcCdc @409 :Float32;
    lgcRawCmpn2DotLaDmcLdc @410 :Float32;
    lgcRawErrCourseDotCas @411 :Float32;
    lgcRawErrCourseDotCdc @412 :Float32;
    lgcRawErrDistYDot @413 :Float32;
    lgcRawFfcDeltaF @414 :Float32;
    lgcRawFltErrCourseCas @415 :Float32;
    lgcRawFltErrCourseCdc @416 :Float32;
    lgcRawFltErrCtrlDistY @417 :Float32;
    lgcRawLmtCoeffPGainCas @418 :Float32;
    lgcRawLmtCoeffPGainCdc @419 :Float32;
    lgcRawLmtCoeffPGainLdc @420 :Float32;
    lgcReqDeltaF @421 :Float32;
    lgcResetNu @422 :Bool;
    lgcSafetyFunctionActiveNu @423 :UInt8;
    lgcStActParSet @424 :UInt8;
    lgcSumCrvReqFbFrz1pm @425 :Float32;
    lgcSumCrvReqFbGrdLim1pm @426 :Float32;
    lgcSumCrvReqFbSatLim1pm @427 :Float32;
    lgcSumCrvReqFb1pm @428 :Float32;
    lgcTgtCrvDeNoLatSlp1pm @429 :Float32;
    lgcTgtCrvDe1pm @430 :Float32;
    lgcTgtCrvNoDe1pm @431 :Float32;
    lgcTimeDt1Cac @432 :Float32;
    lgcTimeDt1LcCac @433 :Float32;
    lgcTimeDt1LcLdc @434 :Float32;
    lgcTimeDt1Ldc @435 :Float32;
    lgcTimeDt1OfCac @436 :Float32;
    lgcTimeDt1OfLdc @437 :Float32;
    lgcTimeDt1SfCac @438 :Float32;
    lgcTimeDt1SfLdc @439 :Float32;
    lgcTimeFltErrCourse @440 :Float32;
    lgcTimeFltErrCourseCdc @441 :Float32;
    lgcTimeFltErrDistY @442 :Float32;
    lgcTimePt1Cac @443 :Float32;
    lgcTimePt1DeltaFCmd @444 :Float32;
    lgcTimePt1LcCac @445 :Float32;
    lgcTimePt1LcLdc @446 :Float32;
    lgcTimePt1Ldc @447 :Float32;
    lgcTimePt1OfCac @448 :Float32;
    lgcTimePt1OfLdc @449 :Float32;
    lgcTimePt1SfCac @450 :Float32;
    lgcTimePt1SfLdc @451 :Float32;
    tctiActualTrqEps @452 :Float32;
    tctiNegReqTrajHeadTpl @453 :Float32;
    tctiRoadBankAngle @454 :Float32;
    tcEnaUnplauUnitDelayBool @455 :Bool;
    tcFreeze1RsFlipFlopBool @456 :Bool;
    tcFreeze2RsFlipFlopBool @457 :Bool;
    tcFreezeRsFlipFlopBool @458 :Bool;
    tcHoldWarnRsFlipFlopBool @459 :Bool;
    lqrDeltaFCmdRad @460 :Float32;
    lqrDeltaFFeedbackRad @461 :Float32;
    lqrDeltaFFeedforwardRad @462 :Float32;
    lqrEnaCntrlByTgq @463 :Bool;
    lqrEnaFreezeByTgq @464 :Bool;
    lqrEnaResetByTgq @465 :Bool;
    lqrITermRad @466 :Float32;
    lqrMatKK1 @467 :Float32;
    lqrMatKK2 @468 :Float32;
    lqrMatKK3 @469 :Float32;
    lqrMatKK4 @470 :Float32;
    lqrReqDeltaFLimitDeg @471 :Float32;
    lqrE1Contribution @472 :Float32;
    lqrE1dotContribution @473 :Float32;
    lqrE2Contribution @474 :Float32;
    lqrE2dotContribution @475 :Float32;
    lqrHeadingError @476 :Float32;
    lqrHeadingErrorRate @477 :Float32;
    lqrLateralError @478 :Float32;
    lqrLateralErrorRate @479 :Float32;
    lqrNumIteration @480 :Float32;
    lqrYawrateTerm @481 :Float32;
    lqrDeltaFLeadCmdRad @482 :Float32;
}

struct LcfVehTpDebug {
    pCalcEnableDebug @0 :TpEnableDebug;
    pFrentTransfDebug @1 :TpFrenetTransfDebug;
    pTrajCalcDebug @2 :TpTrajecotryCalcDebug;
    pFrenetBackDebug @3 :TpFrenetBackDebug;
}

struct LcfVehTpInput {
    fEgoVelXMps @0 :Float32;
    fEgoAccelXMps2 @1 :Float32;
    fEgoCurve1pm @2 :Float32;
    fKappaSumCommand1pm @3 :Float32;
    fCycleTimeVehSec @4 :Float32;
    fEpsManualTrqActValNm @5 :Float32;
    bSysStOffLatDmc @6 :Bool;
    bSysStReqLatDmc @7 :Bool;
    uiOdometerStateNu @8 :UInt8;
    fEgoYawRateRps @9 :Float32;
    uiVehSync4LcoUs @10 :UInt32;
    uiSenToVehTStampUs @11 :UInt32;
    fPlanningHorizonSec @12 :Float32;
    uiSysStateLcfNu @13 :UInt8;
    fPredTimeCurveSec @14 :Float32;
    fPredTimeHeadAngSec @15 :Float32;
    bTriggerReplan @16 :Bool;
    fLeCridBndPosX0Met @17 :Float32;
    fLeCridBndPosY0Met @18 :Float32;
    fLeCridBndHeadAngRad @19 :Float32;
    fLeCridBndCrv1pm @20 :Float32;
    fLeCridBndCrvChng1pm2 @21 :Float32;
    fLeCridBndLengthMet @22 :Float32;
    fRiCridBndPosX0Met @23 :Float32;
    fRiCridBndPosY0Met @24 :Float32;
    fRiCridBndHeadAngRad @25 :Float32;
    fRiCridBndCrv1pm @26 :Float32;
    fRiCridBndCrvChng1pm2 @27 :Float32;
    fRiCridBndLengthMet @28 :Float32;
    fTgtTrajPosX0Met @29 :Float32;
    fTgtTrajPosY0Met @30 :Float32;
    fTgtTrajHeadingAngRad @31 :Float32;
    fTgtTrajCurve1pm @32 :Float32;
    fTgtTrajCrvChng1pm2 @33 :Float32;
    fTgtTrajLengthMet @34 :Float32;
    bLatencyCompActivated @35 :Bool;
    fSensorTimeStampSec @36 :Float32;
    uiTrajPlanServQuNu @37 :UInt8;
    fWeightTgtDistYNu @38 :Float32;
    fWeightEndTimeNu @39 :Float32;
    fDistYToLeTgtAreaMet @40 :Float32;
    fDistYToRiTgtAreaMet @41 :Float32;
    fFTireAclMaxMps2 @42 :Float32;
    fFTireAclMinMps2 @43 :Float32;
    fObstacleVelXMps @44 :Float32;
    fObstacleAccelXMps2 @45 :Float32;
    fObstacleWidthMet @46 :Float32;
    fObstacleDistXMet @47 :Float32;
    fObstacleDistYMet @48 :Float32;
    fMaxJerkAllowedMps3 @49 :Float32;
    uiTrajGuiQualifierNu @50 :UInt8;
    uiLeLnQualityPer @51 :UInt8;
    uiRiLnQualityPer @52 :UInt8;
    uiLeCrvQualityPer @53 :UInt8;
    uiRiCrvQualityPer @54 :UInt8;
    uiLatCtrlModeNu @55 :UInt8;
    uiControllingFunctionNu @56 :UInt8;
}

struct LcfVehTp {
    tpInput @0 :LcfVehTpInput;
    tpOutput @1 :LcfVehTpOutput;
    tpDebug @2 :LcfVehTpDebug;
}

struct LcfVehTpOutput {
    bReplanCurValues @0 :Bool;
    uiTrajGuiQualifierNu @1 :UInt8;
    fCurDistYMet @2 :Float32;
    fTrajDistYMet @3 :Float32;
    fTrajTgtCurve1pm @4 :Float32;
    fCurHeadingRad @5 :Float32;
    fTrajHeadInclPrevRad @6 :Float32;
    fTrajHeadingRad @7 :Float32;
    fTrajTgtCrvGrd1pms @8 :Float32;
    fTrajHeadingPrevRad @9 :Float32;
    fTrajTgtCrvPrev1pm @10 :Float32;
    fCtrlErrDistYMet @11 :Float32;
    fCtrlErrHeadingAngleRad @12 :Float32;
    fCtrlErrHeadAglPrevRad @13 :Float32;
    fTrajDistYPrevMet @14 :Float32;
    fDeltaTargetCrv1pm @15 :Float32;
    fDeltaTargetPosY0Met @16 :Float32;
    fDeltaTargetHeadingRad @17 :Float32;
    bUseTargetCorridor @18 :Bool;
    bTargetSwitch @19 :Bool;
    bGradLimitActive @20 :Bool;
    bPlausiCheckStatus @21 :Bool;
    uiSQuStatusTrajPlanNu @22 :UInt16;
    fTrajTgtCrvGrdPrev1pms @23 :Float32;
    uiDQuStatusTrajPlanNu @24 :UInt16;
}

struct LegacyAolT {
    fDistToRef @0 :Float32;
}

struct LegacyObjT {
    fMaxAccelY @0 :Float32;
    fRcsTargetThresholdUncomp @1 :Float32;
    uiLifeTime @2 :UInt16;
    eDynamicSubProperty @3 :UInt8;
}

struct LegacyVehDynT {
    yawRateMaxJitter @0 :Float32;
    bStandStill @1 :Bool;
}

struct LongCtrlInputCustomInT {
    avlcReversibleFail @0 :Bool;
    avlcIrreversibleFail @1 :Bool;
    cameraAvailability @2 :Bool;
    escActive @3 :Bool;
    absActive @4 :Bool;
    aebActive @5 :Bool;
    apaActive @6 :Bool;
    aycActive @7 :Bool;
    tcsActive @8 :Bool;
    epbActive @9 :Bool;
    hdcActive @10 :Bool;
    usrActive @11 :Bool;
    nnpActive @12 :Bool;
    isaActive @13 :Bool;
    doorOpen @14 :Bool;
    tsrInfo @15 :TsrInfo;
    navInfo @16 :NavInfo;
}

struct LongCtrlInputCustomT {
    longCtrlInputCustom @0 :LongCtrlInputCustomInT;
}

struct LongCtrlInputT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    kinCtrlDynInput @2 :KinCtrlDynInputT;
    displayOutput @3 :KinDisplayInputT;
    custom @4 :LongCtrlInputCustomT;
}

struct LongCtrlOutputCustomT {
    customOutput @0 :CustomOutputT;
}

struct LongitudinalT {
    motVar @0 :MotVarVehDynT;
    veloCorr @1 :VeloCorrVehDynT;
    accelCorr @2 :AccelCorrVehDynT;
}

struct LsdEnvironmentDataT {
    sSigHeader @0 :SignalHeaderT;
    blockage @1 :UInt32;
    brightness @2 :UInt32;
    weather @3 :UInt32;
    scenario @4 :UInt32;
    cityLight @5 :UInt32;
}

struct LsdLightObjectAttributesT {
    classConfidence @0 :Float32;
    objectType @1 :UInt16;
    status @2 :UInt16;
    vehicleOncomingProbability @3 :UInt8;
    vehiclePrecedingProbability @4 :UInt8;
}

struct LsdLightObjectGeneralT {
    objectId @0 :UInt8;
    objectIdPaired @1 :UInt8;
    objectcyclesTraced @2 :UInt16;
}

struct LsdLightObjectKinematicT {
    world @0 :LsdLightObjectKinematicWorldT;
}

struct LsdLightObjectKinematicWorldT {
    x @0 :Float32;
    y @1 :Float32;
    z @2 :Float32;
    distance @3 :Float32;
    leftAngle @4 :Float32;
    rightAngle @5 :Float32;
    topAngle @6 :Float32;
    bottomAngle @7 :Float32;
}

struct LsdLightObjectListT {
    sSigHeader @0 :SignalHeaderT;
    numObjects @1 :UInt32;
    lightObjects @2 :List(LsdLightObjectT);
}

struct LsdLightObjectT {
    general @0 :LsdLightObjectGeneralT;
    kinematic @1 :LsdLightObjectKinematicT;
    attributes @2 :LsdLightObjectAttributesT;
}

struct MotionStateVehDynT {
    motState @0 :UInt8;
    confidence @1 :Float32;
}

struct MotVarVehDynT {
    velocity @0 :Float32;
    accel @1 :Float32;
    varVelocity @2 :Float32;
    varAccel @3 :Float32;
}

struct NavInfo {
    navSpeedLimit @0 :UInt8;
    navActive @1 :Bool;
}

struct ObjectT {
    relLongSpeed @0 :Int16;
    relLatSpeed @1 :Int16;
    longDisplacement @2 :Int16;
    latDisplacement @3 :Int16;
    relLongAccel @4 :Int16;
    relLatAccel @5 :Int16;
    quality @6 :UInt8;
    class @7 :UInt8;
    objectStatus @8 :ObjStatusT;
    width @9 :Int16;
    objectId @10 :Int8;
}

struct ObjOfInterestT {
    cExternalId @0 :UInt8;
    eObjOoi @1 :Int8;
}

struct ObjStatusT {
    measured @0 :Bool;
    tracked @1 :Bool;
    new @2 :Bool;
    standing @3 :Bool;
    stopped @4 :Bool;
    moving @5 :Bool;
    detected @6 :Bool;
}

struct OdprDebugT {
    sFopDebug @0 :OdprfopDebugT;
    sFohDebug @1 :OdprfohDebugT;
}

struct OdprfohDebugT {
    fYawRateObjSyncRps @0 :Float32;
    bAccObjValidBool @1 :UInt8;
    bAccObjFreezeStopBool @2 :UInt8;
    bObjCutOutBool @3 :UInt8;
    bObjCutInBool @4 :UInt8;
    bLsmInactiveBool @5 :UInt8;
    bTrajInvalidBool @6 :UInt8;
    bTempAccObjDtctChkBool @7 :UInt8;
    bTempPreHistValidChkBool @8 :UInt8;
    bTempPreHistValidTurnOnDelayBool @9 :UInt8;
    fTempMaxObjDistLsmMet @10 :Float32;
    bTempAccObjPosXHystBool @11 :UInt8;
    bSaveNewEntryBool @12 :UInt8;
    bResetHistoryBool @13 :UInt8;
    bEnableHistoryBool @14 :UInt8;
    fTempPosXDiffMet @15 :Float32;
    fTempPosXFromPosMet @16 :Float32;
    fTempPosXFromVelMet @17 :Float32;
    bTempPosXFromPosChkBool @18 :UInt8;
    bTempPosXFromVelChkBool @19 :UInt8;
    bTempPosXChkBool @20 :UInt8;
    bTransitionEnableBool @21 :UInt8;
    bTransitionResetBool @22 :UInt8;
    bCutInOngoingBool @23 :UInt8;
    bCutOutOngoingBool @24 :UInt8;
    bFreezeStopOngingBool @25 :UInt8;
    bObjValidOngoingBool @26 :UInt8;
    fTransitionFactorAFac @27 :Float32;
    fAccObjPosYTransPfMet @28 :Float32;
    fAccObjPosYTransMet @29 :Float32;
    fAccObjRelYawAngTransRad @30 :Float32;
    bCutInDetectedBool @31 :UInt8;
    bCutOutDetectedBool @32 :UInt8;
    bAccObjFreezeStop2Bool @33 :UInt8;
    bObjValidityBool @34 :UInt8;
    bAccObjValidRisingChkBool @35 :UInt8;
    fTempPosYCorrMet @36 :Float32;
    fTempPosYCorrBefMet @37 :Float32;
    fTempPosYCorrAftMet @38 :Float32;
    fTempPosYpt1PfMet @39 :Float32;
    fTempPosYpt1LsmMet @40 :Float32;
    bTempTransAllBool @41 :UInt8;
    bTempTransPfBool @42 :UInt8;
    fTempFactorAFct @43 :Float32;
    fTempTransPosYpt1PfMet @44 :Float32;
    fTempTransPosYpt1LsmMet @45 :Float32;
    fTempTransYawAngMet @46 :Float32;
    fTempPosYpt1LowpassTSec @47 :Float32;
    fTempPosYlsmLowpassTSec @48 :Float32;
    bTrajInvalid1stBool @49 :UInt8;
    bTrajInvalid3rdBool @50 :UInt8;
    fPosY01stMet @51 :Float32;
    fPosY03rdMet @52 :Float32;
    fHeading1stRad @53 :Float32;
    fHeading3rdRad @54 :Float32;
    fCrv1pm @55 :Float32;
    fTempFeatPtsMinPosXMet @56 :Float32;
    fTempAlphaFct @57 :Float32;
    fTempYawRateRps @58 :Float32;
    fTempYawRateOutRps @59 :Float32;
    fPolySelecWeightFac @60 :Float32;
    fPolySelecPosY03rdMet @61 :Float32;
    fPolySelecHeading3rdRad @62 :Float32;
    fPolySelecCrv1pm @63 :Float32;
    fStrightProb1Perc @64 :Float32;
    fStrightProb2Perc @65 :Float32;
    fStrightProb3Perc @66 :Float32;
    fTraceQual1Perc @67 :Float32;
    fTraceQual2Perc @68 :Float32;
    fTraceQual3Perc @69 :Float32;
    fTraceQual4Perc @70 :Float32;
    fTraceQual5Perc @71 :Float32;
    fTempEgoPosXMet @72 :Float32;
    fTempPosYCrvMet @73 :Float32;
    fTempPosYAbsMet @74 :Float32;
    uiTempStrightProbPerc @75 :UInt8;
    fTempMeanSampleAgeSec @76 :Float32;
    fTempMeanDevToTraj3rdMet @77 :Float32;
    fTempNumValidSamplesNu @78 :Float32;
    fTempVelXMaxMps @79 :Float32;
    fTempHistLengthMet @80 :Float32;
    fTempTrajLengthMet @81 :Float32;
    uiTempTraceQualPerc @82 :UInt8;
    fStrgtFadCurvature1pm @83 :Float32;
    fStrgtFadPosY0Met @84 :Float32;
    fStrgtFadHeadingRad @85 :Float32;
    fTempFadingFac @86 :Float32;
    bLowPassResetFlgBool @87 :UInt8;
    bLowPassEnableFlgBool @88 :UInt8;
    fLowPassPosY0Met @89 :Float32;
    fLowPassHeadingRad @90 :Float32;
    fLowPassCurvature1pm @91 :Float32;
    fLowPassChngOfCrv1pm2 @92 :Float32;
    bTempTrajInvalidChkBool @93 :UInt8;
    fTempVehVelXKph @94 :Float32;
    fTempPosYLowpassTSec @95 :Float32;
    fTempHeadingLowpassTSec @96 :Float32;
    fTempCurveLowpassTSec @97 :Float32;
    fTempCrvChngLowpassTSec @98 :Float32;
    fLengthLsmMet @99 :Float32;
    fPosY0LsmMet @100 :Float32;
    fHeadLsmRad @101 :Float32;
    fCrvLsm1pm @102 :Float32;
    fTempC0CalculateLpm @103 :Float32;
    fTempMaxCrvLpm @104 :Float32;
    bModeTransitionEnableBool @105 :UInt8;
    bModeTransitionResetBool @106 :UInt8;
    bModeLsmTransOngoingBool @107 :UInt8;
    fModeTransitionValueAFac @108 :Float32;
    fTempPosY0Met @109 :Float32;
    fTempHeadingRad @110 :Float32;
    fTempTrajLength2Met @111 :Float32;
    fTempCrvLpm @112 :Float32;
    bTempLsmChangeFlagBool @113 :UInt8;
    bTempLsmIvdEdgeBool @114 :UInt8;
    vecY @115 :List(Float32);
    vecX @116 :List(Float32);
    weight @117 :List(Float32);
    vecAge @118 :List(Float32);
    fMaxSampleAgeSec @119 :Float32;
    bPolyfitEnBool @120 :UInt8;
    bStartPointValidBool @121 :UInt8;
    bLengthValidBool @122 :UInt8;
}

struct OdprfohOutT {
    fTgtObjPosX0Met @0 :Float32;
    fTgtObjPosY0Met @1 :Float32;
    fTgtObjHeadingRad @2 :Float32;
    fTgtObjCurve1pm @3 :Float32;
    fTgtObjCurveDer1pm2 @4 :Float32;
    fTgtObjLengthMet @5 :Float32;
    fAccObjTraceCurve1pm @6 :Float32;
    uiAccObjTraceCurveQualityPerc @7 :UInt8;
    fTgtObjClothoidInvalidCheckBtf @8 :UInt16;
    uiAccObjTraceQualityPerc @9 :UInt8;
    fLastStoredPointXMet @10 :Float32;
    fLastStoredPointYMet @11 :Float32;
    fMaxGapEgoToHistoryMet @12 :Float32;
    fMeanDevToTraj1stMet @13 :Float32;
    fMeanDevToTraj3rdMet @14 :Float32;
    fMinHistoryLengthMet @15 :Float32;
    fMinHistoryStartPosXMet @16 :Float32;
    uiNumOfValidSamplesNu @17 :UInt8;
    bAddNewSampleBool @18 :UInt8;
    uiObjTraceStraightProbPerc @19 :UInt8;
    fLastStoredPointAgeSec @20 :Float32;
    fFirstStoredPointAgeSec @21 :Float32;
    fFirstStoredPointXMet @22 :Float32;
    fMeanStoredPointAgeSec @23 :Float32;
}

struct OdprfopDebugT {
    bObjHysteresisAllowedBool @0 :UInt8;
    bNoObjIdSwitchDetectedBool @1 :UInt8;
    bObjPosYStepDetectedBool @2 :UInt8;
    bObjPosXStepDetectedBool @3 :UInt8;
    bResetObjValidityBool @4 :UInt8;
    fTempAccObjPosXMet @5 :Float32;
    fTempObjPosYMet @6 :Float32;
    bObjPosXChkBool @7 :UInt8;
    bObjPosYChkBool @8 :UInt8;
    bNoIdSwitchBool @9 :UInt8;
    bAccObjDetectedBool @10 :UInt8;
    bObjQualityValidBool @11 :UInt8;
    bObjMeasStateValidBool @12 :UInt8;
    bObjPosYValidBool @13 :UInt8;
    bObjPosXValidBool @14 :UInt8;
    bObjWidthValidBool @15 :UInt8;
    bObjRelVelXValidBool @16 :UInt8;
    bObjRelVelYValidBool @17 :UInt8;
    bObjRelAclXValidBool @18 :UInt8;
    bObjTypeValidBool @19 :UInt8;
    bObjFusionStateValidBool @20 :UInt8;
    bObjLatMovInvalidBool @21 :UInt8;
    bDistYOffsetChkBool @22 :UInt8;
    fMinObjPosYMet @23 :Float32;
    bFusionResetBool @24 :UInt8;
    bMeasStResetBool @25 :UInt8;
    bObjAndEgoStopBool @26 :UInt8;
    bBothStopBool @27 :UInt8;
    bLatMovResetBool @28 :UInt8;
    fOffsetObjPosYMet @29 :Float32;
    fMinCurvature1pm @30 :Float32;
    fDistYOffsetMet @31 :Float32;
    fTempObjPosXMet @32 :Float32;
    fTempLongDistMaxMet @33 :Float32;
    bTempPosXHystBool @34 :UInt8;
    fMinWidthTgtObjMet @35 :Float32;
    fMaxLongVelMet @36 :Float32;
    fMaxLatVelMet @37 :Float32;
    fMaxLongAclMet @38 :Float32;
    bTempObjTypeChkBool @39 :UInt8;
    bTempObjTypeChkWithHystBool @40 :UInt8;
    bTempFusionTurnOnBool @41 :UInt8;
    bTempMeasStTurnOnBool @42 :UInt8;
    fTempObjVelXMps @43 :Float32;
    bTempObjStandstillBool @44 :UInt8;
    bTempEgoStandstillBool @45 :UInt8;
    bPreCycObjDetectedBool @46 :UInt8;
    fTempPosXMet @47 :Float32;
    bTempResetBool @48 :UInt8;
    bTempEnableCalcBool @49 :UInt8;
    fPosYLowPassMet @50 :Float32;
    fTempVehYawAngRad @51 :Float32;
    bYawAngEnableSampleBool @52 :UInt8;
    fOwnCalculatedYawAngRad @53 :Float32;
    fTempYawAngleCalcRad @54 :Float32;
    bTempResetObjChngBool @55 :UInt8;
    fTempPosYInMet @56 :Float32;
    fTempYawAngleRad @57 :Float32;
}

struct OdprfopOutT {
    fAccObjRelAclXMps2 @0 :Float32;
    fAccObjRelVelXMps @1 :Float32;
    fAccObjRelAclYMps2 @2 :Float32;
    fAccObjRelVelYMps @3 :Float32;
    fAccObjPosXMet @4 :Float32;
    fAccObjPosYMet @5 :Float32;
    fAccObjPosXStdDevMet @6 :Float32;
    fAccObjPosYStdDevMet @7 :Float32;
    uiAccObjInvalidCheckBtf @8 :UInt16;
    fAccObjTimeStampSec @9 :Float32;
    fEstimateObjPosXMet @10 :Float32;
    fEstimateObjPosYMet @11 :Float32;
    fAccObjRelHeadingAngleRad @12 :Float32;
}

struct OdprInAbprLaneInfoT {
    bLaneChangeDetectedBool @0 :UInt8;
    uiLeftLaneInvalidCheckBtf @1 :UInt16;
    uiLeftLaneQualityPerc @2 :UInt8;
    fLeftLaneClothoidPosY0Met @3 :Float32;
    fLeftLaneClothoidHeadingRad @4 :Float32;
    fLeftLaneClothoidCurve1pm @5 :Float32;
    fLeftLaneClothoidCurveDer1pm2 @6 :Float32;
    fLeftLaneClothoidLengthMet @7 :Float32;
    uiRightLaneInvalidCheckBtf @8 :UInt16;
    uiRightLaneQualityPerc @9 :UInt8;
    fRightLaneClothoidPosY0Met @10 :Float32;
    fRightLaneClothoidHeadingRad @11 :Float32;
    fRightLaneClothoidCurve1pm @12 :Float32;
    fRightLaneClothoidCurveDer1pm2 @13 :Float32;
    fRightLaneClothoidLengthMet @14 :Float32;
}

struct OdprInAccFrObjT {
    bObjectDetectedBool @0 :UInt8;
    fObjRelAclXMps2 @1 :Float32;
    fObjRelVelXMps @2 :Float32;
    fObjRelAclYMps2 @3 :Float32;
    fObjRelVelYMps @4 :Float32;
    fObjPosXMet @5 :Float32;
    fObjPosYMet @6 :Float32;
    fObjPosXStdDevMet @7 :Float32;
    fObjPosYStdDevMet @8 :Float32;
    uiObjQualityPerc @9 :UInt8;
    uiObjClassTypeNu @10 :UInt8;
    uiObjMeasStateNu @11 :UInt8;
    fObjWidthMet @12 :Float32;
    uiObjSensorSourceBtf @13 :UInt16;
    uiObjTimeStampUsec @14 :UInt32;
    uiObjIdNu @15 :Int8;
    fObjRelHeadingAngleRad @16 :Float32;
}

struct OdprInReqT {
    sAccObject @0 :OdprInAccFrObjT;
    sEgoVehSig @1 :OdprInVedVehDynT;
    sSystemPara @2 :OdprInSystemParaT;
    sLaneData @3 :OdprInAbprLaneInfoT;
}

struct OdprInSystemParaT {
    fSystemCylceTimeSec @0 :Float32;
}

struct OdprInVedVehDynT {
    fEgoVelXMps @0 :Float32;
    fEgoCurve1pm @1 :Float32;
    fEgoYawRateRps @2 :Float32;
}

struct OdprOutProT {
    sFopOutData @0 :OdprfopOutT;
    sFohOutData @1 :OdprfohOutT;
}

struct OvertakeAssistInfo {
    overtakeAssistFlag @0 :Bool;
    targetTimeGap @1 :Float32;
    targetDistance @2 :Float32;
    headwaySetting @3 :Int16;
}

struct PaccInfo {
    paccState @0 :Bool;
    paccAcceleration @1 :Float32;
    paccRawAcceleration @2 :Float32;
    controlPath @3 :PathGeometry;
}

struct ParameterObjSelT {
    fVegoMin @0 :Float32;
    fVegoMax @1 :Float32;
    fVTargetMin @2 :Float32;
    fTargetRangeMax @3 :Float32;
    fXMinBreakThrough @4 :Float32;
    fXMaxBreakthrough @5 :Float32;
}

struct PathGeometry {
    lateralDeviation @0 :Float32;
    headingAngle @1 :Float32;
    curvature @2 :Float32;
    curvatureRate @3 :Float32;
    viewRange @4 :Float32;
}

struct QualifiersT {
    fProbabilityOfExistence @0 :Float32;
    ucObstacleProbability @1 :UInt8;
    uMeasuredTargetFrequencyNear @2 :UInt8;
    uMeasuredTargetFrequencyFar @3 :UInt8;
}

struct RegSenDebug {
    header @0 :StdHeader.Header;
    #HBA
    sHbaDebug @1 :HbaDebug;
    #TSR
    tsrInput @2 :TsrInput;
    tsrOutput @3 :TsrOutput;
    tsrDebug @4 :TsrDebug;
}

struct RegSenOutput {
    header @0 :StdHeader.Header;
    #IHBC
    hbaOutput @1 :HbaOutputT;
    #TSR
    tsrOutput @2 :TsrOutput;
}

struct SadOutCustDegadationStateT {
    bWarningsNotPossible @0 :Bool;
    bBrakePreCondNotPossible @1 :Bool;
    bPreBrakeNotPossible @2 :Bool;
    bPreCrashNotPossible @3 :Bool;
}

struct SadOutCustHaptWaningTypeT {
    eSignal @0 :UInt8;
    eFctChan @1 :UInt8;
    uActiveHyp @2 :UInt8;
    eWarnSens @3 :UInt8;
}

struct SadOutCustHypReactionsT {
    bLatentPreWarningActive @0 :Bool;
    bLatentAcuteWarningActive @1 :Bool;
    bDynamicPreWarningActive @2 :Bool;
    bDynamicAcuteWarningActive @3 :Bool;
    bDynamicHapticAcuteWarningActive @4 :Bool;
    bHbaActive @5 :Bool;
    bPreFillActive @6 :Bool;
    bPreBrakeActive @7 :Bool;
}

struct SadOutCustPrebrakeT {
    fPreBrakeDecel @0 :Float32;
    bPreBrakeDecelEnabled @1 :Bool;
    bPreBrakeStdstillRequest @2 :Bool;
    bPreBrakeCamConfirmed @3 :Bool;
    uPreBrakeLevel @4 :UInt8;
    bPreBrakeEmergency @5 :Bool;
    uActiveHyp @6 :UInt8;
    eFctChan @7 :UInt8;
}

struct SadOutCustPreCrashT {
    fPreCrashTtc @0 :Float32;
    fPreCrashCv @1 :Float32;
    bEmaActive @2 :Bool;
    eEbaFctChan @3 :UInt8;
    uActiveHyp @4 :UInt8;
}

struct SadOutCustPrefillT {
    bPrefillActive @0 :Bool;
    bPreRunActive @1 :Bool;
    eGeneratorControl @2 :UInt8;
    uActiveHyp @3 :UInt8;
    eFctChan @4 :UInt8;
}

struct SadOutCustVlcDataT {
    fVelocityFactor @0 :Float32;
    iObjRef @1 :Int8;
    fSafeObjDistance @2 :Float32;
}

struct SadOutCustVlcQualifierT {
    ePedFunctionQualifier @0 :UInt8;
    eVehFunctionQualifier @1 :UInt8;
    eUnclassifiedFunctionQualifier @2 :UInt8;
    ePreCrashFunctionQualifier @3 :UInt8;
}

struct SadOutCustWarningsT {
    sPreStaticWarning @0 :SadOutCustWarningTypeT;
    sAcuteStaticWarning @1 :SadOutCustWarningTypeT;
    sPreDynamicWarning @2 :SadOutCustWarningTypeT;
    sAcuteDynamicWarning @3 :SadOutCustWarningTypeT;
    sAcuteDynamicHaptWarning @4 :SadOutCustHaptWaningTypeT;
}

struct SadOutCustWarningTypeT {
    eSignal @0 :UInt8;
    eFctChan @1 :UInt8;
    uActiveHyp @2 :UInt8;
}

struct SadOutputCustomT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    sDegradationState @2 :SadOutCustDegadationStateT;
    sHba @3 :HeadOutCustHbaT;
    sPrefill @4 :SadOutCustPrefillT;
    sPreBrake @5 :SadOutCustPrebrakeT;
    sWarnings @6 :SadOutCustWarningsT;
    sPreCrash @7 :SadOutCustPreCrashT;
    rgHypReactions @8 :List(SadOutCustHypReactionsT);
    sVlcData @9 :SadOutCustVlcDataT;
    sVlcQualifiers @10 :SadOutCustVlcQualifierT;
    sThreatlvl @11 :HeadOutCustThreatLvlT;
    sTcuSysInfo @12 :TcuSysInfoT;
}

struct SAlpAdjLaneInfoSt {
    fPosY0Met @0 :Float32;
    fHeadingAngleRad @1 :Float32;
    fCurvature1pm @2 :Float32;
    fCurvatureRate1pm2 @3 :Float32;
    fValidLengthMet @4 :Float32;
    uQualityNu @5 :UInt8;
    uMarkerTypeNu @6 :UInt8;
    bAvailableBool @7 :Bool;
}

struct SAlpEgoLaneInfoSt {
    uEgoQualityBtf @0 :UInt16;
    fPosY0Met @1 :Float32;
    fHeadingAngleRad @2 :Float32;
    fCurvature1pm @3 :Float32;
    fCurvatureRate1pm2 @4 :Float32;
    fValidLengthMet @5 :Float32;
}

struct SAlpInReqSt {
    fEgoVelXMps @0 :Float32;
    bLaneChangeDetectedNu @1 :Bool;
    fLaneWidthMet @2 :Float32;
    fCycleTimeSec @3 :Float32;
    aAlpEgoLanes @4 :List(SAlpEgoLaneInfoSt);
    aAlpAdjLanes @5 :List(SAlpAdjLaneInfoSt);
}

struct SAlpOutputSt {
    bLeftAdjLnStepDeouncedBool @0 :Bool;
    bRightAdjLnStepDeouncedBool @1 :Bool;
    bLeftAdjLnBridgingBool @2 :Bool;
    bRightAdjLnBridgingBool @3 :Bool;
    bLeftAdjLnStepOnBridgingBool @4 :Bool;
    bRightAdjLnStepOnBridgingBool @5 :Bool;
    uiLeftAdjLnInvalidQuBtf @6 :UInt16;
    uiRightAdjLnInvalidQuBtf @7 :UInt16;
    uiLeftAdjLnAliveCountPer @8 :Float32;
    uiRightAdjLnAliveCountPer @9 :Float32;
    fLeftAdjLnDistanceMet @10 :Float32;
    fLeftAdjLnYawAngleRad @11 :Float32;
    fLeftAdjLnCurvature1pm @12 :Float32;
    fLeftAdjLnCrvRate1pm2 @13 :Float32;
    fLeftAdjLnValidLengthMet @14 :Float32;
    fRightAdjLnDistanceMet @15 :Float32;
    fRightAdjLnYawAngleRad @16 :Float32;
    fRightAdjLnCurvature1pm @17 :Float32;
    fRightAdjLnCrvRate1pm2 @18 :Float32;
    fRightAdjLnValidLengthMet @19 :Float32;
}

struct SCfvDebugT {
    bEnaStepDtctLf @0 :UInt8;
    fAbsDistYStepLf @1 :Float32;
    bRawInValidDistYStepLf @2 :UInt8;
    bDlyValidDistYStepLf @3 :UInt8;
    fTimerDistYStepValidLf @4 :Float32;
    bResetDistYStepLf @5 :UInt8;
    fAbsHeadingStepLf @6 :Float32;
    bRawInValidHeadingStepLf @7 :UInt8;
    bDlyValidHeadingStepLf @8 :UInt8;
    fTimerHeadingStepValidLf @9 :Float32;
    bResetHeadingStepLf @10 :UInt8;
    fAbsCrvStepLf @11 :Float32;
    bRawInValidCrvStepLf @12 :UInt8;
    bDlyValidCrvStepLf @13 :UInt8;
    fTimerCrvStepValidLf @14 :Float32;
    bResetCrvStepLf @15 :UInt8;
    fAbsCrvRateStepLf @16 :Float32;
    bRawInValidCrvRateStepLf @17 :UInt8;
    bDlyValidCrvRateStepLf @18 :UInt8;
    fTimerCrvRateStepValidLf @19 :Float32;
    bResetCrvRateStepLf @20 :UInt8;
    bEnaStepDtctCntr @21 :UInt8;
    fAbsDistYStepCntr @22 :Float32;
    bRawInValidDistYStepCntr @23 :UInt8;
    bDlyValidDistYStepCntr @24 :UInt8;
    fTimerDistYStepValidCntr @25 :Float32;
    bResetDistYStepCntr @26 :UInt8;
    fAbsHeadingStepCntr @27 :Float32;
    bRawInValidHeadingStepCntr @28 :UInt8;
    bDlyValidHeadingStepCntr @29 :UInt8;
    fTimerHeadingStepValidCntr @30 :Float32;
    bResetHeadingStepCntr @31 :UInt8;
    fAbsCrvStepCntr @32 :Float32;
    bRawInValidCrvStepCntr @33 :UInt8;
    bDlyValidCrvStepCntr @34 :UInt8;
    fTimerCrvStepValidCntr @35 :Float32;
    bResetCrvStepCntr @36 :UInt8;
    fAbsCrvRateStepCntr @37 :Float32;
    bRawInValidCrvRateStepCntr @38 :UInt8;
    bDlyValidCrvRateStepCntr @39 :UInt8;
    fTimerCrvRateStepValidCntr @40 :Float32;
    bResetCrvRateStepCntr @41 :UInt8;
    bEnaStepDtctRi @42 :UInt8;
    fAbsDistYStepRi @43 :Float32;
    bRawInValidDistYStepRi @44 :UInt8;
    bDlyValidDistYStepRi @45 :UInt8;
    fTimerDistYStepValidRi @46 :Float32;
    bResetDistYStepRi @47 :UInt8;
    fAbsHeadingStepRi @48 :Float32;
    bRawInValidHeadingStepRi @49 :UInt8;
    bDlyValidHeadingStepRi @50 :UInt8;
    fTimerHeadingStepValidRi @51 :Float32;
    bResetHeadingStepRi @52 :UInt8;
    fAbsCrvStepRi @53 :Float32;
    bRawInValidCrvStepRi @54 :UInt8;
    bDlyValidCrvStepRi @55 :UInt8;
    fTimerCrvStepValidRi @56 :Float32;
    bResetCrvStepRi @57 :UInt8;
    fAbsCrvRateStepRi @58 :Float32;
    bRawInValidCrvRateStepRi @59 :UInt8;
    bDlyValidCrvRateStepRi @60 :UInt8;
    fTimerCrvRateStepValidRi @61 :Float32;
    bResetCrvRateStepRi @62 :UInt8;
}

struct SCfvInputT {
    bLaneChangeDtct @0 :UInt8;
    bValidLaneWidth @1 :UInt8;
    uBtfKalmanFilterLf @2 :UInt8;
    fQualityMeasureLf @3 :Float32;
    fPosY0FltLf @4 :Float32;
    fHeadingFltLf @5 :Float32;
    fCrvFltLf @6 :Float32;
    fCrvRateFltLf @7 :Float32;
    uBtfKalmanFilterCntr @8 :UInt8;
    fQualityMeasureCntr @9 :Float32;
    fPosY0FltCntr @10 :Float32;
    fHeadingFltCntr @11 :Float32;
    fCrvFltCntr @12 :Float32;
    fCrvRateFltCntr @13 :Float32;
    uBtfKalmanFilterRi @14 :UInt8;
    fQualityMeasureRi @15 :Float32;
    fPosY0FltRi @16 :Float32;
    fHeadingFltRi @17 :Float32;
    fCrvFltRi @18 :Float32;
    fCrvRateFltRi @19 :Float32;
}

struct SCfvOutputT {
    bValidKalmanFilterLf @0 :UInt8;
    bInValidDistYStepLf @1 :UInt8;
    bInValidHeadingStepLf @2 :UInt8;
    bInValidCrvStepLf @3 :UInt8;
    bInValidCrvRateStepLf @4 :UInt8;
    bValidKalmanFilterCntr @5 :UInt8;
    bInValidDistYStepCntr @6 :UInt8;
    bInValidHeadingStepCntr @7 :UInt8;
    bInValidCrvStepCntr @8 :UInt8;
    bInValidCrvRateStepCntr @9 :UInt8;
    bPossibleLaneBridgeCntr @10 :UInt8;
    bValidKalmanFilterRi @11 :UInt8;
    bInValidDistYStepRi @12 :UInt8;
    bInValidHeadingStepRi @13 :UInt8;
    bInValidCrvStepRi @14 :UInt8;
    bInValidCrvRateStepRi @15 :UInt8;
}

struct SClpDebugT {
    bLaneChangeDtct @0 :UInt8;
    bLastAvailableUlpLf @1 :UInt8;
    bDisaStepDtctLf @2 :UInt8;
    bEnaByPosY0UlpLf @3 :UInt8;
    fAbsDevPosY0UlpLf @4 :Float32;
    bEnaDistYStepDtctLf @5 :UInt8;
    bLatDistStepTrigLf @6 :UInt8;
    fLaneWidthUlpLf @7 :Float32;
    bValidLaneWidthUlpLf @8 :UInt8;
    fHistoryLaneWidthLf @9 :Float32;
    fAbsDevLaneWidthUlpLf @10 :Float32;
    bInVldLaneWidthDevLf @11 :UInt8;
    bRawValidNewCorrLf @12 :UInt8;
    fTimerRawValidNewCorrLf @13 :Float32;
    fAbsDevLaneWidthFltLf @14 :Float32;
    bEnaByLaneWidthFltLf @15 :UInt8;
    bLaneWidthAgainNormalLf @16 :UInt8;
    fTimerAgainNormalLf @17 :Float32;
    bRawDistYStepDtctLf @18 :UInt8;
    bStepDebouncedLf @19 :UInt8;
    bRawStepDebouncedLf @20 :UInt8;
    fTimerStepDebouncedLf @21 :Float32;
    bResetRawDistYStepDtctLf @22 :UInt8;
    bLastAvailableUlpRi @23 :UInt8;
    bDisaStepDtctRi @24 :UInt8;
    bEnaByPosY0UlpRi @25 :UInt8;
    fAbsDevPosY0UlpRi @26 :Float32;
    bEnaDistYStepDtctRi @27 :UInt8;
    bLatDistStepTrigRi @28 :UInt8;
    fLaneWidthUlpRi @29 :Float32;
    bValidLaneWidthUlpRi @30 :UInt8;
    fHistoryLaneWidthRi @31 :Float32;
    fAbsDevLaneWidthUlpRi @32 :Float32;
    bInVldLaneWidthDevRi @33 :UInt8;
    bRawValidNewCorrRi @34 :UInt8;
    fTimerRawValidNewCorrRi @35 :Float32;
    fAbsDevLaneWidthFltRi @36 :Float32;
    bEnaByLaneWidthFltRi @37 :UInt8;
    bLaneWidthAgainNormalRi @38 :UInt8;
    fTimerAgainNormalRi @39 :Float32;
    bRawDistYStepDtctRi @40 :UInt8;
    bStepDebouncedRi @41 :UInt8;
    bRawStepDebouncedRi @42 :UInt8;
    fTimerStepDebouncedRi @43 :Float32;
    bResetRawDistYStepDtctRi @44 :UInt8;
    fMinValidlength @45 :Float32;
    fHystMinValidlength @46 :Float32;
    bRawValidLengthLf @47 :UInt8;
    bValidLengthLf @48 :UInt8;
    fTimerLengthValidLf @49 :Float32;
    bRawValidLengthRi @50 :UInt8;
    bValidLengthRi @51 :UInt8;
    fTimerLengthValidRi @52 :Float32;
    uStMarerTypeLf @53 :UInt16;
    uStMarerTypeRi @54 :UInt16;
    uStLaneColorLf @55 :UInt16;
    uStLaneColorRi @56 :UInt16;
    bOutRangePosY0Lf @57 :UInt8;
    bOutRangeHeadingLf @58 :UInt8;
    bOutRangeCrvLf @59 :UInt8;
    bOutRangeCrvRateLf @60 :UInt8;
    bOutRangeLengthLf @61 :UInt8;
    bOutRangePosY0Ri @62 :UInt8;
    bOutRangeHeadingRi @63 :UInt8;
    bOutRangeCrvRi @64 :UInt8;
    bOutRangeCrvRateRi @65 :UInt8;
    bOutRangeLengthRi @66 :UInt8;
    fMinRangePosY0 @67 :Float32;
    fMinRangeHeading @68 :Float32;
    fMinRangeCrv @69 :Float32;
    fMinRangeCrvRate @70 :Float32;
    fMinRangeLength @71 :Float32;
    bEnaByLineWidthMergeRi @72 :UInt8;
    bEnaByCurvatureMergeRi @73 :UInt8;
    bEnaByAtMaxCurvatureMergeRi @74 :UInt8;
    bTempHdAtMaxUnCplMergeRi @75 :UInt8;
    bEnaByHdAtMaxUnCplMergeRi @76 :UInt8;
    bRawEnaByHdUnCplMergeRi @77 :UInt8;
    bEnaByHdUnCplMergeRi @78 :UInt8;
    bRawMergeUnCplRi @79 :UInt8;
    bEnaByLineWidthMergeLf @80 :UInt8;
    bEnaByCurvatureMergeLf @81 :UInt8;
    bEnaByAtMaxCurvatureMergeLf @82 :UInt8;
    bTempHdAtMaxUnCplMergeLf @83 :UInt8;
    bEnaByHdAtMaxUnCplMergeLf @84 :UInt8;
    bRawEnaByHdUnCplMergeLf @85 :UInt8;
    bEnaByHdUnCplMergeLf @86 :UInt8;
    bRawMergeUnCplLf @87 :UInt8;
    fHeadingAtMaxUnCplLf @88 :Float32;
    fHeadingAtMaxUnCplRi @89 :Float32;
    fPosYAtMaxUnCplLf @90 :Float32;
    fPosYAtMaxUnCplRi @91 :Float32;
    fCurvatureAtMaxUnCplLf @92 :Float32;
    fCurvatureAtMaxUnCplRi @93 :Float32;
}

struct SClpInputT {
}

struct SClpOutputT {
    bNotAvailableLf @0 :UInt8;
    bDistYStepDtctLf @1 :UInt8;
    bLengthInvalidLf @2 :UInt8;
    bNotAvailableRi @3 :UInt8;
    bValidnewCorridorLf @4 :UInt8;
    bValidnewCorridorRi @5 :UInt8;
    bDistYStepDtctRi @6 :UInt8;
    bLengthInvalidRi @7 :UInt8;
    bLnQualityInvalidLf @8 :UInt8;
    bLnQualityInvalidRi @9 :UInt8;
    bLaneTypeInvalidLf @10 :UInt8;
    bLaneColorInvalidLf @11 :UInt8;
    bLaneQualityInvalidLf @12 :UInt8;
    bLaneTypeInvalidRi @13 :UInt8;
    bLaneColorInvalidRi @14 :UInt8;
    bLaneQualityInvalidRi @15 :UInt8;
    fValidLengthLf @16 :Float32;
    fValidLengthRi @17 :Float32;
    bLaneVirtualCplLf @18 :UInt8;
    bLaneVirtualCplRi @19 :UInt8;
    bCamStatusQualifierValid @20 :UInt8;
    bUpDownHillDegrade @21 :UInt8;
    bInValidLengthLf @22 :UInt8;
    bInValidLengthRi @23 :UInt8;
    bInValidMakerTypeLf @24 :UInt8;
    bInValidMakerTypeRi @25 :UInt8;
    bInValidLaneColorLf @26 :UInt8;
    bInValidLaneColorRi @27 :UInt8;
    bLaneChangeDtct @28 :UInt8;
    fPercStraightDtct @29 :Float32;
    fPercExitLf @30 :Float32;
    fPercExitRi @31 :Float32;
    fPercUpDownHillDtct @32 :Float32;
    fRawLaneWidthUnCpl @33 :Float32;
    fRawLaneWidthCpl @34 :Float32;
    uRangeCheckQualifier @35 :UInt16;
    bConstructionSiteDtct @36 :UInt8;
    bExitUnCplLf @37 :UInt8;
    bExitUnCplRi @38 :UInt8;
    bSineWaveUnCpl @39 :UInt8;
    bNewCorridorValid @40 :UInt8;
    bLineMergeDtcRi @41 :UInt8;
    bLineMergeDtcLf @42 :UInt8;
}

struct SDlbDebugT {
    fSysCycleTime @0 :Float32;
}

struct SDlbInputT {
    bUpDownHillDegr @0 :UInt8;
    bBridgeUnCplLf @1 :UInt8;
    fOverallQualityUnCplLf @2 :Float32;
    bBridgeUnCplRi @3 :UInt8;
    fOverallQualityUnCplRi @4 :Float32;
    fPosX0CplLf @5 :Float32;
    fPosY0CplLf @6 :Float32;
    fHeadingCplLf @7 :Float32;
    fCrvCplLf @8 :Float32;
    fCrvRateCplLf @9 :Float32;
    fValidLengthCplLf @10 :Float32;
    fQualityCplLf @11 :Float32;
    uLaneTypeCplLf @12 :UInt8;
    uEventTypeCplLf @13 :UInt8;
    bAvailableCplLf @14 :UInt8;
    uEventQualityCplLf @15 :UInt8;
    fEventDistanceCplLf @16 :Float32;
    fStdDevPosY0CplLf @17 :Float32;
    fStdDevHeadingCplLf @18 :Float32;
    fStdDevCrvCplLf @19 :Float32;
    fStdDevCrvRateCplLf @20 :Float32;
    uColorCplLf @21 :UInt8;
    fOverallQualityCplLf @22 :Float32;
    fPosX0CplRi @23 :Float32;
    fPosY0CplRi @24 :Float32;
    fHeadingCplRi @25 :Float32;
    fCrvCplRi @26 :Float32;
    fCrvRateCplRi @27 :Float32;
    fValidLengthCplRi @28 :Float32;
    fQualityCplRi @29 :Float32;
    uLaneTypeCplRi @30 :UInt8;
    uEventTypeCplRi @31 :UInt8;
    bAvailableCplRi @32 :UInt8;
    uEventQualityCplRi @33 :UInt8;
    fEventDistanceCplRi @34 :Float32;
    fStdDevPosY0CplRi @35 :Float32;
    fStdDevHeadingCplRi @36 :Float32;
    fStdDevCrvCplRi @37 :Float32;
    fStdDevCrvRateCplRi @38 :Float32;
    uColorCplRi @39 :UInt8;
    fOverallQualityCplRi @40 :Float32;
}

struct SDlbOutputT {
    fPosX0Lf @0 :Float32;
    fPosY0Lf @1 :Float32;
    fHeadingLf @2 :Float32;
    fCrvLf @3 :Float32;
    fCrvRateLf @4 :Float32;
    fValidLengthLf @5 :Float32;
    fQualityLf @6 :Float32;
    uLaneTypeLf @7 :UInt8;
    uEventTypeLf @8 :UInt8;
    bAvailableLf @9 :UInt8;
    uEventQualityLf @10 :UInt8;
    fEventDistanceLf @11 :Float32;
    fStdDevPosY0Lf @12 :Float32;
    fStdDevHeadingLf @13 :Float32;
    fStdDevCrvLf @14 :Float32;
    fStdDevCrvRateLf @15 :Float32;
    uColorLf @16 :UInt8;
    fOverallQualityLf @17 :Float32;
    fPosX0Ri @18 :Float32;
    fPosY0Ri @19 :Float32;
    fHeadingRi @20 :Float32;
    fCrvRi @21 :Float32;
    fCrvRateRi @22 :Float32;
    fValidLengthRi @23 :Float32;
    fQualityRi @24 :Float32;
    uLaneTypeRi @25 :UInt8;
    uEventTypeRi @26 :UInt8;
    bAvailableRi @27 :UInt8;
    uEventQualityRi @28 :UInt8;
    fEventDistanceRi @29 :Float32;
    fStdDevPosY0Ri @30 :Float32;
    fStdDevHeadingRi @31 :Float32;
    fStdDevCrvRi @32 :Float32;
    fStdDevCrvRateRi @33 :Float32;
    uColorRi @34 :UInt8;
    fOverallQualityRi @35 :Float32;
}

struct SDmqDebugT {
    fQualityPenaltyByPosY0 @0 :Float32;
    fQualityPenaltyByHeading @1 :Float32;
    fQualityPenaltyByCrv @2 :Float32;
    fQualityPenaltyByCrvRate @3 :Float32;
    fRawQualityPenalty @4 :Float32;
    fQualityPenalty @5 :Float32;
    fLengthQuality @6 :Float32;
    fMeasureQuality @7 :Float32;
    fRawOverallQuality @8 :Float32;
    bDlyEnaLaneVirtualCpl @9 :UInt8;
    fAbsStepPosY0 @10 :Float32;
    fAbsStepHeading @11 :Float32;
    fAbsStepCrv @12 :Float32;
    fAbsStepCrvRate @13 :Float32;
    fLastPosY0 @14 :Float32;
    fLastHeading @15 :Float32;
    fLastCrv @16 :Float32;
    fLastCrvRate @17 :Float32;
    bLastAvailable @18 :UInt8;
    fLastQualityPenalty @19 :Float32;
    fLastOverallQuality @20 :Float32;
    bLastDlyEnaLaneVirtualCpl @21 :UInt8;
    fMeasureCrvQuality @22 :Float32;
    fLengthCrvQuality @23 :Float32;
    fTimerEnaLaneVirtualCpl @24 :Float32;
    fRawCrvQuality @25 :Float32;
}

struct SDmqInputT {
    fPosY0 @0 :Float32;
    fHeading @1 :Float32;
    fCrv @2 :Float32;
    fCrvRate @3 :Float32;
    fValidLength @4 :Float32;
    bAvailable @5 :UInt8;
    fStdDevPosY0 @6 :Float32;
    fStdDevHeading @7 :Float32;
    fStdDevCrv @8 :Float32;
    fStdDevCrvRate @9 :Float32;
    fQuality @10 :Float32;
    bLaneChangeDtct @11 :UInt8;
    bUpDnHillDgrd @12 :UInt8;
}

struct SDmqOutputT {
    fOverallQuality @0 :Float32;
    fCrvQuality @1 :Float32;
    bEnaLaneVirtualCpl @2 :UInt8;
}

struct SEciDebugT {
    bValidBoth @0 :UInt8;
    bValidOnlyLf @1 :UInt8;
    bValidOnlyRi @2 :UInt8;
}

struct SEciInputT {
    bCamStatusQualifierValid @0 :UInt8;
    bNotAvailableLf @1 :UInt8;
    bDistYStepDtctLf @2 :UInt8;
    bLengthInvalidLf @3 :UInt8;
    bNotAvailableRi @4 :UInt8;
    bDistYStepDtctRi @5 :UInt8;
    bLengthInvalidRi @6 :UInt8;
    uRangeCheckQualifier @7 :UInt16;
    bLaneTypeInvalidLf @8 :UInt8;
    bLaneColorInvalidLf @9 :UInt8;
    bLaneQualityInvalidLf @10 :UInt8;
    bLaneTypeInvalidRi @11 :UInt8;
    bLaneColorInvalidRi @12 :UInt8;
    bLaneQualityInvalidRi @13 :UInt8;
    fValidLengthLf @14 :Float32;
    fValidLengthRi @15 :Float32;
    bLaneVirtualCplLf @16 :UInt8;
    bLaneVirtualCplRi @17 :UInt8;
    uLaneValidQualifier @18 :UInt8;
    uBridgePossible @19 :UInt8;
    bKalmanValidLf @20 :UInt8;
    bDistYStepDebouncedLf @21 :UInt8;
    bHeadingStepDebouncedLf @22 :UInt8;
    bCrvStepDebouncedLf @23 :UInt8;
    bCrvRateStepDebouncedLf @24 :UInt8;
    bKalmanValidCntr @25 :UInt8;
    bKalmanValidRi @26 :UInt8;
    bDistYStepDebouncedRi @27 :UInt8;
    bHeadingStepDebouncedRi @28 :UInt8;
    bCrvStepDebouncedRi @29 :UInt8;
    bCrvRateStepDebouncedRi @30 :UInt8;
    fFltQualityCntr @31 :Float32;
    uFltStatusCntr @32 :UInt8;
    bDistYStepDebouncedCntr @33 :UInt8;
    bHeadingStepDebouncedCntr @34 :UInt8;
    bCrvStepDebouncedCntr @35 :UInt8;
    bCrvRateStepDebouncedCntr @36 :UInt8;
    fPosX0FltLf @37 :Float32;
    fPosY0FltLf @38 :Float32;
    fHeadingFltLf @39 :Float32;
    fCrvFltLf @40 :Float32;
    fCrvRateFltLf @41 :Float32;
    fPosX0FltCntr @42 :Float32;
    fPosY0FltCntr @43 :Float32;
    fHeadingFltCntr @44 :Float32;
    fCrvFltCntr @45 :Float32;
    fCrvRateFltCntr @46 :Float32;
    fValidLengthFltCntr @47 :Float32;
    fPosX0FltRi @48 :Float32;
    fPosY0FltRi @49 :Float32;
    fHeadingFltRi @50 :Float32;
    fCrvFltRi @51 :Float32;
    fCrvRateFltRi @52 :Float32;
    fLaneWidth @53 :Float32;
    bLineMergeDtcRi @54 :UInt8;
    bLineMergeDtcLf @55 :UInt8;
}

struct SEciOutputT {
    fLaneWidth @0 :Float32;
    uLaneInvalidQualifierLf @1 :UInt16;
    uLaneInvalidQualifierRi @2 :UInt16;
    uVisualValidQualifier @3 :UInt8;
    fFltQualityCntr @4 :Float32;
    uFltStatusCntr @5 :UInt8;
    uLaneValidQualDmc @6 :UInt8;
    uOutRangeCheckQualifier @7 :UInt16;
    fPosX0CtrlLf @8 :Float32;
    fPosY0CtrlLf @9 :Float32;
    fHeadingCtrlLf @10 :Float32;
    fCrvCtrlLf @11 :Float32;
    fCrvRateCtrlLf @12 :Float32;
    fValidLengthCtrlLf @13 :Float32;
    fPosX0CtrlCntr @14 :Float32;
    fPosY0CtrlCntr @15 :Float32;
    fHeadingCtrlCntr @16 :Float32;
    fCrvCtrlCntr @17 :Float32;
    fCrvRateCtrlCntr @18 :Float32;
    fValidLengthCtrlCntr @19 :Float32;
    fPosX0CtrlRi @20 :Float32;
    fPosY0CtrlRi @21 :Float32;
    fHeadingCtrlRi @22 :Float32;
    fCrvCtrlRi @23 :Float32;
    fCrvRateCtrlRi @24 :Float32;
    fValidLengthCtrlRi @25 :Float32;
}

struct SElgDebugT {
    sEsiInput @0 :SEsiInputT;
    sEsiOutput @1 :SEsiOutputT;
    sEsiDebug @2 :SEsiDebugT;
    sEciInput @3 :SEciInputT;
    sEciOutput @4 :SEciOutputT;
    sEciDebug @5 :SEciDebugT;
}

struct SElgInputT {
    bLaneChangeDtct @0 :UInt8;
    uLaneTypeLf @1 :UInt8;
    uLaneTypeRi @2 :UInt8;
    bConstructionSiteDtct @3 :UInt8;
    uOverallQualityLf @4 :UInt8;
    uOverallQualityRi @5 :UInt8;
    fOverallCrvQualityLf @6 :Float32;
    fOverallCrvQualityRi @7 :Float32;
    fTstamp @8 :Float32;
    uRangeCheckQualifier @9 :UInt16;
    fVehVelX @10 :Float32;
    bCamStatusQualifierValid @11 :UInt8;
    bUpDownHillDegrade @12 :UInt8;
    fYawLf @13 :Float32;
    bYawValidLf @14 :UInt8;
    fYawRi @15 :Float32;
    bYawValidRi @16 :UInt8;
    fPosY0Lf @17 :Float32;
    fHeadingLf @18 :Float32;
    fCrvLf @19 :Float32;
    bAvailableLf @20 :UInt8;
    uQualityLf @21 :UInt8;
    fOverallQualityLf @22 :Float32;
    fPosY0Ri @23 :Float32;
    fHeadingRi @24 :Float32;
    fCrvRi @25 :Float32;
    bAvailableRi @26 :UInt8;
    uQualityRi @27 :UInt8;
    fOverallQualityRi @28 :Float32;
    bCamStatusQualityValid @29 :UInt8;
    bNotAvailableLf @30 :UInt8;
    bDistYStepDtctLf @31 :UInt8;
    bLengthInvalidLf @32 :UInt8;
    bNotAvailableRi @33 :UInt8;
    bDistYStepDtctRi @34 :UInt8;
    bLengthInvalidRi @35 :UInt8;
    bLaneTypeInvalidLf @36 :UInt8;
    bLaneColorInvalidLf @37 :UInt8;
    bLaneQualityInvalidLf @38 :UInt8;
    bLaneTypeInvalidRi @39 :UInt8;
    bLaneColorInvalidRi @40 :UInt8;
    bLaneQualityInvalidRi @41 :UInt8;
    fValidLengthLf @42 :Float32;
    fValidLengthRi @43 :Float32;
    bLaneVirtualCplLf @44 :UInt8;
    bLaneVirtualCplRi @45 :UInt8;
    uLaneValidQualifier @46 :UInt8;
    uBridgePossible @47 :UInt8;
    bKalmanValidLf @48 :UInt8;
    bDistYStepDebouncedLf @49 :UInt8;
    bHeadingStepDebouncedLf @50 :UInt8;
    bCrvStepDebouncedLf @51 :UInt8;
    bCrvRateStepDebouncedLf @52 :UInt8;
    bKalmanValidCntr @53 :UInt8;
    bDistYStepDebouncedCntr @54 :UInt8;
    bHeadingStepDebouncedCntr @55 :UInt8;
    bCrvStepDebouncedCntr @56 :UInt8;
    bCrvRateStepDebouncedCntr @57 :UInt8;
    bKalmanValidRi @58 :UInt8;
    bDistYStepDebouncedRi @59 :UInt8;
    bHeadingStepDebouncedRi @60 :UInt8;
    bCrvStepDebouncedRi @61 :UInt8;
    bCrvRateStepDebouncedRi @62 :UInt8;
    fFltQualityCntr @63 :Float32;
    uFltStatusCntr @64 :UInt8;
    fPosX0FltLf @65 :Float32;
    fPosY0FltLf @66 :Float32;
    fHeadingFltLf @67 :Float32;
    fCrvFltLf @68 :Float32;
    fCrvRateFltLf @69 :Float32;
    fPosX0FltCntr @70 :Float32;
    fPosY0FltCntr @71 :Float32;
    fHeadingFltCntr @72 :Float32;
    fCrvFltCntr @73 :Float32;
    fCrvRateFltCntr @74 :Float32;
    fValidLengthFltCntr @75 :Float32;
    fPosX0FltRi @76 :Float32;
    fPosY0FltRi @77 :Float32;
    fHeadingFltRi @78 :Float32;
    fCrvFltRi @79 :Float32;
    fCrvRateFltRi @80 :Float32;
    fLaneWidth @81 :Float32;
    bLineMergeDtcRi @82 :UInt8;
    bLineMergeDtcLf @83 :UInt8;
}

struct SElgOutputT {
    bLaneChangeDtct @0 :UInt8;
    uLaneTypeLf @1 :UInt8;
    uLaneTypeRi @2 :UInt8;
    bConstructionSiteDtct @3 :UInt8;
    uOverallQualityLf @4 :UInt8;
    uOverallQualityRi @5 :UInt8;
    uCrvQualityLf @6 :UInt8;
    uCrvQualityRi @7 :UInt8;
    fAbdTimeStamp @8 :Float32;
    uRangeCheckQualifier @9 :UInt16;
    fPosY0SafeLf @10 :Float32;
    fHeadingSafeLf @11 :Float32;
    fCrvSafeLf @12 :Float32;
    fPosY0SafeRi @13 :Float32;
    fHeadingSafeRi @14 :Float32;
    fCrvSafeRi @15 :Float32;
    uInvalidQualifierSafeLf @16 :UInt8;
    uInvalidQualifierSafeRi @17 :UInt8;
    fLaneWidth @18 :Float32;
    uLaneInvalidQualifierLf @19 :UInt16;
    uLaneInvalidQualifierRi @20 :UInt16;
    uVisualValidQualifier @21 :UInt8;
    fFltQualityCntr @22 :Float32;
    uFltStatusCntr @23 :UInt8;
    uLaneValidQualDmc @24 :UInt8;
    uOutRangeCheckQualifier @25 :UInt16;
    fPosX0CtrlLf @26 :Float32;
    fPosY0CtrlLf @27 :Float32;
    fHeadingCtrlLf @28 :Float32;
    fCrvCtrlLf @29 :Float32;
    fCrvRateCtrlLf @30 :Float32;
    fValidLengthCtrlLf @31 :Float32;
    fPosX0CtrlCntr @32 :Float32;
    fPosY0CtrlCntr @33 :Float32;
    fHeadingCtrlCntr @34 :Float32;
    fCrvCtrlCntr @35 :Float32;
    fCrvRateCtrlCntr @36 :Float32;
    fValidLengthCtrlCntr @37 :Float32;
    fPosX0CtrlRi @38 :Float32;
    fPosY0CtrlRi @39 :Float32;
    fHeadingCtrlRi @40 :Float32;
    fCrvCtrlRi @41 :Float32;
    fCrvRateCtrlRi @42 :Float32;
    fValidLengthCtrlRi @43 :Float32;
}

struct SensorMountingT {
    state @0 :List(UInt8);
    latPos @1 :Float32;
    longPos @2 :Float32;
    vertPos @3 :Float32;
    longPosToCoG @4 :Float32;
    pitchAngle @5 :Float32;
    orientation @6 :UInt8;
    rollAngle @7 :Float32;
    yawAngle @8 :Float32;
}

struct SensorSpecificT {
    fRcs @0 :Float32;
    bCamConfirmed @1 :Bool;
}

struct SensorT {
    state @0 :List(UInt8);
    coverDamping @1 :Float32;
    fCoverageAngle @2 :Float32;
    fLobeAngle @3 :Float32;
    fCycleTime @4 :Float32;
    uNoOfScans @5 :UInt8;
}

struct SEsiDebugT {
    fRawLateraVelLf @0 :Float32;
    fRawLateraVelRi @1 :Float32;
    fCoeffFltSafe @2 :Float32;
    fLateraVelLf @3 :Float32;
    fLateraVelRi @4 :Float32;
    bSuppressLaneStepDtctLf @5 :UInt8;
    fAbsDistYStepLf @6 :Float32;
    fAbsHeadingStepLf @7 :Float32;
    fAbsCrvStepLf @8 :Float32;
    bDistYStepDtctLf @9 :UInt8;
    bHeadingStepDtctLf @10 :UInt8;
    bCrvStepDtctLf @11 :UInt8;
    bRawDistYStepFlipLf @12 :UInt8;
    fTimerDistYStepFlipLf @13 :Float32;
    bRstDistYStepFlipLf @14 :UInt8;
    bDistYStepFlipLf @15 :UInt8;
    bRawHeadingStepFlipLf @16 :UInt8;
    fTimerHeadingStepFlipLf @17 :Float32;
    bRstHeadingStepFlipLf @18 :UInt8;
    bHeadingStepFlipLf @19 :UInt8;
    bRawCrvStepFlipLf @20 :UInt8;
    fTimerCrvStepFlipLf @21 :Float32;
    bRstCrvStepFlipLf @22 :UInt8;
    bCrvStepFlipLf @23 :UInt8;
    bSetByQualityLf @24 :UInt8;
    fTimerInValidByQltyLf @25 :Float32;
    bInValidByQltyLf @26 :UInt8;
    bInValidByAllQltyLf @27 :UInt8;
    bSuppressLaneStepDtctRi @28 :UInt8;
    fAbsDistYStepRi @29 :Float32;
    fAbsHeadingStepRi @30 :Float32;
    fAbsCrvStepRi @31 :Float32;
    bDistYStepDtctRi @32 :UInt8;
    bHeadingStepDtctRi @33 :UInt8;
    bCrvStepDtctRi @34 :UInt8;
    bRawDistYStepFlipRi @35 :UInt8;
    fTimerDistYStepFlipRi @36 :Float32;
    bRstDistYStepFlipRi @37 :UInt8;
    bDistYStepFlipRi @38 :UInt8;
    bRawHeadingStepFlipRi @39 :UInt8;
    fTimerHeadingStepFlipRi @40 :Float32;
    bRstHeadingStepFlipRi @41 :UInt8;
    bHeadingStepFlipRi @42 :UInt8;
    bRawCrvStepFlipRi @43 :UInt8;
    fTimerCrvStepFlipRi @44 :Float32;
    bRstCrvStepFlipRi @45 :UInt8;
    bCrvStepFlipRi @46 :UInt8;
    bSetByQualityRi @47 :UInt8;
    fTimerInValidByQltyRi @48 :Float32;
    bInValidByQltyRi @49 :UInt8;
    bInValidByAllQltyRi @50 :UInt8;
}

struct SEsiInputT {
    fVehVelX @0 :Float32;
    bCamStatusQualifierValid @1 :UInt8;
    bLaneChangeDtct @2 :UInt8;
    bUpDownHillDegrade @3 :UInt8;
    fYawLf @4 :Float32;
    bYawValidLf @5 :UInt8;
    fYawRi @6 :Float32;
    bYawValidRi @7 :UInt8;
    fPosY0Lf @8 :Float32;
    fHeadingLf @9 :Float32;
    fCrvLf @10 :Float32;
    bAvailableLf @11 :UInt8;
    uQualityLf @12 :UInt8;
    fOverallQualityLf @13 :Float32;
    fPosY0Ri @14 :Float32;
    fHeadingRi @15 :Float32;
    fCrvRi @16 :Float32;
    bAvailableRi @17 :UInt8;
    uQualityRi @18 :UInt8;
    fOverallQualityRi @19 :Float32;
}

struct SEsiOutputT {
    fPosY0SafeLf @0 :Float32;
    fHeadingSafeLf @1 :Float32;
    fCrvSafeLf @2 :Float32;
    fPosY0SafeRi @3 :Float32;
    fHeadingSafeRi @4 :Float32;
    fCrvSafeRi @5 :Float32;
    uInvalidQualifierSafeLf @6 :UInt8;
    uInvalidQualifierSafeRi @7 :UInt8;
}

struct SHodDebugT {
    hodTrqEstHandFlt @0 :Float32;
    hodTrqEstHandStp @1 :Float32;
    hodTrqHandsOffLim @2 :Float32;
    hodEnaRightOn @3 :Bool;
    hodEnaRightInt @4 :Bool;
    hodEnaRightOff @5 :Bool;
    hodEnaLeftOn @6 :Bool;
    hodEnaLeftInt @7 :Bool;
    hodEnaLeftOff @8 :Bool;
    hodCntEstHandTrq @9 :Float32;
    hodCntHandsOffLim @10 :Float32;
}

struct SHodOutputT {
    hodStEstHandTrq @0 :UInt8;
    hodStSysWarning @1 :UInt8;
    hodRatDrvAttention @2 :Float32;
    hodHandsOffAbuseWarn @3 :Bool;
    hodEnaHandsOffCnfm @4 :Bool;
}

struct SideSlipVehDynT {
    sideSlipAngle @0 :Float32;
    variance @1 :Float32;
}

struct SignalHeaderT {
    uiTimeStamp @0 :UInt32;
    uiMeasurementCounter @1 :UInt16;
    uiCycleCounter @2 :UInt16;
    eSigStatus @3 :UInt8;
}

struct SKlmDebugT {
    bValidLaneLf @0 :UInt8;
    bEnaDegrUpdateLf @1 :UInt8;
    fRawCrvKlmLf @2 :Float32;
    fStartPosXForCrvLf @3 :Float32;
    fEndPosXForCrvLf @4 :Float32;
    fRawEstCrvKlmLf @5 :Float32;
    fRawTiFltCrvEstLf @6 :Float32;
    fTiFltCrvEstLf @7 :Float32;
    fEstCrvKlmLf @8 :Float32;
    bValidLaneCntr @9 :UInt8;
    fDelatYaw @10 :Float32;
    fDeltaPosX @11 :Float32;
    fDeltaPosY @12 :Float32;
    fRawPredPosY @13 :Float32;
    fPredPosY @14 :Float32;
    fPredLatDistLf @15 :Float32;
    fPredLatDistRi @16 :Float32;
    bEnaByLfKlmDiffCntr @17 :UInt8;
    bEnaByRiKlmDiffCntr @18 :UInt8;
    bLatDistDevLf @19 :UInt8;
    bLatDistDevRi @20 :UInt8;
    fFacFadingCntr @21 :Float32;
    fRawPosY0Cntr @22 :Float32;
    fRawHeadingCntr @23 :Float32;
    fRawCrvCntr @24 :Float32;
    fRawCrvRateCntr @25 :Float32;
    bResetRateLimitCntr @26 :UInt8;
    fPosY0Cntr @27 :Float32;
    fHeadingCntr @28 :Float32;
    fCrvCntr @29 :Float32;
    fCrvRateCntr @30 :Float32;
    fOverallQualityCntr @31 :Float32;
    bRawEnaDegrUpdate @32 :UInt8;
    bEnaByEdgeFail @33 :UInt8;
    bEnaBySrTrig @34 :UInt8;
    bEnaDegrUpdateCntr @35 :UInt8;
    fCoeffByStraightDtct @36 :Float32;
    fRawDegrWeightCntr @37 :Float32;
    fLengthValidCntr @38 :Float32;
    fStartPosXForCrvCntr @39 :Float32;
    fEndPosXForCrvCntr @40 :Float32;
    fRawEstCrvKlmCntr @41 :Float32;
    fRawTiFltCrvEstCntr @42 :Float32;
    fTiFltCrvEstCntr @43 :Float32;
    fEstCrvKlmCntr @44 :Float32;
    fCrvByCrvFltCntr @45 :Float32;
    fCrvRateByCrvFltCntr @46 :Float32;
    uBtfStatusByCrvFltLf @47 :UInt8;
    fQualityByCrvFltLf @48 :UInt8;
    bValidLaneRi @49 :UInt8;
    bEnaDegrUpdateRi @50 :UInt8;
    fRawCrvKlmRi @51 :Float32;
    fStartPosXForCrvRi @52 :Float32;
    fEndPosXForCrvRi @53 :Float32;
    fRawEstCrvKlmRi @54 :Float32;
    fRawTiFltCrvEstRi @55 :Float32;
    fTiFltCrvEstRi @56 :Float32;
    fEstCrvKlmRi @57 :Float32;
}

struct SKlmInputT {
    uLaneValidQualifer @0 :UInt8;
    bValidNewCorr @1 :UInt8;
    bUpDownHillDegrade @2 :UInt8;
    bValidKlmFltCntr @3 :UInt8;
    fOverallQualityLf @4 :Float32;
    fOverallQualityRi @5 :Float32;
    fPosY0UlpLf @6 :Float32;
    fHeadingUlpLf @7 :Float32;
    fCrvUlpLf @8 :Float32;
    fCrvRateUlpLf @9 :Float32;
    fValidLengthUlpLf @10 :Float32;
    fPosY0UlpRi @11 :Float32;
    fHeadingUlpRi @12 :Float32;
    fCrvUlpRi @13 :Float32;
    fCrvRateUlpRi @14 :Float32;
    fValidLengthUlpRi @15 :Float32;
    fLaneWidth @16 :Float32;
    fVehYawRateStd @17 :Float32;
    fVehVelX @18 :Float32;
    fVehYawRate @19 :Float32;
    bLaneChangeDtct @20 :UInt8;
    fStraightDtct @21 :Float32;
}

struct SKlmOutputT {
    fPosY0KlmLf @0 :Float32;
    fHeadingKlmLf @1 :Float32;
    fCrvKlmLf @2 :Float32;
    fCrvRateKlmLf @3 :Float32;
    uBtfStatusKlmLf @4 :UInt8;
    fQualityMeasureLf @5 :Float32;
    bLatDistDevLf @6 :UInt8;
    fPosY0KlmCntr @7 :Float32;
    fHeadingKlmCntr @8 :Float32;
    fCrvKlmCntr @9 :Float32;
    fCrvRateKlmCntr @10 :Float32;
    uBtfStatusKlmCntr @11 :UInt8;
    fQualityMeasureCntr @12 :Float32;
    fPosY0KlmRi @13 :Float32;
    fHeadingKlmRi @14 :Float32;
    fCrvKlmRi @15 :Float32;
    fCrvRateKlmRi @16 :Float32;
    uBtfStatusKlmRi @17 :UInt8;
    fQualityMeasureRi @18 :Float32;
    bLatDistDevRi @19 :UInt8;
}

struct SLbpDebugT {
    sUlpInput @0 :SUlpInputT;
    sUlpOutput @1 :SUlpOutputT;
    sUlpDebug @2 :SUlpDebugT;
    sClpInput @3 :SClpInputT;
    sClpOutput @4 :SClpOutputT;
    sClpDebug @5 :SClpDebugT;
    sLfpInput @6 :SLfpInputT;
    sLfpOutput @7 :SLfpOutputT;
    sLfpDebug @8 :SLfpDebugT;
    sElgInput @9 :SElgInputT;
    sElgOutput @10 :SElgOutputT;
    sElgDebug @11 :SElgDebugT;
}

struct SLbpInputT {
    fSysCycleTime @0 :Float32;
    uCamStatusQualifier @1 :UInt8;
    fVehVelX @2 :Float32;
    fPosY0 @3 :List(Float32);
    fHeadingAngle @4 :List(Float32);
    fCurvature @5 :List(Float32);
    fCurvatureRate @6 :List(Float32);
    fValidLength @7 :List(Float32);
    uQuality @8 :List(UInt8);
    uMarkerType @9 :List(UInt8);
    uEventType @10 :List(UInt8);
    fEventDistance @11 :List(Float32);
    uEventQuality @12 :List(UInt8);
    bAvailable @13 :List(UInt8);
    fStdDevPosY0 @14 :List(Float32);
    fStdDevHeadingAngle @15 :List(Float32);
    fStdDevCurvature @16 :List(Float32);
    fStdDevCurvatureRate @17 :List(Float32);
    fVehYawRateStd @18 :Float32;
    fVehYawRate @19 :Float32;
    bParallelModelActiv @20 :UInt8;
    uLaneChange @21 :UInt8;
    fAgeMeter @22 :Float32;
    uColor @23 :List(UInt8);
    uLeftIndex @24 :UInt8;
    uRightIndex @25 :UInt8;
    fTstamp @26 :Float32;
    uRoadWorks @27 :UInt8;
    fFeatureCoordsXyLe @28 :Float32;
    fFeatureCoordsXyRi @29 :Float32;
    bCompState @30 :UInt8;
    fVertCurvature @31 :Float32;
    fVertCurvatureRate @32 :Float32;
    fVertAvailable @33 :Float32;
    fVertSlopeChange @34 :Float32;
    fVertValidLength @35 :Float32;
    uWeatherCond @36 :UInt8;
    fSineWaveDtct @37 :Float32;
}

struct SLbpOutputT {
    uLaneInvalidQualifierLf @0 :UInt16;
    uLaneInvalidQualifierRi @1 :UInt16;
    fLaneWidth @2 :Float32;
    bLaneChangeDtct @3 :UInt8;
    fPosX0CtrlLf @4 :Float32;
    fPosY0CtrlLf @5 :Float32;
    fHeadingCtrlLf @6 :Float32;
    fCrvCtrlLf @7 :Float32;
    fCrvRateCtrlLf @8 :Float32;
    fValidLengthCtrlLf @9 :Float32;
    fPosX0CtrlCntr @10 :Float32;
    fPosY0CtrlCntr @11 :Float32;
    fHeadingCtrlCntr @12 :Float32;
    fCrvCtrlCntr @13 :Float32;
    fCrvRateCtrlCntr @14 :Float32;
    fValidLengthCtrlCntr @15 :Float32;
    fPosX0CtrlRi @16 :Float32;
    fPosY0CtrlRi @17 :Float32;
    fHeadingCtrlRi @18 :Float32;
    fCrvCtrlRi @19 :Float32;
    fCrvRateCtrlRi @20 :Float32;
    fValidLengthCtrlRi @21 :Float32;
    fPosY0SafeLf @22 :Float32;
    fHeadingSafeLf @23 :Float32;
    fCrvSafeLf @24 :Float32;
    fPosY0SafeRi @25 :Float32;
    fHeadingSafeRi @26 :Float32;
    fCrvSafeRi @27 :Float32;
    uInvalidQualifierSafeLf @28 :UInt8;
    uInvalidQualifierSafeRi @29 :UInt8;
    uLaneValidQualDmc @30 :UInt8;
    uVisualValidQualifier @31 :UInt8;
    uLaneTypeLf @32 :UInt8;
    uLaneTypeRi @33 :UInt8;
    bConstructionSiteDtct @34 :UInt8;
    uOverallQualityLf @35 :UInt8;
    uOverallQualityRi @36 :UInt8;
    uCrvQualityLf @37 :UInt8;
    uCrvQualityRi @38 :UInt8;
    fAbdTimeStamp @39 :Float32;
    uRangeCheckQualifier @40 :UInt16;
    fFltQualityCntr @41 :Float32;
    uFltStatusCntr @42 :UInt8;
    uOutRangeCheckQualifier @43 :UInt16;
    uPercStraightDtct @44 :UInt8;
    uPercExitLf @45 :UInt8;
    uPercExitRi @46 :UInt8;
    bBridgePossibleUnCplLf @47 :UInt8;
    bBridgePossibleUnCplRi @48 :UInt8;
    uPercUpDownHillDtct @49 :UInt8;
    fLaneWidthUnCpl @50 :Float32;
    fLaneWidthCpl @51 :Float32;
    uOverallQualityUnCplLf @52 :UInt8;
    uOverallQualityUnCplRi @53 :UInt8;
    uOverallQualityCplLf @54 :UInt8;
    uOverallQualityCplRi @55 :UInt8;
    uBtfBridgeUnCpl @56 :UInt8;
    bLineMergeDtcRi @57 :UInt8;
    bLineMergeDtcLf @58 :UInt8;
}

struct SLccraDebugT {
    lccraDebugDelaytimeStr @0 :BusDelayTime;
    lccraDebugMiOsStr @1 :BusDebugMiOs;
    lccraDebugWeightStr @2 :BusWeight;
}

struct SLccraInReqT {
    lbpLeLnClthPosY0Met @0 :Float32;
    lbpLeLnClthHeadingRad @1 :Float32;
    lbpLeLnClthCrv1pm @2 :Float32;
    lbpLeLnClthCrvChng1pm2 @3 :Float32;
    lbpLeLnValidBool @4 :Bool;
    lbpRiLnClthPosY0Met @5 :Float32;
    lbpRiLnClthHeadingRad @6 :Float32;
    lbpRiLnClthCrv1pm @7 :Float32;
    lbpRiLnClthCrvChng1pm2 @8 :Float32;
    lbpRiLnValidBool @9 :Bool;
    alpLeLnClthPosY0Met @10 :Float32;
    alpLeLnClthHeadingRad @11 :Float32;
    alpLeLnClthCrv1pm @12 :Float32;
    alpLeLnClthCrvChng1pm2 @13 :Float32;
    alpLeLnValidBool @14 :Bool;
    alpRiLnClthPosY0Met @15 :Float32;
    alpRiLnClthHeadingRad @16 :Float32;
    alpRiLnClthCrv1pm @17 :Float32;
    alpRiLnClthCrvChng1pm2 @18 :Float32;
    alpRiLnValidBool @19 :Bool;
    fusionTargetObjectStr @20 :BusObject;
    vedEgoVelocityMps @21 :Float32;
    vedEgoYawRateRps @22 :Float32;
    vedEgoClthCrv1pm @23 :Float32;
    vedEgoClthCrvChng1pm2 @24 :Float32;
    abprLnWidthMet @25 :Float32;
    abprLnWidthValidBool @26 :Bool;
}

struct SLccraOutProT {
    lccraLeftSafeFlagBool @0 :Bool;
    lccraRightSafeFlagBool @1 :Bool;
    lccraLeftFrontSafeFlagBool @2 :Bool;
    lccraLeftRearSafeFlagBool @3 :Bool;
    lccraRightFrontSafeFlagBool @4 :Bool;
    lccraRightRearSafeFlagBool @5 :Bool;
    lccraLeftHighLightIdNu @6 :Int32;
    lccraRightHighLightIdNu @7 :Int32;
}

struct SLcfopsDebugT {
    uiVersionNum @0 :UInt32;
}

struct SLcfopsInReqT {
    inObjects @0 :LcfFusionObjectsArrayT;
    fEgoCurve1pm @1 :Float32;
}

struct SLcfopsOutProT {
    outObjects @0 :BusObject;
}

struct SLdpsaDebugT {
    ldpttLnBdryPstnXLfMi @0 :Float32;
    ldpttLnBdryPstnYLfMi @1 :Float32;
    ldpttLnBdryHeadAglLfRad @2 :Float32;
    ldpttLnBdryCurvLfReMi @3 :Float32;
    ldpttLnBdryCurvRateLfReMi2 @4 :Float32;
    ldpttLnBdryVldLengLfMi @5 :Float32;
    ldpttLnBdryPstnXRiMi @6 :Float32;
    ldpttLnBdryPstnYRiMi @7 :Float32;
    ldpttLnBdryHeadAglRiRad @8 :Float32;
    ldpttLnBdryCurvRiReMi @9 :Float32;
    ldpttLnBdryCurvRateRiReMi2 @10 :Float32;
    ldpttLnBdryVldLengRiMi @11 :Float32;
    ldpttLnBdryPstnXCentMi @12 :Float32;
    ldpttLnBdryPstnYCentMi @13 :Float32;
    ldpttLnBdryHeadAglCentRad @14 :Float32;
    ldpttLnBdryCurvCentReMi @15 :Float32;
    ldpttLnBdryCurvRateCentReMi2 @16 :Float32;
    ldpttLnBdryVldLengCentMi @17 :Float32;
    ldpttTgtPstnYLfMi @18 :Float32;
    ldpttTgtPstnYRiMi @19 :Float32;
    ldptvFTireAccMxMps2 @20 :Float32;
    ldptvFTireAccMnMps2 @21 :Float32;
    ldptvDstcYTgtAreaLfMi @22 :Float32;
    ldptvDstcYTgtAreaRiMi @23 :Float32;
    ldptvFctTgtDistYFct @24 :Float32;
    ldptvTrajPlanServQuFct @25 :Float32;
    ldptvPredTiCurvSec @26 :Float32;
    ldptvPredTiAglSec @27 :Float32;
    ldptvTiLmtEnDuraSec @28 :Float32;
    ldptvJerkLmtMxMps3 @29 :Float32;
    ldptvVeloXObstMps @30 :Float32;
    ldptvAccXObstMps2 @31 :Float32;
    ldptvDstcXObstMi @32 :Float32;
    ldptvDstcYObstMi @33 :Float32;
    ldptvLmtCurvMxReMi @34 :Float32;
    ldptvLmtCurvGradIncMxReMps @35 :Float32;
    ldptvLmtCurvGradDecMxReMps @36 :Float32;
    ldptvSnsTiStampSec @37 :Float32;
    ldptvSteWhlGradLmtFct @38 :Float32;
    ldptvSteWhlGradReS @39 :Float32;
    ldptvTrqRampGradReS @40 :Float32;
    ldptvMxTrqScalGradLmtFct @41 :Float32;
    ldptvMxTrqScalGradReS @42 :Float32;
    ldptvWeightEndTiFct @43 :Float32;
    ldptvPlanningHorizonSec @44 :Float32;
    ldptvDmcDeraLvlFct @45 :Float32;
    ldptvWidObstMi @46 :Float32;
    ldptvLmtCurvGradCtrlMxReMps @47 :Float32;
    ldpscCrvSensiDecayRiMi @48 :Float32;
    ldpscDlcThdMode2Mi @49 :Float32;
    ldpscDlcThdMode3Mi @50 :Float32;
    ldpscDlcThdMode1Mi @51 :Float32;
    ldpscDstcToLnTrsdMi @52 :Float32;
    ldpscDstcToLnTrsdCrvCpstnRiMi @53 :Float32;
    ldpscDstcToLnTrsdRiMi @54 :Float32;
    ldpdtCrvThdMaxRiReMi @55 :Float32;
    ldpdtCrvThdHystRiReMi @56 :Float32;
    ldpdtRawDstcToLnRiMi @57 :Float32;
    ldpdtDstcToLnRiMi @58 :Float32;
    ldpdtRawLatVehSpdRiMps @59 :Float32;
    ldpdtLatVehSpdRiMps @60 :Float32;
    ldpdtTiToLnRiSec @61 :Float32;
    ldpscTiToLnTrsdSec @62 :Float32;
    ldpvseMaxLatVelMps @63 :Float32;
    ldpdtCrvThdMaxLfReMi @64 :Float32;
    ldpdtCrvThdHystLfReMi @65 :Float32;
    ldpdtRawDstcToLnLfMi @66 :Float32;
    ldpdtDstcToLnLfMi @67 :Float32;
    ldpscDstcToLnTrsdCrvCpstnLfMi @68 :Float32;
    ldpscCrvSensiDecayLeMi @69 :Float32;
    ldpscDstcToLnTrsdLfMi @70 :Float32;
    ldpdtRawLatVehSpdLfMps @71 :Float32;
    ldpdtLatVehSpdLfMps @72 :Float32;
    ldpdtTiToLnLfSec @73 :Float32;
    ldpvseMaxCrvBySpdReMi @74 :Float32;
    ldpvseHystCrvBySpdReMi @75 :Float32;
    ldpscWrBlockTimeSec @76 :Float32;
    ldpscRampoutTimeSec @77 :Float32;
    ldpttRawLnBdryPstnYLfMi @78 :Float32;
    ldpttRawLnBdryPstnYRiMi @79 :Float32;
    ldpttTgtLatDstcRiMi @80 :Float32;
    ldpttTgtLatDstcLfMi @81 :Float32;
    ldpttRawBdryPstnYCentMi @82 :Float32;
    ldptvLatVelMps @83 :Float32;
    ldpscSuppValidDebug @84 :UInt16;
    ldptvTrajCtrlStSt @85 :UInt8;
    ldpdtCurveTypeRiSt @86 :UInt8;
    ldpvseSidCdtnLdpRiSt @87 :UInt8;
    ldpdtCurveTypeLeSt @88 :UInt8;
    ldpvseSidCdtnLdpLfSt @89 :UInt8;
    ldpvseIvldLdpSt @90 :UInt8;
    ldptvHighStatReqB @91 :UInt8;
    ldptvLatCpstnEnB @92 :UInt8;
    ldptvLmtEnB @93 :UInt8;
    ldptvTrigReplanB @94 :UInt8;
    ldpdtRdyTrigLdpB @95 :UInt8;
    ldpdtEnaSafetyB @96 :UInt8;
    ldpdtEnaByInVldQlfrRiB @97 :UInt8;
    ldpdtEnaByInVldQlfrSfRiB @98 :UInt8;
    ldpdtLnTrigVldRiB @99 :UInt8;
    ldpdtCclByInVldQlfrRiB @100 :UInt8;
    ldpdtLnCclVldRiB @101 :UInt8;
    ldpdtLnMakVldRiB @102 :UInt8;
    ldpscRawTrigByDlcRiB @103 :UInt8;
    ldpscEnaTlcTrigRiB @104 :UInt8;
    ldpscRawTrigByTlcRiB @105 :UInt8;
    ldpscDlyTrigByTlcRiB @106 :UInt8;
    ldpscEnaLdwTrigRiB @107 :UInt8;
    ldpscRstLdwTrigRiB @108 :UInt8;
    ldpscHoldLdwTrigRiB @109 :UInt8;
    ldpscResetForSafeRiB @110 :UInt8;
    ldpscSetForSafeRiB @111 :UInt8;
    ldpvseEdgeRiseTurnSglRiB @112 :UInt8;
    ldpvseTrnSglRiB @113 :UInt8;
    ldpvseRdyTrigLdwB @114 :UInt8;
    ldpvseVehLatSpdVldRiB @115 :UInt8;
    ldpscTrigBySideCondRiB @116 :UInt8;
    ldpscTrigByPrjSpecRiB @117 :UInt8;
    ldpscTrigRiB @118 :UInt8;
    ldpdtEnaByCstruSiteLfB @119 :UInt8;
    ldpdtEnaByInVldQlfrLfB @120 :UInt8;
    ldpdtEnaByInVldQlfrSfLfB @121 :UInt8;
    ldpdtLnTrigVldLfB @122 :UInt8;
    ldpdtCclByInVldQlfrLfB @123 :UInt8;
    ldpdtLnCclVldLfB @124 :UInt8;
    ldpdtLnMakVldLfB @125 :UInt8;
    ldpscRawTrigByDlcLfB @126 :UInt8;
    ldpscEnaTlcTrigLfB @127 :UInt8;
    ldpscRawTrigByTlcLfB @128 :UInt8;
    ldpscDlyTrigByTlcLfB @129 :UInt8;
    ldpscEnaLdwTrigLfB @130 :UInt8;
    ldpscRstLdwTrigLfB @131 :UInt8;
    ldpscHoldLdwTrigLfB @132 :UInt8;
    ldpscResetForSafeLfB @133 :UInt8;
    ldpscSetForSafeLfB @134 :UInt8;
    ldpvseEdgeRiseTurnSglLfB @135 :UInt8;
    ldpvseTrnSglLfB @136 :UInt8;
    ldpvseVehLatSpdVldLfB @137 :UInt8;
    ldpscTrigBySideCondLfB @138 :UInt8;
    ldpscTrigByPrjSpecLfB @139 :UInt8;
    ldpscTrigLfB @140 :UInt8;
    ldpscTrigB @141 :UInt8;
    ldpscEnaDgrSideB @142 :UInt8;
    ldpscFnsByDgrStLfB @143 :UInt8;
    ldpscFnsByLatDistLfB @144 :UInt8;
    ldpscFnsByHeadingLfB @145 :UInt8;
    ldpscFnsByLatSpdLfB @146 :UInt8;
    ldpscDgrFnsLfB @147 :UInt8;
    ldpscFnsByDgrStRiB @148 :UInt8;
    ldpscFnsByLatDistRiB @149 :UInt8;
    ldpscFnsByHeadingRiB @150 :UInt8;
    ldpscFnsByLatSpdRiB @151 :UInt8;
    ldpscDgrFnsRiB @152 :UInt8;
    ldpscMinLdwBySysStB @153 :UInt8;
    ldpscEdgeRiseForMinLdwB @154 :UInt8;
    ldpscHoldForMinLdwB @155 :UInt8;
    ldpscFlagMinTimeLdwB @156 :UInt8;
    ldpscDgrFnsB @157 :UInt8;
    ldpscAbortBySpecificB @158 :UInt8;
    ldpscAbortByVehStB @159 :UInt8;
    ldpscAbortByDrvStB @160 :UInt8;
    ldpscAbortByCtrlStB @161 :UInt8;
    ldpscAbortBySysStB @162 :UInt8;
    ldpscAbortByAvlStB @163 :UInt8;
    ldpscAbortByPrjSpecB @164 :UInt8;
    ldpscAbortB @165 :UInt8;
    ldpscStrgRdyB @166 :UInt8;
    ldpscSuppBySpecificB @167 :UInt8;
    ldpscSuppByVehStB @168 :UInt8;
    ldpscSuppByDrvStB @169 :UInt8;
    ldpscSuppByCtrlStB @170 :UInt8;
    ldpscSuppBySysStB @171 :UInt8;
    ldpscSuppyByAvlStB @172 :UInt8;
    ldpscSuppPrjSpecB @173 :UInt8;
    ldpscSuppresionB @174 :UInt8;
    ldpscWeakRdyBySpecificB @175 :UInt8;
    ldpscWeakRdyByVehStB @176 :UInt8;
    ldpscWeakRdyByDrvStB @177 :UInt8;
    ldpscWeakRdyByCtrlStB @178 :UInt8;
    ldpscWeakRdyBySysStB @179 :UInt8;
    ldpscWeakRdyByAvlStB @180 :UInt8;
    ldpscWeakRdyByPrjSpecB @181 :UInt8;
    ldpscBlockTimeBySysOutB @182 :UInt8;
    ldpscRawBlockTimeByRampOutB @183 :UInt8;
    ldpscBlockTimeByRampOutB @184 :UInt8;
    ldpscBlockTimeB @185 :UInt8;
    ldpscWkRdyB @186 :UInt8;
    ldpscCancelBySpecificB @187 :UInt8;
    ldpscCancelByVehStB @188 :UInt8;
    ldpscCancelByDrvStB @189 :UInt8;
    ldpscCancelByCtrlStB @190 :UInt8;
    ldpscCancelBySysStB @191 :UInt8;
    ldpscCancelByAvlStB @192 :UInt8;
    ldpscCancelByPrjSpecB @193 :UInt8;
    ldpscMaxDurationBySysStB @194 :UInt8;
    ldpscEdgRiseForSysStB @195 :UInt8;
    ldpscMaxDurationByStDlyB @196 :UInt8;
    ldpscTiWarmMxB @197 :UInt8;
    ldpscErrSideByTrigLfB @198 :UInt8;
    ldpscErrSideBySideCondLfB @199 :UInt8;
    ldpscErrSidByPrjSpecLfB @200 :UInt8;
    ldpscErrSidCdtnLfB @201 :UInt8;
    ldpscSideCondByDgrLfB @202 :UInt8;
    ldpscCanelBySideLfB @203 :UInt8;
    ldpscSideCondByDgrRiB @204 :UInt8;
    ldpscErrSideByTrigRiB @205 :UInt8;
    ldpscErrSideBySideCondRiB @206 :UInt8;
    ldpscErrSidByPrjSpecRiB @207 :UInt8;
    ldpscErrSidCdtnRiB @208 :UInt8;
    ldpscCanelBySideRiB @209 :UInt8;
    ldpscErrSidCdtnB @210 :UInt8;
    ldpscCLatDevByDlcLfB @211 :UInt8;
    ldpscCLatDevByDgrLfB @212 :UInt8;
    ldpscCclLatDevLfB @213 :UInt8;
    ldpscCLatDevByDlcRiB @214 :UInt8;
    ldpscCLatDevByDgrRiB @215 :UInt8;
    ldpscCclLatDevRiB @216 :UInt8;
    ldpscCclLatDevB @217 :UInt8;
    ldpscCancelB @218 :UInt8;
    ldpscSuppressionB @219 :UInt8;
    ldpvseTgtCntrByLnWidthB @220 :UInt8;
    ldpvseTgtCntrLnEnB @221 :UInt8;
    ldptvCurvInnerB @222 :UInt8;
    ldptvCurvOuterB @223 :UInt8;
    ldpscDegradationB @224 :UInt8;
    ldpscDegradationEdgeRiseB @225 :UInt8;
    ldpscDegrB @226 :UInt8;
    ldpscDlyTiOfTiToLnRiMnSec @227 :Float32;
    ldpscHdTiTrigRiSec @228 :Float32;
    ldpvseHodTiTrnSglRiSec @229 :Float32;
    ldpscDlyTiOfTiToLnLfMnSec @230 :Float32;
    ldpscHdTiTrigLfSec @231 :Float32;
    ldpvseHodTiTrnSglLfSec @232 :Float32;
    ldpscHdTiWarmingSec @233 :Float32;
    ldpscDlyTiTgtFnsSec @234 :Float32;
    ldpscHdTiFnsSec @235 :Float32;
    ldpscHdTiWarmMxSec @236 :Float32;
    ldpttLwLnBdryPstnXLfMi @237 :Float32;
    ldpttLstLnBdryPstnXLfMi @238 :Float32;
    ldpttLstLnBdryVldLengLfMi @239 :Float32;
    ldpttLwLnBdryVldLengLfMi @240 :Float32;
    ldpttLstLnBdryPstnYLfMi @241 :Float32;
    ldpttLstLnWidCalcMi @242 :Float32;
    ldpttLwLnBdryPstnYLfMi @243 :Float32;
    ldpttLwLnBdryPstnYRiMi @244 :Float32;
    ldpttLwLnBdryPstnXRiMi @245 :Float32;
    ldpttLwLnBdryHeadAglRiRad @246 :Float32;
    ldpttLwLnBdryCurvRiReMi @247 :Float32;
    ldpttLwLnBdryCurvRateRiReMi2 @248 :Float32;
    ldpttLwLnBdryVldLengRiMi @249 :Float32;
    ldpttLstTgtLatDstcRiMi @250 :Float32;
    ldpttLstMxTgtLatDevRiMi @251 :Float32;
    ldpttLstTgtLatDevRiMi @252 :Float32;
    ldpttLstTgtLatDevLfMi @253 :Float32;
    ldpttLstTgtLatDstcLfMi @254 :Float32;
    ldpttLstMxTgtLatDevLfMi @255 :Float32;
    ldpttLwLnBdryPstnYCentMi @256 :Float32;
    ldpttLstLnBdryPstnYCentMi @257 :Float32;
    ldpttLstLnBdryCurvCentReMi @258 :Float32;
    ldpttLwLnBdryCurvCentReMi @259 :Float32;
    ldptvLstPlanningHorizonSec @260 :Float32;
    ldptvLstWeightEndTiFct @261 :Float32;
    ldpttLwLnBdryHeadAglCentRad @262 :Float32;
    ldpttLstLnBdryHeadAglCentRad @263 :Float32;
    ldpttLwLnBdryCurvRateCentReMi2 @264 :Float32;
    ldpttLstLnBdryCurvRateCentReMi2 @265 :Float32;
    ldpttLwLnBdryPstnXCentMi @266 :Float32;
    ldpttLstLnBdryPstnXCentMi @267 :Float32;
    ldpttLwLnBdryVldLengCentMi @268 :Float32;
    ldpttLstLnBdryVldLengCentMi @269 :Float32;
    ldptvLstSteWhlGradReS @270 :Float32;
    ldptvLstDmcDeraLvlFct @271 :Float32;
    ldpttLstLnBdryHeadAglLfRad @272 :Float32;
    ldpttLwLnBdryHeadAglLfRad @273 :Float32;
    ldpttLstLnBdryCurvLfReMi @274 :Float32;
    ldpttLwLnBdryCurvLfReMi @275 :Float32;
    ldpttLstLnBdryCurvRateLfReMi2 @276 :Float32;
    ldpttLwLnBdryCurvRateLfReMi2 @277 :Float32;
    ldpscHdTiDegrSec @278 :Float32;
    ldpscLstPrevDgrSideSt @279 :UInt8;
    ldpscDgrSideOldSt @280 :UInt8;
    ldpdtUHysCltdCurvVldRiB @281 :UInt8;
    ldpdtBHysHeadAglTrigVldRiB @282 :UInt8;
    ldpdtUHysHeadAglCclVldRiB @283 :UInt8;
    ldpscHdTiTrigRiEnB @284 :UInt8;
    ldpscDisTrigRiB @285 :UInt8;
    ldpvseEdgeRisTrnSglLfB @286 :UInt8;
    ldpvseBHysLatVehSpdVldRiB @287 :UInt8;
    ldpvseUHysLatVehSpdVldRiB @288 :UInt8;
    ldpdtUHysCltdCurvVldLfB @289 :UInt8;
    ldpdtBHysHeadAglTrigVldLfB @290 :UInt8;
    ldpdtUHysHeadAglCclVldLfB @291 :UInt8;
    ldpscHdTiTrigLfEnB @292 :UInt8;
    ldpscDisTrigLfB @293 :UInt8;
    ldpvseEdgeRisTrnSglRiB @294 :UInt8;
    ldpvseBHysLatVehSpdVldLfB @295 :UInt8;
    ldpvseUHysLatVehSpdVldLfB @296 :UInt8;
    ldpscEdgeRisWarmingB @297 :UInt8;
    ldpvseBHysSpdVehB @298 :UInt8;
    ldpvseUHysSteAglB @299 :UInt8;
    ldpvseUHysSteAglSpdB @300 :UInt8;
    ldpvseBHysAccVehXB @301 :UInt8;
    ldpvseBHysAccVehYB @302 :UInt8;
    ldpvseUHysVehCurvB @303 :UInt8;
    ldpvseBHysLnWidB @304 :UInt8;
    ldpscEdgeRisFnsB @305 :UInt8;
    ldpscEdgeRisWarmMxB @306 :UInt8;
    ldpttCtrlIniEnB @307 :UInt8;
    ldpttLstControlB @308 :UInt8;
    ldptvLstCtrlSt @309 :UInt8;
    ldpscEdgeRisDegrB @310 :UInt8;
    ldpscDegrOldB @311 :UInt8;
    ldpscSysOldSt @312 :UInt8;
}

struct SLdpsaInputT {
    ldpsaiVehSpdActuMps @0 :Float32;
    ldpsaiSpdVelShowKmph @1 :Float32;
    ldpsaiVehAccSpdXNpkg @2 :Float32;
    ldpsaiVehAccSpdYNpkg @3 :Float32;
    ldpsaiVehCurvReMi @4 :Float32;
    ldpsaiTrnSglSt @5 :UInt8;
    ldpsaiWheSteAglDgr @6 :Float32;
    ldpsaiSteAglSpdDgpm @7 :Float32;
    ldpsaiLdpSwitchEnB @8 :UInt8;
    ldpsaiLdpModSt @9 :UInt8;
    ldpsaiLdpErrCdtnB @10 :UInt8;
    ldpsaiDtctLnChagB @11 :UInt8;
    ldpsaiLnWidCalcMi @12 :Float32;
    ldpsaiPstnXLfMi @13 :Float32;
    ldpsaiPstnXRiMi @14 :Float32;
    ldpsaiPstnYLfMi @15 :Float32;
    ldpsaiPstnYSafeLfMi @16 :Float32;
    ldpsaiPstnYRiMi @17 :Float32;
    ldpsaiPstnYSafeRiMi @18 :Float32;
    ldpsaiHeadAglLfRad @19 :Float32;
    ldpsaiHeadAglSafeLfRad @20 :Float32;
    ldpsaiHeadAglRiRad @21 :Float32;
    ldpsaiHeadAglSafeRiRad @22 :Float32;
    ldpsaiCurvLfReMi @23 :Float32;
    ldpsaiCurvSafeLfReMi @24 :Float32;
    ldpsaiCurvRiReMi @25 :Float32;
    ldpsaiCurvSafeRiReMi @26 :Float32;
    ldpsaiCurvRateLfReMi2 @27 :Float32;
    ldpsaiCurvRateRiReMi2 @28 :Float32;
    ldpsaiVldLengLfMi @29 :Float32;
    ldpsaiVldLengRiMi @30 :Float32;
    ldpsaiIvldLnSafeLfSt @31 :UInt8;
    ldpsaiLnIVldLfSt @32 :UInt16;
    ldpsaiIvldLnSafeRiSt @33 :UInt8;
    ldpsaiLnIVldRiSt @34 :UInt16;
    ldpsaiVehStIvldSt @35 :UInt16;
    ldpsaiIvldStDrvSt @36 :UInt8;
    ldpsaiCtrlStEnSt @37 :UInt8;
    ldpsaiStErrorSt @38 :UInt8;
    ldpsaiCtrlStNoAvlbSt @39 :UInt8;
    ldpsaiPrjSpecQuSt @40 :UInt8;
    ldpsaiDtctCstruSiteB @41 :UInt8;
    ldpsaiCycleTimeSec @42 :Float32;
    ldpsaiAbdTimeStampSec @43 :Float32;
    ldpsaiPstnYCentMi @44 :Float32;
    ldpsaiVehYawRateRps @45 :Float32;
    ldpsaiAebActiveB @46 :UInt8;
    nvramLdpSwitchB @47 :UInt8;
    nvramLdpStartupSpdKmph @48 :Float32;
    ldpsaiVehStartupSpdHmiKmph @49 :Float32;
}

struct SLdpsaOutputT {
    ldpscDgrSideSt @0 :UInt8;
    ldpscRdyToTrigB @1 :UInt8;
    ldpscSysOutSt @2 :UInt8;
    ldpdtLnPstnLfMi @3 :Float32;
    ldpdtLnPstnRiMi @4 :Float32;
    ldpdtLnHeadLfRad @5 :Float32;
    ldpdtLnHeadRiRad @6 :Float32;
    ldpdtLnCltdCurvLfReMi @7 :Float32;
    ldpdtLnCltdCurvRiReMi @8 :Float32;
    ldpvseNvramVehStartupSpdKmph @9 :Float32;
    ldpscNvramldpSwitchB @10 :UInt8;
}

struct SLdwsaDebugT {
    lddtLnHeadLfRad @0 :Float32;
    lddtRawLatVehSpdLfMps @1 :Float32;
    lddtCrvThdMaxLfReMi @2 :Float32;
    lddtCrvThdHystLfReMi @3 :Float32;
    lddtLnCltdCurvLfReMi @4 :Float32;
    lddtLatVehSpdLfMps @5 :Float32;
    ldvseMaxLatVelMps @6 :Float32;
    lddtCrvThdMaxRiReMi @7 :Float32;
    lddtCrvThdHystRiReMi @8 :Float32;
    lddtLnCltdCurvRiReMi @9 :Float32;
    lddtLnHeadRiRad @10 :Float32;
    lddtRawLatVehSpdRiMps @11 :Float32;
    lddtLatVehSpdRiMps @12 :Float32;
    ldvseMaxCrvBySpdReMi @13 :Float32;
    ldvseHystCrvBySpdReMi @14 :Float32;
    lddtLnPstnRiMi @15 :Float32;
    lddtRawDstcToLnRiMi @16 :Float32;
    lddtDstcToLnRiMi @17 :Float32;
    ldwcDlcThdMode2Mi @18 :Float32;
    ldwcDlcThdMode3Mi @19 :Float32;
    ldwcDlcThdMode1Mi @20 :Float32;
    ldwcCrrctByLnWidthFct @21 :Float32;
    ldwcDstcToLnTrsdMi @22 :Float32;
    ldwcDstcToLnTrsdCrvCpstnRiMi @23 :Float32;
    ldwcDstcToLnTrsdRiMi @24 :Float32;
    lddtTiToLnRiSec @25 :Float32;
    ldwcTiToLnTrsdSec @26 :Float32;
    lddtLnPstnLfMi @27 :Float32;
    lddtRawDstcToLnLfMi @28 :Float32;
    lddtDstcToLnLfMi @29 :Float32;
    ldwcDstcToLnTrsdCrvCpstnLfMi @30 :Float32;
    ldwcDstcToLnTrsdLfMi @31 :Float32;
    lddtTiToLnLfSec @32 :Float32;
    ldvseSidCdtnLdwLfSt @33 :UInt8;
    ldvseSidCdtnLdwRiSt @34 :UInt8;
    ldvseIvldLdwSt @35 :UInt8;
    ldwcSuppValidDebug @36 :UInt16;
    lddtRdyTrigLdwB @37 :UInt8;
    lddtEnaSafetyB @38 :UInt8;
    lddtEnaByCstruSiteLfB @39 :UInt8;
    lddtEnaByInVldQlfrLfB @40 :UInt8;
    lddtEnaByInVldQlfrSfLfB @41 :UInt8;
    lddtLnTrigVldLfB @42 :UInt8;
    lddtCclByInVldQlfrLfB @43 :UInt8;
    lddtLnCclVldLfB @44 :UInt8;
    lddtLnMakVldLfB @45 :UInt8;
    ldvseRdyTrigLdwB @46 :UInt8;
    ldvseVehLatSpdVldLfB @47 :UInt8;
    ldvseTrnSglLfB @48 :UInt8;
    lddtEnaByInVldQlfrRiB @49 :UInt8;
    lddtEnaByInVldQlfrSfRiB @50 :UInt8;
    lddtLnTrigVldRiB @51 :UInt8;
    lddtCclByInVldQlfrRiB @52 :UInt8;
    lddtLnCclVldRiB @53 :UInt8;
    lddtLnMakVldRiB @54 :UInt8;
    ldvseVehLatSpdVldRiB @55 :UInt8;
    ldvseTrnSglRiB @56 :UInt8;
    ldwcFnsByLatSpdLfB @57 :UInt8;
    ldwcEnaDgrSideB @58 :UInt8;
    ldwcRawTrigByDlcRiB @59 :UInt8;
    ldwcEnaTlcTrigRiB @60 :UInt8;
    ldwcRawTrigByTlcRiB @61 :UInt8;
    ldwcDlyTrigByTlcRiB @62 :UInt8;
    ldwcEnaLdwTrigRiB @63 :UInt8;
    ldwcRstTlcTrigRiB @64 :UInt8;
    ldwcResetForSafeRiB @65 :UInt8;
    ldwcSetForSafeRiB @66 :UInt8;
    ldwcTrigBySideCondRiB @67 :UInt8;
    ldwcTrigByPrjSpecRiB @68 :UInt8;
    ldwcTrigRiB @69 :UInt8;
    ldwcRawTrigByDlcLfB @70 :UInt8;
    ldwcEnaTlcTrigLfB @71 :UInt8;
    ldwcRawTrigByTlcLfB @72 :UInt8;
    ldwcDlyTrigByTlcLfB @73 :UInt8;
    ldwcEnaLdwTrigLfB @74 :UInt8;
    ldwcRstTlcTrigLfB @75 :UInt8;
    ldwcResetForSafeLfB @76 :UInt8;
    ldwcSetForSafeLfB @77 :UInt8;
    ldwcTrigBySideCondLfB @78 :UInt8;
    ldwcTrigByPrjSpecLfB @79 :UInt8;
    ldwcTrigLfB @80 :UInt8;
    ldwcFnsByDgrStLfB @81 :UInt8;
    ldwcFnsByLatDistLfB @82 :UInt8;
    ldwcFnsByHeadingLfB @83 :UInt8;
    ldwcDgrFnsLfB @84 :UInt8;
    ldwcFnsByDgrStRiB @85 :UInt8;
    ldwcFnsByLatDistRiB @86 :UInt8;
    ldwcFnsByHeadingRiB @87 :UInt8;
    ldwcFnsByLatSpdRiB @88 :UInt8;
    ldwcDgrFnsRiB @89 :UInt8;
    ldwcMinLdwBySysStB @90 :UInt8;
    ldwcEdgeRiseForMinLdwB @91 :UInt8;
    ldwcHoldForMinLdwB @92 :UInt8;
    ldwcFlagMinTimeLdwB @93 :UInt8;
    ldwcDgrFnsB @94 :UInt8;
    ldwcCancelBySpecificB @95 :UInt8;
    ldwcCancelByVehStB @96 :UInt8;
    ldwcCancelByDrvStB @97 :UInt8;
    ldwcCancelByCtrlStB @98 :UInt8;
    ldwcCancelBySysStB @99 :UInt8;
    ldwcCancelByAvlStB @100 :UInt8;
    ldwcCancelByPrjSpecB @101 :UInt8;
    ldwcMaxDurationBySysStB @102 :UInt8;
    ldwcEdgRiseForSysStB @103 :UInt8;
    ldwcMaxDurationByStDlyB @104 :UInt8;
    ldwcTiWarmMxB @105 :UInt8;
    ldwcErrSideByTrigLfB @106 :UInt8;
    ldwcErrSideBySideCondLfB @107 :UInt8;
    ldwcErrSidByPrjSpecLfB @108 :UInt8;
    ldwcErrSidCdtnLfB @109 :UInt8;
    ldwcSideCondByDgrLfB @110 :UInt8;
    ldwcCanelBySideLfB @111 :UInt8;
    ldwcSideCondByDgrRiB @112 :UInt8;
    ldwcErrSideByTrigRiB @113 :UInt8;
    ldwcErrSideBySideCondRiB @114 :UInt8;
    ldwcErrSidByPrjSpecRiB @115 :UInt8;
    ldwcErrSidCdtnRiB @116 :UInt8;
    ldwcCanelBySideRiB @117 :UInt8;
    ldwcErrSidCdtnB @118 :UInt8;
    ldwcCLatDevByDlcLfB @119 :UInt8;
    ldwcCLatDevByDgrLfB @120 :UInt8;
    ldwcCclLatDevLfB @121 :UInt8;
    ldwcCLatDevByDlcRiB @122 :UInt8;
    ldwcCLatDevByDgrRiB @123 :UInt8;
    ldwcCclLatDevRiB @124 :UInt8;
    ldwcCclLatDevB @125 :UInt8;
    ldwcCancelB @126 :UInt8;
    ldwcAbortBySpecificB @127 :UInt8;
    ldwcAbortByVehStB @128 :UInt8;
    ldwcAbortByDrvStB @129 :UInt8;
    ldwcAbortByCtrlStB @130 :UInt8;
    ldwcAbortBySysStB @131 :UInt8;
    ldwcAbortByAvlStB @132 :UInt8;
    ldwcAbortByPrjSpecB @133 :UInt8;
    ldwcAbortB @134 :UInt8;
    ldwcStrgRdyBySpecificB @135 :UInt8;
    ldwcStrgRdyByVehStB @136 :UInt8;
    ldwcStrgRdyByDrvStB @137 :UInt8;
    ldwcStrgRdyByCtrlStB @138 :UInt8;
    ldwcStrgRdyBySysStB @139 :UInt8;
    ldwcStrgRdyByAvlStB @140 :UInt8;
    ldwcStrgRdyByPrjSpecB @141 :UInt8;
    ldwcStrgRdyB @142 :UInt8;
    ldwcDegradationB @143 :UInt8;
    ldwcDegradationEdgeRiseB @144 :UInt8;
    ldwcDegrB @145 :UInt8;
    ldwcTrigB @146 :UInt8;
    ldwcSuppBySpecificB @147 :UInt8;
    ldwcSuppByVehStB @148 :UInt8;
    ldwcSuppByDrvStB @149 :UInt8;
    ldwcSuppByCtrlStB @150 :UInt8;
    ldwcSuppBySysStB @151 :UInt8;
    ldwcSuppyByAvlStB @152 :UInt8;
    ldwcSuppPrjSpecB @153 :UInt8;
    ldwcSuppresionB @154 :UInt8;
    ldwcWeakRdyBySpecificB @155 :UInt8;
    ldwcWeakRdyByVehStB @156 :UInt8;
    ldwcWeakRdyByDrvStB @157 :UInt8;
    ldwcWeakRdyByCtrlStB @158 :UInt8;
    ldwcWeakRdyBySysStB @159 :UInt8;
    ldwcWeakRdyByAvlStB @160 :UInt8;
    ldwcWeakRdyByPrjSpecB @161 :UInt8;
    ldwcBlockTimeBySysOutB @162 :UInt8;
    ldwcRawBlockTimeByRampOutB @163 :UInt8;
    ldwcBlockTimeByRampOutB @164 :UInt8;
    ldwcBlockTimeB @165 :UInt8;
    ldwcWkRdyB @166 :UInt8;
    ldwcSuppressionB @167 :UInt8;
    ldvseHodTiTrnSglLfSec @168 :Float32;
    ldvseHodTiTrnSglRiSec @169 :Float32;
    ldwcDlyTiOfTiToLnRiMnSec @170 :Float32;
    ldwcHdTiTrigRiSec @171 :Float32;
    ldwcDlyTiOfTiToLnLfMnSec @172 :Float32;
    ldwcHdTiTrigLfSec @173 :Float32;
    ldwcHdTiWarmingSec @174 :Float32;
    ldwcDlyTiTgtFnsSec @175 :Float32;
    ldwcHdTiWarmMxSec @176 :Float32;
    ldwcHdTiDegrSec @177 :Float32;
    ldwcHdTiFnsSec @178 :Float32;
    ldwcDgrSideOldSt @179 :UInt8;
    lddtUHysCltdCurvVldLfB @180 :UInt8;
    lddtBHysHeadAglTrigVldLfB @181 :UInt8;
    lddtUHysHeadAglCclVldLfB @182 :UInt8;
    ldvseBHysLatVehSpdVldLfB @183 :UInt8;
    ldvseUHysLatVehSpdVldLfB @184 :UInt8;
    ldvseEdgeRisTrnSglRiB @185 :UInt8;
    lddtUHysCltdCurvVldRiB @186 :UInt8;
    lddtBHysHeadAglTrigVldRiB @187 :UInt8;
    lddtUHysHeadAglCclVldRiB @188 :UInt8;
    ldvseBHysLatVehSpdVldRiB @189 :UInt8;
    ldvseUHysLatVehSpdVldRiB @190 :UInt8;
    ldvseEdgeRisTrnSglLfB @191 :UInt8;
    ldvseUHysSteAglB @192 :UInt8;
    ldvseBHysSpdVehB @193 :UInt8;
    ldvseUHysSteAglSpdB @194 :UInt8;
    ldvseBHysAccVehXB @195 :UInt8;
    ldvseBHysAccVehYB @196 :UInt8;
    ldvseUHysVehCurvB @197 :UInt8;
    ldvseBHysLnWidB @198 :UInt8;
    ldwcHdTiTrigRiEnB @199 :UInt8;
    ldwcDisTrigRiB @200 :UInt8;
    ldwcHdTiTrigLfEnB @201 :UInt8;
    ldwcDisTrigLfB @202 :UInt8;
    ldwcEdgeRisWarmingB @203 :UInt8;
    ldwcEdgeRisWarmMxB @204 :UInt8;
    ldwcEdgeRisDegrB @205 :UInt8;
    ldwcDegrOldB @206 :UInt8;
    ldwcEdgeRisFnsB @207 :UInt8;
}

struct SLdwsaInReqT {
    ldwsaiVehSpdActuMps @0 :Float32;
    ldwsaiSpdVelShowKmph @1 :Float32;
    ldwsaiVehAccSpdXNpkg @2 :Float32;
    ldwsaiVehAccSpdYNpkg @3 :Float32;
    ldwsaiVehCurvReMi @4 :Float32;
    ldwsaiTrnSglSt @5 :UInt8;
    ldwsaiWheSteAglDgr @6 :Float32;
    ldwsaiSteAglSpdDgpm @7 :Float32;
    ldwsaiLdwSwitchEnB @8 :UInt8;
    ldwsaiLdwModSt @9 :UInt8;
    ldwsaiLdwErrCdtnB @10 :UInt8;
    ldwsaiDtctLnChagB @11 :UInt8;
    ldwsaiLnWidCalcMi @12 :Float32;
    ldwsaiPstnYLfMi @13 :Float32;
    ldwsaiPstnYSafeLfMi @14 :Float32;
    ldwsaiPstnYRiMi @15 :Float32;
    ldwsaiPstnYSafeRiMi @16 :Float32;
    ldwsaiHeadAglLfRad @17 :Float32;
    ldwsaiHeadAglSafeLfRad @18 :Float32;
    ldwsaiHeadAglRiRad @19 :Float32;
    ldwsaiHeadAglSafeRiRad @20 :Float32;
    ldwsaiCurvLfReMi @21 :Float32;
    ldwsaiCurvSafeLfReMi @22 :Float32;
    ldwsaiCurvRiReMi @23 :Float32;
    ldwsaiCurvSafeRiReMi @24 :Float32;
    ldwsaiIvldLnSafeLfSt @25 :UInt8;
    ldwsaiLnIVldLfSt @26 :UInt16;
    ldwsaiIvldLnSafeRiSt @27 :UInt8;
    ldwsaiLnIVldRiSt @28 :UInt16;
    ldwsaiVehStIvldSt @29 :UInt16;
    ldwsaiIvldStDrvSt @30 :UInt8;
    ldwsaiCtrlStEnSt @31 :UInt8;
    ldwsaiStErrorSt @32 :UInt8;
    ldwsaiCtrlStNoAvlbSt @33 :UInt8;
    ldwsaiPrjSpecQuSt @34 :UInt8;
    ldwsaiDtctCstruSiteB @35 :UInt8;
    ldwsaiCycleTimeSec @36 :Float32;
    ldwsaiVehYawRateRps @37 :Float32;
    ldwsaiAebActiveB @38 :UInt8;
    nvramLdwSwitchB @39 :UInt8;
    nvramLdwStartupSpdKmph @40 :Float32;
    ldwsaiVehStartupSpdHmiKmph @41 :Float32;
    ldwsaiLdpSwitchOnB @42 :UInt8;
}

struct SLdwsaOutProT {
    ldwcDgrSideSt @0 :UInt8;
    ldwcRdyToTrigB @1 :UInt8;
    ldwcSysOutSt @2 :UInt8;
    ldvseNvramVehStartupSpdKmph @3 :Float32;
    ldwcNvramldwSwitchB @4 :UInt8;
}

struct SLfpDebugT {
    sLwvInput @0 :SLwvInputT;
    sLwvOutput @1 :SLwvOutputT;
    sLwvDebug @2 :SLwvDebugT;
    sKlmInput @3 :SKlmInputT;
    sKlmOutput @4 :SKlmOutputT;
    sKlmDebug @5 :SKlmDebugT;
    sCfvInput @6 :SCfvInputT;
    sCfvOutput @7 :SCfvOutputT;
    sCfvDebug @8 :SCfvDebugT;
}

struct SLfpInputT {
    fVehVelX @0 :Float32;
    bNewCorridorValid @1 :UInt8;
    bUpDownHillDegrade @2 :UInt8;
    fPosY0Lf @3 :Float32;
    fHeadingLf @4 :Float32;
    uQualityLf @5 :UInt8;
    uRangeCheckQualifierLf @6 :UInt16;
    bNotAvailableLf @7 :UInt8;
    bDistYStepDtctLf @8 :UInt8;
    bLengthInvalidLf @9 :UInt8;
    bBridgeUnCplLf @10 :UInt8;
    bQualityNotValidLf @11 :UInt8;
    fPosY0Ri @12 :Float32;
    fHeadingRi @13 :Float32;
    uQualityRi @14 :UInt8;
    uRangeCheckQualifierRi @15 :UInt16;
    bNotAvailableRi @16 :UInt8;
    bDistYStepDtctRi @17 :UInt8;
    bLengthInvalidRi @18 :UInt8;
    bBridgeUnCplRi @19 :UInt8;
    bQualityNotValidRi @20 :UInt8;
    bValidKlmFltCntr @21 :UInt8;
    fOverallQualityLf @22 :Float32;
    fOverallQualityRi @23 :Float32;
    fPosY0UlpLf @24 :Float32;
    fHeadingUlpLf @25 :Float32;
    fCrvUlpLf @26 :Float32;
    fCrvRateUlpLf @27 :Float32;
    fValidLengthUlpLf @28 :Float32;
    fPosY0UlpRi @29 :Float32;
    fHeadingUlpRi @30 :Float32;
    fCrvUlpRi @31 :Float32;
    fCrvRateUlpRi @32 :Float32;
    fValidLengthUlpRi @33 :Float32;
    fLaneWidth @34 :Float32;
    fVehYawRateStd @35 :Float32;
    fVehYawRate @36 :Float32;
    bLaneChangeDtct @37 :UInt8;
    fStraightDtct @38 :Float32;
}

struct SLfpOutputT {
    uLaneValidQualifier @0 :UInt8;
    uBridgePossible @1 :UInt8;
    bKalmanValidLf @2 :UInt8;
    bDistYStepDebouncedLf @3 :UInt8;
    bHeadingStepDebouncedLf @4 :UInt8;
    bCrvStepDebouncedLf @5 :UInt8;
    bCrvRateStepDebouncedLf @6 :UInt8;
    bKalmanValidCntr @7 :UInt8;
    bDistYStepDebouncedCntr @8 :UInt8;
    bHeadingStepDebouncedCntr @9 :UInt8;
    bCrvStepDebouncedCntr @10 :UInt8;
    bCrvRateStepDebouncedCntr @11 :UInt8;
    bKalmanValidRi @12 :UInt8;
    bDistYStepDebouncedRi @13 :UInt8;
    bHeadingStepDebouncedRi @14 :UInt8;
    bCrvStepDebouncedRi @15 :UInt8;
    bCrvRateStepDebouncedRi @16 :UInt8;
    fFltQualityCntr @17 :Float32;
    uFltStatusCntr @18 :UInt8;
    fPosX0FltLf @19 :Float32;
    fPosY0FltLf @20 :Float32;
    fHeadingFltLf @21 :Float32;
    fCrvFltLf @22 :Float32;
    fCrvRateFltLf @23 :Float32;
    bLatDistDevLf @24 :UInt8;
    fPosX0FltCntr @25 :Float32;
    fPosY0FltCntr @26 :Float32;
    fHeadingFltCntr @27 :Float32;
    fCrvFltCntr @28 :Float32;
    fCrvRateFltCntr @29 :Float32;
    fPosX0FltRi @30 :Float32;
    fPosY0FltRi @31 :Float32;
    fHeadingFltRi @32 :Float32;
    fCrvFltRi @33 :Float32;
    fCrvRateFltRi @34 :Float32;
    bLatDistDevRi @35 :UInt8;
    fLaneWidth @36 :Float32;
}

struct SLmcDebugT {
    bSetLmcByLength @0 :UInt8;
    bSetLmcByOverallQuality @1 :UInt8;
    bSetLmcByAvlChng @2 :UInt8;
    bSetLmcByQlyChng @3 :UInt8;
    bSetLmcEnable @4 :UInt8;
    bRstLmcByCrv @5 :UInt8;
    bRstLmcByAvlQly @6 :UInt8;
    bRstLmcByMaxLength @7 :UInt8;
    bRstLmcByPolyFit @8 :UInt8;
    bRstLmcByLaneChng @9 :UInt8;
    bRstLmcByLowQly @10 :UInt8;
    bRstLmcEnable @11 :UInt8;
    bEnaMotionCmpn @12 :UInt8;
    fDeltaPosX @13 :Float32;
    fDeltaPosY @14 :Float32;
    fDeltaYaw @15 :Float32;
    bEnaPolyFit3rd @16 :UInt8;
    fMaxValidLengthCpmn @17 :Float32;
    fMinValidLengthCpmn @18 :Float32;
    fPosY03rd @19 :Float32;
    fHeading3rd @20 :Float32;
    fCrv3rd @21 :Float32;
    fCrvRate3rd @22 :Float32;
    fDevTraj3rd @23 :Float32;
    bEnaApplySmthByCmpn @24 :UInt8;
    bEnaApplySmthByPosY @25 :UInt8;
    bEnaApplySmthByHeading @26 :UInt8;
    bRawEnaApplySmth @27 :UInt8;
    bEnaApplySmth @28 :UInt8;
    bRstPosY0Smth @29 :UInt8;
    bRstHeadingSmth @30 :UInt8;
    bRstCrvSmth @31 :UInt8;
    bRstCrvRateSmth @32 :UInt8;
    bEnaPosY0Smth @33 :UInt8;
    bEnaHeadingSmth @34 :UInt8;
    bEnaCrvSmth @35 :UInt8;
    bEnaCrvRateSmth @36 :UInt8;
}

struct SLmcInputT {
    fPosY0 @0 :Float32;
    fHeading @1 :Float32;
    fCrv @2 :Float32;
    fCrvRate @3 :Float32;
    fValidLength @4 :Float32;
    bAvailable @5 :UInt8;
    fQuality @6 :Float32;
    fVehVelX @7 :Float32;
    fVehYawRate @8 :Float32;
    fOverallQuality @9 :Float32;
    uMarkerType @10 :UInt8;
    uColor @11 :UInt8;
    bLaneChangeDtct @12 :UInt8;
}

struct SLmcOutputT {
    fPosX0Cpmn @0 :Float32;
    fPosY0Cpmn @1 :Float32;
    fHeadingCpmn @2 :Float32;
    fCrvCpmn @3 :Float32;
    fCrvRateCpmn @4 :Float32;
    fValidLengthCpmn @5 :Float32;
    uMarkerTypeCpmn @6 :UInt8;
    bAvailableCpmn @7 :UInt8;
    uColorCpmn @8 :UInt8;
    fOverallQuality @9 :Float32;
}

struct SLongControlStatusT {
    eLongControlStatus @0 :UInt8;
}

struct SLwvDebugT {
    bValidByRangeLf @0 :UInt8;
    bLaneValidLf @1 :UInt8;
    bValidByRangeRi @2 :UInt8;
    bLaneValidRi @3 :UInt8;
    bLaneVirtualLf @4 :UInt8;
    bLaneVirtualRi @5 :UInt8;
    bBridgeByVirtualLf @6 :UInt8;
    bBridgeByVirtualRi @7 :UInt8;
    bRawLaneBridgeLf @8 :UInt8;
    bRawLaneBridgeRi @9 :UInt8;
    fBridgeDistanceLf @10 :Float32;
    fBridgeDistanceRi @11 :Float32;
    bEnableByDistanceLf @12 :UInt8;
    bEnableByDistanceRi @13 :UInt8;
    fBridgeTimeLf @14 :Float32;
    fBridgeTimeRi @15 :Float32;
    bEnableByTimeLf @16 :UInt8;
    bEnableByTimeRi @17 :UInt8;
    bLaneBridgeLf @18 :UInt8;
    bLaneBridgeRi @19 :UInt8;
    bLaneWidthReset @20 :UInt8;
    fRawLaneWidth @21 :Float32;
    bLowPassReset @22 :UInt8;
    fTimeLowPass @23 :Float32;
    fRawTimeLowPass @24 :Float32;
    fMaxLaneWidth @25 :Float32;
    fMinLaneWidth @26 :Float32;
    fCoeffLowPass @27 :Float32;
}

struct SLwvInputT {
    fVehVelX @0 :Float32;
    bNewCorridorValid @1 :UInt8;
    bUpDownHillDegrade @2 :UInt8;
    bBridgePossible @3 :UInt8;
    fPosY0Lf @4 :Float32;
    fHeadingLf @5 :Float32;
    uQualityLf @6 :UInt8;
    uRangeCheckQualifierLf @7 :UInt16;
    bNotAvailableLf @8 :UInt8;
    bDistYStepDtctLf @9 :UInt8;
    bLengthInvalidLf @10 :UInt8;
    bBridgeUnCplLf @11 :UInt8;
    bQualityNotValidLf @12 :UInt8;
    fPosY0Ri @13 :Float32;
    fHeadingRi @14 :Float32;
    uQualityRi @15 :UInt8;
    uRangeCheckQualifierRi @16 :UInt16;
    bNotAvailableRi @17 :UInt8;
    bDistYStepDtctRi @18 :UInt8;
    bLengthInvalidRi @19 :UInt8;
    bBridgeUnCplRi @20 :UInt8;
    bQualityNotValidRi @21 :UInt8;
}

struct SLwvOutputT {
    uLaneValidQualifer @0 :UInt8;
    fLaneWidth @1 :Float32;
    bLaneWidthValid @2 :UInt8;
}

struct SrrObjSt {
    objectId @0 :UInt8;
    fDistX @1 :Float32;
    fDistY @2 :Float32;
    fVrelX @3 :Float32;
    fVrelY @4 :Float32;
    fArelX @5 :Float32;
    fArelY @6 :Float32;
    fWidth @7 :Float32;
    fLength @8 :Float32;
    iDynamicProperty @9 :UInt8;
    fRcs @10 :Float32;
    fDistXStd @11 :Float32;
    fDistYStd @12 :Float32;
    fVrelXStd @13 :Float32;
    fVrelYStd @14 :Float32;
    fArelXStd @15 :Float32;
    fArelYStd @16 :Float32;
    fOrientationStd @17 :Float32;
    fProbabilityOfExistence @18 :Float32;
    iObjMaintenanceState @19 :UInt8;
    iClassification @20 :UInt8;
    fOrientation @21 :Float32;
}

struct SrrPosSt {
    uiSrrLoc @0 :UInt32;
    fPosX @1 :Float32;
    fPosY @2 :Float32;
    fPosZ @3 :Float32;
    fHeadAngle @4 :Float32;
}

struct SrrWarningSt {
    u8WarningDow @0 :UInt8;
    u8WarningFcw @1 :UInt8;
    u8WarningLca @2 :UInt8;
    u8WarningRcta @3 :UInt8;
}

struct STjasaDebugT {
    tjacmbCombinedPosX0Met @0 :Float32;
    tjacmbCombinedPosY0Met @1 :Float32;
    tjacmbCombinedHeadingRad @2 :Float32;
    tjacmbCombinedCrv1pm @3 :Float32;
    tjacmbCombinedCrvChng1pm2 @4 :Float32;
    tjacmbCombinedLengthMet @5 :Float32;
    tjacmbLaneCrvStdDevNu @6 :Float32;
    tjacmbTraceCrvStdDevNu @7 :Float32;
    tjaslcReadyToTriggerNu @8 :UInt8;
    tjaslcManeuverStateNu @9 :UInt8;
    tjaslcLaneChangeTrigNu @10 :UInt8;
    tjalkaLnBndValidNu @11 :UInt8;
    tjacmbCancelBool @12 :UInt8;
    tjalkaLanePredictValidBool @13 :UInt8;
    tjalkaCancelBool @14 :UInt8;
    tjalkaStrongReadyBool @15 :UInt8;
    tjalkaWeakReadyBool @16 :UInt8;
    tjaobfObjLaneValidDurationBool @17 :UInt8;
    tjaobfTgtObjDataValidBool @18 :UInt8;
    tjagenLkaOnlySwitchBool @19 :UInt8;
    tjaobfStrongReadyBool @20 :UInt8;
    tjaobfCancelBool @21 :UInt8;
    tjaobfWeakReadyBool @22 :UInt8;
    tjagenClearanceBool @23 :UInt8;
    tjagenDegradationBool @24 :UInt8;
    tjaslcCancelBool @25 :UInt8;
    tjaslcStrongReadyBool @26 :UInt8;
    tjaslcWeakReadyBool @27 :UInt8;
    tjaslcTakeOverValidBool @28 :UInt8;
    tjacmbObjectCorridorBool @29 :UInt8;
    tjacmbStrongReadyBool @30 :UInt8;
    tjacmbWeakReadyBool @31 :UInt8;
    tjagenFunctionSwitchBool @32 :UInt8;
    tjagenCodeFunctionBool @33 :UInt8;
    tjagenErrorBool @34 :UInt8;
    tjagenAbortBool @35 :UInt8;
    tjagenCancelNu @36 :UInt8;
    tjagenStrongReadyBool @37 :UInt8;
    tjagenWeakReadyBool @38 :UInt8;
    tjattgPredictionEnableBool @39 :UInt8;
    tjagenCancelStatusBtf @40 :UInt8;
    tjagenWeakReadyInvalidBtf @41 :UInt8;
    tjalkaLaneCenterInvalidBtf @42 :UInt16;
    tjaobfObjInLaneInvalidBtf @43 :UInt16;
    tjaobfObjFollowInvalidBtf @44 :UInt16;
    tjaslcLeLaneChangeInvalidBtf @45 :UInt16;
    tjaslcRiLaneChangeInvalidBtf @46 :UInt16;
    tjaslcTriggerInvalidBtf @47 :UInt16;
    tjacmbCombinedInvalidBtf @48 :UInt16;
    tjastmTjaInvalidBtf @49 :UInt16;
    tjalkaLnQualityInvBtf @50 :UInt8;
    tjaobfTgtObjDataInvalidBtf @51 :UInt8;
    tjaslcCancelAbortBtf @52 :UInt8;
    tjagenStrongReadyInvalidBtf @53 :UInt8;
    lkaLeLnCrvQualityValidBool @54 :UInt8;
    lkaLeLnQualityValidBool @55 :UInt8;
    slcStrongReadyBothSidesBool @56 :UInt8;
    slcWeakReadyLeftBool @57 :UInt8;
    slcWeakReadyBothSidesBool @58 :UInt8;
    slcWeakReadyRightBool @59 :UInt8;
    slcTriggerRightBool @60 :UInt8;
    slcTriggerLeftBool @61 :UInt8;
    slcLeverLeftEngagedBool @62 :UInt8;
    slcLeverRightEngagedBool @63 :UInt8;
    slcMaxInitDurationExceededBool @64 :UInt8;
    slcManvStatePassiveBool @65 :UInt8;
    slcAbortBool @66 :UInt8;
    slcCancelBool @67 :UInt8;
    slcLcmEndBool @68 :UInt8;
    slcLcmStartBool @69 :UInt8;
    slcLaneCheckValidBool @70 :UInt8;
    slcNewEgoLaneBool @71 :UInt8;
    slcLcmCancelBool @72 :UInt8;
    stmCancelBool @73 :UInt8;
    slcPrevResetBool @74 :UInt8;
    obfAccObjValidLaneCheck @75 :UInt8;
    obfAccObjValid @76 :UInt8;
    obfLeftLaneCheckValidBool @77 :UInt8;
    obfAccObjValidBool @78 :UInt8;
    obfAccObjSwitch @79 :UInt8;
    obfMinDist2LeftBndInvalid @80 :UInt8;
    obfTargetOutsideEgoLaneBool @81 :UInt8;
    obfRightLaneCheckValidBool @82 :UInt8;
    obfMinDist2RightBndInvalid @83 :UInt8;
    obfLaneCheckValidBool @84 :UInt8;
    obfDistOrEgoLaneInvalidBool @85 :UInt8;
    obfTargetObjDataWrBool @86 :UInt8;
    obfTargetObjDataSrBool @87 :UInt8;
    cmbObjectFollowingOnlyBool @88 :UInt8;
    cmbLaneQualityInvalidBool @89 :UInt8;
    ttgObjectUpdateBool @90 :UInt8;
    ttgLaneUpdateBool @91 :UInt8;
    ttgCmbObjectCorridorBool @92 :UInt8;
    ttgEnableBool @93 :UInt8;
    ttgLdPredictFinishBool @94 :UInt8;
    ttgLdEnableBool @95 :UInt8;
    ttgCmbEnableBool @96 :UInt8;
    ttgOdEnableBool @97 :UInt8;
    ttgResetBool @98 :UInt8;
    ttgPredictEnableBool @99 :UInt8;
    tjagenFunctionQuitBool @100 :UInt8;
    tjagenSuspendedAndQuitDebug @101 :UInt16;
    tjagenSuspendEndBool @102 :UInt8;
    tjagenSuspendStartBool @103 :UInt8;
    tjastmSysStateTjaInNu @104 :UInt8;
    stmSuspendTimeExpiredBool @105 :UInt8;
    stmPrevRampoutBool @106 :UInt8;
    slcLcpLeft2ActiveBool @107 :UInt8;
    slcLcpRight2ActiveBool @108 :UInt8;
    slcLcpRight2PassiveBool @109 :UInt8;
    slcLcpLeft2PassiveBool @110 :UInt8;
    tjastmDrvTakeOverBool @111 :UInt8;
    tjatowDriverTakeOverWarningNu @112 :UInt8;
    tjalkaLaneIncoherenceBtf @113 :UInt8;
    tjalkaLnIncoherenceStatusNu @114 :UInt8;
    slcVehSpdTooLowInfo @115 :Bool;
    slcLaneChangeOnGoingInfo @116 :Bool;
    slcLaneChangeEndInfo @117 :Bool;
    slcLaneChangePendingInfo @118 :Bool;
    slcLaneChangeCancleInfo @119 :Bool;
    slcAllowGoBackBool @120 :Bool;
    slcCenterDistToBoundaryMet @121 :Float32;
    slcIntoAbortNu @122 :Bool;
    slcLaneChangeDirectionInNu @123 :UInt8;
    slcSameLaneChangeDetcBool @124 :Bool;
    slcLaneChangeBackDetcBool @125 :Bool;
    slcExitAbortBool @126 :Bool;
    slcExitAbortNewEgoBool @127 :Bool;
    slcAbortStateEnum @128 :UInt8;
    slcLaneChangeDirectionAbortEnum @129 :UInt8;
}

struct STjasaInReqT {
    abprLaneChangeDetectedBool @0 :UInt8;
    abprLaneWidthMet @1 :Float32;
    abprConstructionSiteBool @2 :UInt8;
    abprUncoupledLaneWidthMet @3 :Float32;
    abprAbdTimeStampSec @4 :Float32;
    abprLeftLaneTypeEnum @5 :UInt8;
    abprLeLnClthPosX0Met @6 :Float32;
    abprLeLnClthPosY0Met @7 :Float32;
    abprLeLnInvalidQuBtf @8 :UInt16;
    abprLeLnQualityPerc @9 :UInt8;
    abprLeCrvQualityPerc @10 :UInt8;
    abprLeLnClthHeadingRad @11 :Float32;
    abprLeLnClthCrv1pm @12 :Float32;
    abprLeLnClthCrvChng1pm2 @13 :Float32;
    abprLeLnClthLengthMet @14 :Float32;
    abprLeAdjLnClthPosX0Met @15 :Float32;
    abprLeAdjLnClthPosY0Met @16 :Float32;
    abprLeAdjLnInvalidQuBtf @17 :UInt16;
    abprLeAdjLnClthHeadingRad @18 :Float32;
    abprLeAdjLnClthCrv1pm @19 :Float32;
    abprLeAdjLnClthCrvChng1pm2 @20 :Float32;
    abprLeAdjLnClthLengthMet @21 :Float32;
    abprRightLaneTypeEnum @22 :UInt8;
    abprRiLnClthPosX0Met @23 :Float32;
    abprRiLnClthPosY0Met @24 :Float32;
    abprRiLnInvalidQuBtf @25 :UInt16;
    abprRiLnQualityPerc @26 :UInt8;
    abprRiCrvQualityPerc @27 :UInt8;
    abprRiLnClthHeadingRad @28 :Float32;
    abprRiLnClthCrv1pm @29 :Float32;
    abprRiLnClthCrvChng1pm2 @30 :Float32;
    abprRiLnClthLengthMet @31 :Float32;
    abprRiAdjLnClthPosX0Met @32 :Float32;
    abprRiAdjLnClthPosY0Met @33 :Float32;
    abprRiAdjLnInvalidQuBtf @34 :UInt16;
    abprRiAdjLnClthHeadingRad @35 :Float32;
    abprRiAdjLnClthCrv1pm @36 :Float32;
    abprRiAdjLnClthCrvChng1pm2 @37 :Float32;
    abprRiAdjLnClthLengthMet @38 :Float32;
    abprCntrLnClthPosX0Met @39 :Float32;
    abprCntrLnClthPosY0Met @40 :Float32;
    abprCntrLnClthHeadingRad @41 :Float32;
    abprCntrLnClthCrv1pm @42 :Float32;
    abprCntrLnClthCrvChng1pm2 @43 :Float32;
    abprCntrLnClthLengthMet @44 :Float32;
    odpfohTgtObjClothoidInvBtf @45 :UInt16;
    odpfohTgtObjPosX0Met @46 :Float32;
    odpfohTgtObjPosY0Met @47 :Float32;
    odpfohTgtObjHeadAngRad @48 :Float32;
    odpfohTgtObjCrv1pm @49 :Float32;
    odpfohTgtObjCrvChng1pm2 @50 :Float32;
    odpfohTgtObjLengthMet @51 :Float32;
    odpfopAccFrObjTStampSec @52 :Float32;
    odpfopAccObjInvBitfieldBtf @53 :UInt16;
    odpfopAccObjPosXMet @54 :Float32;
    odpfopAccObjPosYMet @55 :Float32;
    lcaActiveLeftBool @56 :UInt8;
    lcaActiveRightBool @57 :UInt8;
    lcaWarningLeftBool @58 :UInt8;
    lcaWarningRightBool @59 :UInt8;
    bsdActiveLeftBool @60 :UInt8;
    bsdActiveRightBool @61 :UInt8;
    bsdWarningLeftBool @62 :UInt8;
    bsdWarningRightBool @63 :UInt8;
    customPrjSpecQuBtf @64 :UInt16;
    sOdpsopMsFlagRearLeftNu @65 :UInt8;
    sOdpsopMsFlagRearRightNu @66 :UInt8;
    trjplnQuStatusTrajPlanNu @67 :UInt16;
    trjctrQuServTrajCtrNu @68 :UInt8;
    mdctrControllingFunctionNu @69 :UInt8;
    lcfrcvSysCycleTimeSenSec @70 :Float32;
    vDyVehVelocityKph @71 :Float32;
    lcfrcvTurnSignalLeverHoldBool @72 :UInt8;
    lcfrcvTurnSignalLeftBool @73 :UInt8;
    lcfrcvTurnSignalRightBool @74 :UInt8;
    lcfrcvDrivingModeNu @75 :UInt8;
    lcfrcvLkaSwitchNu @76 :UInt8;
    lcfrcvTjaSwitchNu @77 :UInt8;
    lcfrcvErrorStateTjaBool @78 :UInt8;
    lcfrcvErrorStateLkaBool @79 :UInt8;
    vDyVehAclXMps2 @80 :Float32;
    vDyVehAclYMps2 @81 :Float32;
    lcfrcvSysStOnLatDmcBool @82 :UInt8;
    lcfrcvSteerWAngleDeg @83 :Float32;
    vDyVehCrv1pm @84 :Float32;
    vDyVehYawRateRps @85 :Float32;
    vDyVehVelXMps @86 :Float32;
    lcfrcvVehStoppedNu @87 :UInt8;
    vdpdrvActiveStCtrlBtf @88 :UInt8;
    vdpdrvSysStErrorBtf @89 :UInt8;
    vdpdrvVehStInvalidBtf @90 :UInt16;
    vdpdrvDrvStInvalidBtf @91 :UInt8;
    vdpdrvSysStNotAvailableBtf @92 :UInt8;
    vDyDashboardVelocityKph @93 :Float32;
    lcfrcvManualTorqueNm @94 :Float32;
    lcfrcvSteerWAngleGradDegps @95 :Float32;
    lcfrcvSlchmiSwitchBool @96 :UInt8;
    lcfrcvTjaAudioSwitchBool @97 :UInt8;
    lccraLeftSafeFlagBool @98 :Bool;
    lccraRightSafeFlagBool @99 :Bool;
    lccraLeftFrontSafeFlagBool @100 :Bool;
    lccraRightFrontSafeFlagBool @101 :Bool;
    lccraLeftRearSafeFlagBool @102 :Bool;
    lccraRightRearSafeFlagBool @103 :Bool;
    lcfrcvPilotOnLeverSwitchBool @104 :Bool;
    lcfrcvPilotOffLeverSwitchBool @105 :Bool;
    vlcvehAccStatusNu @106 :UInt8;
    lcfsenNbDclcSwitchNvramNu @107 :Bool;
    lccraLeftHighLightIdNu @108 :Int32;
    lccraRightHighLightIdNu @109 :Int32;
    lcfrcvTurnLightReqStNu @110 :UInt8;
}

struct STjasaOutProT {
    tjastmSysStateTjaNu @0 :UInt8;
    tjastmLatCtrlModeNu @1 :UInt8;
    tjattgLeCridrBndPosX0Met @2 :Float32;
    tjattgLeCridrBndPosY0Met @3 :Float32;
    tjattgLeCridrBndHeadAngRad @4 :Float32;
    tjattgLeCridrBndCrv1pm @5 :Float32;
    tjattgLeCridrBndCrvChng1pm2 @6 :Float32;
    tjattgLeCridrBndLengthMet @7 :Float32;
    tjattgRiCridrBndPosX0Met @8 :Float32;
    tjattgRiCridrBndPosY0Met @9 :Float32;
    tjattgRiCridrBndHeadAngRad @10 :Float32;
    tjattgRiCridrBndCrv1pm @11 :Float32;
    tjattgRiCridrBndCrvChng1pm2 @12 :Float32;
    tjattgRiCridrBndLengthMet @13 :Float32;
    tjattgTgtTrajPosX0Met @14 :Float32;
    tjattgTgtTrajPosY0Met @15 :Float32;
    tjattgTgtTrajHeadAngRad @16 :Float32;
    tjattgTgtTrajCrv1pm @17 :Float32;
    tjattgTgtTrajCrvChng1pm2 @18 :Float32;
    tjattgTgtTrajLengthMet @19 :Float32;
    tjatvgDistYTolLeTgtAreaMet @20 :Float32;
    tjatvgDistYTolRiTgtAreaMet @21 :Float32;
    tjatvgFTireAclMaxMps2 @22 :Float32;
    tjatvgFTireAclMinMps2 @23 :Float32;
    tjatvgWeightTgtDistYNu @24 :Float32;
    tjatvgWeightEndTimeNu @25 :Float32;
    tjatvgPredTimeCrvSec @26 :Float32;
    tjatvgPredTimeHeadAngSec @27 :Float32;
    tjatvgMaxCrvTrajGuiCtl1pm @28 :Float32;
    tjatvgMaxCrvGrdBuildup1pms @29 :Float32;
    tjatvgMaxCrvGrdRed1pms @30 :Float32;
    tjatvgMaxCrvGrdTgc1pms @31 :Float32;
    tjatvgPlanningHorizonSec @32 :Float32;
    tjatvgTrqRampGrad1ps @33 :Float32;
    tjatvgStrWhStifLimitNu @34 :Float32;
    tjatvgMaxTrqScalLimitNu @35 :Float32;
    tjatvgStrWhStifGrad1ps @36 :Float32;
    tjatvgMaxTrqScalGrad1ps @37 :Float32;
    tjatvgMaxJerkAllowedMps3 @38 :Float32;
    tjatvgSensorTStampSec @39 :Float32;
    tjatvgObstacleVelXMps @40 :Float32;
    tjatvgObstacleAclXMps2 @41 :Float32;
    tjatvgObstacleWidthMet @42 :Float32;
    tjatvgObstacleDistXMet @43 :Float32;
    tjatvgObstacleDistYMet @44 :Float32;
    tjatvgLimiterTimeDurationSec @45 :Float32;
    tjattgTgtCorridorInvalidBtf @46 :UInt16;
    tjatvgTrajPlanServQuNu @47 :UInt8;
    tjatvgDeratingLevelNu @48 :UInt8;
    tjatvgCrvAmplActivatedNu @49 :UInt8;
    tjatvgLimiterActivatedNu @50 :UInt8;
    tjatvgLimiterTypeNu @51 :UInt8;
    tjattgTransTriggerReplanBool @52 :UInt8;
    tjatvgTriggerReplanNu @53 :UInt8;
    tjatvgHighStatAccuBool @54 :UInt8;
    tjatvgLtcyCompActivatedNu @55 :UInt8;
    tjatvgTrajGuiQuNu @56 :UInt8;
    tjastmSysStateHwaNu @57 :UInt8;
    tjastmNpilotSysInfo @58 :UInt8;
    tjastmPilotAudioPlay @59 :UInt8;
    tjastmLatCtrlHandsOffReleaseWarnNu @60 :UInt8;
    tjaslcLaneChangWarningNu @61 :UInt8;
    tjaslcSlcAudioPlayNu @62 :UInt8;
    tjastmPilotEnableAccSwitchNu @63 :UInt8;
    tjastmPilotDisableAccSwitchNu @64 :UInt8;
    tjaslcNbDclcSwitchNvramNu @65 :Bool;
    tjaslcSlcHighLightIdNu @66 :Int32;
    tjaslcLaneChangeInfo @67 :UInt8;
    tjaslcTurnLtDirctionReqNu @68 :UInt8;
}

struct StRatioT {
    swa @0 :SwaT;
    vel @1 :VelT;
}

struct StructBBox2DBus {
    initialized @0 :Bool;
    topLeftX @1 :Float32;
    topLeftY @2 :Float32;
    bottomRightX @3 :Float32;
    bottomRightY @4 :Float32;
    confidence @5 :Float32;
    isLeftSideTruncated @6 :Bool;
    isRightSideTruncated @7 :Bool;
    isTopSideTruncated @8 :Bool;
    isBottomSideTruncated @9 :Bool;
}

struct StructCameraBBox2DInfoBus {
    rawDetectionBox @0 :StructBBox2DBus;
    trackedBox @1 :StructBBox2DBus;
}

struct StructTrafficLightObjectBus {
    label @0 :UInt16;
    statusLabel @1 :UInt16;
    typeLabel @2 :UInt16;
    cameraBboxInfo @3 :StructCameraBBox2DInfoBus;
}

struct SUlbDebugT {
    fLaneWidthUnCpl @0 :Float32;
    fPosYAtMaxUnCplLf @1 :Float32;
    fPosYAtMaxUnCplRi @2 :Float32;
    fRotPosYUnCplLf @3 :Float32;
    fRotPosYUnCplRi @4 :Float32;
    fLaneWidthAtMaxUnCpl @5 :Float32;
    fDiffLaneWidthUnCpl @6 :Float32;
    bRawNotParallelUnCpl @7 :UInt8;
    bDlyNotParallelUnCpl @8 :UInt8;
    bNotParallelUnCpl @9 :UInt8;
    fTimerRawNotParallel @10 :Float32;
    fDeltaYawCplLf @11 :Float32;
    fDeltaXUnCplLf @12 :Float32;
    fDeltaYUnCplLf @13 :Float32;
    fDeltaYawCplRi @14 :Float32;
    fDeltaXUnCplRi @15 :Float32;
    fDeltaYUnCplRi @16 :Float32;
    fUnitDeltaXUnCplLf @17 :Float32;
    fUnitDeltaYUnCplLf @18 :Float32;
    fUnitDeltaYawCplRi @19 :Float32;
    fUnitDeltaYawCplLf @20 :Float32;
    fUnitDeltaXUnCplRi @21 :Float32;
    fUnitDeltaYUnCplRi @22 :Float32;
    fDlyPosXUnCplLf @23 :Float32;
    fDlyPosYUnCplLf @24 :Float32;
    fDlyPosXUnCplRi @25 :Float32;
    fDlyPosYUnCplRi @26 :Float32;
    fCpmnPosXUnCplLf @27 :Float32;
    fCpmnPosYUnCplLf @28 :Float32;
    fCpmnPosXUnCplRi @29 :Float32;
    fCpmnPosYUnCplRi @30 :Float32;
    fRotCmpnPosYUnCplLf @31 :Float32;
    fRotCmpnPosYUnCplRi @32 :Float32;
    fDevLatDistCplLf @33 :Float32;
    fDevLatDistCplRi @34 :Float32;
    fDevLatDistDev @35 :Float32;
    bTrigLatDistDev @36 :UInt8;
    fPenaltyPosY0UnCplLf @37 :Float32;
    fPenaltyHeadingUnCplLf @38 :Float32;
    fPenaltyCrvUnCplLf @39 :Float32;
    fPenaltyCrvRateUnCplLf @40 :Float32;
    fRawPenaltyUnCplLf @41 :Float32;
    fPenaltyUnCplLf @42 :Float32;
    fPenaltyPosY0UnCplRi @43 :Float32;
    fPenaltyHeadingUnCplRi @44 :Float32;
    fPenaltyCrvUnCplRi @45 :Float32;
    fPenaltyCrvRateUnCplRi @46 :Float32;
    fRawPenaltyUnCplRi @47 :Float32;
    fPenaltyUnCplRi @48 :Float32;
    bRawStepDtctUnCplLf @49 :UInt8;
    bRawStepDtctUnCplRi @50 :UInt8;
    bStepDtctUnCplLf @51 :UInt8;
    bStepDtctUnCplRi @52 :UInt8;
    fHeadingPolyUnCplLf @53 :Float32;
    fHeadingPolyUnCplRi @54 :Float32;
    fDevHeadingUnCplLf @55 :Float32;
    fDevHeadingUnCplRi @56 :Float32;
    fDevHeadingDevUnCpl @57 :Float32;
    bTrigHeadingDevUnCpl @58 :UInt8;
    fCrvRefUnCplLf @59 :Float32;
    fCrvRefUnCplRi @60 :Float32;
    fDevCrvUnCplLf @61 :Float32;
    fDevCrvUnCplRi @62 :Float32;
    fDevCrvDevUnCpl @63 :Float32;
    bTrigCrvDevUnCpl @64 :UInt8;
    bNotParallelBridgeUnCplLf @65 :UInt8;
    bSetBridgeUnCplLf @66 :UInt8;
    bResetBridgeUnCplLf @67 :UInt8;
    bDlySetBridgeUnCplLf @68 :UInt8;
    fTimerSetUnCplLf @69 :Float32;
    bResetByCrvUnCplLf @70 :UInt8;
    bNotParallelBridgeUnCplRi @71 :UInt8;
    bSetBridgeUnCplRi @72 :UInt8;
    bResetBridgeUnCplRi @73 :UInt8;
    bDlySetBridgeUnCplRi @74 :UInt8;
    fTimerSetUnCplRi @75 :Float32;
    bResetByCrvUnCplRi @76 :UInt8;
    fTempUlb @77 :Float32;
    bTempUlb @78 :UInt8;
}

struct SUlbInputT {
    fPosY0UnCplLf @0 :Float32;
    fHeadingUnCplLf @1 :Float32;
    fCrvUnCplLf @2 :Float32;
    fCrvRateUnCplLf @3 :Float32;
    fValidLengthUnCplLf @4 :Float32;
    bAvailableUnCplLf @5 :UInt8;
    fPosY0UnCplRi @6 :Float32;
    fHeadingUnCplRi @7 :Float32;
    fCrvUnCplRi @8 :Float32;
    fCrvRateUnCplRi @9 :Float32;
    fValidLengthUnCplRi @10 :Float32;
    bAvailableUnCplRi @11 :UInt8;
    fVehYawRate @12 :Float32;
    fVehVelX @13 :Float32;
    bLaneChangeDtct @14 :UInt8;
    bDistYStepDtctLf @15 :UInt8;
    bDistYStepDtctRi @16 :UInt8;
    bUpDownHillDegrade @17 :UInt8;
    uPercExitLf @18 :UInt8;
    uPercExitRi @19 :UInt8;
    bLineMergeDtcRi @20 :UInt8;
    bLineMergeDtcLf @21 :UInt8;
}

struct SUlbOutputT {
    bBridgeUnCplLf @0 :UInt8;
    bBridgeUnCplRi @1 :UInt8;
    uBtfBridgeUnCpl @2 :UInt8;
}

struct SUlpDebugT {
    sDmqInputCplLf @0 :SDmqInputT;
    sDmqOutputCplLf @1 :SDmqOutputT;
    sDmqDebugCplLf @2 :SDmqDebugT;
    sDmqInputCplRi @3 :SDmqInputT;
    sDmqOutputCplRi @4 :SDmqOutputT;
    sDmqDebugCplRi @5 :SDmqDebugT;
    sLmcInputCplLf @6 :SLmcInputT;
    sLmcOutputCplLf @7 :SLmcOutputT;
    sLmcDebugCplLf @8 :SLmcDebugT;
    sLmcInputCplRi @9 :SLmcInputT;
    sLmcOutputCplRi @10 :SLmcOutputT;
    sLmcDebugCplRi @11 :SLmcDebugT;
    sUlbInput @12 :SUlbInputT;
    sUlbOutput @13 :SUlbOutputT;
    sUlbDebug @14 :SUlbDebugT;
    sDlbInput @15 :SDlbInputT;
    sDlbOutput @16 :SDlbOutputT;
    sDlbDebug @17 :SDlbDebugT;
}

struct SUlpInputT {
    bLaneChangeDtct @0 :UInt8;
    bUpDnHillDgrd @1 :UInt8;
    fPosY0CplLf @2 :Float32;
    fHeadingCplLf @3 :Float32;
    fCrvCplLf @4 :Float32;
    fCrvRateCplLf @5 :Float32;
    fValidLengthCplLf @6 :Float32;
    bAvailableCplLf @7 :UInt8;
    fStdDevPosY0CplLf @8 :Float32;
    fStdDevHeadingCplLf @9 :Float32;
    fStdDevCrvCplLf @10 :Float32;
    fStdDevCrvRateCplLf @11 :Float32;
    fPosY0CplRi @12 :Float32;
    fHeadingCplRi @13 :Float32;
    fCrvCplRi @14 :Float32;
    fCrvRateCplRi @15 :Float32;
    fValidLengthCplRi @16 :Float32;
    bAvailableCplRi @17 :UInt8;
    fStdDevPosY0CplRi @18 :Float32;
    fStdDevHeadingCplRi @19 :Float32;
    fStdDevCrvCplRi @20 :Float32;
    fStdDevCrvRateCplRi @21 :Float32;
    fQualityCplLf @22 :Float32;
    fQualityCplRi @23 :Float32;
    fVehVelX @24 :Float32;
    fVehYawRate @25 :Float32;
    uMarkerTypeCplLf @26 :UInt8;
    uMarkerTypeCplRi @27 :UInt8;
    uColorCplLf @28 :UInt8;
    uColorCplRi @29 :UInt8;
    uPercExitLf @30 :UInt8;
    uPercExitRi @31 :UInt8;
    uEventTypeCplLf @32 :UInt8;
    uEventTypeCplRi @33 :UInt8;
    uEventQualityCplLf @34 :UInt8;
    uEventQualityCplRi @35 :UInt8;
    fEventDistanceCplLf @36 :Float32;
    fEventDistanceCplRi @37 :Float32;
    bDistYStepDtctLf @38 :UInt8;
    bDistYStepDtctRi @39 :UInt8;
    bLineMergeDtcRi @40 :UInt8;
    bLineMergeDtcLf @41 :UInt8;
}

struct SUlpOutputT {
    fPosX0Lf @0 :Float32;
    fPosY0Lf @1 :Float32;
    fHeadingLf @2 :Float32;
    fCrvLf @3 :Float32;
    fCrvRateLf @4 :Float32;
    fValidLengthLf @5 :Float32;
    fQualityLf @6 :Float32;
    uLaneTypeLf @7 :UInt8;
    uEventTypeLf @8 :UInt8;
    bAvailableLf @9 :UInt8;
    uEventQualityLf @10 :UInt8;
    fEventDistanceLf @11 :Float32;
    fStdDevPosY0Lf @12 :Float32;
    fStdDevHeadingLf @13 :Float32;
    fStdDevCrvLf @14 :Float32;
    fStdDevCrvRateLf @15 :Float32;
    uColorLf @16 :UInt8;
    fOverallQualityLf @17 :Float32;
    fPosX0Ri @18 :Float32;
    fPosY0Ri @19 :Float32;
    fHeadingRi @20 :Float32;
    fCrvRi @21 :Float32;
    fCrvRateRi @22 :Float32;
    fValidLengthRi @23 :Float32;
    fQualityRi @24 :Float32;
    uLaneTypeRi @25 :UInt8;
    uEventTypeRi @26 :UInt8;
    bAvailableRi @27 :UInt8;
    uEventQualityRi @28 :UInt8;
    fEventDistanceRi @29 :Float32;
    fStdDevPosY0Ri @30 :Float32;
    fStdDevHeadingRi @31 :Float32;
    fStdDevCrvRi @32 :Float32;
    fStdDevCrvRateRi @33 :Float32;
    uColorRi @34 :UInt8;
    fOverallQualityRi @35 :Float32;
    fOverallCrvQualityLf @36 :Float32;
    fOverallCrvQualityRi @37 :Float32;
    bBridgePossibleUnCplLf @38 :UInt8;
    bBridgePossibleUnCplRi @39 :UInt8;
    fOverallQualityUnCplLf @40 :Float32;
    fOverallQualityUnCplRi @41 :Float32;
    fOverallQualityCplLf @42 :Float32;
    fOverallQualityCplRi @43 :Float32;
    uBtfBridgeUnCpl @44 :UInt8;
}

struct SwaT {
    ang @0 :List(Float32);
    rat @1 :List(Float32);
}

struct TCamLaneInputData {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    courseInfo @2 :List(TCourseInfo);
    laneMarkerInfo @3 :List(TLaneMarkerInfo);
    constructionSite @4 :TConstructionSite;
}

struct TConstructionSite {
    bMultipleMarker @0 :Bool;
    bLeftBarrier @1 :Bool;
    bRightBarrier @2 :Bool;
    bCrossingMarker @3 :Bool;
    bInhibitSingleLane @4 :Bool;
    bHold @5 :Bool;
}

struct TCourseInfo {
    fAngle @0 :Float32;
    courseInfoSegNear @1 :TCourseInfoSeg;
}

struct TCourseInfoSeg {
    fC0 @0 :Float32;
    fC1 @1 :Float32;
    fLength @2 :Float32;
}

struct TcuSysInfoT {
    uiCycleCounter @0 :UInt32;
    uiCycleStart @1 :UInt32;
    uiCycleEnd @2 :UInt32;
}

struct TLaneMarkerInfo {
    fMarkerDist @0 :Float32;
    markerColor @1 :UInt8;
    markerType @2 :UInt8;
    uExistanceProbability @3 :UInt8;
}

struct TpEnableDebug {
    bTrajPlanEnble @0 :Bool;
    bTrigTrajReplan @1 :Bool;
    bReplanModeArcLength @2 :Bool;
    bReplanCurValues @3 :Bool;
    bReplanTgtValues @4 :Bool;
    bTrigCustFctChange @5 :Bool;
    bTrajGuiQuChange @6 :Bool;
    bTrigCustFctActn @7 :Bool;
    bTrigReplanTgtTraj @8 :Bool;
    bEnblSpecPlanStrategy @9 :Bool;
    fDelayVehGuiSec @10 :Float32;
    bTrigLargeDeviation @11 :Bool;
    fPredictionTimeCrvSec @12 :Float32;
    fPredictionTimeHeadSec @13 :Float32;
    bCorridorJumpDetected @14 :Bool;
    bLatDmcReqFinished @15 :Bool;
}

struct TpFrenetBackDebug {
    fTrajHeadingRad @0 :Float32;
    fTrajDistYMet @1 :Float32;
    fTrajTgtCurve1pm @2 :Float32;
    fTrajTgtCrvGrd1pms @3 :Float32;
    uiTrajGuiQualifierNu @4 :UInt8;
    fTrajHeadingPrevRad @5 :Float32;
    fTrajTgtCrvPrev1pm @6 :Float32;
    fCurHeadingRad @7 :Float32;
    fCurDistYMet @8 :Float32;
    fTrajHeadInclPrevRad @9 :Float32;
    fCtrlErrDistYMet @10 :Float32;
    fCtrlErrHeadingAngleRad @11 :Float32;
    fCtrlErrHeadAglPrevRad @12 :Float32;
    fTrajDistYPrevMet @13 :Float32;
    fDeltaTargetCrv1pm @14 :Float32;
    fDeltaTargetPosY0Met @15 :Float32;
    fDeltaTargetHeadingRad @16 :Float32;
    bUseTargetCorridor @17 :Bool;
    bTargetSwitch @18 :Bool;
    bGradLimitActive @19 :Bool;
    bPlausiCheckStatus @20 :Bool;
    uiSQuStatusTrajPlanNu @21 :UInt16;
    fTrajTgtCrvGrdPrev1pms @22 :Float32;
    uiDQuStatusTrajPlanNu @23 :UInt16;
}

struct TpFrenetTransfDebug {
    fCurDistYMet @0 :Float32;
    fCurDistY1stDerivMps @1 :Float32;
    fCurDistY2ndDerivMps2 @2 :Float32;
    fTrajVelRefCurveMps @3 :Float32;
    fTrajAclRefCurveMps2 @4 :Float32;
    afTargetDistYMet @5 :List(Float32);
    fTargetDistY1stDerivMps @6 :List(Float32);
    fTargetDistY2ndDerivMps2 @7 :List(Float32);
    afLeDistYMet @8 :List(Float32);
    afTargetPointsNu @9 :List(Float32);
    fTrajDistYPrevMet @10 :Float32;
    fTrajDistY1stToPrevMps @11 :Float32;
    fTrajDistY2ndToPrevMps2 @12 :Float32;
    uiNumOfTgtPointsNu @13 :UInt8;
    fTrajPlanningHorizonSec @14 :Float32;
    fDistY1stToDevHeadMps @15 :Float32;
    fDistY2ndToDevHeadMps2 @16 :Float32;
    fCurDistYPreviewMet @17 :Float32;
    fCurDistY1stToPrevMps @18 :Float32;
    fPreviewTimeHeadingSec @19 :Float32;
    fPlanHorizonVisRangeSec @20 :Float32;
    uiNumOfPointsCridrLeftNu @21 :UInt8;
}

struct TpTrajecotryCalcDebug {
    fTrajDistYMet @0 :Float32;
    fTrajDistY1stDerivMps @1 :Float32;
    fTrajDistY2ndDerivMps2 @2 :Float32;
    fTrajDistY3rdDerivMps3 @3 :Float32;
    uiQuStatusTrajPlanNu @4 :UInt16;
    afTrajParamNu @5 :List(Float32);
    bTrajEnd @6 :Bool;
    bLengthOk @7 :Bool;
    bMatrixInverseOk @8 :Bool;
    fEndPointTrajectoryNu @9 :Float32;
    fPassedTrajLenPercentPer @10 :Float32;
    fMaxJerkTrajMps3 @11 :Float32;
    bMaxJerkOk @12 :Bool;
    fMaxAclTrajMps2 @13 :Float32;
    fOptimalCostNu @14 :Float32;
    fWeightTargetDistYNu @15 :Float32;
    fWeightEndTimeNu @16 :Float32;
}

struct TsrDebug {
    tsrDstcFmLastLowsignMi @0 :Float32;
    tsrDstcFmLastUpsignMi @1 :Float32;
    tsrForbidden1NumSt @2 :List(Float32);
    tsrForbidden2NumSt @3 :List(Float32);
    tsrGearValidB @4 :Bool;
    tsrLowSignB @5 :Bool;
    tsrLowSpdAlarmTimeEnB @6 :Bool;
    tsrLowSpdWarningB @7 :Bool;
    tsrLowSpdKph @8 :Float32;
    tsrLowerLimitDstcValidB @9 :Bool;
    tsrOutLowSpdB @10 :Bool;
    tsrOutUpSpdB @11 :Bool;
    tsrRelLowSpdNumSt @12 :List(Float32);
    tsrRelUpSpdNumSt @13 :List(Float32);
    tsrSpeedValidB @14 :Bool;
    tsrSwitchB @15 :Bool;
    tsrSynLowSpdNumSt @16 :Float32;
    tsrSynUpSpdNumSt @17 :Float32;
    tsrUpSignB @18 :Bool;
    tsrUpSpdAlarmTimeEnB @19 :Bool;
    tsrUpSpdWarningB @20 :Bool;
    tsrUpSpdKph @21 :Float32;
    tsrUpperLimitDstcValidB @22 :Bool;
    tsrSysTimeSec @23 :Float32;
    tsrEnSpeedLowerLimitBtf @24 :Float32;
    tsrEnSpeedUpperLimitBtf @25 :Float32;
}

struct TsrInfo {
    tsrSpeedLimit @0 :UInt8;
    tsrFlag @1 :UInt8;
    tsrActive @2 :Bool;
}

struct TsrInput {
    rcvSysCycleTimeSec @0 :Float32;
    rcvErrorStTsrBool @1 :Bool;
    hmiTsrSwitchEnBool @2 :Bool;
    adcs12TsrAudioEnBool @3 :Bool;
    hmiTsrWarnSwitchBool @4 :Bool;
    vDyVehVelocityKph @5 :Float32;
    vDyDashboardVelocityKph @6 :Float32;
    vsdpVehStInvalidBtf @7 :UInt8;
    camSpeedUpperLimitBtf @8 :UInt16;
    camSpeedLowerLimitBtf @9 :UInt16;
    camSpeedRelUpperLimitBtf @10 :UInt16;
    camSpeedRelLowerLimitBtf @11 :UInt16;
    camSpeedUpperLabelsBtf @12 :List(UInt32);
    camSpeedLowerLabelsBtf @13 :List(UInt32);
    camSpeedUpperNumNu @14 :UInt8;
    camSpeedLowerNumNu @15 :UInt8;
    camSpeedLimitUpperxM @16 :List(Float32);
    camSpeedLimitUpperyM @17 :List(Float32);
    camSpeedLimitLowerxM @18 :List(Float32);
    camSpeedLimitLoweryM @19 :List(Float32);
    camForbiddenSignNumNu @20 :UInt8;
    camForbiddenSignBtf @21 :UInt32;
    camForbiddenLablesBtf @22 :List(UInt32);
    camForbiddenSignPositionxM @23 :List(Float32);
    camForbiddenSignPositionyM @24 :List(Float32);
    nvramTsrSwitchEnBool @25 :Bool;
    camTrafficLightsBtf @26 :List(StructTrafficLightObjectBus);
    tsrResetSwitchBool @27 :Bool;
}

struct TsrOutput {
    tsrForbiddenSignBtf @0 :List(Float32);
    tsrSpeedUpperLimitBtf @1 :Float32;
    tsrSpeedLowerLimitBtf @2 :Float32;
    tsrStrLightColorSt @3 :UInt16;
    tsrLeftLightColorSt @4 :UInt16;
    tsrRightLightColorSt @5 :UInt16;
    tsrVehicleSpdWarningValidBtf @6 :UInt16;
    tsrVehicleSpdMachineEnBtf @7 :UInt8;
    tsrSpeedWarningBool @8 :Bool;
    tsrSwitchEnNvramBool @9 :Bool;
    tsrSysSt @10 :UInt8;
}

struct VedBrakeT {
    absCtrl @0 :Bool;
    tcsCtrl @1 :Bool;
    state @2 :List(UInt8);
}

struct VedPowerTrainT {
    actualGear @0 :UInt8;
    targetGear @1 :UInt8;
    engineRunning @2 :Bool;
    fillByte @3 :UInt8;
    state @4 :List(UInt8);
}

struct VedVehDynT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    longitudinal @2 :LongitudinalT;
    lateral @3 :LateralT;
    motionState @4 :MotionStateVehDynT;
    legacy @5 :LegacyVehDynT;
    state @6 :List(UInt8);
}

struct VedVehParT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    vehParMain @2 :VehParMainT;
    vehParAdd @3 :VehParAddT;
    sensorMounting @4 :SensorMountingT;
    sensor @5 :SensorT;
}

struct VedVehSigAddT {
    state @0 :List(UInt8);
    envTemp @1 :Float32;
    wiperState @2 :UInt8;
    wiperStage @3 :UInt8;
    wiperOutParkPos @4 :Bool;
    wiperWasherFrontState @5 :UInt8;
    rainSensor @6 :Bool;
    turnSignal @7 :UInt8;
    fogLampFront @8 :Bool;
    fogLampRear @9 :Bool;
    roadWhlAngFr @10 :Float32;
    roadWhlAngRe @11 :Float32;
    odometer @12 :Float32;
    gasPedalPos @13 :Float32;
    kickDown @14 :Bool;
    brakePedalPressed @15 :Bool;
    driverTired @16 :Bool;
    speedUnit @17 :UInt8;
    speedoSpeed @18 :Float32;
    trailerConnection @19 :UInt8;
    trailerConnBeforeShutDown @20 :UInt8;
    trailerLengthInput @21 :UInt8;
    parkAidDetL @22 :UInt8;
    parkAidDetCl @23 :UInt8;
    parkAidDetCr @24 :UInt8;
    parkAidDetR @25 :UInt8;
    ignitionSwitch @26 :UInt8;
    eSuspensionSystem @27 :UInt8;
    eHeightLevel @28 :UInt8;
    wheelHeightLevel @29 :VedWheelHeightLevelT;
    alignmentByte1 @30 :Int16;
    alignmentByte2 @31 :Int16;
}

struct VedVehSigMainT {
    state @0 :List(UInt8);
    yawRate @1 :Float32;
    yawRateTemp @2 :Float32;
    stWheelAngle @3 :Float32;
    latAccel @4 :Float32;
    whlVelFrLeft @5 :Float32;
    whlVelFrRight @6 :Float32;
    whlVelReLeft @7 :Float32;
    whlVelReRight @8 :Float32;
    yawRateInt @9 :Float32;
    yawRateIntTemp @10 :Float32;
    longAccel @11 :Float32;
    rearWhlAngle @12 :Float32;
    vehVelocityExt @13 :Float32;
    vehLongAccelExt @14 :Float32;
    whlDirFrLeft @15 :UInt8;
    whlDirFrRight @16 :UInt8;
    whlDirReLeft @17 :UInt8;
    whlDirReRight @18 :UInt8;
    whlTicksDevFrLeft @19 :UInt8;
    whlTicksDevFrRight @20 :UInt8;
    whlTicksDevReLeft @21 :UInt8;
    whlTicksDevReRight @22 :UInt8;
    actGearPos @23 :UInt8;
    brakeActLevel @24 :UInt16;
    parkBrakeState @25 :UInt8;
    vehLongDirExt @26 :UInt8;
    vehLongMotStateExt @27 :UInt8;
    curveC0Ext @28 :Float32;
    curveC1Ext @29 :Float32;
    sideSlipAngleExt @30 :Float32;
}

struct VedVehSigT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :EnvmSignalHeaderT;
    vehSigMain @2 :VedVehSigMainT;
    vehSigAdd @3 :VedVehSigAddT;
    powerTrain @4 :VedPowerTrainT;
    brake @5 :VedBrakeT;
}

struct VedWheelHeightLevelT {
    frontLeft @0 :Int16;
    frontRight @1 :Int16;
    rearLeft @2 :Int16;
    rearRight @3 :Int16;
}

struct VehParAddT {
    state @0 :List(UInt8);
    vehicleWidth @1 :Float32;
    vehicleLength @2 :Float32;
    curbWeight @3 :Float32;
    overhangFront @4 :Float32;
    frontAxleRoadDist @5 :Float32;
    wheelWidth @6 :Float32;
    passableHeight @7 :Float32;
    distCameraToHoodX @8 :Float32;
    distCameraToHoodY @9 :Float32;
    steeringVariant @10 :UInt8;
}

struct VehParMainT {
    state @0 :List(UInt8);
    selfSteerGrad @1 :Float32;
    steeringRatio @2 :StRatioT;
    wheelBase @3 :Float32;
    trackWidthFront @4 :Float32;
    trackWidthRear @5 :Float32;
    vehWeight @6 :Float32;
    cntrOfGravHeight @7 :Float32;
    axisLoadDistr @8 :Float32;
    whlLoadDepFrontAxle @9 :Float32;
    whlLoadDepRearAxle @10 :Float32;
    whlCircumference @11 :Float32;
    drvAxle @12 :UInt8;
    whlTcksPerRev @13 :UInt8;
    frCrnrStiff @14 :Float32;
    reCrnrStiff @15 :Float32;
}

struct VeloCorrVehDynT {
    corrFact @0 :Float32;
    corrVar @1 :Float32;
    corrVelo @2 :Float32;
    corrVeloVar @3 :Float32;
    minVelo @4 :Float32;
    maxVelo @5 :Float32;
    corrQual @6 :UInt8;
    bRollerTestBench @7 :Bool;
}

struct VelT {
    vel @0 :List(Float32);
    rat @1 :List(Float32);
}

struct VlcAccLeverInputT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    setSwitch @2 :Bool;
    decelSwitch @3 :Bool;
    resumeAccelSwitch @4 :Bool;
    cancel @5 :Bool;
    accMode @6 :Bool;
    mainSwitch @7 :Bool;
    headwayInc @8 :Bool;
    headwayDec @9 :Bool;
    headwaySwitch @10 :Bool;
    headway @11 :UInt8;
    pilotSwitch @12 :Bool;
    fcaSetting @13 :Bool;
}

struct VlcAccObjectT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    autosar @2 :ObjectT;
    longSpeed @3 :Int16;
    longSpeedHistory @4 :List(Int16);
    longAccel @5 :Int16;
    latDisplFromLane @6 :Int16;
    laneInformation @7 :Int8;
    maxAllowedDecel @8 :Int16;
    maxAllowedAccel @9 :Int16;
    controlAccel @10 :Int16;
    neededDecel @11 :Int16;
    ttc @12 :UInt16;
    avlcCutInOutPotential @13 :UInt8;
    usageStatus @14 :AccObjectUsageStatusT;
    requestedDistanceModifiedAct @15 :Int16;
    requestedDistanceModifiedPred @16 :Int16;
    controlSmoothness @17 :UInt8;
    alertModificationFactor @18 :Int16;
    lastObjectId @19 :Int8;
    longAccelModified @20 :Int16;
    tts @21 :UInt16;
    accelRequestFuzzy @22 :Int16;
    accelRequestTts @23 :Int16;
    accelRequestDmin @24 :Int16;
    fuzzyAreaArray @25 :List(Int32);
    fuzzyAreaPosArray @26 :List(Int32);
    fuzzySignalInput @27 :FuzzySignalInputT;
    fuzzyRuleInput @28 :FuzzyRuleInputT;
}

struct VlcAccOutputDataT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    distanceCtrlAccelMax @2 :Int16;
    distanceCtrlAccelMin @3 :Int16;
    maxAvlcAcceleration @4 :Int16;
    maxAvlcDeceleration @5 :Int16;
    requestedTimegap @6 :UInt16;
    requestedDistance @7 :Int16;
    requestedMaxIntrusion @8 :Int16;
    recommendedVelocity @9 :Int16;
    headwaySetting @10 :UInt8;
    avlcOutputStatus @11 :AccOutputStatusT;
    situationClass @12 :AccSituationClassifierT;
}

struct VlcCtrlDataT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    opMode @2 :UInt32;
    fCycleTime @3 :Float32;
}

struct VlcCustomInputT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    sLongControlStatus @2 :SLongControlStatusT;
    eTurnIndicator @3 :UInt8;
}

struct VlcDfv2SenInfoT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    standStill @2 :Bool;
    overrideAccel @3 :Bool;
    curLongCtrlVelocity @4 :Int16;
    curLongCtrlAccel @5 :Int16;
    maxAccelLimit @6 :Int16;
    minAccelLimit @7 :Int16;
    headwaySetting @8 :UInt8;
    probLaneChgLeft @9 :UInt8;
    probLaneChgRight @10 :UInt8;
    accOn @11 :Bool;
    accNotOff @12 :Bool;
    decelLimOverride @13 :Bool;
    ctrlToRelevObj @14 :Bool;
    objectEffective @15 :Bool;
    ebaActivationObjId @16 :Int8;
}

struct VlcLongCtrlOutputT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    kinOutput @2 :KinOutputT;
    driverData @3 :KinDriverOutputT;
    kinFctInfo @4 :KinFctInfoT;
    custom @5 :LongCtrlOutputCustomT;
}

struct VlcParametersT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    eba @2 :CparEbaParametersT;
}

struct VlcSenAccOoiT {
    uiVersionNumber @0 :UInt32;
    sSigHeader @1 :SignalHeaderT;
    accOoiNextLong @2 :AccOoiNextLongT;
    accOoiHiddenNextLong @3 :AccOoiT;
    accOoiNextLeft @4 :AccOoiT;
    accOoiNextRight @5 :AccOoiT;
    accOoiHiddenNextLeft @6 :AccOoiT;
    accOoiHiddenNextRight @7 :AccOoiT;
}

struct VlcSenDebug {
    header @0 :StdHeader.Header;
    # SEN input
    #VLCCtrlData_t SenCtrlData
    #VED_VehDyn_t EgoDynObjSync
    #VED_VehDyn_t EgoDynRaw
    #VED_VehPar_t EgoStaticData
    #ECAMtCyclEnvmode_t ECAMtCyclEnvmode
    #Envm_t_GenObjectList EmGenObjList
    #Envm_t_CRObjectList EmARSObjList
    #VLC_DFV2SenInfo_t DFVLongOut
    #VLCCustomInput_t VLCCustomInput
    #Com_AlgoParameters_t BswAlgoParameters
    #VLC_Parameters_t CPAR_VLC_Parameters
    #t_CamLaneInputData CamLaneData
    #VLCSen_NVRAMData_t VLCSenNvRams
    #VLCSen_Parameters_t Param
    # AEB Hypothesis
    vlccdHypotheses @1 :HypothesisIntfT;
    # ACC OOI data
    vlcAccOoiData @2 :VlcSenAccOoiT;
}

struct VlcVehDebug {
    header @0 :StdHeader.Header;
    # VLCVEH input
    vehCtrlData @1 :VlcCtrlDataT;
    egoDynRaw @2 :VedVehDynT;
    vehSig @3 :VedVehSigT;
    accLever @4 :VlcAccLeverInputT;
    longCtrlResp @5 :LongCtrlInputT;
    accDisplayObj @6 :VlcAccObjectT;
    accOutput @7 :VlcAccOutputDataT;
    vlcAccOoiData @8 :VlcSenAccOoiT;
    camLaneData @9 :TCamLaneInputData;
    # VLCVEH output
    dfvLongOut @10 :VlcDfv2SenInfoT;
    longCtrlOutput @11 :VlcLongCtrlOutputT;
    headOutputCustom @12 :SadOutputCustomT;
    isaInfo @13 :IsaInfo;
    paccInfo @14 :PaccInfo;
    # VLCVEH debug
    vlcAccCtrlData @15 :CcAccelerationControlDataT;
    vlcCcCtrlData @16 :CcControlDataT;
}

struct YawRateVehDynT {
    yawRate @0 :Float32;
    variance @1 :Float32;
    quality @2 :Float32;
}
