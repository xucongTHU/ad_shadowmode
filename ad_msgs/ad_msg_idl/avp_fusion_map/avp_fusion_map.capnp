@0xa1db0f0cfa5973a8;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_fusion_map");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct Point2F {
    x @0 :Float32;
    y @1 :Float32;
}

struct Point3F {
    x @0 :Float32;
    y @1 :Float32;
    z @2 :Float32;
}

struct Pose2F {
    position @0 : Point2F;
    yaw @1 : Float32;
}

struct BoundaryPoint {
    position @0 : Point3F;
    age @1 : Int32;
    confidence @2 : Int32;

    # source type
    # invalid = 0;
    # fsline = 1;
    # lidar = 2;
    source @3 : UInt8;
}

struct LinkNode {
    node @0 : Point3F;
    type @1 : Int32;
    confidence @2 : Int32;
}

struct RoadBoundary {
    timestamp @0 : UInt64;
    leftBoundary @1 : List(BoundaryPoint);
    rightBoundary @2 : List(BoundaryPoint);
    leftBoundaryPointNum @3 : UInt64;
    rightBoundaryPointNum @4 : UInt64;

    nodeList @5 : List(LinkNode);
}

struct SlotBoundary {
    timestamp @0 : UInt64;
    leftBoundary @1 : List(BoundaryPoint);
    rightBoundary @2 : List(BoundaryPoint);
    leftBoundaryPointNum @3 : UInt64;
    rightBoundaryPointNum @4 : UInt64;
}

struct FusionSlotOut {
    
    corners @0: List(Point2F);       # four corners

    objects @1: List(Point2F);       # two objects
    pointOblique @2: List(Point2F);  # two objects at slot end

    refPoint @3: Pose2F;  # reference point
    startPose @4: Pose2F; # pose of ego car
    endPose @5: Pose2F;   # pose of the slot

    # from line01 to nearest obstacle - parallel to line01
    spaceDepth@6: Float32; 
    # perpendicular to line01 - slot useful width
    spaceLength@7: Float32; 
    # the distance between stop limiter and slot entry line.
    limiterDepth@8: Float32; 

    roadBound @9 : RoadBoundary;
    slotBound @10 : SlotBoundary;

    id@11 : UInt32; # from OCR or SLAM
    apaId@12 : UInt32;

    # slot type
    # vertical = 0;
    # horizontal = 1;
    # oblique_30 = 2;
    # oblique_45 = 3;
    # oblique_60 = 4;
    # threeDimensional = 5;
    # spaceVertical = 6;
    # spaceHorizontal = 7;
    # spaceInclined = 8;
    # default = 9;
    type@13 : UInt8;

    # slot state
    # unknown = 0;
    # available = 1;
    # occupied = 2;
    state@14 : UInt8;

    # invalid reason
    # default = 0;
    # wellDetected = 1;
    # size = 2;
    # angle = 3;
    # distance = 4;
    # boundary = 5;
    # pre_plan = 6; 
    # depth = 7;
    # od = 8;
    invalidReason@15 : UInt8;

    # slot source
    # default = 0;
    # ipm = 1;
    # ultrasonic = 2;
    # freespace = 3;
    # hmi = 4;
    source@16 : UInt8;

    confidence@17 : UInt8;      # [0-100] 70 is good 80 is better.

    # Parking Direction Bitmask Definitions
    # Each bit represents a specific parking direction:
    # 0b10000000 - FRONT_IN
    # 0b01000000 - REAR_IN
    # 0b00100000 - FRONT_LEFT_OUT
    # 0b00010000 - FRONT_CENTER_OUT
    # 0b00001000 - FRONT_RIGHT_OUT
    # 0b00000100 - REAR_LEFT_OUT
    # 0b00000010 - REAR_CENTER_OUT
    # 0b00000001 - REAR_RIGHT_OUT

    # Recommended parking direction, represented by a single bit
    parkingDirectionRecommend@19 : UInt8;

    needReplanFlag@20 : UInt8;  # 0: dont need; 1:need replan

    # Available parking directions, each bit represents a direction
    parkingDirectionAvailable@18 : UInt8;
    regionID @21: Int16;
    z @22: Float32;
}

struct FusionSlotOutArray {
    header @0 :StdHeader.Header;
    slots @1: List(FusionSlotOut);
    targetSlot @2: List(FusionSlotOut);
    slamSlots @3: List(FusionSlotOut);
}