@0xbc2d13563a25316f;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::peception_input_data");
using Header = import "/ad_msg_idl/std_msgs/header.capnp";

struct CameraPerceptionInputData {
    header @0 :Header.Header;

    # center_camera_fov120 center_camera_fov30 left_front_camera left_rear_camera right_front_camera right_rear_camera rear_camera
    timestampNs @1 :List(UInt64);
}
