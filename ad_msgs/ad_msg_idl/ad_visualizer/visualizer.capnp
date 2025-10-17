@0xc3f917eaa5408319;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::visualizer");

struct InternalInfo {
    reset @0 :Bool;
    vehicleId @1 : Text;
}

struct DaemonRpcMsg {
    status @0 :Bool;
}
