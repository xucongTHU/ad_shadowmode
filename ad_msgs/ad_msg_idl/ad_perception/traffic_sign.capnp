@0xe9baf9a43d5701cd;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

using Base = import "/ad_msg_idl/ad_perception/base.capnp";
using import "/ad_msg_idl/std_msgs/typed_data_list.capnp".Float32List;
using import "/ad_msg_idl/ad_perception/object_label.capnp".ObjectLabel;

struct TrafficSign {
    # BaseObject

    # enum SensorID
    sensorId @0 :Int32;
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
    motionInfo @13 :Base.MotionInfo;
    motionStatus @14 :Int32;
    motionStaticScore @15 :Float32;

    # enum MaintenanceStatus
    maintenanceStatus @16 :Int32;
    # enum OcclusionStatus
    occlusionStatus @17 :Int32;

    # ModelOutputFeatures
    feature @18 :List(Base.ModelOutputFeature);

    # TrafficSign Specific

    # CameraBBox2DInfo
    cameraBboxInfo @19 :Base.CameraBBox2DInfo;
}
