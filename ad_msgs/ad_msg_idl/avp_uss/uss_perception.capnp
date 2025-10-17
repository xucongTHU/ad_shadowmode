@0xea95d35d297ad71e;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");

struct UssEcho {
  recvTimeNs @0 :UInt64;
  distance @1 :List(UInt32);
  width @2 :List(Int16);
  height @3 :List(Int16);
}

struct UssObstacleInfo{
  frameTimestampNs  @0 :UInt64;
  ussEchoInfoList  @1  :List(UssEcho);
}

struct UssPEBInfo
{
  frameTimestampNs  @0 :UInt64;
  distance  @1  :List(Int32);
}

struct  UssSlot
{
  type  @0  :UInt32;
  obj0X  @1  :Float32;
  obj0Y  @2  :Float32;
  obj1X  @3  :Float32;
  obj1Y  @4  :Float32;
  depth @5  :Float32;
}

struct  UssSlotsInfo
{
  frameTimestampNs  @0 :UInt64;
  slotsList @1  :List(UssSlot);
}

struct UssPoint
{
  x  @0  :Float32;
  y  @1  :Float32;
  type @2 :Int32;
}

struct UssObjLine
{
  frameTimestampNs  @0 :UInt64;
  point0  @1  :UssPoint;
  point1  @2  :UssPoint;
}

struct UssObjLinesInfo
{
  frameTimestampNs  @0 :UInt64;
  objLinesList  @1  :List(UssObjLine);
}

struct UssFusedPoint
{
  timestamp @0 :UInt64;
  age @1 :UInt16;
  x @2 :Int16;
  y @3 :Int16;
  lowProb @4 :UInt8;
  highProb @5 :UInt8;
  existProb @6 :UInt8;
  freeProb @7 :UInt8;
}

struct UssFusedStaticMap
{
  timestamp @0 :UInt64;
  seq @1 :UInt16;
  fusedPointsSize @2 :UInt16;
  fusedPoints @3 :List(UssFusedPoint);
}

struct Point
{
  x @0 :Float32;
  y @1 :Float32;
}

struct UssFreespace
{
  timestamp @0 :UInt64;
  polygonPointsSize @1 :UInt32;
  signalWay @2 :UInt32;
  probability @3 :Float32;
  polygonPoints @4 :List(Point);
}

struct UssFreespaces
{
  timestamp @0 :UInt64;
  freespacesSize @1 :UInt32;
  freespaces @2 :List(UssFreespace);
}
