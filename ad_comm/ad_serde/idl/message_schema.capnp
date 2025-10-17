@0xfb44cad4304587b4;

using Cxx = import "/capnp/c++.capnp";
using Schema = import "/capnp/schema.capnp";
$Cxx.namespace("senseAD::serde::idl");

struct MessageSchema {
    payload @0 :AnyStruct;
    typeId @1 :UInt64;
    schemas @2 :List(Schema.Node);
}
