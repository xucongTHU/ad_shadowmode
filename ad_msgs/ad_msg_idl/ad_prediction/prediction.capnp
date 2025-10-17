@0xcc281b610ce1cc43;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::prediction");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct PredictionTrajectoryPoint {
    polygonContour @0 :List(StdGeometry.Point);

    position @1 :StdGeometry.Point;
    direction @2 :StdGeometry.Point;
    speed @3 :StdGeometry.Point;

    variancePosition @4 :StdGeometry.Point;
    varianceSpeed @5 :StdGeometry.Point;

    covariancePosition @6 :Float32;
    covarianceSpeed @7 :Float32;
}

struct PredictionTrajectory {
    predictorName @0 :Text;
    targetRoadId @1 :Text;
    targetLaneId @2 :Text;

    pointIntervalTimeNs @3 :UInt64;
    numPoints @4 :UInt32;
    trajectoryPointArray @5 :List(PredictionTrajectoryPoint);

    # Info
    isAbnormal @6 :Bool;
    isCollision @7 :Bool;
    isSelected @8 :Bool;
    probability @9 :Float32;
    score @10 :Float32;
}

struct PredictionObject {
    id @0 :Int32;
    obstacleTypeEnum @1 :UInt32;
    objectLabelEnum @2 :UInt32;

    lengthM @3 :Float32;
    widthM @4 :Float32;
    heightM @5 :Float32;

    polygonContour @6 :List(StdGeometry.Point);

    position @7 :StdGeometry.Point;
    speed @8 :StdGeometry.Point;
    acceleration @9 :StdGeometry.Point;
    headingDirection @10 :StdGeometry.Point;

    infoStateEnum @11 :UInt32;
    sceneTypeEnum @12 :UInt32;
    importanceLevelEnum @13 :UInt32;
    moveStep @14 :UInt32;

    intentionType @15 :UInt32;
    targetRoadId @16 :Text;
    targetLaneId @17 :Text;

    trajectoryArray @18 :List(PredictionTrajectory);
    historyTrajectoryPoints @19 :List(StdGeometry.Point);
}

struct Prediction {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    # Timestamp
    predictionTimeNs @1 :UInt64;
    fusionTimeNs @2 :UInt64;

    # Objects
    objectList @3 :List(PredictionObject);
}
