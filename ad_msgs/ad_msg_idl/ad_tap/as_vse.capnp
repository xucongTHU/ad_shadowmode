@0xbee99e92c156b187;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::tap");




struct AsVseHeader {
    vseVersionMajor @0 :UInt8;
    vseVersionMinor @1 :UInt8;
    u16VseReserve01 @2 :UInt8;
    startTimestampMs @3 :UInt32;
    startTimestampUsHigh @4 :UInt32;
    startTimestampUsLow @5 :UInt32;
    endTimestampMs @6 :UInt32;
    endTimestampUsHigh @7 :UInt32;
    endTimestampUsLow @8 :UInt32;
}

struct AsVseSeries {
    timestampMs @0 :List(UInt64);
    yawrateRps @1 :List(Float32);
    speedMps @2 :List(Float32);
    slipRad @3 :List(Float32);
    rearPositionLongM @4 :List(Float32);
    rearPositionLatM @5 :List(Float32);
    headingRad @6 :List(Float32);
    pitchRad @7 :List(Float32);
    rollRad @8 :List(Float32);
}
struct AsVseOut {
    vseHeader @0 :AsVseHeader;
    stateSeries @1 :AsVseSeries;
    hostLength @2 :Float32;
    hostWidth @3 :Float32;
    hostDistToFront @4 :Float32;
    speed @5 :Float32;
    displaySpeed @6 :Float32;
    yawRate @7 :Float32;
    yawRateUnfilteredRps @8 :Float32;
    longAccel @9 :Float32;
    latAccel @10 :Float32;
    hostPitchRad @11 :Float32;
    hostRollRad @12 :Float32;
    vcsSideslip @13 :Float32;
    vcsLongVel @14 :Float32;
    vcsLatVel @15 :Float32;
    sensorSideslip @16 :Float32;
    sensorLongVel @17 :Float32;
    sensorLatVel @18 :Float32;
    rearCurvature @19 :Float32;
    rearCurvaturerate @20 :Float32;
    rearCurvatureRadius @21 :Float32;
    rearSideslip @22 :Float32;
    steerWheelAngle @23 :Float32;
    steerWheelAngleRate @24 :Float32;
    slopeLongRad @25 :Float32;
    slopeLatRad @26 :Float32;
    gasPedalPos @27 :Float32;
    brakePedalPos @28 :Float32;
    brakePressure @29 :Float32;
    gasPedalPosRate @30 :Float32;
    brakePedalPosRate @31 :Float32;
    brakePressureRate @32 :Float32;
    hazardLampSt @33 :UInt8;
    frontFogLampSt @34 :UInt8;
    backFogLampSt @35 :UInt8;
    leftLightFaultSt @36 :UInt8;
    rightLightFaultSt @37 :UInt8;
    epbApplied @38 :UInt8;
    highBeamSt @39 :UInt8;
    vdcActiveSt @40 :UInt8;
    tcsActiveSt @41 :UInt8;
    hdcActiveSt @42 :UInt8;
    escOffSt @43 :UInt8;
    flDoorSt @44 :UInt8;
    frDoorSt @45 :UInt8;
    rlDoorSt @46 :UInt8;
    rrDoorSt @47 :UInt8;
    turnLightSwitchPos @48 :UInt8;
    leftLightSt @49 :UInt8;
    rightLightSt @50 :UInt8;
    engHoodUnLockWarn @51 :UInt8;
    trunkSt @52 :UInt8;
    frontWiperWorkSt @53 :UInt8;
    fReverseStatus @54 :UInt8;
    driverBeltSwitchSt @55 :UInt8;
    actualGear @56 :UInt8;
    brakePedalType @57 :UInt8;
    fcwWarnDist @58 :UInt8;
    longSafeSwitch @59 :UInt8;
    bsdSwitch @60 :UInt8;
    dowSwitch @61 :UInt8;
    fctbSwitch @62 :UInt8;
    rctbSwitch @63 :UInt8;
    rcwSwitch @64 :UInt8;
    brakePedalApld @65 :UInt8;
    absActive @66 :UInt8;
    aebActive @67 :UInt8;
    powerMode @68 :UInt8;
    longCtrlBrakeActive @69 :UInt8;
    straightDriving @70 :UInt8;
    highDynamic @71 :UInt8;
    hostState @72 :UInt16;
    espBrakeAvailableSt @73 :UInt32;
    functionDidConfig@74 :UInt32;
}
