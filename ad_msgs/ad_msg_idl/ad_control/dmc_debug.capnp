@0xa7c4822217d46fcd;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::control");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct DMCInput {
    # DMC Input From ADAS
    fCrvCtrlCntr @0 : Float32;            
    fHeadingCtrlCntr @1 : Float32;       
    fPosY0CtrlCntr @2 : Float32;  
    ldpscDgrSideSt @3 : UInt8;         
    ldpscSysOutSt @4 : UInt8;           
    uiLatCtrlModeNu @5 : UInt8;          
    cdcCtrlErrDistY @6 : Float32;        
    cdcCtrlErrHeading @7 : Float32;      
    tjatctDeltaFCmd @8 : Float32;        
    lgcEnableCtrlNu @9 : Bool;          
    fTrajTgtCurve1pm @10 : Float32;       
    adasLonAccCmd @11 : Float32;        
    adasLonAccEnableflag @12 : Bool;   

    # DMC Input From NOP
    nopLonAccCmd @13 : Float32;         
    nopDeltaFCmd @14 : Float32;         
    nopLonAccEnableFlag @15 : Bool;    
    nopDeltaFEnableFlag @16 : Bool;    

    # DMC Input for LoDMC
    engTRQNm @17 : Float32;
    vehicleVelXMps @18 : Float32;
    vehAccelMps2 @19 : Float32;
    vehAccelCommandMps2 @20 : Float32;
}

struct DMCDebug {
    # Debug msg for DMC1
    adpDycCorrFactor @0 : Float32;
    camLateralErrorSign @1 : Float32;
    dycFilterKappaCommand @2 : Float32;
    dycSteerAngleFeedforward @3 : Float32;
    hecYawRateFilter @4 : Float32;
    initialisationFlag @5 : Bool;
    latKappaLinzFilterOutput @6 : Float32;
    latOcIntegratorOutput @7 : Float32;
    latSatDynamicThresholdInt @8 : Float32;
    lateralErrorDelta @9 : Float32;
    lateralErrorMean @10 : Float32;
    meanSampleUpdateSum @11 : Float32;
    meanVehicleVelocity @12 : Float32;
    meanVehicleVelocitySum @13 : Float32;
    meanKappaCommand @14 : Float32;
    meanKappaCommandSum @15 : Float32;
    newUpdateAval @16 : Bool;
    newUpdateAvalSum @17 : Float32;
    sacAngleCommandCorr @18 : Float32;
    sacAngleCommandYawrateFback @19 : Float32;
    sacArbitratedAngleCmd @20 : Float32;
    sacArbitratedAngleCmdRaw @21 : Float32;
    sacCompensationAngleCommand @22 : Float32;
    sacControlError @23 : Float32;
    sacDeratedAngleCommand @24 : Float32;
    sacIntegratorSatOut @25 : Float32;
    sacRateLimAngleCommand @26 : Float32;
    sacTrqDeratingFactor @27 : Float32;
    sacYrcAngleCommand @28 : Float32;
    sacYrcControlError @29 : Float32;
    satReqDynSteerAngleMax @30 : Float32;
    satReqSteerAngleMax @31 : Float32;
    satSaturatedAngleCommand @32 : Float32;
    tdfCompositeDeratingFactor @33 : Float32;
    tdfIdleDeratingFactor @34 : Float32;
    tdfSelectedStateDeratingFactor @35 : Float32;
    vehDeltaFOc @36 : Float32;
    vehDeltaFOffset @37 : Float32;
    tdfDriverCounterSteering @38 : Bool;
    tdfMaxSteerTorque @39 : Float32;
    tdfSelectedTorqueSource @40 : Float32;
    tdfTorqueDerFactorHFPath @41 : Float32;
    tdfTorqueDeratingFactor @42 : Float32;
    tdfTorqueDeratingSlopArb @43 : Float32;
    tdfTrqDeratingThresholdArb @44 : Float32;
    tdfVehicleSteerTorqueFactor @45 : Float32;
    tdfTorqueRequestFactor @46 : Float32;
    vehSteerTorqueComp @47 : Float32;
    latOcStatePreload @48 : Float32;
    latOcCalHoldFlag @49 : Bool;
    latOcCalHoldFlagShrt @50 : Bool;
    latOcDisableFlag @51 : Bool;
    latOcDysActive @52 : Bool;
    latOcFilteredKappaCam @53 : Float32;
    latOcHighDriverTorque @54 : Bool;
    latOcImplausLateralError @55 : Bool;
    latOcIntegratorInput @56 : Float32;
    latOcIntegratorInputKappa @57 : Float32;
    latOcIntegratorInputKappaDbg @58 : Float32;
    latOcIntegratorSatOut @59 : Float32;
    latOcKappaActive @60 : Bool;
    latOcKappaConEnbFlag @61 : Bool;
    latOcKappaLatencyState @62 : Float32;
    latOcKappaStatusDbg @63 : Float32;
    latOcMaxDeltaFDotFlag @64 : Bool;
    latOcMaxDrvTrqFlag @65 : Bool;
    latOcMaxHeaErrFlag @66 : Bool;
    latOcMaxKappaCmdFlag @67 : Bool;
    latOcMaxLatAccFlag @68 : Bool;
    latOcSatIntegratorState @69 : Float32;
    latOcTriggerFlagKappa @70 : Bool;
    latOcTriggerFlagKappaState @71 : Float32;
    latOcOffsetFilterOmega @72 : Float32;
    latOcMinVehVelFlag @73 : Bool;
    latOcMaxFltKappaCmdFlag @74 : Bool;

