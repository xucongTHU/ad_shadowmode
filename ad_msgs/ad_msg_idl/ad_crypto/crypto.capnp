@0xd61f1fbb4ea02b80;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::crypto");

struct SeedReqMethod {
    reqId @0 :UInt32;
}

struct SeedResMethod {
    resId @0 :UInt32;
    seedValue @1 :Data;
}

struct DiagKeyReqMethod {
    reqId @0 :UInt32;
    seed @1 :Data;
}

struct DiagKeyResMethod {
    resId @0 :UInt32;
    keyValue @1 :Data;
}

