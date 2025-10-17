@0xe12f79fd4f16483c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_parking_cmd");

struct AvmOptions {
    enum CameraID {
        front @0;
        rear @1;
        left @2;
        right @3;
    }

    avmEnable @0 :Bool;
    cameraID @1 :CameraID;
    dynamicLineEnbale @2 :Bool;
    transparentChassisEnable @3 :Bool;
}

struct QueryParam {
}

struct ChangeParam {
    options @0:AvmOptions;
}

struct AvmOptionsRequest {
    param :union {
        queryParam @0 :QueryParam;
        changeParam @1 :ChangeParam;
    }
}


struct QueryResult {
    options @0:AvmOptions;
}

struct ChangeResult {
}

struct AvmOptionsResponse {
    success @0:Bool;

    result :union {
        queryResult @1 :QueryResult;
        changeResult @2 :ChangeResult;
    }
}

