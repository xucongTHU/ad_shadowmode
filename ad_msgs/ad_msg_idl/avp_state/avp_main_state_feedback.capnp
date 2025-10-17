@0xdf0924c5bbecbe52;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_state");

struct AvpMainStateFeedbackRequest {
    # feedback uid from AvpMainState.msg
    uid @0 :UInt64;
    # feedback State enum: AvpFeedbackStatus
    state @1 :UInt32;
    # feedback from which module
    module @2 :UInt32;
    err @3 :Text;
}

struct AvpMainStateFeedbackResponse {
    # return status:  AvpFeedbackStatus
    ret @0 :UInt32;
}
