@0xb175832f1ac71b5c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_state_v3");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AvpStateV3Def = import "/ad_msg_idl/avp_state_v3_def/avp_state_v3_def.capnp";

using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingCMD;
using import "/ad_msg_idl/avp_parking_cmd/hmi_command.capnp".HMICommand;
using import "/ad_msg_idl/avp_hmi_v3/avp_hmi_req.capnp".AvpHmiReq;
using import "/ad_msg_idl/avp_hmi_v3/avp_hmi_req.capnp".AvpHmiButton;
using import "/ad_msg_idl/avp_hmi_v3/avp_map.capnp".AvpMapList;
using import "/ad_msg_idl/avp_hmi_v3/avp_map.capnp".AvpMapItem;
using import "/ad_msg_idl/avp_hmi_v3/avp_customize_slot_req.capnp".AvpCustomizeSlot;
using import "/ad_msg_idl/avp_hmi_v3/avp_customize_slot_req.capnp".AvpCustomizeSlotJudgeResult;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingOutDirection;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingOutDirectionRequest;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingInDirection;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingInAlignGroup;

enum AvpMainFuncType {
    none @0;
    # 泊车
    apa @1;
    # 建图循迹学习路线
    hpm @2;
    # 循迹
    hpa @3;
    # 建图召唤学习路线
    hsm @4;
    # 召唤
    hps @5;
    # 倒车循迹
    ra @6;
}


struct SlamMappingParam {
    latitude @0 : Float64;
    longitude @1 : Float64;
    height @2 : Float64;
}

struct SlamLocalParam {
    id @0 : UInt64;
    isGnssMatch @1 :  Bool;
}

struct SlamDummyParam {
}

struct SlamRequest {
    id @0 : UInt64;

    param :union {
        mapping @1 :SlamMappingParam;
        localization @2 :SlamLocalParam;
        reset @3 :SlamDummyParam;
    }
}

# topic: /parking/uic_fsm/cmd
struct UicFsmCmd {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdMsgsHeader.Header;

    # state uid, update when status is updated
    uid @1 :UInt64;

    # avp status
    avpStatus @2 :AvpStateV3Def.AvpMainStatus;

    # parking cmd
    avpParkingCmd @3 :ParkingCMD;

    # hmi cmd
    avpHmiCmd @4 :HMICommand;

    # hmi request
    avpHmiReq @5 :AvpHmiReq;

    # current running func type
    avpMainFuncType @6 :AvpMainFuncType;

    # parking out direction
    parkingOutDirection @7 :ParkingOutDirectionRequest;

    slamRequest @8 : SlamRequest;
    resetId @9 : UInt64;

    # 用户自定义车位, 注意，车位ID取值范围[2000, 2998], 当ID为2999或为空时, 标识取消自定义车位
    customizeSlot @10 :AvpCustomizeSlot;
}


struct AvpHmiButtonItem {
    buttonId @0 :AvpHmiButton;
    text @1 : Text;
    visible @2 :Bool;
    enabled @3 :Bool;
    hints @4 : Text;
}

struct AvpHmiTip {
    # 弹窗显示的文本
    # text为空时, 立即消去弹窗
    text @0 :Text;
    # timeout弹窗显示倒计时, 单位ms, 每次发送时需要减去已流失的时间
    # timeout > 0时, HMI侧以此时间更新本地的显示倒计时时间;
    # timeout = -1时, 长时显示;
    timeout @1 :Int64;
    level @2 :Text;
    buttons @3 :List(AvpHmiButtonItem);
}

# 使能/失能 AVP泊车域的结果
struct SwitchAvpResult {
    # 对应 struct AvpHmiReq 请求中的 id
    reqId @0 :UInt64 = 0;
    # 使能结果
        # 0: 无效值
        # 1: 切换成功
        # 2: 切换失败
    result @1 :UInt8 = 0;
    # 提示文言
    text @2 :Text;
}

struct UicVisualizerInfo {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdMsgsHeader.Header;

    # state uid, update when status is updated
    uid @1 :UInt64;

    # avp status
    avpStatus @2 :AvpStateV3Def.AvpMainStatus;

    # tips text
    hmiText @3 :Text;

    # plan debug message
    planningStatusText @4 :Text;

    # control debug message
    controlStatusText @5 :Text;

    # slam debug message
    slamStatusText @6 :Text;

    # buttons lists for buttons enable
    buttons @7 :List(AvpHmiButtonItem);

    # parking out direction buttons
    # 推荐泊出方向(车头)
    recommendationDirections @8 :List(ParkingOutDirection);

    # selected parking out direction
    selectedDirection @9: ParkingOutDirection;

    # current running func type
    avpMainFuncType @10 :AvpMainFuncType;

    # 弹窗提示信息
    # 如果弹窗字段未填充, 则HMI侧继续显示前次弹窗信息
    avpTip @11: AvpHmiTip;

    # 地图管理信息
    avpMaps @12 :AvpMapList;

    # 用户新建成地图
    newMapInfo @13: AvpMapItem;

    # 推荐泊出方向(车尾),  recommendationDirections 和 recommendationDirectionsTail 同一时刻最多有且只有一个不为空
    recommendationDirectionsTail @14 :List(ParkingOutDirection);

    # 推荐泊入方向
    recommendationParkingInDirections @15 :ParkingInDirection;

    customizeSlotJudgeResult @16 :AvpCustomizeSlotJudgeResult;

    # selected parking out direction tail
    selectedDirectionTail @17: ParkingOutDirection;

    # 泊车入库位姿选项集
    parkingInAlignSet @18 :ParkingInAlignGroup;
}

# topic: /avp_state_machine/avp_switch_mode_result
struct AvpSwitchModeResult {
    header @0 :StdMsgsHeader.Header;
    uid @1 :UInt64;
    # 当前状态
    state @2 :UInt8;
    # 行泊切换结果
        # 0: invalid
        # 1: 切换成功
        # 2: 切换失败
    switchState @3 :UInt8;
}

