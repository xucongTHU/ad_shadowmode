@0xdf4defc02ca8bca5;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::planning");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

using import "/ad_msg_idl/std_msgs/typed_data_list.capnp".Float64List;

struct TrajectoryPoint {
    timestampNs @0 :UInt64;
    position @1 :StdGeometry.Point;
    direction @2 :StdGeometry.Point;
    velocity @3 :Float64;
    theta @4 :Float32;
    curvature @5 :Float32;
    sumDistance @6 :Float64;
    timeDifference @7 :Float64;
    yawRate @8 :Float64;
    acceleration @9 :Float64;
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
    }
    trajectoryMode @5 :TrajectoryMode;

    # path planning state,NO_REQUEST=0,BOTH_REQUEST=1,LAT_ONLY_REQUEST=2,LONG_ONLY_REQUEST=3,ERROR=4
    trajState @6 :UInt8;
    # traj base time
    trajTimestampNs @7 :UInt64;
    # used map
    enum MapType {
        mapless @0;
        hdmap @1;
        memoryMap @2;
    }
    mapType @8 :MapType;
    longiPosError @9 :Float64;
    longiVelError @10 :Float64;
    longiAccError @11 :Float64;
    headingError @12 :Float64;
    lateralPosError @13 :Float64;
    trajectoryEnds @14 :List(TrajectoryPoint);

    # Turn Steer For Next Path
    enum TurnSteer {
        steerNone @0;
        steerReturn @1;
        steerCompleted @2;
        steerCompletedHalf @3;
    }
    turnSteerEnum @15 :TurnSteer;
    trajSumDistance @16 :Float64;
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
    colors @2 :List(Float64);
    # line width
    width @3 :Float64;
    # type
    type @4 :Text;
}

struct ReferencePoint {
    point @0 :StdGeometry.Point;
    heading @1 :Float32;
    curvature @2 :Float32;
    speedLimit @3 :Float32;
}

struct TrafficLightInfo {
    enum TrafficLightShape {
        none @0;
        unknown @1;
        circle @2;
        pedestrian @3;
        leftArrow @4;
        straightArrow @5;
        rightArrow @6;
        aroundArrow @7;
        leftStraightArrow @8;
        leftAroundArrow @9;
        rightStraightArrow @10;
        bicycle @11;
        trafficClose @12;
        timer @13;
        trafficAllow @14;
        other @15;
    }
    enum TrafficLightStatus {
        unknown @0;
        invalid @1;
        off @2;
        green @3;
        yellow @4;
        red @5;
    }
    id @0 :UInt64;
    position @1 :StdGeometry.Point;
    shape @2: TrafficLightShape;
    status @3: TrafficLightStatus;
    countdown @4: UInt8;
}

struct StopPoint {
    id @0 :UInt64;
    position @1 :StdGeometry.Point;
}

struct TrafficSignInfo {
    enum TrafficSignType {
        unknown @0;
        roadworks @1;
        stop @2;
        overtakingprohibited @3;
        endofprohibitiononovertaking @4;
        childrenandschoolzone @5;
        minspeedlimit @6;
        maxspeedlimit @7;
        endofspeedlimit @8;
        noentrance @9;
        allspeedlimitcancel @10;
        noparkingsign @11;
        startofhighway @12;
        endofhighway @13;
        leftcurve @14;
        rightcurve @15;
        seriescurves @16;
        others @17;
        speedlimit5 @18;
        speedlimit10 @19;
        speedlimit20 @20;
        speedlimit30 @21;
        speedlimit40 @22;
        speedlimit50 @23;
        speedlimit60 @24;
        speedlimit70 @25;
        speedlimit80 @26;
        speedlimit90 @27;
        speedlimit100 @28;
        speedlimit110 @29;
        speedlimit120 @30;
        endspeedlimit5 @31;
        endspeedlimit10 @32;
        endspeedlimit20 @33;
        endspeedlimit30 @34;
        endspeedlimit40 @35;
        endspeedlimit50 @36;
        endspeedlimit60 @37;
        endspeedlimit70 @38;
        endspeedlimit80 @39;
        endspeedlimit90 @40;
        endspeedlimit100 @41;
        endspeedlimit110 @42;
        endspeedlimit120 @43;
        minimumspeedlimit50 @44;
        minimumspeedlimit60 @45;
        minimumspeedlimit70 @46;
        minimumspeedlimit80 @47;
        minimumspeedlimit90 @48;
        minimumspeedlimit100 @49;
        minimumspeedlimit110 @50;
        overtakerestriction @51;
        endingofovertakerestriction @52;
        variablespeedlimit10 @53;
        variablespeedlimit20 @54;
        variablespeedlimit30 @55;
        variablespeedlimit40 @56;
        variablespeedlimit50 @57;
        variablespeedlimit60 @58;
        variablespeedlimit70 @59;
        variablespeedlimit80 @60;
        variablespeedlimit90 @61;
        variablespeedlimit100 @62;
        variablespeedlimit110 @63;
        variablespeedlimit120 @64;
        variablesignothers @65;
        mergeleft @66;
        mergeright @67;
        attentiontopedestrians @68;
        attentiontochildren @69;
        stopsign @70;
        slowdownandgiveway @71;
        noleftturn @72;
        norightturn @73;
        nouturn @74;
        noaudiblewarning @75;
        leftturnsign @76;
        rightturnsign @77;
        pedestriancrossing @78;
        uturnlane @79;
        numberoflanesbecomingless @80;
        lanereducing @81;
        workzonesign @82;
        lanechanged @83;
        leftturnlane @84;
        rightturnlane @85;
        tollgate @86;
        leftturnandforwardlane @87;
        rightturnandforwardlane @88;
        leftturnanduturnlane @89;
        buslane @90;
        closetotollgate @91;
        schoolaheadlowdown @92;
        ramp @93;
        militarycontrolzone @94;
        radioobservatory @95;
        trafficsignall @96;
        speedlimit15 @97;
        speedlimit35 @98;
        endspeedlimit35 @99;
        xlandr @100;
        xparking @101;
        xenter @102;
        xheight @103;
        xweight @104;
        xentry @105;
        xstraight @106;
        skip @107;
        xwidth @108;
        straightlane @109;
    }
    id @0 : UInt64;
    centroId @1 : StdGeometry.Point;
    length @2 : Float32;
    width @3 : Float32;
    height @4 : Float32;
    detectionStatus @5 : UInt8;
    confidence @6 : UInt8;
    type @7 : TrafficSignType;
}

