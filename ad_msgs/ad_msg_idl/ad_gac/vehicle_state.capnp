@0xa8fcc98b8fac2e14;

using Gac = import "/ad_msg_idl/ad_gac/gac.capnp";

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");

struct ChassisState {
    stamp @0 :Gac.TimeStamp;
    vehicleSpeed @1 :Float32;
    frontLeftWheelSpeed @2 :Float32;
    frontRightWheelSpeed @3 :Float32;
    rearLeftWheelSpeed @4 :Float32;
    rearRightWheelSpeed @5 :Float32;
    frontLeftWheelPulse @6 :Float32;
    frontRightWheelPulse @7 :Float32;
    rearLeftWheelPulse @8 :Float32;
    rearRightWheelPulse @9 :Float32;
    lonAcceleration @10 :Float32;
    latAcceleration @11 :Float32;
    yawRate @12 :Float32;
    frontWheelAngle @13 :Float32;
    longitudinalControlValue @14 :Float32;
    energySourceLevel @15 :UInt8;
    gearPos @16 :UInt8;
    gearLevel @17 :UInt8;
    stateFlags @18 :UInt32;
    invalidFlags @19 :UInt32;
}



struct VehicleBodyState {
    stamp @0 :Gac.TimeStamp;
    frontLeftTirePressure @1 :Float32;
    frontRightTirePressure @2 :Float32;
    rearLeftTirePressure @3 :Float32;
    rearRightTirePressure @4 :Float32;
    stateFlags @5 :UInt32;
    invalidFlags @6 :UInt32;
}



struct ExternalState {
    stamp @0 :Gac.TimeStamp;
    actuatorStates @1 :UInt32;
    functionStates @2 :UInt32;
    acuSwitch @3 :UInt32;
    acuCard @4 :UInt32;
    ndaLcStyleReq @5 :UInt8;
    iaccFunctionReq @6 :UInt8;
    ndaVoiceMode @7 :UInt8;
    acuButtonState @8 :UInt8;
    crashState @9 :Bool;
    epsWheelAngle @10 :Float32;
    totalOdometer @11 :UInt32;
    auid @12 :UInt8;
    deleteaccount @13 :UInt16;
}



struct UserOperation {
    stamp @0 :Gac.TimeStamp;
    targetSpeed @1 :UInt8;
    lkaLdwSensitivity @2 :UInt8;
    alcSensitivity @3 :UInt8;
    operations @4 :UInt32;
    configurations @5 :UInt32;
    invalidFlags @6 :UInt32;
    steeringWheelTorque @7 :Float32;
    accPedal @8 :Float32;
    rainLevel @9 :UInt8;
    steeringWheelAngularSpeed @10 :UInt32;
    masterCylinderPressure @11 :Float32;
    outdoorTemperature @12 :Float32;
    ebbActoutputrodtravel @13 :Float32;
}




struct MotionActuatorState {
    stamp @0 :Gac.TimeStamp;
    steeringWheelAngularSpeed @1 :Float32;
    epsTorque @2 :Float32;
    espDeceleration @3 :Float32;
    requestMotorTorque @4 :Float32;
    motorTorque @5 :Float32;
}
