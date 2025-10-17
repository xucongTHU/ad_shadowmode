@0xbed807e473154309;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_parking_cmd");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct ParkingCMD {
  # Header include seq, timestamp(last node pub time), and frame_id(sensor model)
  header @0 :StdMsgsHeader.Header;

  # The time of cmd sent, in nano second.
  timestampNs @1 :UInt64;

  # parking slot target id, uint64(-1) is implied if target is invalid
  # type should same as senseauto-msgs/avp_smart_slam_msgs/msg/ParkingSlot.msg:id
  # do not forget to change type together
  id @2 :UInt64 = 0xffffffffffffffff;
}

struct ParkingCMDResponse {
    ok @0 :Bool;
}

enum ParkingOutDirection {
  none @0;
  front @1;
  frontLeft @2;
  frontRight @3;
  left @4;
  right @5;
}

struct ParkingOutDirectionRequest {
  # Header include seq, timestamp(last node pub time), and frame_id(sensor model)
  header @0 :StdMsgsHeader.Header;

  # The time of cmd sent, in nano second.
  timestampNs @1 :UInt64;
  # 用户所选泊出方向(车头)
  direction @2 :ParkingOutDirection;

  # 用户所选泊出方向(车尾), direction 和 directionTail 必须且仅有一个为 none
  directionTail @3 :ParkingOutDirection;
}

struct ParkingOutDirectionResponse {
  ok @0 :Bool;
}

enum VehicleHeadingType {
  none @0;
  vehicleHead @1;
  vehicleTail @2;
}

struct ParkingInType {
  parkingInType @0 :VehicleHeadingType;
  recommendation @1 :Bool;
}

struct ParkingInDirection {
  reqId @0 :UInt64;

  # 该 List 不空时, 仅会有一项的 recommendation 为 true
  direction @1 :List(ParkingInType);
}

enum ParkingInAlignType {
  # 无效值
  none @0;
  # 居中
  center @1;
  # 靠左
  left @2;
  # 靠右
  right @3;
}

struct ParkingInAlignItem {
  parkingInAlign @0 :ParkingInAlignType;
  active @1 :Bool;
}

struct ParkingInAlignGroup {
  reqId @0 :UInt64;

  # 该 List 不空时, 仅会有一项的 active 为 true
  aligns @1 :List(ParkingInAlignItem);
}
