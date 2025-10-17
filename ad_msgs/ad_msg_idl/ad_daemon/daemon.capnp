@0x87cfa6d8d09fdcaa;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::daemon::msg");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";
using Monitor = import "/ad_msg_idl/ad_monitor/monitor.capnp";

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

struct FunctionCost {
    functionName @0 :Text;
    costTime @1 :Float32;
}

struct DaemonRequest {
    header @0 :Header.Header;
    # ms
    timeOut @1 :UInt32;
    union {
        startProject @2 :StartProject;
        stopProject @3 :StopProject;
        resetProject @4 :ResetProject;
        startRecord @5 :StartRecord;
        stopRecord @6 :StopRecord;
        copyRecord @7 :CopyRecord;
        startNode @8 :StartNode;
        stopNode @9 :StopNode;
        queryStatus @10 :QueryStatus;
        attachProcess @11 :AttachProcess;
        queryOptions @12 :QueryOptions;
        queryVisualizationNodes @13 :QueryVisualizationNodes;
        contReplay @14 :ContReplay;
        replayerStatusOnce @15 :ReplayerStatusOnce;
        switchMode @16 :SwitchMode;
        persistTag @17 :PersistTag;
        getVehicle @18 :GetVehicle;
        getNetworkStatus @19 :GetNetworkStatus;
        getDiskInfo @20 :GetDiskInfo;
        getPkgVersion @21 :GetPkgVersion;
        calibration @22 :Calibration;
        restartNode @23 :RestartNode;
        getCopyPercentage @24 :GetCopyPercentage;
    }
}

struct DaemonRespond {
    header @0 :Header.Header;
    resp @1 : CmdRespond;
    union {
        cmdRespond @2 :CmdRespond;
        queryStatusResponse @3 :QueryStatusResponse;
        attachProcessResponse @4 :AttachProcessResponse;
        queryOptionsResponse @5 :QueryOptionsResponse;
        queryVisualizationNodesResponse @6 :QueryVisualizationNodesResponse;
        getDiskInfoResponse @7 :GetDiskInfoResponse;
        getCopyPercentageResponse @8 :GetCopyPercentageResponse;
    }
}

struct NodeCtrlMsg {
    header @0 :Header.Header;
    nodeList @1 :List(Ctrl);
}

struct Ctrl {
    nodeName @0 :Text;
    enum CtrlRequest {
        none @0;
        stop @1;
        driving @2;
        pilot @3;
        parking @4;
    }
    request @1 :CtrlRequest;
}

struct CmdRespond {
    result @0 :Bool;
    detail @1 :Text;
}

struct StartProject {
    dataPath @0 :Text;
    mode @1 :Text;
    vehicle @2 :Text;
    case @3 :Text;
    place @4 :Text;
    routingCase @5 :Text;
    customArguments @6 :List(Text);
    envArguments @7 :List(Text);
    attachable @8 :Bool;
    useCachedConfig @9 :Bool;
    customMode @10 :Text;
    nsysNode @11 :Text;
    rosBag @12 :Text;
    topic @13 :Text;
    isVisualizerMode @14 :Bool;
    withOutVideo @15 :Bool;
}

struct StopProject {
    placeHolder @0 :Void;
}

struct ResetProject {
    dataPath @0 :Text;
}

struct StartRecord {
    placeHolder @0 :Void;
}

struct StopRecord {
    placeHolder @0 :Void;
}

struct StopRecordResponse {
    result @0 :Bool;
    detail @1 :Text;
    path @2 :Text;
}

struct CopyRecord {
    path @0 :Text;
}

struct GetCopyPercentage {
    placeHolder @0 :Void;
}

struct GetCopyPercentageResponse {
    copyPercentage @0 :Float64;
}

struct StartNode {
    nodeName @0 :Text;
    bashPath @1 :Text;
}

struct StopNode {
    nodeName @0 :List(Text);
}

struct RestartNode {
    nodeName @0 :Text;
    bashPath @1 :Text;
}

struct QueryStatus {
    placeHolder @0 :Void;
}

struct QueryStatusResponse {
    isRunning @0 :Bool;
    currentState @1 :Text;
    drivingMode @2 :Text;
    nodeList @3 :List(QueryNodeResponse);
}

struct QueryNodeResponse {
    enum ProcState {
        unknown @0;
        running @1;
        failed @2;
    }
    name @0 :Text;
    isRunning @1 :ProcState;
    pid @2 :Int32;
    status @3 :ComponentStateInfo.Status;
    ignoreStatus @4 :Bool;
    soc @5 :Text;
    mode @6 :ComponentStateInfo.Mode;
    isLostMsg @7: Bool;
}

struct AttachProcess {
    processName @0 :Text;
}

struct AttachProcessResponse {
    result @0 :Bool;
    detail @1 :Text;
    soc @2 :SocInfo;
}

struct SocInfo {
    machineType @0 :Text;
    user @1 :Text;
    ip @2 :Text;
    password @3 :Text;
}

struct QueryOptions {
    placeHolder @0 :Void;
}

struct QueryOptionsResponse {
    mode @0 :List(Text);
    vehicle @1 :List(Text);
    case @2 :List(Text);
    layOuts @3 :List(Text);
    place @4 :List(Text);
    route @5 :List(Text);
    sensorScheme @6 :List(Text);
}

struct QueryVisualizationNodes {
    placeHolder @0 :Void;
}

struct QueryVisualizationNodesResponse {
    nodes @0 :List(Text);
}

struct ContReplay {
    dataPath @0 :Text;
    mode @1 :Text;
    vehicl @2 :Text;
    case @3 :Text;
    place @4 :Text;
    routingcase @5 :Text;
    customArguments @6 :List(Text);
    envArguments @7 :List(Text);
    attachable @8 :Bool;
    useCachedConfig @9 :Bool;
    customMode @10 :Text;
    nsysNode @11 :Text;
    rosBag @12 :Text;
    topic @13 :Text;
    isReplayerReady @14 :Bool;
}

struct ReplayerStatusOnce {
    placeHolder @0 :Void;
}

struct ReplayerStatusOnceResponse {
    replayerStatus @0 :Text;
    isReplayerRunning @1 :Bool;
}

struct SwitchMode {
    mode @0 :Text;
}

struct PersistTag {
    fileType @0 :Text;
    content @1 :Text;
}

struct GetVehicle {
    placeHolder @0 :Void;
}

struct GetNetworkStatus {
    placeHolder @0 :Void;
}

struct GetDiskInfo {
    placeHolder @0 :Void;
}

struct GetDiskInfoResponse {
    isDiskExist @0 :Bool;
    isWritable @1 :Bool;
    diskOwner @2 :Text;
}

struct GetPkgVersion {
    placeHolder @0 :Void;
}

struct Calibration {
    placeHolder @0 :Void;
}