    # Reserved for DMC2
    latArbEpsTorqueRequest @75 : Float32;
    latDirectFeedthroughTrqPar @76 : Float32;
    latEpsTorqueRequestInterface @77 : Float32;
    latEpsTorqueRequestOutputLimted @78 : Float32;
    latTorqueGradientPar @79 : Float32;
    latTorqueRequestNoFric @80 : Float32;
    rateLimiterOutput @81 : Float32;
    sacActiveDampingTorque @82 : Float32;
    sacControlErrorDMC2 @83 : Float32;
    sacDeltaFDotFiltered @84 : Float32;
    sacDisable @85 : Bool;
    sacDynamicFeedforwardTorque @86 : Float32;
    sacFilteredAngleCommand @87 : Float32;
    sacIntegratorTorque @88 : Float32;
    sacProportionalTorque @89 : Float32;
    sacTorqueSatOut @90 : Float32;
    satSaturatedAngleCommandDMC2 @91 : Float32;

    # Reserved for LoDMC
    engTrqArw @92 : Float32;
    engTrqReqFF @93 : Float32;
    engTrqReqInteg @94 : Float32;
    engTrqReqProp @95 : Float32;
    uintDelayTRQRate @96 : Float32;
    uintDelayTRQRateFilter @97 : Float32;
    unitDelayTRQARWFitler @98 : Float32;
    unitDelayTRQIntergral @99 : Float32;
    unitDelayTorqueReq @100 : Float32;
}

struct DMCOutput {
    fSteerAngleDeg @0 : Float32;           
    fSteerWhlAngleDeg @1 : Float32;        
    fLonAccCmd @2 : Float32;                
    uiEPSRequestNu @3 : UInt8;             
                                    
    uiLonAccRequestNu @4 : UInt8;          

    # Reserved for DMC2
    fSteerWhlTqAddNm @5 : Float32;         
    fDampingLevelRmpNu @6 : Float32;       
    bDMCValidStatus @7 : Bool;              

    # Reserved for LoDMC
    fTorqueReqNm @8 : Float32;  
    fDecelReqMps2 @9 : Float32;  
    bBrakeActiveNu @10 : Bool;  
}


struct DMCInfo {
    header @0 : StdMsgsHeader.Header;
    sDMCInput @1 : DMCInput;
    sDMCDebug @2 : DMCDebug;
    sDMCOutput @3 : DMCOutput;
}