struct ReferenceLane {
    # lc 1, joint 2
    # left positive, right negative
    id @0 :Int8;
    isTargetLane @1 :Bool;
    jointed @2 :Bool;
    source @3 :UInt8;
    # size 200
    points @4 :List(ReferencePoint);
    stopPoints @5 :List(StopPoint);
    trafficLightInfo @6 :TrafficLightInfo;
    positionToStopLine @7: UInt8;
    trafficSignInfo @8: List(TrafficSignInfo);
}

# /scenario_model/reference_lanes
struct ReferenceLanes {
    header @0 :StdHeader.Header;
    lanes @1 :List(ReferenceLane);
}

# /scenario_model/relative_localization
struct RelativeLocalization {
    header @0 :StdHeader.Header;
    # unknown 0, available 1, unavailable 2
    hdMapStatus @1 :UInt8;
    # unit m
    disToRamp @2: Float32;
    disToSplit @3 :Float32;
    disToMerge @4 :Float32;
    # none 0, left 1, right 2
    splitDirection @5 :Int8;
    mergeDirection @6 :Int8;
    totalLaneNum @7 :UInt8;
    # from right to left, start from 0
    currentLaneIndex @8 :UInt8;
    # line type, unknown 0, cross enabled 1, cross disabled 2
    leftLineType @9 :UInt8;
    rightLineType @10 :UInt8;
    leftDisToSolid @11 :Float32;
    rightDisToSolid @12 :Float32;
    laneType @13 :UInt8;
}

struct StopPointByObject {
    point @0 :StdGeometry.Point;
    id @1 :Text;
}

struct ObjectLabel {
    id @0 :Int32;
    label @1 :UInt8;
}

struct ObjectTag {
    id @0 :Int32;
    tag @1 :UInt8;
}

