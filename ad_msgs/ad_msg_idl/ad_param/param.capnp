 @0xe77d3eb287bba38f;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::param");

struct ParamReq {
    name @0 :Text;
    key @1 :Text;
    vakue @2 :Text;
}

struct ParamRes {
    name @0 :Text;
    key @1 :Text;
    vakue @2 :Text;
}

struct CfgSetparamReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
    paramValue @2 :Data;
    paramType @3 :Int32;
    isDefaultValue @4 :Bool;
    persistFlag @5 :Int32;
}

struct CfgSetparamResMethod {
    result @0 :Int32;
}

struct CfgGetparamReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
    paramType @2 :Int32;
}

struct CfgGetparamResMethod {
    paramValue @0 :Data;
    paramType @1 :Int32;
    result @2 :Int32;
}

struct CfgResetparamReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
}

struct CfgResetparamResMethod {
    result @0 :Int32;
}


struct CfgMonitorparamReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
}

struct CfgMonitorparamResMethod {
    result @0 :Int32;
}

struct CfgUnmonitorparamReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
}

struct CfgUnmonitorparamResMethod {
    result @0 :Int32;
}

struct CfgDelparamReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
}

struct CfgDelparamResMethod {
    result @0 :Int32;
}

struct CfgParamupdatedataresReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
}

struct CfgParamupdatedataresResMethod {
    result @0 :Int32;
}

struct CfgGetmonitorclientsReqMethod {
    clientName @0 :Text;
    paramName @1 :Text;
}

struct CfgGetmonitorclientsResMethod {
    monitorClients @0 :List(Text);
    result @1 :Int32;
}

struct CfgGetclientinfolistReqMethod {
    clientName @0 :Text;
}

struct CfgMethodEventInfo {
    type @0 :Int32;
    paramName @1 :Text;
    count @2 :Int32;
}

struct CfgClientInfo {
    clientName @0 :Text;
    monitorParams @1 :List(Text);
    runningStatus @2 :Int32;
    startTime @3 :Text;
    endTime @4 :Text;
    methodeventInfo @5 :List(CfgMethodEventInfo);
}

struct CfgGetclientinfolistResMethod {
    clientlist @0 :List(CfgClientInfo);
    result @1 :Int32;
}

struct CfgGetparaminfolistReqMethod {
    clientName @0 :Text;
}

struct CfgParamInfo {
    paramName @0 :Text;
    dataType @1 :Int32;
    dataSize @2 :Int32;
    lastUpdateClientName @3 :Text;
    lastUpdateTime @4 :Text;
    perFlag @5 :Int32;
}

struct CfgGetparaminfolistResMethod {
    paramlist @0 :List(CfgParamInfo);
    result @1 :Int32;
}

struct CfgParamupdatedataEvent {
    paramName @0 :Text;
    paramValue @1 :Data;
    paramType @2 :Int32;
    clientName @3 :Text;
    updateClientName @4 :Text;
}

struct CfgParamupdatedataresEvent {
    clientName @0 :Text;
    paramName @1 :Text;
    result @2 :Int32;
}