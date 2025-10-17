@0xd2f410d0f757995a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::act_acan");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct Bcs2A {
    bcsAbsActiveSt @0 :UInt8;
    bcsAbsFaultSt @1 :UInt8;
    bcsEbdActiveSt @2 :UInt8;
    bcsEbdFaultSt @3 :UInt8;
    bcsEngTorqDecActiveStFa @4 :UInt8;
    bcsEngTorqIncActiveSt @5 :UInt8;
    bcsTcsActiveSt @6 :UInt8;
    bcsTcsFaultSt @7 :UInt8;
    bcsVdcActiveSt @8 :UInt8;
    bcsVdcFaultSt @9 :UInt8;
    bcsVehSpd @10 :Float32;
    bcsVehSpdVd @11 :UInt8;
}

struct ActAcan {
    # Header include seq, timestamp, and frame_id(vehicle_type)
    header @0 :StdHeader.Header;

    bcs2A @1 :Bcs2A;
}