struct PlanningDebug {
    header @0 :StdHeader.Header;
    timestamp @1 :UInt64;
    num @2 :UInt32;
    predictTimestamp @3 :UInt64;
    x @4 :List(Float64);
    y @5 :List(Float64);
    v @6 :List(Float64);
    yaw @7 :List(Float64);
    a @8 :List(Float64);
    jerk @9 :List(Float64);
    speedLimit @10 :Float64;
    stopDistance @11 :Float64;
    speedRef @12 :Float64;
    placeholder1 @13 :Float64;
    placeholder2 @14 :Float64;
    placeholder3 @15 :Float64;
    originPointX @16 :Float64;
    deltaXScenemap @17 :Float64;
    deltaXEstimation @18 :Float64;
    obstacleTagBinary @19 :Text;
    # pp text box
    ppTextboxString @20 :Text;
    # pp planning lines
    lines @21 :List(PlanningLine);
    longiDpSuccess @22 :Bool;
    lateralDpSuccess @23 :Bool;
    longiSolveSuccess @24 :Bool;
    latSolveSuccess @25 :Bool;
    # planning running state
    scenarioType @26 :UInt8;
    # LANE_KEEP_SCENARIO = 0
    # LANE_CHANGE_SCENARIO = 1
    # LANE_BORROW_SCENARIO = 2
    # JUNCTION_PROTECTED_SCENARIO = 3
    stageType @27 :UInt8;
    # LANE_KEEP_DEFAULT = 0;
    # LANE_KEEP_DODGELEFT = 1;
    # LANE_KEEP_DODGERIGHT = 2;
    # // stage for LANE_CHANGE scenario
    # LANE_CHANGE_ACCELERATION = 11;
    # LANE_CHANGE_ABORTION = 12;
    # LANE_CHANGE_EXECUTION = 13;
    # // stage for LANE_BORROW scenario
    # LANE_BORROW_DECISION = 21;
    # LANE_BORROW_OUT = 22;
    # LANE_BORROW_BACK = 23;
    # LANE_BORROW_ABORTION = 24;
    # // stage for protected junction scenario
    # JUNCTION_PROTECTED_CREEP = 31;
    # JUNCTION_PROTECTED_CRUISE = 32;
    leaderId @28 :Text;
    stageName @29 :Text;
    # stop point, unit, m
    stopPointsByObject @30 :List(StopPointByObject);
    objectLabel @31 :List(ObjectLabel);
    objectTag @32 :List(ObjectTag);
    policies @33 :List(Trajectory);
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
    area @6 :UInt8;
    # lateal action, LANEKEEP = 0, LANECHANGE = 1, NUDGE = 2
    latact @7 :UInt8;
    # longitudinal action, STOP = 0, CRUISE = 1, SLOWDOWN = 2
    lngact @8 :UInt8;
    # change direction, NO_CHANGE = 0, CHANGE_LEFT = 1, CHANGE_RIGHT = 2
    changeDirection @9 :UInt8;
    preChangeDirection @10 :UInt8;
    # current refline in ENU coordinate, unit, m
    currentReflineEnu @11 :List(StdGeometry.Point);
    # target refline in ENU coordinate, unit, m
    targetReflineEnu @12 :List(StdGeometry.Point);
    # speed limit, unit, m/s
    speedLimit @13 :Float32;
    # headway time, float32, s
    headwayTime @14 :Float32;
    # stop point, unit, m
    stopPoint @15 :StdGeometry.Point;
    # speed limit point
    speedLimitPoint @16 :StdGeometry.Point;
    # flag for pre lane change
    routingLaneChangeFlag @17 :UInt8;
    # lane change end point
    laneTypeChangePoint @18 :StdGeometry.Point;
    # flag for constinuous lane change
    routingChangeLaneTwiceFlag @19 :Bool;
    # lane change end point in continuous change
    laneTypeChangeTwicePoint @20 :StdGeometry.Point;
    # flag for refline change
    reflineChangeFlag @21 :Bool;
    # turning signal blub, right = 1, left = 2, both = 3, none = 0
    signalBlub @22 :UInt8;
    # turning type, GO_STRAIGHT = 0, TURN_RIGHT = 1, TURN_LEFT = 2
    turntype @23 :UInt8;
    # flag for traffic light
    trafficStop @24 :Bool;
    # nop system state, DISABLE=0,ERROR=1,PASSIVE=2,STANDBY=3,ACTIVE=4,SUSPEND_LONG=5,DEGRATE=6,TAKE_OVER=7,SUSPEND_LAT=8,SUSPEND_BOTH=9
    nopSystemState @25 :UInt8;
    # start time of dmpp
    dmppStartTime @26 :UInt64;
    # distance to next ramp
    dist2ramp @27 :Float64;
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
    faultVehicleCount @8 :UInt32;
    efficientLane @9 :UInt32;
    efficientCount @10 :List(Float64);
    effLcScenario @11 :UInt32;
    longsceneApplied @12 :List(UInt32);
    highlightLongscene @13 :UInt32;
    latsceneTrans @14 :List(UInt32);
    latStage @15 :UInt32;
    dist2junction @16 :Float64;
    currRoadid @17 :Text;
    currLandid @18 :Text;
    ifInJunction @19 :UInt32;
    reflineChangeFlag @20 :UInt32;
    routingChangeLaneTwiceFlag @21 :UInt32;
    trafficlights @22 :List(TrafficLight);
    reflineBlockForUnknown @23 :List(Bool);
    junctionLinkBlockForUnknown @24 :List(Bool);
    blockUnknownIdLine @25 :List(Int32);
    cipvIds @26 :List(Int32);
    egoCarD @27 :Float64;
    # data buffer used to temporarily store data
    buffer @28 :Text;
    # Line Info
    leftLineId @29 :Int64;
    rightLineId @30 :Int64;
    leftLineType @31 :Int16;
    rightLineType @32 :Int16;
    # dm text box
    dmTextboxString @33 :Text;
    cipvTakeover @34 :Bool;
    confirmEfficiencyChange @35 :Bool;
}

struct NopOutHandsOff {
    uilateralCtrHandsOffWarningNu @0 :UInt8;
    bEnaHandsOffCnfm @1 :Bool;
}

