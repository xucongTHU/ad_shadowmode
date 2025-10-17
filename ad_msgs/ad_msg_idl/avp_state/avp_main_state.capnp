@0x8f10e1dc887cfa46;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_state");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct AvpMainState {
  # Header include seq, timestamp(last node pub time), and frame_id
  header @0 :StdMsgsHeader.Header;

  # uid
  uid @1 :UInt64;

  # State enum uint8
  state @2 :UInt32;

  switchState @3 :UInt8;

  # AVP common status
  enum State {
    # 软件启动
    avpStartUp @0;
    # 地面行车
    avpTravelOutside @1;
    # 地面行车-靠近停车场
    avpTravelAroundParking @2;
    # 匿名停车场
    avpAnonymousParking @3;
    # 功能选择-定位成功
    avpFunctionSelectNoTraj @4;
    # 功能选择-轨迹匹配成功
    avpFunctionSelect @5;

    # HPP status
    # 11
    hpaExit @6;
    # 12
    hpaMapping @7;
    # 13
    hpaMappingVerify @8;
    # 14
    hpaMappingDone @9;
    # 15
    hpaSearchingDeleted @10;
    # 16
    hpaStandbyDeleted @11;
    # 17
    hpaCruise @12;
    # 18
    hpaCruiseStopping @13;
    # 19
    hpaCruiseStopDone @14;
    # 20
    hpaAlongParking @15;
    # 21
    hpaCruisePause @16;
    # 22
    flagHppCruiseEnd @17;
    # AVP parking status
    # 29
    avpParkingQuest @18;
    # 30
    avpParkingStopDone @19;
    # 31
    avpParkingSteerDoing @20;
    # 32
    avpParkingSteerDone @21;
    # 33
    avpParkingDrive @22;
    # 34
    avpParkingStopDoing @23;
    # 35
    avpParkingDone @24;
    # 36
    avpParkingStandby @25;
    # 37
    avpParkingPauseDeleted @26;
    # 40
    flagAvpParkingEnd @27;
    # exception handle
    # 200
    flagAvpException @28;
    # 253
    avpEmergencyBrake @29;
    # 254
    avpParkingAbnormal @30;
    # End
    avpStateEnd @31;
  }

  # Module
  enum Module {
    perceptionOd @0;
    perceptionRd @1;
    planning @2;
    control @3;
    slam @4;
    perceptionFusion @5;
    totalSize @6;  # make it at last
  }

  ## Common feedback state
  enum Fbs {
    # 通用成功
    ok @0;
    # 通用失败
    fail @1;
    # 暂停
    pause @2;
    # EMB
    emergencyBrake @3;
    # EMB解除
    emergencyBrakeRelease @4;

    ## 规划状态
    # 轨迹结束
    trajFinish @5;
    # 循迹路线跟踪
    routeTracking @6;
    # 循迹路线跟丢
    routeTrackLost @7;
    # 发现空闲车位
    foundIdleSlot @8;

    ## 靠近停车场
    nearParking @9;
    ## 远离停车场
    farawayParking @10;

    smoothlySummon @11;

    # 255
    none @12;
  }
}
