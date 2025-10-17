@0xe4934bae7c5002a4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::monitor");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";

struct FunctionCost {
    functionName @0 :Text;
    costTime @1 :Float32;
}

struct HeartBeat {
    header @0 :Header.Header;

    # node name
    nodeName @1 :Text;
    nodeStatus @2 :Text;
    # additional state information for different nodes
    nodeCustomIntInfo @3 :Int32;
}

struct FrameState {
    topicName @0 :Text;
    hz @1 :Float32;
    maxDiffNs @2 :Int64;
    latestDiffNs @3 :Int64;
}

struct TopicStateInfo {
    pub @0 :List(FrameState);
    sub @1 :List(FrameState);
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
        suspend @7;
        resume @8;
    }
    # node name
    nodeName @0 :Text;
    request @1 :CtrlRequest;
}

struct MonitorSummary {
    header @0 :Header.Header;
    type @1 :Text;
    enum Level {
        info @0;
        warning @1;
        error @2;
        fatal @3;
    }
    level @2 :Level;
    summaryData @3 :Text;
    suggestion @4 :Text;
}

struct MsgTrackSource {
    # source message's name
    name @0 :Text;
    # source message's id
    sequenceId @1 :Int32;

    # source message's receive time
    receiveTimeNs @2 :UInt64;
}

struct MsgTrackInfo {
    # name of nodelet
    name @0 :Text;
    # message's id; used for message tracking
    sequenceId @1 :Int32;

    # system time when publishing this message
    publishTimeNs @2 :UInt64;

    # sensor time
    # hardware time when receiving the frame
    sensorHardwareTimeNs @3 :UInt64;
    # system time when receiving the frame
    sensorTimeNs @4 :UInt64;

    additionTimes @5 :List(UInt64);

    # sources, source name/id and receive time
    sources @6 :MsgTrackSource;
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
}

struct SelfCheckRequest {
  name @0 :Text;
}

struct SelfCheckResponse {
    # get self_test result or not
    success @0 :Bool;
    # percentage of self test
    percentage @1 :Float32;
    # sensor state health
    status @2 :Bool;
    # items waiting for check
    waitingList @3 :List(Text);
    # format: <MajorPosition>_<SecondaryPosition>_<SensorType>_<ExtendInfo> : error_reason
    errorReason @4 :List(Text);
}

struct LaelapsTestStatusRequest {
    name @0 :Text;
}

struct LaelapsTestStatusResponse {
    # if start test
    startTest @0 :Bool;
}

struct LaelapsMonitorInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor model)
    header @0 :Header.Header;

    # type of the summary
    type @1 :Text;

    # status
    status @2 :Bool;

    # id
    tagId @3 :UInt32;
}

struct MonitorTagInfo {
    requestModule @0 :Text;
    tagCn @1 :Text;
    tagEn @2 :Text;
    description @3 :Text;
    module @4 :Text;
    owner @5 :Text;
    splitMode @6 :UInt32;
}

struct MonitorTagSummary {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor model)
    header @0 :Header.Header;
    tagSummary @1 :List(MonitorTagInfo);
}

struct ManualToAutoRequest {
}

struct ManualToAutoResponse {
    systemStatus @0 :Bool;
}

struct VedioWarningInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :Header.Header;
    status @1 :Bool;
    details @2 :List(Text);
}

struct DiagInfo {
    ts @0 :UInt64;
    receiveTs @1 :UInt64;
    code @2 :Int32;
    status @3 :Int32;
    eventId @4 :Int32;
    level @5 :Int32;
    diff @6 :UInt64;
    msg @7 :Text;
    expected @8 :Text;
    actual @9 :Text;
    process @10 :Text;
    platform @11 :Text;
    classType @12 :Text;
    subClassType @13 :Text;
    reason @14 :Text;
    influence @15 :Text;
    occurrenceCondition @16 :Text;
    recoveryCondition @17 :Text;
    isInternal @18 :Bool;
    object @19 :Text;
}

struct DiagInfoDetail {
    diagInfo @0 :List(DiagInfo);
    curDiagInfo @1 :List(DiagInfo);
    hisDiagInfo @2 :List(DiagInfo);
}

struct PlatformDiagInfo {
    key @0 :Text;
    value @1 :DiagInfoDetail;
}

struct ReportBase {
    supervisionId @0 :UInt64;
    statusLevel @1 :Int32;
    problemDescription @2 :Text;
    enabled @3 :Bool;
    lastLevelChangeTimeNs @4 :UInt64;
    lastUpdateTimeNs @5 :UInt64;
    checkTimeNs @6 :UInt64;
    diagnosisMsg @7 :Text;
}