struct NopOutVehStateRequest {
    bHazardLightReq @0 :Bool;
    bHighBeamReq @1 :Bool;
    bLowBeamReq @2 :Bool;
    #0: not used
    #1: light on off
    uiLightRemindNu @3 :UInt8;
    #the vehicle horn should be triggered in emergency situation
    bVehicleHornReq @4 :Bool;
    #the stolen alarm warn should be triggered in emergency situation
    bStolenAlarmWarnReq @5 :Bool;
}

struct DecisionOutput {
    header @0 :StdHeader.Header;
    #system state machine output
    #INVALID_STATUS = -1
    #NNP_ERROR = 0
    #NNP_DISABLED = 1
    #NNP_PASSIVE = 2
    #NNP_STANDBY = 3
    #NNP_ACTIVATED = 4
    #NNP_SUSPENDED = 5
    #DEGRADATION_PILOT_MODE = 6
    #TO_TRANSITION = 7
    uiSystemStateMachineNu @1 :Int8;
    #0: no
    #1: pilot pre-upgrade reminder
    #2: pilot pre-upgrade reminder level 2
    #3: NOP Active reminder
    #4: NOP pre-degrade reminder
    #5: NOP pre-degrade reminder level 2
    #6: degrade to pilot reminder
    #7: NOP unable to active by sound
    uiP2NStateReminderNu @2 :UInt8;
    #0: no
    #1: NOP ready to activate reminder
    #2: NOP activated reminder
    #3: NOP pre-cancel reminder
    #4: NOP pre-cancel reminder level 2
    #5: NOP pre-cancel reminder level 3
    #6: NOP dropped-out
    #7: NOP_fault reminder
    uiStateReminderNu @3 :UInt8;
    #every bit means a active condtion check result
    #0: active condition pass
    #1: active condition failed
    uiActiveConditionBtf @4 :UInt64;
    #every bit means a suppression condtion check result
    #1: suppression condition pass
    #0: suppression condition failed
    uiSuppressionConditionBtf @5 :UInt64;
    #path planning state
    #NO_REQUEST=0
    #BOTH_REQUEST=1
    #LAT_ONLY_REQUEST=2
    #LONG_ONLY_REQUEST=3
    #ERROR=4
    ppLatLongState @6 :UInt8;
    #INVALID_STATUS = -1
    #EM = 0
    #MRM = 1
    #STAND_AND_WARNING = 2
    #TAKE_OVER_DONE = 3
    uiMrmStatusNu @7 :Int8;
    #other DM module output
    #0: off
    #1: on
    #dependes on NNP_OnOffSet
    uiStateNu @8 :UInt8;
    sHandsOffState @9 :NopOutHandsOff;
    #0: eye on
    #1: eye off
    bEyeOff @10 :Bool;
    #0: none
    #1: Static Obstacle
    #2: TFL
    #3: Pedestrain Walk
    #4: PayModeConfirm
    #5: Lane Reduce
    #6: Narrow Lane
    #7: SpeedAdapt
    #8: Oversized Vehicle Both Side
    #9: Oversized Vehicle One Side
    #10: oncoming vehicle; 11: Best Path Tracing
    uiNnpScenariosNu @11 :UInt8;
    #0: No Request
    #1: Ramp In Start
    #2: Ramp In DONE
    #3: Ramp out Heavy Traffic
    #4: Ramp out Start
    #5: Ramp out Done
    #6: Navigation Task Take Over Request
    #7: Exit Missed
    uiNnprinoNu @12 :UInt8;
    #0: simple
    #1: more tips
    uiVoiceModeNu @13 :UInt8;
    #0: none
    #1: on broadcast
    #2: off broadcast
    uiOnOffAudioPlayNu @14 :UInt8;
    #0: off
    #1: on
    uiAutoOnOffSetNu @15 :UInt8;
    #0: no action
    #1: off
    #2: on
    uiSndStateNu @16 :UInt8;
    #0: basic
    #1: simple
    #2: detailed
    uiVoiceNotifFrequencyModeNu @17 :UInt8;
    #0: none
    #1: NOP cancel play
    uiCancelAudioPlayNu @18 :UInt8;
    #1: basic
    #2: normal
    #3: radical
    uiDrivingModeNu @19 :UInt8;
    #0: voice allowed LC + Lever allowed LC
    #1: automated LC
    uiAlcModeNu @20 :UInt8;
    #0: no
    #1: overriding
    #2: resumed
    #3: not used
    uiOverrideStateNu @21 :UInt8;
    #0: not used
    #1: traffice light detected
    #2: side walk detected
    uiTrafficStateNu @22 :UInt8;
    #the feed back of the message that speed
    #limit need to be update by map or TSR is
    #confirmed by driver
    #0: no request
    #1: request
    uiSpeedAdaptConfirmNu @23 :UInt8;
    #truck driving in the near lane
    #0: not used
    #1: left OSV
    #2: right OSV
    #3: both side OSV
    #4: avoding 2 left
    #5: avoding to right
    #6: accelerating
    #7: no action
    uiOsaStatusNu @24 :UInt8;
    #0: no request
    #1: request
    uiPayModeConfirmNu @25 :UInt8;
    #0: not used
    #1: request
    uiPayModeConfirmReqNu @26 :UInt8;
    #0: none
    #1: left lane change active
    #2: right lane change active
    uiLaneChangeAudioPlayNu @27 :UInt8;
    #0: request active
    #1: request inactive
    uiLcSndRequestNu @28 :UInt8;
    #0: none
    #1: vehicle speed too low
    #2: lane change start
    #3: lane change end
    #6: lane change ongoing
    uiLaneChangeInforNu @29 :UInt8;
    #0: none
    #1: left
    #2: right
    uiLaneChangeDirctionNu @30 :UInt8;
    #0: no request
    #1: request
    uiLaneChangePendingAlertNu @31 :UInt8;
    #ego overtake other car confirm request
    #0: none
    #1: overtake confirm request
    uiOverTakeConfirmNu @32 :UInt8;
    #ACC related output
    #0: no action
    #1: slowly slider
    #2: fast slider
    uiCruiseSpeedAddFeedbcakNu @33 :UInt8;
    #0: no action
    #1: slowly slider
    #2: fast slider
    uiCruiseSpeedMinusFeedbackNu @34 :UInt8;
    #0: no press
    #1: press
    uiCruiseDistanceAddFeedbackNu @35 :UInt8;
    #0: no press
    #1: press
    uiCruiseDistanceMinusFeedbackNu @36 :UInt8;
    uiLongitudeCtrlSetSpeedKph @37 :UInt16;
    #headway setting: level 0 - level 5
    uiCurrentHeadwaySettingNu @38 :UInt8;
    vehState @39 :NopOutVehStateRequest;
    #0: No remind
    #1: dist to ramp less than 1000
    #2: dist to ramp less than 200
    dist2rampReminder @40 :UInt8;
    enteringRamp @41 :Bool;
    #0: No remind
    #1: dist to ramp exit less than 200
    #2: dist to ramp exit less than 50
    dist2rampExitReminder @42 :UInt8;
    mergingMainRoad @43 :Bool;
    #0: No remind
    #1: dist to route end less than 500
    #2: dist to route less than 200
    #2: dist to route less than 50
    dist2routeEndReminder @44 :UInt8;
    #0: no take over
    #1: highway to ramp fail reminder
    #2: merging to main road fail reminder
    takeOverType @45 :UInt8;
    #system state machine display
    #DISP_DISABLED = 0
    #DISP_STANDBY = 1
    #DISP_ACTIVATED = 2
    #DISP_TO_TRANSITION = 3
    #DISP_SUSPENDED = 4
    #DISP_DEGRADATION_PILOT = 5
    #DISP_ERROR = 6
    uiDispStateMachineNu @46 :Int8;
    uiNNPSSMInfo @47 :UInt64;
}

