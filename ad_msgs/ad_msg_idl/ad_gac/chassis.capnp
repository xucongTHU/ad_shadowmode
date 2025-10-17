@0x90e91b7e7b13fa51;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");


struct AutonomyMode{
    autonomyLevel @0:UInt8;
    gearAutonomous @1:Bool;
    speedAutonomous @2:Bool;
    steeringAutonomous @3:Bool;
}

struct Float32WithValid{
    value @0:Float32;
    confidence @1:UInt8;
}

struct Uint8WithValid{
    value @0:UInt8;
    confidence @1:UInt8;
}

struct Gear {
    value @0:UInt8;
}

struct GearInfo {
    gear @0:Gear;
    gearLever @1:Gear;
    gearShiftStatus @2:UInt8;
    driverOverride @3:Uint8WithValid;
    faultCode @4:Int32;
}

struct ActuatorWorkStatus {
    capability @0:UInt8;
    response @1:UInt8;
}

struct ActuatorStatus {
    angleSteer @0:ActuatorWorkStatus;
    torqueSteer @1:ActuatorWorkStatus;
    emergencyAngleSteer @2:ActuatorWorkStatus;
    emergencyTorqueSteer @3:ActuatorWorkStatus;
    steerVibrate @4:ActuatorWorkStatus;
    accelerate @5:ActuatorWorkStatus;
    decelerate @6:ActuatorWorkStatus;
    emergencyDecelerate @7:ActuatorWorkStatus;
    drive @8:ActuatorWorkStatus;
    brake @9:ActuatorWorkStatus;
    vlc @10:ActuatorWorkStatus;
    emergencyStop @11:ActuatorWorkStatus;
    stop @12:ActuatorWorkStatus;
    park @13:ActuatorWorkStatus;
    gear @14:ActuatorWorkStatus;
}

struct SteerInfo {
    steerAngle @0:Float32WithValid;
    steerAngleRate @1:Float32WithValid;
    steerPinionAngle @2:Float32WithValid;
    steerPinionAngleRate @3:Float32WithValid;
    frontSteerAngle @4:Float32WithValid;
    frontSteerAngleRate @5:Float32WithValid;
    driverHandOn @6:Float32WithValid;
    driverHandTorque @7:Float32WithValid;
    steerTorque @8:Float32WithValid;
    motorCurrent @9:Float32WithValid;
    driverOverride @10:Uint8WithValid;
    personalMode @11:UInt8;
    commandFault @12:UInt8;
    epsStatusMaster @13:UInt8;
    epsStatusSlave @14:UInt8;
    faultCode @15:Int32;
    drvrSteerMonrEnaSts @16:Bool;
    handsOffConf @17:Uint8WithValid;
    epsMotorTq @18:Float32WithValid;
    epsMotorTemp @19:UInt8;
    ldwWarnSts @20:UInt8;
    epsOperMod @21:UInt8;
    epsAbortFb @22:UInt8;
    epsTqSensSts @23:UInt8;
    epsSteerAgSensFilr @24:UInt8;
}

struct WheelSpeed {
    wheelDirection @0:UInt8;
    wheelCount @1:Int32;
    wheelCountValid @2:Bool;
    wheelSpeedMps @3:Float32;
    wheelSpeedMpsValid @4:Bool;
}

