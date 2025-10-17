@0xa335e1f0588cb30c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_control");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct ControlState {
  # Header include seq, timestamp(last node pub time), and frame_id

  header @0 :StdMsgsHeader.Header;

  # State enum

  state @1 :UInt32;
  enum State {
    off @0;
    stoping @1;
    stopdone @2;
    turnsteer @3;
    turnsteerdone @4;
    forwardCruise @5;
    backwardCruise @6;
    emergency @7;
  }
}
