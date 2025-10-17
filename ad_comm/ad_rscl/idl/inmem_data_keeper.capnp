@0xae20db786dd1ed05;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

struct InMemDataKeeperNotification {
    enum ClipTaskStatus {
        started @0;
        extended @1;
        done @2;
    }

    clipTaskName @0 : Text;
    status @1 : ClipTaskStatus;
}

struct InMemDataKeeperRestartOption {
    maxClipRadiusSec @0 :UInt32;
}

struct InMemDataKeeperTrigger {
    enum Action {
        start @0;
        extend @1;
        removeWaiting @2;
        restartService @3;
    }

    action @0 :Action;

    # Start & Extend Only
    startOffsetSec @1 :UInt32;
    endOffsetSec @2 :UInt32;
    # Start & Extend & Remove Waiting
    clipName @3 :Text;
    # Restart Only
    restartOption @4 :InMemDataKeeperRestartOption;
}
