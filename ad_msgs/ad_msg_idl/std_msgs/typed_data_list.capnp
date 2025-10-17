@0xb9bc1f73540c731a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::std_msgs");

struct Float64List {
    data @0 :Data;
}

struct Float32List {
    data @0 :Data;
}

struct Int64List {
    data @0 :Data;
}

struct Int32List {
    data @0 :Data;
}

struct UInt64List {
    data @0 :Data;
}

struct UInt32List {
    data @0 :Data;
}

struct RawString {
    data @0 :Text;
}
