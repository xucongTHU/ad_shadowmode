@0xc42fa2f431b174d6;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::data_mining");

using Header = import "/ad_msg_idl/std_msgs/header.capnp";

struct MinningInfo {
    start @0:Int64;
    end @1:Int64;
    tagType @2:Text;
    tagInfoType @3:Text;
    status @4:Bool;
    tagId @5:Int32;
}


struct SplitTagInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :Header.Header;
    minningInfo @1 :MinningInfo;
}
