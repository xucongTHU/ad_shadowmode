@0xfdfa4d4b552d3bec;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using Base = import "/ad_msg_idl/ad_perception/base.capnp";
using import "/ad_msg_idl/std_msgs/typed_data_list.capnp".Float32List;
using import "/ad_msg_idl/ad_perception/object_label.capnp".ObjectLabel;

struct Object {
    # BaseObject

    # enum SensorID
    sensorId @0 :Int32;

    # enum ObjectLabel
    label @1 :ObjectLabel;

    typeConfidence @2 :Float32;
    existenceConfidence @3 :Float32;
    # enum ObjectLabel in perception_common/base/data_type/object_label.hpp
    subType @4 :Int32;

    subTypeProbs @5 :Float32List;
    trackId @6 :Int32;
    trackAgeTimeSecond @7 :Float32;
    lastMeasureUpdateTimestampNs @8 :UInt64;
    currentPredictionTimestampNs @9 :UInt64;

    sizeInfo @10 :Base.SizeInfo;
    polygonBox @11 :Base.PolygonBox;
    directionInfo @12 :Base.DirectionInfo;
    bvCorners @13 :Base.BirdViewCorners;
    motionInfo @14 :Base.MotionInfo;
    motionStatus @15 :Int32;
    motionStaticScore @16 :Float32;

    # enum MaintenanceStatus
    maintenanceStatus @17 :Int32;
    # enum OcclusionStatus
    occlusionStatus @18 :Int32;

    # ModelOutputFeatures
    feature @19 :List(Base.ModelOutputFeature);

    # CameraBBox2DInfo
    cameraBboxInfo @20 :Base.CameraBBox2DInfo;

    enum DerivedType {
        cameraNonVehicle @0;
        cameraPedestrian @1;
        cameraVehicle @2;
        cameraStaticObstacle @3;
        lidarObject @4;
        radarObject @5;
        fusionObject @6;
        # only for visualization
        cameraTrafficlight @7;
    }
    # Derived object type : used only between ros nodes to determine dynamic type
    # of object
    derivedType @21 :DerivedType;

    # Shared: NonAutomobileVehicle Pedestrian Automobile
    attribute @22 :Base.BaseCameraAttribute;

    # Shared: Automobile FusionObject
    automobileLightStatus @23 :Base.AutomobileLightStatus;

    # Static Obstacle Specific
    isRightEdgeDetected @24 :Bool;
    rightEdgeLine @25 :Base.Vector4i;

    # NonAutomobileVehicle Specific
    # enum NonAutomobileVehicleLightStatus
    nonAutomobileVehicleLightStatus @26 :Base.NonAutomobileVehicleLightStatus;
    isWithPed @27 :Bool;

    # Pedestrian Specific
    # action enum AttrBool
    umbrella @28 :Int32;
    call @29 :Int32;
    phone @30 :Int32;
    wave @31 :Int32;
    standing @32 :Int32;
    # direction
    upperBodyDirection @33 :Base.Point2f;
    lowerBodyDirection @34 :Base.Point2f;
    faceDirection @35 :Base.Point2f;
    # intention
    intention @36 :Base.PedestrianIntention;
    isOnBike @37 :Bool;

    # Automobile Specific
    depthPoint @38 :Base.DepthDetPoint;

    # LidarObject Specific
    pointCloudTimestampNs @39 :UInt64;

    # RadarObject Specific
    trackRange @40 :Float64;
    trackRangeRate @41 :Float64;
    trackRangeAccelRate @42 :Float64;
    trackLatRate @43 :Float64;

    # FusionObject Specific
    matchedSensorObjectInfo @44 :List(Base.MatchedSensorObjectInfo);

    # TrafficLight Specific, only for visualization
    colorLabel @45 :UInt32;
}
