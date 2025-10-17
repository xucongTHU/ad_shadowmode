@0xe9df338d1236250a;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AvpStateV3Def = import "/ad_msg_idl/avp_state_v3_def/avp_state_v3_def.capnp";

struct FusionWarningOutput {
    # e.g. 模型加载成功
    loadModuleA @0 :Bool;
}

struct FusionStatus {
    # 模块初始化成功
    initSuccessed @0 :Bool;
    # 模块初始化失败
    initFailed @1 :Bool;
    # x ms 内接收到 OD Freespace 结果
    odReceived500ms @2 :Bool;

    # MAPPING_DONE状态下保存资源成功
    hpmFsSaveFail @3 :Bool = false;
    # MAPPING_DONE状态下保存资源失败
    hpmFsSaveSuccess @4 :Bool = false;

    # 初始化失败-配置文件路径不存在
    initConfigPathNotExist @5 :Bool;

    # 初始化失败-配置文件读取错误
    # 初始化失败-配置文件必要字段缺失
    # 初始化失败-相机模型初始化失败
    # 输入信号缺失-定位信号
    missLoc @6 :Bool;
    # 输入信号缺失-OD信号
    missOD @7 :Bool;
    # 输入信号缺失-FS信号
    missFS @8 :Bool;
    # 输入信号缺失-车位信号等
    missSlot @9 :Bool;
    # 输入信号异常-定位跳变
    locDrift @10 :Bool;
    # 输入信号异常-时间戳异常
}

enum IndoorOutdoor {
    unknown @0;
    indoor @1;
    outdoor @2;
}

# topic: /parking/fusion/status_info
struct FusionStatusInfo {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    # msg id, update every frame
    msgId @1 :UInt64;

    # avp fsm status & uid
    avpStatus @2 :AvpStateV3Def.AvpMainStatus;
    avpStatusUid @3 :UInt64;

    # status code
    fusionStatus @4 :FusionStatus;

    #  warnning code
    fusionWarning @5 :FusionWarningOutput;

    indoorOutdoor @6 :IndoorOutdoor;
}

struct OdWarningOutput {
    # 模块初始化成功
    initSuccessed @0 :Bool;

    # 模块初始化失败
    initFailed @1 :Bool;

#    # License过期
#    licenseExpired @0 :Bool;
#
#    # 模型读取失败
#    moduleLoadError @1 :Bool;
#
#    # 车辆配置文件路径不存在
#    configPathNotExist @2 :Bool;
#
#    # 车辆配置文件读取错误
#    configLoadError @3 :Bool;
#
#    # 相机无输入（或帧率低）
#    fisheyeLost @4 :Bool;
#
#    # 输入图像时间戳异常
#    fisheyeTimestampError @5 :Bool;
#
#    # 输入图像之间时间戳不同步
#    cameraTimesyncError @6 :Bool;
#
#    # 光照失效、表面遮挡
#    cameraExceptions @7 :Bool;
}

# topic: /parking/od/status_info
struct OdStatusInfo {
    header @0 :StdHeader.Header;

    odWarning @1 :OdWarningOutput;
}

struct RdWarningOutput {
    # 模块初始化成功
    initSuccessed @0 :Bool;

    # 模块初始化失败
    initFailed @1 :Bool;

#    # License过期
#    licenseExpired @0 :Bool;
#
#    # 模型读取失败
#    moduleLoadError @1 :Bool;
#
#    # 车辆配置文件路径不存在
#    configPathNotExist @2 :Bool;
#
#    # 车辆配置文件读取错误
#    configLoadError @3 :Bool;
#
#    # 相机无输入（或帧率低）
#    fisheyeLost @4 :Bool;
#
#    # 输入图像时间戳异常
#    fisheyeTimestampError @5 :Bool;
#
#    # 输入图像之间时间戳不同步
#    cameraTimesyncError @6 :Bool;
#
#    # 光照失效、表面遮挡
#    cameraExceptions @7 :Bool;
}

# topic: /parking/rd/status_info
struct RdStatusInfo {
    header @0 :StdHeader.Header;

    rdWarning @1 :RdWarningOutput;
}
