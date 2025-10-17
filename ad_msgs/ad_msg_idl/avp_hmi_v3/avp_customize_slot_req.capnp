@0xc2bb3b65f6974860;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_hmi_v3");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct AvpcustomizeSlotPoint {
    # IPM坐标系
    x @0 :Float64;
    y @1 :Float64;
}

struct AvpCustomizeSlotFrame {
  # 车位框四个角点坐标
  # 车头左前角点
  p0 @0 :AvpcustomizeSlotPoint;
  # 车头右前角点
  p1 @1 :AvpcustomizeSlotPoint;
  # 车尾右后角点
  p2 @2 :AvpcustomizeSlotPoint;
  # 车尾左后角点
  p3 @3 :AvpcustomizeSlotPoint;
}

struct AvpCustomizeSlot {
  # 请求 UUID
  reqId @0 :UInt64 = 0;
  # 用户自定义车位
  slotFrame @1 :AvpCustomizeSlotFrame;
  # AVP 内部使用的 id
  innerSlotId @2 :UInt64;
}

struct AvpCustomizeSlotReq {
  header @0 :StdMsgsHeader.Header;
  # 用户自定义车位
  customizeSlot @1 :AvpCustomizeSlot;
}

struct AvpCustomizeSlotRes {
    ok @0 :Bool;
}

enum AvpCustomizeSlotResultCode {
  none @0;
  ok @1;
  fail @2;
}

struct AvpCustomizeSlotJudgeResult {
  # 对应 struct AvpCustomizeSlot 请求中的 reqId
  reqId @0 :UInt64 = 0;
  # 回复
  result @1 :AvpCustomizeSlotResultCode;
}
