@0xd7bd2a9a3358169d;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::simulator");

using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";
using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct RealEgoInfo {
    isValid @0 :Bool;
    x @1 :Float32;
    y @2 :Float32;
    yaw @3 :Float32;
    length @4 :Float32;
    width @5 :Float32;
    height @6 :Float32;
    type @7 :Int32;
    id @8 :Int32;
}

struct SimulationHMInfo {
    header @0 :StdHeader.Header;

    caseName @1 :Text;
    evaluatorStatus @2 :Text;
    isLogCase @3 :Bool;
    realEgoInfo @4 :RealEgoInfo;
}

struct SUMOEgoVehicleState {
    header @0 :StdHeader.Header;
    positionX @1 :Float32;
    positionY @2 :Float32;

    heading @3 :Float32;

    velocity @4 :Float32;
}

struct SUMOTrafficLightState {
    # Array variable message of VehicleStatus
    header @0 :StdHeader.Header;
    roadFrom @1 :List(Text);
    roadTo @2 :List(Text);
    state @3 :List(Text);
}

struct SUMOTrafficLightStateArray {
    header @0 :StdHeader.Header;
    lights @1 :List(SUMOTrafficLightState);
}

struct SUMOVehicleStatus {
    # Message to send information about Vehicles in the scene

    # id : The idenfification of the vehicle
    # pos_x : Vehicle x position
    # pos_y : Vehicle y position
    # heading : Vehicle heading (Yaw angle)
    # velocity : Linear velocity of the vehicle
    # max_vel : Maximum velocity

    header @0 :StdHeader.Header;
    vehicleId @1 :Text;
    posX @2 :Float32;
    posY @3 :Float32;
    heading @4 :Float32;
    velocity @5 :Float32;
    maxVel @6 :Float32;
    lane @7 :Int32;
    signals @8 :Int32;
    height @9 :Float32;
    width @10 :Float32;
    length @11 :Float32;

    predictionX @12 :List(Float32);
    predictionY @13 :List(Float32);
}

struct SUMOVehicleStatusArray {
    # Array variable message of VehicleStatus
    header @0 :StdHeader.Header;
    vehiclesdetected @1 :List(SUMOVehicleStatus);
}

struct HMIInteraction {
    header @0 :StdHeader.Header;
    # Header include seq, timestamp, and frame_id

    interactionType @1 :Int32;
    # enum InteractionType
    # uint32 NOOPERATION = 0
    # uint32 INCREASECLICK = 1
    # uint32 INCREASEDOUBLECLICK = 2
    # uint32 DECREASECLICK = 3
    # uint32 DECREASEDOUBLECLICK = 4
    # uint32 ADJUSTHEADWAY = 5
    # uint32 LEFTTURNLEVER = 6
    # uint32 RIGHTTURNLEVER = 7
    # uint32 LEFTVOICE = 8
    # uint32 RIGHTVOICE = 9
    # uint32 LANECHANGECANCEL = 10
}

struct RealVehicleInfo {
    id @0 :Int32;
    label @1 :Int16;
    trajectory @2 :Geo.Point;
}

struct InterVehicleInfo {
    id @0 :Int32;
    label @1 :Int16;
}

struct SimulatorReport {
    simStartTime @0 :Int64;
    trajectoryTime @1 :Int64;
    keyTime @2 :Int64;
    egoTrajectory @3 :Geo.Point;
    realVehicleInfoList @4 :List(RealVehicleInfo);
    interVehicleInfoList @5 :List(InterVehicleInfo);
}