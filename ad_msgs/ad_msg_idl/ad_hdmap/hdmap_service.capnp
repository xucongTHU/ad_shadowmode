@0xde56fbf3ee9a428a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::hdmap");

using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";
using Hdmap = import "/ad_msg_idl/ad_hdmap/hdmap_msgs.capnp";


enum RoutingRequestType {
    requestBaseRoute @0;
    dynamicReroute @1;
}

struct GetSubMapRequest {
    centerPositionEnu @0 :StdGeometry.Point;
    # Range of request sub map
    rangeRadiusM @1 :Float64;
}

struct GetSubMapResponse {
    # Response result info
    success @0 :Bool;
    message @1 :Bool;
    # Response sub map
    subMap @2 :Hdmap.SubMap;
}

struct GetSectionBriefRequest {
    sectionId @0 :Text;
}

struct GetSectionBriefResponse {
    # Response result info
    success @0 :Bool;
    message @1 :Text;
    # Response section brief
    brief @2 :Hdmap.SectionBrief;
}

struct QueryRouteRequest {
    # Temporally added for Cloud Service
    # Only query possible route with no side effect

    # Target point
    departureLla @0 :Hdmap.MapCoordLLA;
    destinationLla @1 :Hdmap.MapCoordLLA;
    # Points LLA
    midPointsLla @2 :List(Hdmap.MapCoordLLA);
    blacklistPointsLla @3 :List(Hdmap.MapCoordLLA);
    # Pull over at destination
    # if set true, will use the rightest lane at destination
    destinationPullOver @4 :Bool;
}

struct QueryRouteResponse {
    # Response result info
    success @0 :Bool;
    message @1 :Text;

    # Routing info
    nodeList @2 :List(Hdmap.RoutingNode);
    navigationLine @3 :List(Hdmap.RoutingLine);
    # Response point info
    departureLla @4 :Hdmap.MapCoordLLA;
    destinationLla @5 :Hdmap.MapCoordLLA;

    # Debug info
    debugInfo @6 :Text;
}

struct RerouteByENURequest {
    # Target point
    departureEnu @0 :StdGeometry.Point;
    destinationEnu @1 :StdGeometry.Point;
    # Points ENU
    midPointsEnu @2 :List(StdGeometry.Point);
    blacklistPointsEnu @3 :List(StdGeometry.Point);
    # Pull over at destination
    # if set true, will use the rightest lane at destination
    destinationPullOver @4 :Bool;
    # Request Type
    requestType @5 :RoutingRequestType;
}

struct RerouteByENUResponse {
    success @0 :Bool;
    message @1 :Text;
}

struct RerouteByLaneIDRequest {
    # Target point
    departureEnu @0 :StdGeometry.Point;
    destinationEnu @1 :StdGeometry.Point;
    # Lane ID
    midLaneIds @2 :List(Text);
    blacklistLanes @3 :List(Text);
    # Pull over at destination
    # if set true, will use the rightest lane at destination
    destinationPullOver @4 :Bool;
    # Request Type
    requestType @5 :RoutingRequestType;
}

struct RerouteByLaneIDResponse {
    success @0 :Bool;
    message @1 :Text;
}

struct RerouteByLLARequest {
    # Target point
    departureLla @0 :Hdmap.MapCoordLLA;
    destinationLla @1 :Hdmap.MapCoordLLA;
    # Points LLA
    midPointsLla @2 :List(Hdmap.MapCoordLLA);
    blacklistPointsLla @3 :List(Hdmap.MapCoordLLA);
    # Pull over at destination
    # if set true, will use the rightest lane at destination
    destinationPullOver @4 :Bool;
    # Request Type
    requestType @5 :RoutingRequestType;
}

struct RerouteByLLAResponse {
    success @0 :Bool;
    message @1 :Text;
}
