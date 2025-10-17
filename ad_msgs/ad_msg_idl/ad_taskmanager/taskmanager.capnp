@0xd129083d8353c74c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::taskmanager");
using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

# TODO(kit) Remove This
struct TaskCommand {
    header @0 :StdHeader.Header;

    enum TaskType {
        start @0;
        stop @1;
    }
    # enum task_type
    taskType @1 :TaskType;
}

struct DoorCommand {
    doorOpen @0 :Bool;
}

struct MapCoordLLA {
    # LLA coordinate
    longitude @0 :Float64;
    latitude @1 :Float64;
    altitude @2 :Float64;
}

struct SelfCheck {
    enum Type {
        hardware @0;
        software @1;
    }
    type @0 :Type;

    enum State {
        checking @0;
        checkFailed @1;
        checkSuccess @2;
    }
    state @1 :State;

    progress @2 :Float32;
}

struct TaskEvent {
    header @0 :StdHeader.Header;

    # enum level
    enum EventLevel {
        normal @0;
        warn @1;
        error @2;
    }
    # normal event: will log
    # warning event: will alert safety officer
    # error event: will Quit wire control and alert safety officer
    eventLevel @1 :EventLevel;

    # enum alarm type
    enum AlarmType {
        audio @0;
        light @1;
        ipad @2;
    }
    # now only support AUDIO
    alarmType @2 :AlarmType;

    # enum event type
    enum EventType {
        system @0;
        location @1;
        planning @2;
        control @3;
        selfCheck @4;
        station @5;
        record @6;
    }

    # To determine what to broadcast
    eventType @3 :EventType;

    # details will log, Convenient to debug
    details @4 :List(Text);
}

struct TaskStatus {
    header @0 :StdHeader.Header;


    # enum mission_status
    enum MissionStatus {
        unknown @0;
        ready @1;
        waitingForStartSignal @2;
        waitingForMission @3;
        starting @4;
        running @5;
        stopping @6;
    }

    missionStatus @1 :MissionStatus;
    missionName @2 :Text;

    # Target point
    departureLla @3 :MapCoordLLA;
    destinationLla @4 :MapCoordLLA;
    # Points LLA
    midPointsLla @5 :List(MapCoordLLA);
    blacklistPointsLla @6 :List(MapCoordLLA);
    navigationLineLla @7 :List(MapCoordLLA);
    # if ture , control will open door
    openTheDoor @8 :Bool;

    # car pose and speed
    vehicleSpeed @9 :Float32;
    vehiclePose @10 :MapCoordLLA;

    # AutoDrive status
    autoDrive @11 :Bool;

    # Vehicle health state
    vehicleHealth @12 :Bool;

    # Station State
    enum StationState {
        driving @0;
        inbound @1;
        berth @2;
        outbound @3;
        arriverFinal @4;
        pullingFail @5;
    }
    stationState @13 :StationState;

    selfCheck @14 :List(SelfCheck);

    # Data recording state
    enum DataRecordState {
        startingRecord @0;
        recording @1;
        stopRecording @2;
        notRecorded @3;
    }
    dataRecordState @15 :DataRecordState;
}
