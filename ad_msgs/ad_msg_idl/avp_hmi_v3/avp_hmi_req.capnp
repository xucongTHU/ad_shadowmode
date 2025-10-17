@0x80e9b00ddd753564;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_hmi_v3");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_hmi_v3");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

enum AvpHmiButton {
  # 注意: 枚举项只能新增,不能删除,不能更改顺序
  # 无效值
  none @0;

  # 功能暂停
  avpSuspend @1;
  # 功能继续
  avpContinue @2;
  # 取消(退出)当前功能
  avpQuit @3;

  # 点击APA选项卡
  apaSelect @4;
  # 开始泊入
  apaStartParkingIn @5;
  # 开始泊出
  apaStartParkingOut @6;

  # 点击HPP选项卡
  hppSelect @7;
  # 开始记忆泊车
  hppStartCruise @8;
  # 开始召唤
  hppStartHps @9;
  # 开始Hpa路线学习
  hppStartLearn @10;
  # 保存地图(完成路线学习)
  hppSave @11;
  # hpp选择加载地图
  hppSelectMap @12;
  # hpp删除地图
  hppDeleteMap @13;
  # 开始沿途泊车
  hppAlongParkingStart @14;
  # 开始Hps路线学习
  hppStartLearnSummon @15;

  # 弹窗上不需要回复的按钮, 即用户点击后不需要通知给UIC侧
  # 确定
  tipWithoutRespOk @16;

  # 记忆泊车完成后提示更换默认巡航终点弹窗按钮
  # 确定
  tipHppFinishChangeTargetSlotOk @17;
  # 取消
  tipHppFinishChangeTargetSlotCancel @18;

  # 进入路线管理按钮
  mapManagerEnter @19;
  # 进入地图预览按钮
  mapPreviewEnter @20;

  # 开始倒车(RA Control)
  raStartReverse @21;

  # 切換泊入方向, 用戶點擊該按鈕後，需要發送req id至UIC, 通過req id來判斷是否接收到UIC的應答
  apaSwitchParkingInDirection @22;

  # 进入自定义车位功能
  enterCustomizeSlot @23;

  # 泊车入库位姿选择按钮, 用戶點擊該按鈕後，需要發送req id至UIC, 通過req id來判斷是否接收到UIC的應答
  # value 参考 ParkingInAlignType 定义
  apaSelectParkingInAlignType @24;

  # 使能循迹倒车
  raSelect @25;

  # 使能离车泊入
  leaveParking @26;

  # 使能泊车域
  enableAvp @27;
  # 失能泊车域
  disableAvp @28;
}

struct AvpHmiReq {
  header @0 :StdMsgsHeader.Header;
  # UUID, 切换车头/车尾泊入/使能泊车域/失能泊车域功能需要用到, 来标识请求与 pub 的回复
  id @1 :UInt64 = 0;
  # the button which is clicked
  button @2 :AvpHmiButton = none;
  # the button value for targetslot/hppselect... (default: 0)
  value @3 :Int64 = 0;
}

struct AvpHmiRes {
    ok @0 :Bool;
}
