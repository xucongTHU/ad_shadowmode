@0xafd24393fa912fd5;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_control");

struct NoneParam {
}

struct ApaParam {
}

struct HpaParam {
    enum Type { 
        hpp @0;
        hps @1;
    }

    type @0 :Type;
}

struct HpmParam {
}

struct StandbyParam {
}

struct ChangeModeRequest {
    param :union {
        apaParam @0 :ApaParam;
        hpaParam @1 :HpaParam;
        hpmParam @2 :HpmParam;
        standbyParam @3 :StandbyParam;
        noneParam @4 :NoneParam;
    }
}

struct ChangeModeResponse {
    ok @0 :Bool;
}
