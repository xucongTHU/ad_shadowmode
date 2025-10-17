@0x91060c62e2cc340b;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::sensor");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::sensor");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

# 定义参考: https://ones.ainewera.com/wiki/#/team/Ttz6FJha/space/5wiUYxnF/page/L8oZnXuN

# topic: /sensor/uss/channel
# R->A
struct UssEchoOut {
    echoDistance @0 :UInt16;    # 回波距离
    shortDistance @1 :UInt16;   # 短距离
    echoHeight @2 :UInt16;      # 回波高度
    highPriority @3 :UInt8;     # 高优先级
    lowPriority @4 :UInt8;      # 低优先级
    radialSpeed @5 :Int32;      # 径向速度
    confirmNumber @6 :UInt8;    # 确认计数
    receiveTime @7 :UInt32;     # 接收时间
    confidence @8 :Float32;     # 置信度
}

struct UssChannelOut {
    echoOut @0 :List(UssEchoOut);  # 超声波回波数据组
    echoCount @1 :UInt8;           # 有效回波计数（0-5）
    transmitTime @2 :UInt32;       # 数据发送时间戳
    blindDistance @3 :UInt16;      # 盲区距离
    closeRangeFlag @4 :UInt8;      # 近距离标志位
    validFlag @5 :UInt8;           # 数据有效性标记
    sendType @6 :UInt8;            # 发波类型
    signalWayId @7 :UInt8;         # 信号ID
}

struct UssChannelOutput {
    channelData @0 :List(UssChannelOut);  # 通道数据
    frontStatus @1 :UInt8;                # 前部状态
    rearStatus @2 :UInt8;                 # 后部状态
}

# topic: /sensor/uss/debug
# R->A
struct UssLiveMessage {
    id @0 :UInt16;              # 消息ID
    target @1 :UInt16;          # 目标地址
    dataSize @2 :UInt32;        # 有效载荷数据长度 (≤1024)
    timestamp @3 :UInt64;       # 时间戳
    data @4 :List(UInt8);        # 二进制负载数据
}

# topic: /sensor/uss/driver_diag
# R->A

struct UssDiagDataReport {
    diagValue @0 :List(UInt8);
}
# topic: /parking/uss/threshold_setting
# A->R
struct UssSensorConfig {
    name @0 :Text;         # 传感器名称
    posX @1 :Int16;        # X坐标
    posY @2 :Int16;        # Y坐标
    posZ @3 :Int16;        # Z坐标
    direction @4 :Float32; # 方向
    viewRange @5 :UInt8;   # 视野范围
}

struct UssInternalConfig {
    internalF1F2 @0 :UInt32;  # 前部区间1-2
    internalF2F3 @1 :UInt32;  # 前部区间2-3
    internalF3F4 @2 :UInt32;  # 前部区间3-4
    internalR1R2 @3 :UInt32;  # 后部区间1-2
    internalR2R3 @4 :UInt32;  # 后部区间2-3
    internalR3R4 @5 :UInt32;  # 后部区间3-4
    majorVersion @6 :UInt8;   # 主版本号
    minorVersion @7 :UInt8;   # 次版本号
}

struct AmThreshold {
    stageInterval @0 :List(UInt8);  # 阶段间隔
    stageHeight @1 :List(UInt8);    # 阶段高度
}

struct AmThresholdData {
    tempApa @0 :Int16;          # APA温度
    dampApa @1 :UInt8;          # APA阻尼
    scaling @2 :UInt8;          # 缩放因子
    ussAmThreshold @3 :AmThreshold;  # 超声波幅度阈值
}

struct UssConfig {
    ussSensorConfig @0 :List(UssSensorConfig);  # 传感器配置列表
    ussInternalConfig @1 :UssInternalConfig;    # 内部配置
    validSegments @2 :UInt8;                    # 有效段数
    ussThresholdData @3 :List(AmThresholdData); # 阈值数据列表
}

# topic: /parking/pdc/pdc_info
# A->R
struct PdcOutput {
    timestampNs @0 :UInt64;
    sectorDis @1 :List(UInt16);
    sourceFlag @2 :List(UInt8);
}