@0xce15c675c1249ad7;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::rscl::idl");

struct CrossSystemStatInfo {
    recvTraffic         @0 :Float64;

    recvFrameFrequency  @1 :UInt64;
    recvFrameTotal      @2 :UInt64;

    lostFrameTotal      @3 :UInt64;
    lostFrameRatio      @4 :UInt64;

    maxLatency          @5 :UInt64;

    sentTraffic         @6 :Float64;

    sentFrameFrequency  @7 :UInt64;
    sentFrameTotal      @8 :UInt64;

    droppedFrameTotal   @9 :UInt64;
    droppedFrameRatio   @10 :UInt64;
}
