@0xd68cc87787c6d29d;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_planning");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct PlanningState {
  # Header include seq, timestamp(last node pub time), and frame_id
  header @0 :StdMsgsHeader.Header;

  # State enum
  state @1 :UInt32;
  enum State {
    park @0;
    neutral @1;
    stop @2;
    cruise @3;
  }
}
