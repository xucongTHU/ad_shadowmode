@0xa23911782e4fe446;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_smart_slam");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AvpStateV3Def = import "/ad_msg_idl/avp_state_v3_def/avp_state_v3_def.capnp";

struct SlamWarningOutput {
    initErr @0 :Bool;
    mapQueryErr @1 :Bool;
    mapLoadingErr @2 :Bool;
    mapSavingErr @3 :Bool;
    vehicleConfigeErr @4 :Bool;
}

enum InitTrigger {
    none @0;
    stateMachine @1;
    sensorGnss @2;
    sensorDualAntenna @3;
    buttonMapSelect @4;
}

enum SlamMode {
    uninitialized @0;
    apaSlam @1;
    hppSlam @2;
    hppLocalization @3;
    hppAuto @4;
}

enum LocalizationStatus {
    uninitialized @0;
    initializing @1;

     # while localization_success_ is true
    initSuccessed @2;

    # hppLocalization && vehicle in map
    canCruise @3;
}

enum LocType {
    # Localization not working.
    none @0;

    # undefined type.
    unknownLoc @1;

    vSlamLoc @2;
    drLoc @3;
}

struct SlamStatus {
    initSuccessed @0 :Bool;
    initFailed @1 :Bool;
    mode @2 :SlamMode;
    locState @3 :LocalizationStatus;
    mappingFinished @4 :Bool;
    currentTrigger @5 :InitTrigger;
    locType @6 :LocType;
    mapId @7 :UInt64;
    mileage @8 :Float64;
}

# topic: /parking/slam/status_info
struct SlamStatusInfo {
	# Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    # msg id, update every frame
    msgId @1 :UInt64;

    # avp fsm status & uid
    avpStatus @2 :AvpStateV3Def.AvpMainStatus;
    avpStatusUid @3 :UInt64;

    # slam status code
    slamStatus @4 :SlamStatus;

    # slam warnning code
    slamWarnning @5 :SlamWarningOutput;
    lastErrorString @6 :Text;
}