struct XDebugMsgs {
    header @0 :StdHeader.Header;
    json @1 :Text;
}

struct DriverIntentionInfo{
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    # m/s
    manualSetSpeed @1 :Float64;

    #0: non_active
    #1: active
    fuctionStatus @2 :Bool;

    #0: none
    #1: left
    #2: right
    laneChangeDirction @3 :UInt8;

    cipvInfo@4 :List(Int32);
}

struct PlanningRequestSystemState {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;
    # enum system state
    enum NopSystemState {
        nopSystemStateDisable @0;
        nopSystemStateError @1;
        nopSystemStatePassive @2;
        nopSystemStateStandby @3;
        nopSystemStateActive @4;
        nopSystemStateSuspendLong @5;
        nopSystemStateDegrate @6;
        nopSystemStateTakeOver @7;
        nopSystemStateSuspendLat @8;
        nopSystemStateSuspendBoth @9;
    }
    # planning request of ifp sys state
    nopSystemStateEnum @1 :NopSystemState;
    # planning request of nop sys state
    lfpSystemStateEnum @2 :NopSystemState;
    # planning request of acc sys state
    accSystemStateEnum @3 :NopSystemState;
    enum SystemState {
        autopilotDisable @0;
        autopilotLfpActive @1;
        autopilotNopActive @2;
        autopilotAccActive @3;
    }
    systemState @4 :SystemState;
}

struct MemoryDriveInfo {
    enum MemoryDriveState {
        disactive @0;
        standby @1;
        active @ 2;
        override @ 3;
        takeover @ 4;
    }

    memoryDriveState @0 : MemoryDriveState;
    travelTime @1: Float64; # unit, s
    remainingMileage @2: Float64; # unit, m
    totalMileage @3: Float64; # unit, m
    memoryTrajectory @4: List(StdGeometry.Point);
}

struct LaneChangeState {
    enum ChangeStatus {
        noChange @0;
        preChange @1;
        changeContinuing @2;
        changeCompleted @3;
        changeFailed @4;
    }

