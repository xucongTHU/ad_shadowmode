@0xb2868090a1bf87f6;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_state_v3");

enum AvpMainStatus {
    none @0;
    error @1;
    startup @2;
    background @3;
    apaSearching @4;
    apaParking @5;
    apaSuspend @6;
    apaAbort @7;
    apaParkingDone @8;
    hppSearching @9;
    hppMapping @10;
    hppMappingDone @11;
    hppCruise @12;
    hppAbort @13;
    raCruise @14;
    raSuspend @15;
    raDone @16;
    raAbort @17;
}

# 当前运行环境
enum AvpRunningMode {
    # 预留
    invalid @0;
    # 行车
    pilot @1;
    # 泊车
    parking @2;
}