struct DiagStatus {
    key @0 :Int32;
    value @1 :Text;
}

struct ResourceStatus {
    key @0 :Text;
    value @1 :Int32;
}

struct Cpu {
    id @0 :UInt32;
    user @1 :Float32;
    system @2 :Float32;
    nice @3 :Float32;
    idle @4 :Float32;
    iowait @5 :Float32;
    hirq @6 :Float32;
    sirq @7 :Float32;
    st @8 :Float32;
    temperature @9 :Float32;
    cpuUsed @10 :Float32;
    loadAvg1 @11 :Float32;
    loadAvg5 @12 :Float32;
    loadAvg15 @13 :Float32;

    level @14 :Int32;
    errMsg @15 :Text;
}

struct AiCoreUsage {
    used @0 :Float64;

    level @1 :Int32;
    errMsg @2 :Text;
}

struct AiCore {
    name @0 :Text;
    used @1 :Float32;
    hwi @2 :Float32;
    swi @3 :Float32;

    level @4 :Int32;
    errMsg @5 :Text;
}

struct MemBandwidth {
    memBandWidth @0 :Float32;
    allMemBandWidth @1: List(Float32);
    aiCoreMemBandWidth @2: List(Float32);

    level @3 :Int32;
    errMsg @4 :Text;
}

struct Bandwidth {
    mpamid @0: Text;
    bw @1 :Float64;

    level @2 :Int32;
    errMsg @3 :Text;
}

struct Gpu {
    id @0 :Text;
    gpuUtil @1 :Int32;
    memoryUtil @2 :Int32;
    encoderUtil @3 :Int32;
    decoderUtil @4 :Int32;
    memoryTotal @5 :Int32;
    memoryUsed @6 :Int32;
    memoryFree @7 :Int32;
    temperature @8 :Int32;
    used @9 :Float32;

    level @10 :Int32;
    errMsg @11 :Text;
}

struct MemoryStat {
    total @0 :UInt32;
    used @1 :UInt32;
    free @2 :UInt32;
    buffered @3 :UInt32;
    cached @4 :UInt32;
    dirty @5 :UInt32;
    memoryUsed @6 :Float32;

    level @7 :Int32;
    errMsg @8 :Text;
}

struct IoStat {
    device @0 :Text;
    mountdir @1 :Text;
    mountroot @2 :Text;
    tps @3 :Float64;
    kbRead @4 :UInt32;
    kbWrite @5 :UInt32;
    kbReadPerSecond @6 :Float32;
    kbWritePerSecond @7 :Float32;
    rAwaitMillisec @8 :UInt32;
    wAwaitMillisec @9 :UInt32;
    total @10 :UInt32;
    used @11 :UInt32;
    free @12 :UInt32;

    level @13 :Int32;
    errMsg @14 :Text;
}

struct NetworkStat {
    nic @0 :Text;
    enable @1: Bool;
    rxBytesRate @2 :UInt32;
    txBytesRate @3 :UInt32;
    rxPacketsRate @4 :UInt32;
    txPacketsRate @5 :UInt32;
    nrRxDropped @6 :UInt32;
    nrTxDropped @7 :UInt32;
    nrRxErrors @8 :UInt32;
    nrTxErrors @9 :UInt32;

    level @10 :Int32;
    errMsg @11 :Text;
}

struct ProcessStat {
    command @0 :Text;
    pid @1 :Int32;
    cpuUsed @2 :Float32;
    minorFaults @3 :UInt64;
    majorFaults @4 :UInt64;
    utime @5 :UInt64;
    stime @6 :UInt64;
    nrThreads @7 :UInt64;

    vsize @8 :UInt64;
    rss @9 :UInt64;
    memoryUsed @10 :Float32;
    waitCpu @11 :Int64;
    pss @12 :UInt64;
    spAlloc @13 :UInt64;

    fps @14 :Float32;
    functionTimeCost @15 :Float32;

    level @16 :Int32;
    errMsg @17 :Text;
}

struct NetworkPeerTransportStat {
    localPort @0 :UInt32;
    remotePort @1 :UInt32;
    rxQueue @2 :UInt32;
    txQueue @3 :UInt32;

    level @4 :Int32;
    errMsg @5 :Text;
}

struct NetworkPeerStat {
    peer @0 :Text;
    latencyAvg @1 :Int32;
    latencyMax @2 :Int32;
    latencyMin @3 :Int32;
    latencyTimeout @4 :Int32;

    tcp @5 :List(NetworkPeerTransportStat);
    udp @6 :List(NetworkPeerTransportStat);

