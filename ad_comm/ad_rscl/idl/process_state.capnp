@0xe2088fb05218ea4b;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

struct ProcessStateInfo {
    enum State {
        initialized @0;
        shuttingDown @1;
        suspended @2;
        suspendFailed @3;
        resumeFailed @4;
    }

    pid @0 :UInt32;
    hostname @1 :Text;

    state @2 :State;

    # wall time in ns
    sentTimeWallTimeNs @3 :UInt64;
}
