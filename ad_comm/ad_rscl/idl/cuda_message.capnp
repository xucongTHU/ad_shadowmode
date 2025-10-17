@0x887b4bc6d39d620e;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::rscl");

struct CudaShmNewMessageNotification {
    publisherPid @0 :UInt64;
    publisherUid @1 :Text;
    seq @2 :UInt64;
}