    level @7 :Int32;
    errMsg @8 :Text;
}

struct SystemResource {
    platform @0 :Text;
    identifier @1 :Text;
    cpus @2 :List(Cpu);
    gpus @3 :List(Gpu);
    memory @4 :MemoryStat;
    ios @5 :List(IoStat);
    networks @6 :List(NetworkStat);
    processes @7 :List(ProcessStat);
    networkPeers @8 :List(NetworkPeerStat);
    tiResourceInfo @9 :Text;
    aiCoreUsage @10: AiCoreUsage;
    aiCore @11 :List(AiCore);
    memBandWidth @12 :MemBandwidth;
    bandwidths @13: List(Bandwidth);
}

struct SystemReport {
    key @0 :Text;
    value @1 :SystemResource;
}

struct PipelineDetail {
    instanceId @0 :UInt64;
    index @1 :UInt32;
    subIndex @2 :UInt32;
    name @3 :Text;
    msg @4 :Text;
    initTimestamp @5 :Int64;
    timestamp @6 :Int64;
    cost @7 :Int64;
    costAvg @8 :Int64;
    costMax @9 :Int64;
    costMin @10 :Int64;
    ts @11 :List(UInt64);
}

struct PipelineCost {
    key @0 :Text;
    value @1 :List(PipelineDetail);
}

struct SWCInfo {
    platform @0 :Text;
    identifier @1 :Text;
    nodeName @2 :Text;
    topicName @3 :Text;
    frameRatio @4 :Float32;
    maxGapNs @5 :Float32;
    minGapNs @6 :Float32;
    avgSizeKb @7 :Float32;
    isKeyFrame @8 :Int32;
    side @9 :Int32;
    functionCost @10 :Int32;
    frameTs  @11 :UInt64;

    level @12 :Int32;
    errMsg @13 :Text;
}

struct SensorInfo {
    platform @0 :Text;
    identifier @1 :Text;
    nodeName @2 :Text;
    topicName @3 :Text;
    frameRatio @4 :Float32;
    maxGapNs @5 :Float32;
    minGapNs @6 :Float32;
    cost @7 :Int32;
    insRtk @8 :Int32;
    insName @9 :Text;

    level @10 :Int32;
    errMsg @11 :Text;
}

struct SubPlatformReport {
    classType @0 :Text;
    subClassType @1 :Text;
    tipCount @2 :UInt64;
    warnCount @3 :UInt64;
    criticalCount @4 :UInt64;
    fatalCount @5 :UInt64;
    tipDuration @6 :UInt64;
    warnDuration @7 :UInt64;
    criticalDuration @8 :UInt64;
    fatalDuration @9 :UInt64;
    healthyInfo @10 :List(Float32);
    level  @11 :Int32;
    object @12 :Text;
}

struct PlatformReport {
    platform @0 :Text;
    module @1 :Text;
    sub @2 :List(SubPlatformReport);
}
struct Data {
    id @0 :Text;
    count @1 :UInt64;
}
struct Time {
    id @0 :Text;
    min @1 :UInt64;
    max @2 :UInt64;
    avg @3 :UInt64;
}
struct Condition {
    id @0 :Text;
    valid @1 :UInt64;
    invalid @2 :UInt64;
}

struct WorkshopInfo {
    name @0 :Text;
    dataList @1 :List(Data);
    timeList @2 :List(Time);
    condList @3 :List(Condition);
}

struct MonitorReport {
    header @0 :Header.Header;
    systemReport @1 :List(SystemReport);
    pipelineCost @2 :List(PipelineCost);
    diagInfo @3 :List(PlatformDiagInfo);
    swcInfo @4 :List(SWCInfo);
    sensorInfo @5 :List(SensorInfo);
    diagList @6 :List(DiagInfo);
    platformReport @7 :List(PlatformReport);
    workshopInfo @8 :List(WorkshopInfo);
}

struct Sensor {
    enum Level {
        info @0;
        warn @1;
        error @2;
    }

    id @0 :Text;
    level @1 :Level;
    name @2 :Text;
    comment @3 :Text;
}

struct Alarm {
    enum Level {
        info @0;
        warn @1;
        error @2;
    }

    timestamp @0 :UInt64;
    id @1 :Text;
    level @2 :Level;
    major @3 :Text;
    minor @4 :Text;
    value @5 :Text;
    object @6 :Text;
    occurTimestamp @7 :UInt64;
    solution @8 :Text;
}

struct DiagAlarm {
    header @0 :Header.Header;
    sensorList @1: List(Sensor);
    alarmList @2: List(Alarm);
}