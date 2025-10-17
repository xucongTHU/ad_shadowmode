@0xb88b444a5bb889b0;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::tap");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AsCoreOutCapnp = import "/ad_msg_idl/ad_tap/as_coreout.capnp";
using AsControlCapnp = import "/ad_msg_idl/ad_tap/as_control.capnp";
using AsVseCapnp = import "/ad_msg_idl/ad_tap/as_vse.capnp";

struct ActiveSafetyDebug {
  header @0: StdHeader.Header;
  vse   @1: AsVseCapnp.AsVseOut;
  coreOut @2: AsCoreOutCapnp.AsCoreOut;
  ctrl @3: AsControlCapnp.AsControl;
}
