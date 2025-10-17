@0xc9d780fa15c08ec9;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::sensor");

struct RecordCommand {
    enum Mode {
        stop @0;
        start @1;
    }
    mode @0 :Mode;
}