    changeStatus @0: ChangeStatus;
    targetPosition @1: StdGeometry.Point; # base on vehicle coordinate
    heading @2: Float64;  # unit, rad, base on vehicle coordinate
    gapFrontId @3: Int32; # gap front vehicle id, default 0
    gapRearId @4: Int32;  # gap rear vehicle id, default 0

     # 20250331 modify laneChangeState variable
    hmiChangingState @5: UInt16;
    hmiCrossingIntersectionWarning @6:  UInt16;
    hmiEgoIntersectionAction @7:  UInt16;
    hmiTurningDirection @8: UInt16;
    hmiLaneBiasFlag @9: UInt16;
    hmiLaneBiasObjectId @10: UInt32;
}

struct ErrorCode {
    losttopic @0 :UInt8;
    topictimeout @1 :UInt8;
 }

struct FailReason{
    enum PlannerErr {
        success @0;
        nullPtr @1;
        configIOError @2;
        invalidParam @3;
        noActivatedPpl @4;
        internalError @5;
    }
    plannerErr @0 :PlannerErr;

    enum PathPlannerFail {
        success @0;
        inputNullptr @1;
        inputError @2;
        solveFail @3;
        checkSolutionFail @4;
    }
    pathPlannerFail @1 :PathPlannerFail;

    enum LatSolverFail {
        success @0;
        errorInput @1;
        errorNullPtr @2;
        errorSetup @3;
        errorOutputNan @4;
    }
    latSolverFail @2 :LatSolverFail;

    enum AbnomalOrigin {
        normal @0;
        speedJump @1;
        accelerationJump @2;
        longiPosJump @3;
    }
    abnomalOrigin @3 :AbnomalOrigin;

    enum InvalidLongiSpeed {
        valid @0;
        speedSInvalid @1;
        speedSizeInvalid @2;
    }
    invalidLongiSpeed @4 :InvalidLongiSpeed;

    enum LongiSolverFail {
        success @0;
        invalidSOutput @1;
        invalidVOutput @2;
        invalidAccOutput @3;
        invalidJerkOutput @4;
    }
    longiSolverFail @5 :LongiSolverFail;

   reflineBuildResult @6 : UInt8 ;

    enum HnopActiveFail {
        success @0;
        invalidLinkid @1;
        invalidRoadType @2;
        invalidMapType @3;
        noRoutingInfo @4;
    }
    hnopActiveFail @7 :HnopActiveFail;
}

struct KeyObj {
    borrowObjId @0 :UInt32;

    biasObjId @1: UInt32;

    lcRiskObjId @2 :UInt32;

    cutinId @3 :UInt32;

}

struct LcTargetLaneGapInfo {

    targetLaneFrontId @0 :UInt32;

    targetLaneRearId @1 :UInt32;

}

struct PipelineInfo {
    laneKeepPplStatus @0 :Int8;

    leftLanechgPplStatus @1 :Int8;

    leftLaneChgPplSuccessCounter @2 :UInt8;

    rightLanechgPplStatus @3 :Int8;

    rightLaneChgPplSuccessCounter @4 :UInt8;
}

struct EfficiencyLaneVhgInfo {

    isEgoSpdSuppressed @0 :Bool;

    isNeighborFastOrFurther @1 :Bool;

    isNeighborRisk @2 :Bool;

    laneChgEfficiencyScore @3 :Float32;
}

struct SpeedLimitInfo {
    curSpeedLimit @0 :UInt8;

    curLinkType @1 :UInt32;

    nextSpeedLimit @2 :UInt8;

    nextDiffSpeedLimitLinkType @3 :UInt32;

    disToNextDiffSpdLimitLink @4 :UInt32;
}

struct ObjElactoer {
      leadersId @0:List(UInt32);
      neighboursId @1:List(UInt32);
      oncomingId @2:List(UInt32);
      crossingId @3:List(UInt32);
    }

struct PlanningState {
    header @0 :StdHeader.Header;
     enum VehicleState {
        disactive @0;

        lccStandby @1;
        lccActive @2;
        lccOverride @3;
        lccTakeover @4;

        nopStandby @5;
        nopActive @6;
        nopOverride @7;
        nopTakeover @8;
    }
    vehicleState @1 :VehicleState;

    selectedSpeed @2 :Float64;  # unit, kph
    selectedTimeHeadway @3 :Int32;  #unit, /

    enum ILCChangeDirection {
        none @0;
        left @1;
        right @2;
    }
    iLCState @4 :ILCChangeDirection;

    # Closest In Path Vehicle
    cIPV @5 :Int32;

    keyVRUs @6 :List(UInt32);

    speedLimit @7 :Float64;  # unit, kph

    trajectory @8 :List(StdGeometry.Point);

    memoryDriveInfo @9:MemoryDriveInfo;

    laneChangeState @10: LaneChangeState;