struct BrakeInfo {
    brakePedal @0:Float32WithValid;
    brakePedalSwitch @1:Float32WithValid;
    driverOverride @2:Float32WithValid;
    mastCylinderPressure @3:Float32WithValid;
    mastCylinderPosition @4:Float32WithValid;
    brakeTorque @5:Float32WithValid;
    brakeForce @6:Float32WithValid;
    brakeTorqueMax @7:Float32WithValid;
    driverDesiredTorque @8:Float32WithValid;
    brakeTotalTqReq @9:Float32WithValid;
    elecBrkTqReq @10:Float32WithValid;
    hydrauBrkTqReq @11:Float32WithValid;
    brakeOverHeatStatus @12:UInt8;
    aebAvl @13:UInt8;
    aebActv @14:UInt8;
    prefillAvl @15:UInt8;
    prefillActv @16:UInt8;
    brkCtrlAvl @17:UInt8;
    brkCtrlActv @18:UInt8;
    espAbortFb @19:UInt8;
    velocity @20:Float32WithValid;
    wheelSpeedFl @21:WheelSpeed;
    wheelSpeedFr @22:WheelSpeed;
    wheelSpeedRl @23:WheelSpeed;
    wheelSpeedRr @24:WheelSpeed;
    frontWhlIncTarTq @25:Float32WithValid;
    frontWhlDecTarTq @26:Float32WithValid;
    rearWhlIncTarTq @27:Float32WithValid;
    rearWhlDecTarTq @28:Float32WithValid;
    standstill @29:Uint8WithValid;
    roadFriction @30:Float32WithValid;
    brakeTemperature @31:Float32WithValid;
    slope @32:Float32WithValid;
    brakeStatusMaster @33:UInt8;
    brakeStatusSlave @34:UInt8;
    commandFault @35:UInt8;
    epbFaultStatus @36:UInt8;
    epbLockStatus @37:UInt8;
    epbButtonStatus @38:UInt8;
    epbOverride @39:UInt8;
    epbAvl @40:UInt8;
    epbClampForce @41:Float32;
    faultCode @42:UInt8;
    autoHoldActvSts @43:Int32;
    ebdActive @44:Bool;
    ebdStatus @45:UInt8;
    absActive @46:Bool;
    absStatus @47:UInt8;
    escActive @48:Bool;
    escStatus @49:UInt8;
    tcsActive @50:Bool;
    tcsStatus @51:UInt8;
    arpActvSts @52:UInt8;
    arpFctAvl @53:UInt8;
    arpFailrSts @54:UInt8;
    dbfActvSts @55:UInt8;
    hdcActvSts @56:UInt8;
    hhcActvSts @57:UInt8;
    ebaActvSts @58:UInt8;
    ebaAvl @59:UInt8;
    ebaActvRank @60:UInt8;
    awbAvl @61:UInt8;
    awbActv @62:UInt8;
    espABrkDegStWarning @63:UInt8;
    espABrkDegSt @64:UInt8;
    espPriBrStatus @65:UInt8;
}

struct ThrottleInfo{
    throttlePedal @0:Float32WithValid;
    throttlePedalRate @1:Float32WithValid;
    driverOverride @2:Uint8WithValid;
    driveTorque @3:Float32WithValid;
    driveTorqueMax @4:Float32WithValid;
    driveTorqueMin @5:Float32WithValid;
    driverDesiredTorque @6:Float32WithValid;
    engineSpeed @7:Float32WithValid;
    motorSpeed @8:Float32WithValid;
    powerTrainReady @9:Uint8WithValid;
    driveTorqueCtrlAvl @10:UInt8;
    driveTorqueActv @11:UInt8;
    vcuAbortFbk @12:UInt8;
    fuelRange @13:Float32WithValid;
    socHighVoltBattery @14:Float32WithValid;
    sohHighVoltBattery @15:Float32WithValid;
    socLowVoltBattery @16:Float32WithValid;
    sohLowVoltBattery @17:Uint8WithValid;
    vcuStatus @18:UInt8;
    personalMode @19:UInt8;
    commandFault @20:UInt8;
    faultCode @21:Int32;
    frontMotorSpeed @22:Float32WithValid;
    frontAxleActualTorque @23:Float32WithValid;
    rearMotorSpeed @24:Float32WithValid;
    rearAxleActualTorque @25:Float32WithValid;
    trailerStatus @26:UInt8;
}

struct VehicleMotion {
    vx @0:Float32WithValid;
    vy @1:Float32WithValid;
    vz @2:Float32WithValid;
    ax @3:Float32WithValid;
    ay @4:Float32WithValid;
    az @5:Float32WithValid;
    yaw @6:Float32WithValid;
    pitch @7:Float32WithValid;
    roll @8:Float32WithValid;
    yawRate @9:Float32WithValid;
    pitchRate @10:Uint8WithValid;
    rollRate @11:Uint8WithValid;
    yawAcceleration @12:Uint8WithValid;
    pitchAcceleration @13:Uint8WithValid;
    rollAcceleration @14:Uint8WithValid;
}

struct ChassisReportTime {
    sec @0:UInt32;
    nsec @1:UInt32;
}

struct ChassisReportHeader {
    seq @0:UInt32;
    stamp @1:ChassisReportTime;
    frameId @2:Text;
}

struct ChassisReportMsg {
    autoMode @0:AutonomyMode;
    velocity @1:Float32;
    steerAngle @2:Float32;
    frontWheelAngle @3:Float32;
    actualGear @4:Gear;
    master @5:ActuatorStatus;
    slave @6:ActuatorStatus;
    steer @7:SteerInfo;
    brake @8:BrakeInfo;
    vcu @9:ThrottleInfo;
    gear @10:GearInfo;
    egoMotion @11:VehicleMotion;
    header @12:ChassisReportHeader;
}