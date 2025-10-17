@0x8ee95afdcc245824;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::cs3::idl");

struct AddrInfo {
    endpointName @0 :Text;
    ip @1 :Text;
    port @2 :UInt32;
}

struct TopicInfo {
    topicName @0 :Text;
    typeName @1 :Text;
    descriptor @2 :Data;

    # Only sub topic will have
    endpoint @3 :AddrInfo;
}

struct RuntimeState {
    # Uniqued with combination: name + msgmeta + endpoint
    hostSubTopicInfos @0 :List(TopicInfo);
    hostPubTopicInfos @1 :List(TopicInfo);
}
#######################################

struct CoreToCoreReq {
    selfAddr @0 :AddrInfo;
    selfSysState @1 :RuntimeState;
    stateVersion @2 :UInt64;
}

struct CoreToCoreResp {
    selfAddr @0 :AddrInfo;
    selfSysState @1 :RuntimeState;
    stateVersion @2 :UInt64;
}

#######################################

struct SwcCliToCoreReqUpsertState {
    selfProcessState @0 :RuntimeState;

    stateVersion @1 :UInt64;
}

struct SwcCliToCoreRespUpsertState {
}

struct SwcCliToCoreReqRemoveState {
}

struct SwcCliToCoreRespRemoveState {
}

struct SwcCliToCoreReqQueryRemoteEndpoint {
    topicNames @0 :List(Text);
}

struct SwcCliToCoreRespQueryRemoteEndpoint {
    remoteSubTopicInfos @0 :List(TopicInfo);

    stateVersion @1 :UInt64;
}

struct SwcCliToCoreReqQueryRemoteEndpointStateVersion {
}

struct SwcCliToCoreRespQueryRemoteEndpointStateVersion {
    stateVersion @0 :UInt64;
}

struct StatRecordPerTopic {
    topicName @0 :Text;

    receivedTopicMsgCnt @1 :UInt64;
    receivedTopicTrafficByte @2 :Float64;
    receivedTopicLost @3 :UInt64;

    sentTopicMsgCnt @4 :UInt64;
    sentTopicDropped @5 :UInt64;
    sentTopicTrafficByte @6 :Float64;
}

struct StatRecordPerPeer {
    peerAddress @0 :Text;

    receivedPeerPacketLost @1 :UInt64;
    receivedPeerTrafficByte @2 :Float64;
    receivedPeerMaxLatency @3 :UInt64;

    sentPeerTrafficByte @4 :Float64;
}

struct StatReport {
    firstUnixNs @0 :UInt64;
    lastUpdateUnixNs @1 :UInt64;

    receivedTotalMsgCnt @2 :UInt64;
    receivedTotalLost @3 :UInt64;

    sentTotalMsgCnt @4 :UInt64;
    sentTotalDropped @5 :UInt64;

    perTopicStats @6 :List(StatRecordPerTopic);
    perPeerStats @7 :List(StatRecordPerPeer);
}

struct SwcCliToCoreReqReportStat {
    selfProcessName @0 :Text;
    selfPid @1 :UInt32;

    statRecord @2 :StatReport;
}

struct SwcCliToCoreRespReportStat {
}

enum SwcCliToCoreReqType {
    upsertState @0;
    removeState @1;
    queryState @2;
    queryVersion @3;
    reportStat @4;
}

struct SwcCliToCoreReq {
    reqType @0 :SwcCliToCoreReqType;

    upsertReq @1 :SwcCliToCoreReqUpsertState;
    removeReq @2 :SwcCliToCoreReqRemoveState;
    queryReq @3 :SwcCliToCoreReqQueryRemoteEndpoint;
    queryStateVersionReq @4 :SwcCliToCoreReqQueryRemoteEndpointStateVersion;
    reportStatReq @5 :SwcCliToCoreReqReportStat;
}

struct SwcCliToCoreResp {
    reqType @0 :SwcCliToCoreReqType;

    corePid @1 :UInt32;
    coreStartTime @2 :UInt64;

    upsertResp @3 :SwcCliToCoreRespUpsertState;
    removeResp @4 :SwcCliToCoreRespRemoveState;
    queryResp @5 :SwcCliToCoreRespQueryRemoteEndpoint;
    queryStateVersionResp @6 :SwcCliToCoreRespQueryRemoteEndpointStateVersion;
    reportStatResp @7 :SwcCliToCoreRespReportStat;
}
