@0xede08d13059d8565;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_planning");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

using import "/ad_msg_idl/std_msgs/typed_data_list.capnp".Float64List;

struct TrajectoryPoint {
    timestampNs @0 :UInt64;
    position @1 :StdGeometry.Point;
    direction @2 :StdGeometry.Point;
    velocity @3 :Float64;
    theta @4 :Float32;
    steerAngle @5 :Float64;
    curvature @6 :Float32;
    sumDistance @7 :Float64;
    timeDifference @8 :Float64;
    yawRate @9 :Float64;
    acceleration @10 :Float64;
    trajNumberOfSurplus @11 :UInt32;
    isTurnSteerOfNextPath @12 :Bool;
}

struct Trajectory {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    trajPointArray @1 :List(TrajectoryPoint);

    # Gear command
    enum Gear {
        gearNone @0;
        gearPark @1;
        gearReverse @2;
        gearNeutral @3;
        gearDrive @4;
        gearLow @5;
    }
    gearEnum @2 :Gear;

    # Turn signal command
    enum TurnSignal {
        turnNone @0;
        turnLeft @1;
        turnRight @2;
        turnEmergency @3;
    }
    turnSignalEnum @3 :TurnSignal;

    # Status
    updatedByVehicleStatus @4 :Bool;

    # Trajectory Running Mode
    enum TrajectoryMode {
        modeNone @0;
        modeAcc @1;
        modeLka @2;
        modeHppCruise @3;
        modeParking @4;
    }
    trajectoryMode @5 :TrajectoryMode;

    trajectoryEnds @6 :List(TrajectoryPoint);

    # Turn Steer For Next Path
    enum TurnSteer {
        steerNone @0;
        steerReturn @1;
        steerCompleted @2;
        steerCompletedHalf @3;
    }
    turnSteerEnum @7 :TurnSteer;
    trajSumDistance @8 :Float64;
    isFinalSegment @9 :Bool;
    distancecut @10 :Float64;
}

struct TrafficLight {
    header @0 :StdHeader.Header;

    id @1 :Text;
    signalSequence @2 :List(UInt32);
}

struct ReflineDebug {
    header @0 :StdHeader.Header;

    pathPts @1 :List(StdGeometry.Point);
    originIdx @2 :UInt32;
    ifValid @3 :UInt32;
    firstInvalidIdx @4 :UInt32;
}

struct PlanningLine {
    header @0 :StdHeader.Header;

    points @1 :List(StdGeometry.Point);
    colors @2 :Float64List;
    # line width
    width @3 :Float64;
    # type
    type @4 :Text;
}

struct PlanningDebug {
    header @0 :StdHeader.Header;

    timestamp @1 :UInt64;
    num @2 :UInt32;
    predictTimestamp @3 :UInt64;
    x @4 :Float64List;
    y @5 :Float64List;
    v @6 :Float64List;
    yaw @7 :Float64List;
    a @8 :Float64List;
    jerk @9 :Float64List;
    speedLimit @10 :Float64;
    stopDistance @11 :Float64;
    speedRef @12 :Float64;
    placeholder1 @13 :Float64;
    placeholder2 @14 :Float64;
    placeholder3 @15 :Float64;
    obstacleTagBinary @16 :Text;
    # pp text box
    ppTextboxString @17 :Text;
    # pp planning lines
    lines @18 :List(PlanningLine);
    # offline replay info
    onlineStoredData @19 :Text;
}

struct DecisionTarget {
    header @0 :StdHeader.Header;
    # current velocity of ego vehicle, unit, m/s
    currentVelocity @1 :Float32;
    # DM scenarios for display
    lateralScenario @2 :Int32;
    longitudinalScenario @3 :Int32;
    lateralTask @4 :Int32;
    longitudinalTask @5 :Int32;
    # area type, OUTOFMAP = 0, ROAD = 1, JUNCTION = 2, NEARJUNCTION = 3
    area @6 :UInt32;
    # lateal action, LANEKEEP = 0, LANECHANGE = 1, NUDGE = 2
    latact @7 :UInt32;
    # longitudinal action, STOP = 0, CRUISE = 1, SLOWDOWN = 2
    lngact @8 :UInt32;
    # change direction, NO_CHANGE = 0, CHANGE_LEFT = 1, CHANGE_RIGHT = 2
    changeDirection @9 :UInt32;
    # current refline in ENU coordinate, unit, m
    currentReflineEnu @10 :List(StdGeometry.Point);
    # target refline in ENU coordinate, unit, m
    targetReflineEnu @11 :List(StdGeometry.Point);
    # speed limit, unit, m/s
    speedLimit @12 :Float32;
    # stop point, unit, m
    stopPoint @13 :StdGeometry.Point;
    # speed limit point
    speedLimitPoint @14 :StdGeometry.Point;
    # flag for pre lane change
    routingLaneChangeFlag @15 :UInt32;
    # lane change end point
    laneTypeChangePoint @16 :StdGeometry.Point;
    # flag for constinuous lane change
    routingChangeLaneTwiceFlag @17 :Bool;
    # lane change end point in continuous change
    laneTypeChangeTwicePoint @18 :StdGeometry.Point;
    # flag for refline change
    reflineChangeFlag @19 :Bool;
    # turning signal blub, right = 1, left = 2, both = 3, none = 0
    signalBlub @20 :UInt32;
    # turning type, GO_STRAIGHT = 0, TURN_RIGHT = 1, TURN_LEFT = 2
    turntype @21 :UInt32;
    # flag for traffic light
    trafficStop @22 :Bool;
}

