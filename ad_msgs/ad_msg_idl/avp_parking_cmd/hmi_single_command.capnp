@0xdab962ff343d9b28;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_parking_cmd");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AvpState = import "/ad_msg_idl/avp_state/avp_main_state.capnp";

struct HMISingleCommand {
  enum CommandType {
    resumeApa @0;
    exitApa @1;
    confirmAbortApa @2;
    confirmAbortHpa @3;
  }

  header @0 :StdMsgsHeader.Header;
  timestampNs @1 :UInt64;

  request @2 :CommandType;
  state @3 :AvpState.AvpMainState.State;
}
