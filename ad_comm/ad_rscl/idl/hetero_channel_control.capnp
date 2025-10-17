@0xb8e3312da32f2d68;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

struct ControlMessage {
    enum ControlType {
        enableHeteroChannel @0;
        disableHeteroChannel @1;
    }
    controlType @0 : ControlType;
}
