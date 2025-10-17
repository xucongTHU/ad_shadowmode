@0x95bc0983fb4ca581;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_parking_cmd");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct HMICommand {
  header @0 :StdMsgsHeader.Header;
  timestampNs @1 :UInt64;

  #暂停/恢复
  flagPause @2 :Bool;
  #HPM记忆开始/结束
  flagHppStudy @3 :Bool;
  #HPA巡航开始
  flagHpaCruise @4 :Bool;
  #沿途泊车开始/退出
  flagAlongParking @5 :Bool;
  #继续找车位
  flagFindParkingSpace @6 :Bool;
  #开始泊车
  flagStartParking @7 :Bool;
  #开始空间车位计算
  flagSpaceParkingCal @8 :Bool;

  ## 产品化
  #使用匿名地图开始功能
  flagUseAnonMap @9 :Bool;
  #重置系统，返回功能选择界面
  flagResetSystem @10 :Bool;
  flagResetSystemApa @11 :Bool;
  flagResetSystemHpp @12 :Bool;

  ## 产品化，测试用
  #删除匿名地图
  flagDeleteAnonMap @13 :Bool;
  flagShowFsMask @14 :Bool;
  #imp显示分割效果
  flagSegmentation @15 :Bool;
  #显示原ipm图
  flagDisplayOriginalIpm @16 :Bool;

  # for 3.0 debug
  flagHpaLoadDefaultMap @17 :Bool;

  # for hps
  flagHpaSummon @18 :Bool;
  flagHpsStudy @19 :Bool;
}

struct HMICommandResponse {
    ok @0 :Bool;
}
