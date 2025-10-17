@0xc553bad1c08c26ab;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_state");

using AvpState = import "/ad_msg_idl/avp_state/avp_main_state.capnp";
using Module = AvpState.AvpMainState.Module;
using Fbs = AvpState.AvpMainState.Fbs;
using State = AvpState.AvpMainState.State;

struct StateMachineDebugInfo {
  # Debug index uint64
  idx @0 :UInt64;

  # The timestamp at msg create
  timestamp @1 :UInt64;

  # statemachine state
  state @2 :State;

  # state cast to int64_t
  stateNumber @3:UInt64;

  # perceptionOd
  # perceptionRd
  # planning
  # control
  # slam
  # perceptionFusion

  # OD
  module1 @4 :Module = perceptionOd;
  fbExpect1 @5 :Fbs;
  fbCurrent1 @6 :Fbs;

  # RD
  module2 @7 :Module = perceptionRd;
  fbExpect2 @8 :Fbs;
  fbCurrent2 @9 :Fbs;

  # Plan
  module3 @10 :Module = planning;
  fbExpect3 @11 :Fbs;
  fbCurrent3 @12 :Fbs;

  # Control
  module4 @13 :Module = control;
  fbExpect4 @14 :Fbs;
  fbCurrent4 @15 :Fbs;

  # Slam
  module5 @16 :Module = slam;
  fbExpect5 @17 :Fbs;
  fbCurrent5 @18 :Fbs;

  # PerceptionFusion
  module6 @19 :Module = perceptionFusion;
  fbExpect6 @20 :Fbs;
  fbCurrent6 @21 :Fbs;
}