    enum NopActiveEnum {
        unknown @0;
        invalid @1;
        hnopValid @2;
        cnopMaplessValid @3;
        cnopHdmapValid @4;
    }
    nopActive @11: NopActiveEnum;
     # ENUM : PilotUnavailableReason
    enum PilotUnavailableReason{
        success @0;
        scenarioModelErr @1;
        planningInputErr @2;
        planningPreprocessErr @3;
        behaviourPlanningErr @4;
        motionPlanningErr @5;
        planningPostprocessErr @6;
        undefinedErr @7;
    }
    socPlannerStateMachie @12 : UInt32;
    socPilotUnavailableReason @13 : PilotUnavailableReason;
    enum TurnTypeEnum {
      unknown @0;
      laneChange @1;
      leftTurn @2;
      rightTurn @3;
      leftUTurn @4;
      rightUTurn @5;
    }

    turnType @14 : TurnTypeEnum;

    errorType @15 : ErrorCode;

    abnormalType @16 : FailReason;

    enum LcTypeEnum {
        unknown @0;
        ilc @1;
        rlc @2;
        elc @3;
        interflow @4;
        implicit @5;
        highSpeedLimit @6;
        avoidanceLc @7;
    }
    lcType @17 :LcTypeEnum;

    enum BiasTypeEnum {
        noBias @0;
        leftBias @1;
        rightBias @2;
    }
    biasType @18 :BiasTypeEnum;

    enum BorrowTypeEnum {
        noBorrow @0;
        leftBorrow @1;
        rightBorrow @2;
    }
    borrowType @19 :BorrowTypeEnum;

    enum OriginUpdateReasonEnum {
        na @0;
        latUpdate @1;
        lonUpdate @2;
        speedUpdate @3;
        thetaUpdate @4;
        timeUpdate @5;
    }
    originUpdateReason @20 :OriginUpdateReasonEnum;

    enum LaneChgFailReasonEnum {
        success @0;
        lcPPLFailContinuously @1;
        driverCancelLc @2;
        solidlineInLcDirection @3;
    }
    laneChgFailReason @21 :LaneChgFailReasonEnum;

    enum LaneChgRiskObjEnum {
        noRisk @0;
        rear @1;
        neighborFront @2;
        leftFront @3;
        leftRear @4;
        rightFront @5;
        rightRear @6;
        unknown @7;
    }
    laneChgRiskObj @22 :LaneChgRiskObjEnum;

    isLaneChgDesiredSpeed @23 :Bool;

    reflineUsedLaneIds @24 :List(UInt32);

    reflineModyReq @25 :Bool;

    keyObj @26 :KeyObj;

    lcTargetLaneGapInfo @27 :LcTargetLaneGapInfo;

    pipelineInfo @28 :PipelineInfo;

    efficiencyLaneVhgInfo @29 :EfficiencyLaneVhgInfo;

    speedLimitInfo @30 :SpeedLimitInfo;

    objElactoer @31 :ObjElactoer;

    trajPointArraySR @32 :List(TrajectoryPoint);

    enum NearDestinationReminderEnum {
        default @0;
        reminder1000m @1;
        reminder500m @2;
        reminder100m @3;
    }
    nearDestinationReminer @33 :NearDestinationReminderEnum;

    drivingStyleLearningRes @34 :DrivingStyleLearningResponse;

    drivingStyleManageRes @35 :DrivingStyleManageResponse;

    cutInPrevention @36 :CutInPrevention;
}

struct CutInPrevention
{
    reductTimeGap @0 :UInt8;
    stopBlockage @1 :UInt8;
}

struct PlanningVisData {
    header @0 :StdHeader.Header;
    visualData @1: Data;
}

struct PlanningStateManual {
    header @0 :StdHeader.Header;
    accelPdlOvrd @1 :Bool;
    brkPdlPrsd @2 :Bool;
    strWhlOvrd @3 :Bool;
}

struct NOPStatus {
    header @0 :StdHeader.Header;
    enum NOPStatusEnum {
      unknown @0;
      notReady @1;
      available @2;
      active @3;
    }
    nopStatus @1 :NOPStatusEnum;
}

struct Vehicleprocessing{
    header @0 :StdHeader.Header;
    # ENUM : TurningLightSignal
    enum TurningLightSignal {
    neutral @0;
    left  @1;
    error  @2;
    right @3;
    }

    # ENUM : EVPHandsOffAlert
    enum EVPHandsOffAlert {
    handsOn @0;
    detectHandsOff  @1;
    level1Alarm  @2;
    level2Alarm @3;
    level3Alarm @4;
    times3HOR @5;
    rmfMode @6;
    }

    # ENUM : SpdLimWarnReq
    enum SpdLimWarnReq {
        nowarning @0;
        warning @1;
    }

