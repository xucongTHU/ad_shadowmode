@0xd4e7d200a71461c6;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_smart_slam");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct MapInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # The time of map measurement, in nano second.
    timestampNs @1 :UInt64;

    # parking slots
    parkingSlots @2 :List(ParkingSlot);

    # lane lines
    laneLines @3 :List(LaneLine);

    # stop lines
    stopLines @4 :List(StopLine);

    # bumps
    bumps @5 :List(Bump);

    # cross walks
    crossWalks @6 :List(CrossWalk);

    # arrows
    arrows @7 :List(Arrow);

    # parking slots origin
    parkingSlotsOptimize @8 :List(ParkingSlot);

    # map id
    mapId @9 :UInt64;

    regionList @10 :List(RegionInfo);
}

struct ParkingSlot {
    # parking slot id
    id @0 :UInt64;

    psType @1 :UInt32;

    # parking slot width and length
    width @2 :Float64;
    length @3 :Float64;

    # parking slot center point
    center @4 :StdGeometry.Point;

    # parking slot wide and long direction
    wideDirection @5 :StdGeometry.Vector3;
    longDirection @6 :StdGeometry.Vector3;

    # parking slot occupancy property
    isOccupancy @7 :Bool;

    regionID @8 :Int16;
}

struct Arrow {
  # arrow id
  id @0 :UInt64;

  # arrow type
  # 1 for left arrow
  # 2 for right arrow
  # 3 for straight left arrow
  # 4 for straight right arrow
  # 5 for left right arrow
  # 6 for straight arrow
  # 7 for turnover arrow
  # 8 for straight left right arrow
  arrowType @1 :UInt32;

  # sorted corner points on arrow contour
  points @2 :List(StdGeometry.Point);

  regionID @3 :Int16;
}

struct Bump {
  # bump id
  id @0 :UInt64;

  # bump half short side length
  halfWidth @1 :Float32;

  # bump long side length
  length @2 :Float32;

  # bump one end point
  endPt @3 :StdGeometry.Point;

  # bump wide/long side direction
  wideDirection @4 :StdGeometry.Vector3;
  longDirection @5 :StdGeometry.Vector3;


    # parking slot occupancy property
    isOccupancy @6 :Bool;

  regionID @7 :Int16;
}


struct CrossWalk {
  # cross walk id
  id @0 :UInt64;

  # cross walk half short side length
  halfWidth @1 :Float32;

  # cross walk long side length
  length @2 :Float32;

  # cross walk one end point
  endPt @3 :StdGeometry.Point;

  # cross walk wide/long side direction
  wideDirection @4 :StdGeometry.Vector3;
  longDirection @5 :StdGeometry.Vector3;

  regionID @6 :Int16;
}

struct LaneLine {
  # lane line id
  id @0 :UInt64;

  # lane line type, 1 for solid line, 2 for dashed line
  lineType @1 :UInt32;

  # sorted points set on this lane line
  points @2 :List(StdGeometry.Point);

  regionID @3 :Int16;
}

struct StopLine {
  # stop line id
  id @0 :UInt64;

  # start/end point of stop line endpoint
  startPt @1 :StdGeometry.Point;
  endPt @2 :StdGeometry.Point;

  regionID @3 :Int16;
}

struct RegionInfo {
    id @0 :Int16;
    name @1 :Text;

    fromRegionID @2 :Int16;
    toRegionID @3 :Int16;

    fromRegionHeight @4 :Float64;
    toRegionHeight @5 :Float64;

    # currently, if fromRegionID is different to toRegionID, this region is ramp, otherwise is floor
    # isEntranceRamp @6 :Bool;
}

struct MixVprData {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    timestampNs @1 :UInt64;

    # size of list must be 4096
    desc @2 :List(Float32);
}

struct SuperPointData {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    timestampNs @1 :UInt64;

    # Matrix256Xf
    descCols @2 :UInt64;
    desc @3 :Data;

    # Matrix2Xf
    kptsCols @4 :UInt64;
    kpts @5 :Data;

    # VectorXf
    scores @6 :List(Float32);
}