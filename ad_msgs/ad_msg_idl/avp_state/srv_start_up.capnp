@0xb58e82959a5c8260;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_state");

struct SrvStartUpRequest {
    enum AlgMode {
        none @0;
        apa @1;
        hpp @2;
    }
    mode @0 :AlgMode;
}

struct SrvStartUpResponse {
    enum Ret {
        ok @0;
        fail @1;
    }
    # return status:  AvpFeedbackStatus
    ret @0 :Ret;
}
