@0xa687308d65650c49;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::diag");

struct Map(Key, Value) {
  entries @0 :List(Entry);
  struct Entry {
    key @0 :Key;
    value @1 :Value;
  }
}

struct UdsRawFrame
{
    metaInfo @0 :Map(Text, Text);
    sid @1 :UInt8;
    subid @2 :UInt8;
    respAck @3 :UInt8;
    dataLen @4 :UInt32;
    dataVec @5 :Data;
}

struct ReportDemEvent
{
  dtcValue @0 :UInt32;
  alarmStatus @1 :UInt8;
}