    # ENUM : SetTimeGap
    enum SetTimeGap{
    timeGap0 @0;
    timeGap1 @1;
    timeGap2 @2;
    timeGap3 @3;
    timeGap4 @4;
    auto @5;    
    }
    # ENUM : WheelRotDir
    enum WheelRotDir{
        unknown @0;
        forward @1;
        reverse @2;
        notSupported @3;
        failed @4;
    }
    # ENUM : TransEstdGear
    enum TransEstdGear{
        notSupported @0;
        firstGear @1;
        secondGear @2;
        thirdGear @3;
        fourthGear @4;
        fifthGear @5;
        sixthGear @6;
        seventhGear @7;
        eighthGear @8;
        ninthGear @9;
        tenthGear @10;
        unused @11;
        cvtForwardGear @12;
        neutralGear @13;
        reverseGear @14;
        parkGear @15;
    }
    timeStamp @1 :UInt64;
    vehicleSpd @2 : Float32;
    longitudinalAccel @3 : Float32;
    latAccel @4 : Float32;
    yawrate @5 : Float32;
    frontWheelAng @6 : Float32; 
    turningLightSignal @7 :TurningLightSignal;
    longiEngaged @8:Bool;
    lateralEngaged @9:Bool;
    setSpeed @10 : Float32;
    setTimeGap @11 : SetTimeGap;
    wheelSpdFL @12 : Float32;
    wheelSpdFR @13 : Float32;
    wheelSpdRL @14 : Float32;
    wheelSpdRR @15 : Float32;
    wheelSpdLD @16 : Float32;
    wheelSpdRD @17 : Float32;
    wheelSpdLND @18 : Float32;
    wheelSpdRND @19 : Float32;
    wheelRotDirLND @20 : WheelRotDir;
    wheelRotDirRND @21 : WheelRotDir;
    wheelRotDirLD @22 : WheelRotDir;
    wheelRotDirRD @23 : WheelRotDir;
    steerWheelAngle @24 : Float32;
    steerWheelAngleRate @25 : Float32;
    yawRateTimestamp @26 : UInt32;  # unit:ms
    absActive @27 : Bool;
    gearPos @28 : TransEstdGear;
    accIcon @29 :UInt8;
    lfpIcon @30 :UInt8;
    nopIcon @31 :UInt8;
    aebDecelReq @32: Float32;
    autoBrkSysAtvMask @33: UInt8;
    lgSfBrkMode @34: UInt8;
    evphandsoffalert @35: EVPHandsOffAlert;
    spdLimWarnReq @36: SpdLimWarnReq;
    accMode @37: UInt8;
}

struct MCUStateMachineInfo{
    header @0 :StdHeader.Header;
    # ENUM : StateMachine
    enum StateMachine {
    unknonwn @0;
    notReady @1;
    accReady @2;
    accEnable @3;
    accAcitve @4;
    lfpReady @5;
    lfpEnable @6;
    lfpAcitve @7;
    nopReady @8;
    nopEnable @9;
    nopAcitve @10;
    }
    # ENUM : Override
    enum Override{
    unkown @0;
    longiOverride @1;
    lateralOverride @2;
    bothOverride @3;
    }
    # ENUM : PilotSwitch
    enum PilotSwitch{
        off @0;
        accOn @1;
        lfpOn @2; #NOP is the same as LFP
        accOff @3;
        lfpOff @4;
    }
    mcuStateMachene @1 :StateMachine;
    mcuDrvOverride @2 :Override;
    mcuPilotSwitch @3:PilotSwitch;
    mcuAccActvCdt @4:Bool;
    mcuLfpActvCdt @5:Bool;
    mcuNopActvCdt @6:Bool;
}

struct DrivingStyleLearningRequest
{
    enum DSLearningCommand
    {
        default @0;
        startReq @1;
        endReq @2;
    }
    command @0 :DSLearningCommand;
    drivingStyleName @1 :Text;
}

struct DrivingStyleLearningResponse
{
    enum DSLearningStatus
    {
        default @0;
        startRes @1;
        endRes @2;
        failRes @3;
    }
    status @0 :DSLearningStatus;
    drivingStyleName @1 :Text;
}

struct DrivingStyleManageRequest
{
    enum DSManageCommand
    {
        default @0;
        set @1;
        unset @2;
        getStyleList @3;
    }
    command @0 :DSManageCommand;
    drivingStyleName @1 :Text;
}

struct DrivingStyleManageResponse
{
    enum DSManageStatus
    {
        default @0;
        successToSet @1;
        failToSet @2;
        successToGetList @3;
        failToGetList @4;
    }
    status @0 :DSManageStatus;
    drivingStyleNames @1 :List(Text);
}

struct SRControl
{
    header @0 :StdHeader.Header;
    drivingStyleLearningReq @1 :DrivingStyleLearningRequest;
    drivingStyleManageReq @2 :DrivingStyleManageRequest;

    enum MultiModalCommand
    {
        default @0;
        turnLeft @1;
        turnRight @2;
        toAccelerate @3;
        toDecelerate @4;
    }
    multiModal @3 :MultiModalCommand;
}