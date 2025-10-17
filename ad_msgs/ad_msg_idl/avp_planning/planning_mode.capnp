@0xf6c627e361ce0d13;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::planning::msg");

struct NoneParam {
}
struct ApaParam {
    slotId @0 :UInt64;
}
struct HppParam {
}
struct HpmParam {
}
struct StandbyParam {
}

struct ChangeModeRequest {
    param :union {
        apaParam @0 :ApaParam;
        hppParam @1 :HppParam;
        hpmParam @2 :HppParam;
        standbyParam @3 :StandbyParam;
        noneParam @4 :NoneParam;
    }
}

struct ChangeModeResponse {
    ok @0 :Bool;
}
