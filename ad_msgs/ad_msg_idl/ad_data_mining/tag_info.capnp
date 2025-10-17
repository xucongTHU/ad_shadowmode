@0x8b63852d98ca152a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::data_mining");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";

struct LaelapsMinningInfo {
    start @0:Int64;
    end @1:Int64;
    tagType @2:Text;
    tagInfoType @3:Text;
    status @4:Bool;
    tagId @5:Int32;
}

struct MergedLaelapsMinningInfo {
    start @0:Int64;
    end @1:Int64;
    tagType @2:Text;
    miningInfos @3:List(LaelapsMinningInfo);
}

struct TagInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :Header.Header;
    mergedMiningInfos @1 :List(MergedLaelapsMinningInfo);
}
