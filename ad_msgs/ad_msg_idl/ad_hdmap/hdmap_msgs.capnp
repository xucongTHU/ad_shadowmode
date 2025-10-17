@0xe4d73f12f955d7af;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::hdmap");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct MapCoordLLA {
# LLA coordinate
    longitude @0 :Float64;
    latitude @1 :Float64;
    altitude @2 :Float64;
}

struct RoutingLine {
    lla @0 :MapCoordLLA;
    # ENU points
    enu @1 :StdGeometry.Point;
}

struct RoutingNode {
    # Node type and ID
    nodeId @0 :Text;
    nodeType @1 :UInt8;

    laneId @2 :Text;
    roadId @3 :Text;
    accuLength @4 :Float64;

    # Section inherit
    fromSection @5 :Text;
    toSection @6 :Text;

    # Lane inherit
    fromLane @7 :Text;
    toLane @8 :Text;
}

struct Routing {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    # Time cost of routing process in nanosecond
    processTimeNs @1 :UInt64;
    # Name of trigger source for debug
    triggerSrc @2 :Text;

    # Routing info
    nodeList @3 :List(RoutingNode);
    navigationLine @4 :List(RoutingLine);
    # Response point info
    departureEnu @5 :StdGeometry.Point;
    destinationEnu @6 :StdGeometry.Point;

    # Debug info
    debugInfo @7 :Text;
}

struct SubMapMeta {
    # Time for sub map process
    mapLoadTimeNs @0 :UInt64;
    # Current sub map center point (ENU)
    centerPositionEnu @1 :StdGeometry.Point;
    # Range of this sub map
    rangeRadiusM @2 :Float64;
}

struct SubMap {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;
    # Sub map info
    info @1 :SubMapMeta;
    # Data buffer used to temporarily store submap
    data @2 :Text;
}

struct LaneBrief {
    laneId @0 :Text;
    laneLength @1 :Float64;
    leftLaneId @2 :Text;
    rightLaneId @3 :Text;
    preLaneIds @4 :List(Text);
    succLaneIds @5 :List(Text);
    laneType @6 :UInt8;
}

struct SectionBrief {
    sectionId @0 :Text;
    sectionLength @1 :Float64;
    preSectionIds @2 :List(Text);
    succSectionIds @3 :List(Text);
    laneBriefs @4:List(LaneBrief);
}

