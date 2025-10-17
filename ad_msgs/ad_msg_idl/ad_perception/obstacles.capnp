@0xd1a85dc81e9ce609;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using Base = import "/ad_msg_idl/ad_perception/base.capnp";

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using import "/ad_msg_idl/ad_perception/uss_range.capnp".UssRange;

enum ObstacleMotionStatus {
  motionStatusUnknown @0;
  motionStatusMoving @1;
  motionStatusStill @2;
}

struct ObstacleTrajectoryPoint {
  deltaTNs @0 :UInt64;
  center @1 :Base.Vector3f;
  direction @2 :Base.Vector3f;
}

struct ObstacleTrajectory {
  motionStatus @0 :ObstacleMotionStatus;
  confidence @1 :Float32;
  points @2 :List(ObstacleTrajectoryPoint);
}

struct ObstacleCube {
  camera @0 :Text;
  label @1 :Int32;
  typeConfidence @2 :Float32;
  existenceConfidence @3 :Float32;
  trackId @4 :Int32;
  center @5 :Base.Vector3d;
  ## [length width height]
  shape @6 :Base.Vector3d;
  direction @7 :Base.Vector3d;
  motionInfo @8 :Base.MotionInfo;
  trajectory @9 :List(ObstacleTrajectory);
}

struct ObstacleCylindrical {
  camera @0 :Text;
  label @1 :Int32;
  typeConfidence @2 :Float32;
  existenceConfidence @3 :Float32;
  trackId @4 :Int32;
  center @5 :Base.Vector3d;
  radius @6 :Float32;
  height @7 :Float32;
  direction @8 :Base.Vector3d;
  motionInfo @9 :Base.MotionInfo;
  trajectory @10 :List(ObstacleTrajectory);
  staticState @11 :Int32;
  attrScore @12 :Float32;
}

struct ObstacleRaw {
  ## Camera
  camera @0 :Text;

  ## det_info
  label @1 :Int32;
  typeConfidence @2 :Float32;
  existenceConfidence @3 :Float32;
  box @4 :Base.Bbox2D;

  ## direction_info
  directionInfo @5 :Base.DirectionInfo;

  ## attribute_info
  landmark @6 :List(Base.Point2f);
  landmarkScores @7 :List(Float32);
  landmark4 @8 :List(Base.Point2f);
  landmark4Scores @9 :List(Float32);
  landmark9 @10 :List(Base.Point2f);
  landmark9Scores @11 :List(Float32);

  ## position_info
  positionInfo @12 :Base.PositionInfo;

  ## box3d
  box3D @13 :List(Float32);

  ## object static
  staticState @14 :Int32;
  attrScore @15 :Float32;
}

struct Obstacles {
  # header
  header @0 :StdMsgsHeader.Header;

  # cylindrical obstacles
  cylindricalObstacles @1 :List(ObstacleCylindrical);
  # Cube obstacles
  cubeObstacles @2 :List(ObstacleCube);

  # Raw 2D objects
  rawObjects @3 :List(ObstacleRaw);

  #UssRange
  rangeUnitArray @4 :List(UssRange);

  cameras @5 :List(Text);
  # same sequence as cameras
  timestamps @6 :List(UInt64);
}

struct FSLinePt {
  v @0 :Int16;
  label @1 :UInt8;
  isValid @2: Bool;
}

struct FSLineResult {
  fsLinePts @0 :List(FSLinePt);
  timestampNs @1 :UInt64;
  isValid @2 :Bool;
}

struct FSLineFrame {
  header @0 :StdMsgsHeader.Header;
  fisheyeFsLineResults @1 :List(FSLineResult);  # front->left->rear->right
}