struct DecisionDebug {
    header @0 :StdHeader.Header;
    reflines @1 :List(ReflineDebug);
    junctionReflines @2 :List(ReflineDebug);
    arriveTargetLane @3 :Bool;
    lateralStage @4 :UInt32;
    ifNeedRoutingLanechange @5 :UInt32;
    lanechangeCauseSrlIdx @6 :UInt32;
    lanechangeRoutingIdx @7 :UInt32;
    efficientLane @8 :UInt32;
    efficientCount @9 :List(Float64);
    longsceneApplied @10 :List(UInt32);
    highlightLongscene @11 :UInt32;
    latsceneTrans @12 :List(UInt32);
    latStage @13 :UInt32;
    dist2junction @14 :Float64;
    currRoadid @15 :Text;
    currLandid @16 :Text;
    ifInJunction @17 :UInt32;
    faultVehicleCount @18 :UInt32;
    reflineChangeFlag @19 :UInt32;
    routingChangeLaneTwiceFlag @20 :UInt32;
    trafficlights @21 :List(TrafficLight);
    reflineBlockForUnknown @22 :List(Bool);
    junctionLinkBlockForUnknown @23 :List(Bool);
    blockUnknownIdLine @24 :List(Int32);
}

struct MapLinePoint {
  timestampNs @0 :UInt64;
  position @1 :StdGeometry.Point;
  direction @2 :StdGeometry.Point;
  theta @3 :Float64;
  kappa @4 :Float64;
  s @5 :Float64;
  dkappa @6 :Float64;
  ddkappa @7 :Float64;
  xDerivative @8 :Float64;
  yDerivative @9 :Float64;
}

struct PathPoint {
  timestampNs @0 :UInt64;
  position @1 :StdGeometry.Point;
  direction @2 :StdGeometry.Point;
  theta @3 :Float64;
  steer @4 :Float64;
  kappa @5 :Float64;
  s @6 :Float64;
  dkappa @7 :Float64;
  ddkappa @8 :Float64;
  xDerivative @9 :Float64;
  yDerivative @10 :Float64;
}

struct ReferenceLineAndLaneLine {
  header @0 :StdHeader.Header;

  referenceLinePointArray @1 :List(MapLinePoint);

  leftLaneLinePointArray @2 :List(MapLinePoint);
  rightLaneLinePointArray @3 :List(MapLinePoint);

  leftFreeSpacePointArray @4 :List(MapLinePoint);
  rightFreeSpacePointArray @5 :List(MapLinePoint);

  leftOriginFSBoundPointArray @6 :List(MapLinePoint);
  rightOriginFSBoundPointArray @7 :List(MapLinePoint);

  leftOriginBoundPointArray @8 :List(MapLinePoint);
  rightOriginBoundPointArray @9 :List(MapLinePoint);
}

struct TurnReminder {
    header @0 :StdHeader.Header;

    turnLeft @1 : Bool;
    turnRight @2 : Bool;
}

struct OrientedBoundingBox {
    header @0 :StdHeader.Header;
    x @1 :Float32;
    y @2 :Float32;

    length @3 :Float32;
    width @4 :Float32;
    height @5 :Float32;
    yaw @6 :Float32;
}

struct AgentState {
    header @0 :StdHeader.Header;
    position @1 :StdGeometry.Point;
    rollPitchYaw @2 :StdGeometry.Vector3;

    velocity @3 :StdGeometry.Point;
    acceleration @4 :StdGeometry.Point;
}

struct AgentDebug {
    header @0 :StdHeader.Header;
    # for agents
    id @1 :Int64;
    objectLabel @2 :Int16;
    state @3 :AgentState;
    decision @4 :Int64;
    obb @5 :OrientedBoundingBox;
    egoRelevance @6 :Int8;
    faultyVehicleLabel @7 :UInt8;
    waitingTime @8 :Float64;

    # for vehicle light
    leftProb @9 :Float32;
    rightProb @10 :Float32;
    hazardProb @11 :Float32;
    brakeProb @12 :Float32;
    faultyProb @13 :Float32;

    # for blind spot
    polygonPts @14 :List(StdGeometry.Point);
    idExistVeh @15 :Bool;
    blindPercentage @16 :Float32;
}

struct ScenarioAnalysisAgent {
    header @0 :StdHeader.Header;

    id @1 :Int64;
    objectLabel @2 :Int16;

    state @3 :AgentState;
    obb @4 :OrientedBoundingBox;
    egoRelevance @5 :Int8;
    faultyVehicleLabel @6 :UInt8;
}

struct ScenarioAnalysisLaneline {
    header @0 :StdHeader.Header;

    pts @1 :List(StdGeometry.Point);
    canLaneChange @2 :List(Bool);
    valid @3 :List(Bool);
}

struct ScenarioAnalysisRefline {
    header @0 :StdHeader.Header;

    pathPts @1 :List(StdGeometry.Point);
    direction @2 :Int8;
    laneIds @3 :List(Text);
    isValid @4 :Bool;

    leftLaneline @5 :ScenarioAnalysisLaneline;
    rightLaneline @6 :ScenarioAnalysisLaneline;
}

struct ScenarioModel {
    header @0 :StdHeader.Header;

    reflines @1 :List(ScenarioAnalysisRefline);
    agents @2 :List(ScenarioAnalysisAgent);
}

struct ScenarioModelDebug {
    header @0 :StdHeader.Header;

    agents @1 :List(AgentDebug);
}
