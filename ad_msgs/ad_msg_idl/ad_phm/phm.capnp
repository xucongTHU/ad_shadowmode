@0xa8bc2a77c7a50a30;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::phm");

struct FaultEvent
{
    domain @0 :Text;
    occurTime @1 :UInt64;
    faultId @2 :UInt32;
    faultObj @3 :UInt8;
    faultStatus @4 :UInt8;
}

struct RequstType
{
    type @0 :UInt32;
}

struct FileName
{
    fileName @0 :Text;
}

struct CollectionFile
{
    allFileNames @0 :List(FileName);
}