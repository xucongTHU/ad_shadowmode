@0xc839ea842878824e;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::um");

enum StateMachine {
    otaIDEL @0;
    otaPREUPDATE @1;
    otaUPDATING @2;
    otaUPDATED @3;
    otaACTIVING @4;
    otaACTIVED @5;
}

struct OtaMessage {
    stateMachine @0 :StateMachine;
}