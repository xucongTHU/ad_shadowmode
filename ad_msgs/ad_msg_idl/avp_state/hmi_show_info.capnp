@0xc553bad1c08c26ab;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_state");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct HMIShowInfo {
  # Header include seq, timestamp(last node pub time), and frame_id
  header @0 :StdMsgsHeader.Header;

  # Info enum uint8
  hmiShowCode @1 :UInt16;
}
