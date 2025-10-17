@0xb7fbe7c6947524a3;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::l2plus_ved");
using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";
using Header = import "/ad_msg_idl/std_msgs/header.capnp";

struct VEDDrvIntCurveVehDyn {
    curve @0 :Float32;

    variance @1 :Float32;

    gradient @2 :Float32;

}
struct VEDLateral {
    yawRate @0 :VEDYawRateVehDyn;

    offCompStWheelangle @1 :Float32;

    curve @2 :VEDCurveVehDyn;

    drvIntCurve @3 :VEDDrvIntCurveVehDyn;

    accel @4 :VEDLatAccelVehDyn;

    slipAngle @5 :VEDSideSlipVehDyn;

}
struct VEDSideSlipVehDyn {
    sideSlipAngle @0 :Float32;

    variance @1 :Float32;

}
struct VEDMotVarVehDyn {
    velocity @0 :Float32;

    accel @1 :Float32;

    varVelocity @2 :Float32;

    varAccel @3 :Float32;

}
struct VEDAccelCorrVehDyn {
    corrAccel @0 :Float32;

    corrAccelVar @1 :Float32;

}
struct VEDLongitudinal {
    motVar @0 :VEDMotVarVehDyn;

    veloCorr @1 :VEDVeloCorrVehDyn;

    accelCorr @2 :VEDAccelCorrVehDyn;

}
struct VEDInfo {
    header @0 :Header.Header;
    # Header include seq, timestamp(last node pub time), and frame_id(sensor model)
    measurementTimeNs @1 :UInt64;
    # The time of pose measurement, in nano second.
    statusInfo @2 :Text;
    # Report ved status
    # when INVALID or CONVERGING, ved result is not reliable and should not be used.
    linearVelocity @3 :Geo.Vector3;
    # Linear velocity of the Vehicle Reference Point(VRP) in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in meters per second
    linearAcceleration @4 :Geo.Vector3;
    # Linear acceleration of the Vehicle Reference Point(VRP) in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in meters per second^2
    angularVelocity @5 :Geo.Vector3;
    # Angular velocity of the VRP in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in rad per second
    uiVersionNumber @6 :UInt32;
    # Version number of interface
    sigHeader @7 :VEDSignalHeader;
    # Information for synchronisation of SIL simulation
    longitudinal @8 :VEDLongitudinal;
    # Longitudinal motion variables
    lateral @9 :VEDLateral;
    # Lateral motion variables
    motionState @10 :VEDMotionStateVehDyn;

    legacy @11 :VEDLegacyVehDyn;

    state @12 :List(UInt8);

}
struct VEDLatAccelVehDyn {
    latAccel @0 :Float32;

    variance @1 :Float32;

}
struct VEDYawRateVehDyn {
    yawRate @0 :Float32;

    variance @1 :Float32;

    quality @2 :Float32;

}
struct VEDSignalHeader {
    uiTimestamp @0 :UInt32;

    uiMeasurementCounter @1 :UInt16;

    uiCycleCounter @2 :UInt16;

    sigStatus @3 :UInt8;

}
struct VEDCurveVehDyn {
    curve @0 :Float32;

    c1 @1 :Float32;

    gradient @2 :Float32;

    varC0 @3 :Float32;

    varC1 @4 :Float32;

    quality @5 :Float32;

    crvError @6 :Float32;

    crvConf @7 :UInt8;

}
struct VEDMotionStateVehDyn {
    motState @0 :UInt8;

    confidence @1 :Float32;

}
struct VEDVeloCorrVehDyn {
    corrFact @0 :Float32;

    corrVar @1 :Float32;

    corrVelo @2 :Float32;

    corrVeloVar @3 :Float32;

    minVelo @4 :Float32;

    maxVelo @5 :Float32;

    corrQual @6 :UInt8;

    rollerTestBench @7 :Bool;

}
struct VEDLegacyVehDyn {
    yawRateMaxJitter @0 :Float32;

    standStill @1 :Bool;

}
