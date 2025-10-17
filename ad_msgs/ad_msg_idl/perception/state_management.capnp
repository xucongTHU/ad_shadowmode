@0xbc4b80c004a206b1;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::state_management");
using Header = import "/ad_msg_idl/std_msgs/header.capnp";


struct StateManagement {
    enum SceneMode{
        none @0;
        driving @1;
        parking @2;
        parkingHpa @3;
    }
    sceneMode @0 :SceneMode;
}

