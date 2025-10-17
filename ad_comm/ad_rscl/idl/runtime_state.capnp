@0xe4934bae7c5002b4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";

struct FunctionCost {
    functionName @0 :Text;
    costTime @1 :Float32;
}

struct NodeStateCtrlRequest {
    header @0 :Header.Header;
    ctrlInfoList @1 :List(CtrlInfo);
}

struct CtrlInfo {
    enum CtrlRequest {
        none @0;
        # Indicate whether the system is ready for autonomous driving
        ready @1;
        # The node is currently under resetting
        reset @2;
        # The node should shutdown
        stop @3;
        # System is under manually driving mode
        driving @4;
        # System is under pilot mode
        pilot @5;
        # System is under parking mode
        parking @6;
        # The node should release resource but not shutdown
        suspend @7; # DEPRECATED
        resume @8; # DEPRECATED
    }
    # node name
    nodeName @0 :Text;
    request @1 :CtrlRequest;
}

struct ComponentStateInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :Header.Header;

    # name of nodelet or node who sended this msg.
    name @1 :Text;

    enum Status {
        # Before Initialization state
        waiting @0;
        # During the initialization process
        starting @1;
        # The node is running
        running @2;
        # The node is suspended
        suspended @3;
        # The node is resuming
        resuming @4;
        # The node is switing mode
        modeSwitching @5;
        # The node is stopping
        stopping @6;
        # The node has stopped due to some reason
        stopped @7;
        # The node fails due to initialization errors, running errors, etc.
        failed @8;
        # Indicate whether the system is ready for autonomous driving
        ready @9;
        # The node is currently under resetting
        reset @10;
        # The node is currently under an unknown status
        unknown @11;
    }
    status @2 :Status;

    enum Mode {
        manual @0;
        autonomous @1;
        pilot @2;
        parking @3;
    }
    mode @3 :Mode;

    # process id of the component.
    pid @4 :UInt32;

    # wall time in ns
    wallTimeNs @5 :UInt64;

    # function_cost in ms
    functionCostTimeList @6 :List(FunctionCost);
    nodeCostTime @7 :UInt64;

    # extra infomation
    extData @8 :Data;
}

struct FrameState {
    topicName @0 :Text;
    # delta frame / delta time
    hz @1 :Float32;
    maxDiffNs @2 :Int64;
    # the latest delta time
    latestDiffNs @3 :Int64;
    avgMsgKb @4 :Float32;
    # report time - latest update time
    updateDelayNs @5 :Int64;
}

struct TopicStateInfo {
    pub @0 :List(FrameState);
    sub @1 :List(FrameState);
}

struct NodeStateInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :Header.Header;

    # name of nodelet or node who sended this msg.
    name @1 :Text;

    topicState @2 :TopicStateInfo;

    # process id of the node.
    pid @3 :UInt32;

    # wall time in ns
    wallTimeNs @4 :UInt64;

    # platform which node run on (pc/j5/vh)
    platform @5 :Text;
}

struct ServiceStateInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :Header.Header;

    # service type, current only zmq
    type @1 :Text;

    struct StateMap {
        entries @0 :List(Entry);
        struct Entry {
            sourceIp @0 :Text;
            topicState @1 :TopicStateInfo;
        }
    }
    recvState @2 :StateMap;

    # forwarded bytes
    forwardBytes @3 :UInt64;
}
