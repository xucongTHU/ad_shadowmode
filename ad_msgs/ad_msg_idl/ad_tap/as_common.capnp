@0x9f23f496404fa705;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::tap");

struct AsTarget {
  id @0 : UInt32;
  type @1: UInt32;
  source @2: UInt32;
  posX @3: Float32;
  posY @4: Float32;
  spdX @5: Float32;
  spdY @6: Float32;
  accelX @7: Float32;
  accelY @8: Float32;
  heading @9: Float32;
  ttc @10: Float32;
}

enum OnOffStatus{
    notActive @0;
    active @1;
}

