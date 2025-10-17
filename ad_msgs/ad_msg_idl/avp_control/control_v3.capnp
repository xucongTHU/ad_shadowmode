@0x8aec57db6f2dfc07;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_control");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AvpStateV3Def = import "/ad_msg_idl/avp_state_v3_def/avp_state_v3_def.capnp";

using import "/ad_msg_idl/avp_control/control_debug.capnp".ControlDebug;
using import "/ad_msg_idl/ad_vehicle/vehicle.capnp".GearCommand;

struct ControlInnerWarning {
    blockInSteerDoing @0  :Bool;
    blockInStopDoing @1  :Bool;
}

struct ControlWarningOutput {
    # 车门打开
    doorOpened @0 :Bool;
    # 前盖打开
    frontLidOpened @1 :Bool;
    # 后盖打开
    rearLidOpened @2 :Bool;
    # 用户点击退出按钮
    abortWithUser @3 :Bool;
    # 用户干预
    abortWithUserTakeOver @4 :Bool;
    # 泊车过程中超速
    abortWithOverSpeed @5 :Bool;
    # ESC相关功能激活
    abortWithEscActive @6 :Bool;
    # 关联件错误
    abortWithAssociateError @7 :Bool;
    # 传感器错误
    abortWithSensorError @8 :Bool;
    # 硬件错误
    abortWithHardwareError @9 :Bool;
    # 软件错误
    abortWithSoftwareError @10 :Bool;
    # 控制误差过大
    abortWithControlError @11 :Bool;
    # 主驾安全带打开
    driverSeatBeltOpened @12 :Bool;
    # 后视镜折叠
    rearMirrorFold @13 :Bool;
    # 控制内部错误
    innerWarning @14 :ControlInnerWarning;
    # 充电枪链接状态
    chargeConnected @15 :Bool;
}

struct ControlStatus {
    # control模块初始化成功
    initSuccessed @0 :Bool;
    # control模块初始化失败
    initFailed @1 :Bool;
    # 线控握手成功
    handShakeSuccessed @2 :Bool;
    # 线控握手失败
    handShakeFailed @3 :Bool;
    # 控制模块触发暂停
    controlSuspend @4 :Bool;
    # 控制模块触发异常退出
    controlAbort @5 :Bool;
    # 控制正在行车
    controlDriving @6 :Bool;
    # 控制行车完成
    controlDrivingDone @7 :Bool;
    # 控制正在打方向盘
    controlStreering @8 :Bool;
    # 控制打方向盘完成
    controlStreeringDone @9 :Bool;
    # 控制正在停车
    controlStoping @10 :Bool;
    # 控制停车完成
    controlStopingDone @11 :Bool;
    # 车速 (m/s)
    vehicleSpeedMps @12 :Float32;
    # 车辆处于静止状态，根据车速判断，用于统一全局停车判断条件
    vehicleStationary @13 :Bool;
    # 用户是否踩着刹车
    pedalApplied @14 :Bool;
    # 当前档位
    actualGear @15 :GearCommand;
}

# topic: /parking/control/status_info
struct ControlStatusInfo {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    # msg id, update every frame
    msgId @1 :UInt64;

    # avp fsm status & uid
    avpStatus @2 :AvpStateV3Def.AvpMainStatus;
    avpStatusUid @3 :UInt64;

    # control status code
    controlStatus @4 :ControlStatus;

    # maybe?: control warnning code
    controlWarnning @5 :ControlWarningOutput;

    # control debug
    controlDebug @6 :ControlDebug;
}
