@0xebe039d7969d002a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::maplesslm");

using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";
using LocalMap = import "/ad_msg_idl/L2Plus_localmap_msgs/local_msgs.capnp";
using SdMap = import "/ad_msg_idl/l2plus_sdmap/sdmap_msgs.capnp";

struct KeyNode {
    refPointType @0 :UInt8;
    distance @1 :Float64;
    navEnterLinkId @2 :UInt64;
    navEnterLinkHeading @3 :Float64;
    navOutLinkId @4 :UInt64;
    navOutLinkHeading @5 :Float64;
    otherEnterLinkIds @6 :List(UInt64);
    otherEnterLinkHeadings @7 :List(Float64);
    otherOutLinkIds @8 :List(UInt64);
    otherOutLinkHeadings @9 :List(Float64);
    nodeId @10 :SdMap.NodeIDType;
    position @11 :Geo.Point;
}

struct VehicleState {
    timestampNs @0 :UInt64;

    # vehicle pose
    lla @1 :Geo.Point;
    roll @2 :Float64;
    pitch @3 :Float64;
    yaw @4 :Float64;

    # road info
    linkId @5 :UInt64;
    linkHeading @6 :Float64;
    nextlinkId @7 :UInt64;
    nextLinkHeading @8 :Float64;

    # navigation info
    nextKeyNode @9 :KeyNode;
    nextNextKeyNode @10 :KeyNode;
    navDirection @11 :UInt8;

    # lane info
    refLaneId  @12 :UInt8;
    laneNumber  @13 :UInt8;
    curLaneId @14 :UInt64;
}

struct SceneNaviMap {
    roadStructure @0 :LocalMap.RoadStruct;
    vehicleState @1 :VehicleState;
    sdMap @2 :SdMap.SDMap;
    vehicleStateMnop @3 :VehicleState;
    sdMapMnop @4 :SdMap.SDMap;
}
