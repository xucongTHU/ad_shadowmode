@0xd9cad1d6d6f4e4a9;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::sensor");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct ObuEvent {
    id @0 :Text;

    trackId @1 :Text;

    evtType @2 :UInt32;
    evtStatus @3 :UInt32;

    lon @4 :Float64;
    lat @5 :Float64;

    ele @6 :Float64;
    heading @7 :Float64;
    speed @8 :Float64;
    ttc @9 :Float64;
    dist2crash @10 :Float64;
    width @11 :Float64;
    height @12 :Float64;
    length @13 :Float64;

    plateNum @14 :Text;
    obstacleType @15 :UInt32;
    objectLabel @16 :UInt32;

    deviceType @17 :Text;
}

struct ObuEventFrame {
    header @0 :StdHeader.Header;

    sourceType @1 :UInt16;
    # data timestamp
    receivedTimestamp @2 :UInt64;

    # ObjectHeader
    timeStamp @3 :Text;

    # Objects
    evtList @4 :List(ObuEvent);
}

struct ObuFusionFrame {
    header @0 :StdHeader.Header;

    # Timestamp
    fusionTimeNs @1 :UInt64;

    # Objects
    objectList @2 :List(ObuFusionObject);
}

enum ObuFusionType {
    # the perception data from host
    ftHost @0;
    # the obstacle from obu
    ftObu @1;
    # the intersection data of host and obu
    ftMixed @2;
}

struct ObuFusionObject {
    id @0 :Text;
    obstacleType @1 :UInt32;
    objectLabel @2 :UInt32;


    lengthM @3 :Float32;
    widthM @4 :Float32;
    heightM @5 :Float32;

    # The meaning of source_type refers to the source of the data
    # enum: vehicle host, datang_obu, huawei_obu and so on
    sourceType @6 :UInt16;

    enum FusionType {
        # the perception data from host
        ftHost @0;
        # the obstacle from obu
        ftObu @1;
        # the intersection data of host and obu
        ftMixed @2;
    }

    # The meaning of fusion_type is the intersection and union state of the vehicle and obu data
    fusionType @7 :FusionType;

    plateNum @8 :Text;


    heading @9 :Float64;
    spd @10 :Float64;


    polygonContour @11 :List(StdGeometry.Point);

    position @12 :StdGeometry.Point;
    speed @13 :StdGeometry.Point;
    acceleration @14 :StdGeometry.Point;
}

struct ObuObject {
    id @0 :Text;

    obstacleType @1 :UInt32;
    objectLabel @2 :UInt32;

    lon @3 :Float64;
    lat @4 :Float64;
    ele @5 :Float64;

    speed @6 :Float64;
    heading @7 :Float64;
    width @8 :Float64;
    height @9 :Float64;
    length @10 :Float64;

    laneNo @11 :UInt16;

    plateNum @12 :Text;
    brand @13 :Text;

    historyTrajectoryPoints @14 :List(StdGeometry.Point);
}

struct ObuObjectFrame {
    header @0 :StdHeader.Header;

    sourceType @1 :UInt16;

    # data timestamp
    receivedTimestamp @2 :UInt64;

    # ObjectHeader
    timeStamp @3 :Text;

    #enum Type {
    #    pvUnknown @0;
    #    pvAll @1;
    #    pvVehicle @2;
    #    pvNonVehicle @3;
    #}

    type @4 :UInt32;

    # Objects
    objectList @5 :List(ObuObject);
}
