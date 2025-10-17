@0xe4d63f14f995d7df;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::vehicle");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AvpMainFuncTypeDef = import "/ad_msg_idl/avp_state_v3/uic_fsm_cmd.capnp";
using AvpMainStatusDef = import "/ad_msg_idl/avp_state_v3_def/avp_state_v3_def.capnp";


struct TimestampHist {
    # the time when odomstate_info topic was received
    odoTime @0 :UInt64;
     
    # the time when veh_report topic was received
    vehReportTime @1 :UInt64;
    
    # the time when trajectory topic was received
    trajectoryTime @2 :UInt64;
    
    # the time when system_state topic was received
    systemStateTime @3 :UInt64;
}


struct LongStopDriveOff {
    # Main ADC LongitudinalControl requires driving off,  0x1 for request, 0x0 for no request
    longCtrlDriveOff @0 :Bool;
    
    # Longitudinal Control requires deceleration to stop, 0x1 for request, 0x0 for no request
    longCtrlDecToStopReq @1 :Bool;
}

struct PlanningState {
    # dash board display requires 
    vehicleState @0 :UInt32;
    
    # dash board display requires 
    selectedSpeed @1 :UInt32;
}

struct Beam {
    # Brake
    stopLamp @0 :Bool;

    # Turn signal
    turnLeftLamp @1 :Bool;
    turnRightLamp @2 :Bool;

    # 双闪灯
    harzardLamp @3 :Bool;

    # 近光灯
    nearLamp @4 :UInt8;

    # Misc
    reservedLamp @5 :List(Bool);
}

struct BrakeCmd {
    # Brake command
    command @0 :Float32;
    enum CmdType {
        cmdNone @0;
        cmdPedal @1;   # Unitless, mapping to pedal raw
        cmdPercent @2; # Percent of maximum torque, range 0 to 1
        cmdTorque @3;  # Nm
        cmdDecel @4;   # m/s^2
    }
    commandType @1 :CmdType;
    ignore @2 :Bool;

    # Parking Brake (0:release 1:lock)
    parkingBrake @3 :UInt32;

    # brake mode
    enum BrakeMode {
        # 不请求刹车
        idle @0;
        # 请求缓慢刹车(APA发送电刹+机械刹车)
        apaComfortable @1;
        # 请求缓慢刹车(LAPA发送-仅电刹车)
        lApaComfortable @2;
        # 请求急刹(APA/LAPA发送电刹+机械刹车)
        emergency @3;
    }
    brakeMode @4 :BrakeMode;
}

struct BrakeReport {
    # Brake pedal raw
    pedalInput @0 :Float32;
    pedalCommand @1 :Float32;
    pedalActual @2 :Float32;

    # Brake percent (0-1)
    percentCommand @3 :Float32;
    percentActual @4 :Float32;

    # Brake torque (Nm)
    torqueInput @5 :Float32;
    torqueCommand @6 :Float32;
    torqueActual @7 :Float32;

    # Brake decel (m/s^2)
    decelCommand @8 :Float32;
    decelActual @9 :Float32;

    # Wheel torques from sensor (Nm)
    sensorBrakeTorque @10 :Float32;
    sensorWheelTorque @11 :Float32;

    # Parking Brake (0:release 1:lock)
    parkingBrakeActual @12 :Bool;

    # Status
    busFault @13 :Bool;

    decelCommandValidity @14 :Bool;
    decelActualValidity @15 :Bool;
    remotePackingResp @16 :UInt8;
    assistBrakingStatus @17 :UInt8;

    accBrkngReqDenied @18 :Bool;
}

struct Chassis {
    # Vehicle Speed (m/s)
    vehicleMps @0 :Float32;

    # Wheel Speed (m/s)
    wheelSpeedFl @1 :Float32;
    wheelSpeedFr @2 :Float32;
    wheelSpeedRl @3 :Float32;
    wheelSpeedRr @4 :Float32;

    # Encoder (Wheel Pulse)
    wheelPulseFl @5 :Int32;
    wheelPulseFr @6 :Int32;
    wheelPulseRl @7 :Int32;
    wheelPulseRr @8 :Int32;

    # Real tire pressure (kPa)
    tirePressureFl @9 :Float32;
    tirePressureFr @10 :Float32;
    tirePressureRl @11 :Float32;
    tirePressureRr @12 :Float32;

    # Real suspension height (mm)
    suspensionFl @13 :Float32;
    suspensionFr @14 :Float32;
    suspensionRl @15 :Float32;
    suspensionRr @16 :Float32;

    # Real fuel level in percentage (0-100)
    fuelLevel @17 :Float32;

    # Wheel pulse
    wheelSpeedFlD @18 :UInt16;
    wheelSpeedFrD @19 :UInt16;
    wheelSpeedRlD @20 :UInt16;
    wheelSpeedRrD @21 :UInt16;

    #Wheel Angular Velocity
    frontWheelLav @22 :Float32;
    frontWheelRav @23 :Float32;
    rearWheelLav @24 :Float32;
    rearWheelRav @25 :Float32;

    #Hood Status 
    hoodStatus @26 :UInt8;

    sysPwrMode @27 :UInt8;

    passengerSeatbelt  @28 :Bool;
    driverSeatbelt  @29:Bool;
    standStill @30 :Bool;
}

enum GearCommand {
    gearNone @0;
    gearPark @1;
    gearReverse @2;
    gearNeutral @3;
    gearDrive @4;
    gearLow @5;
}

struct GearCmd {
    command @0 :GearCommand;
}

struct ApaCmd {
    # 0X190
    parkStopDist @0 : UInt16;
    parkMaxVel @1 : Float32;
}


struct GearReport {
    # Gear mode
    command @0 :GearCommand;
    actual @1 :GearCommand;

    # Status
    busFault @2 :Bool;

    #man intervene flag
    manIntervene @3 :Bool;
}

struct Imu {
    # Vheicle orginal 3-axis Accel (m/s^2)
    accelX @0 :Float32;
    accelY @1 :Float32;
    accelZ @2 :Float32;

    # Vheicle orginal 3-axis Gyro (rad/s)
    gyroX @3 :Float32;
    gyroY @4 :Float32;
    gyroZ @5 :Float32;

    # GL8 acc Accel (m/s^2)
    accAccelX @6 :Float32;
}

struct Misc {
    # Door
    doorFl @0 :Bool;
    doorFr @1 :Bool;
    doorRl @2 :Bool;
    doorRr @3 :Bool;
    doorHood @4 :Bool;
    doorTrunk @5 :Bool;

    # Button
    btnAccOnoff @6 :Bool;
    btnAccCancel @7 :Bool;
    btnAccResume @8 :Bool;
    btnAccSet @9 :Bool;
    btnAccDisPls @10 :Bool;
    btnAccDisMns @11 :Bool;
    btnAccLkaen  @12 :Bool;
    btnApa  @13 :Bool;
    btnAccReady  @14  :Bool;

    driverSeatBeltst @15 :UInt8;
    leftMirrorFoldUnfoldSt @16 :UInt8;
    rightMirrorFoldUnfoldSt @17 :UInt8;

    # Reserved
    reserved @18 :List(Bool);


}


struct ModeCmd {
    enum Gl8Mode {
        none @0;
        apa @1;
        acc @2;
    }
    # Master switch, enable all vehicle domain
    enable @0 :Bool;
    motionMode @1 :UInt32;
    controlMode @2 :Gl8Mode;
}

enum MotionMode {
    ackermannMode @0;
    skewMode @1;
    spinMode @2;
    lateralMode @3;
    none @4;
}

struct ModeReport {
    # Exit automode will set to false, such as driver override, system fault,
    # etc.
    enable @0 :Bool;

    # Domain status
    throttleEnable @1 :Bool;
    brakeEnable @2 :Bool;
    steeringEnable @3 :Bool;
    gearEnable @4 :Bool;

    # motion mode
    motionMode @5 :MotionMode;
}

struct Navi {
    # Vheicle orginal GPS WGS84
    latitude @0 :Float64;
    longitude @1 :Float64;
    altitude @2 :Float64;

    # NMEA
    nmeaGga @3 :Text;
}

struct Sonar {
    # Sonar (m)
    array @0 :List(Float32);
}

struct SteeringCmd {
    enum CmdType {
        cmdNone @0;
        cmdAngle @1; # clockwise means negative
        cmdTorque @2;
    }
    # Steering commands
    command @0 :Float32;
    commandType @1 :CmdType;

    velocity @2 :Float32; # deg/s
}

struct SteeringReport {
    # Steering angle (deg)
    angleCommand @0 :Float32;
    angleActual @1 :Float32;

    # Steering torque (Nm)
    torqueCommand @2 :Float32;
    torqueActual @3 :Float32;

    # Status
    busFault @4 :Bool;

    # angle rot
    angleRotspd @5 :UInt32;

    enum ManInterveneFlag {
        manInterveneTrue @0;
        manInterveneFalse @1;
    }
    #man intervene flag
    manIntervene @6 :ManInterveneFlag;
    accEps @7 :UInt8;
    manInterveneTorque @8 :UInt32;
}

struct ThrottleCmd {
    enum CmdType {
        cmdNone @0;
        cmdPedal @1;         # Unitless, mapping to pedal raw
        cmdPercent @2;       # Percent of maximum throttle, range 0 to 1
        cmdSpeed @3;         # m/s
        cmdSpeedLateral @4;  # m/s lateral speed
        cmdTorque @5;        # Nm
    }
    # Throttle command
    command @0 :Float32;
    commandType @1 :CmdType;
    ignore @2 :Bool;
}

struct ThrottleReport {
    # Throttle pedal raw
    pedalInput @0 :Float32;
    pedalCommand @1 :Float32;
    pedalActual @2 :Float32;

    # Throttle percent (0-1)
    percentCommand @3 :Float32;
    percentActual @4 :Float32;

    # Engine (rpm)
    engineRpm @5 :Float32;

    # Status
    busFault @6 :Bool;

    # GL8
    accEcm  @7 :UInt8;
    torqueCommand @8 :Float32;
    torqueActual @9 :Float32;

    # override
    accDriverOrvd @10 :Bool;
}

struct AccCmd {
     enum CmdType { 
         cmdNone @0;
         cmdMs2 @1;
         cmdPercent @2; 
     }
    #accelerate command, maximum=5, minimum=-7
    commandValue @0 :Float32;
    #comand type
    commandType @1 :CmdType;
}

struct AccReport {
    #vehicle actual acc control state
    enum ActualState {
        stateActualIn @0;
        stateActualNo @1;
    }
    stateActual @0 :ActualState;
    #vehicle actual accelerate value, minimum=-7 m/s2, maximum=5 m/s2
    valueActual @1 :Float32;

    #vehicle actual accelerate pedal position, minimum = 0%, maximum = 100%
    pedalPosition @2 :Float32;
    enum ManInterveneFlag {
        manInterveneTrue @0;
        manInterveneFalse @1;
    }
    #man intervene flag
    manIntervene @3 :ManInterveneFlag;
}

struct EpbCmd {
    enum EpbReleasedCmd {
        epbCmdReleased @0;
        epbCmdNotReleased @1;
    }
    commandValue @0 :EpbReleasedCmd;
}

struct EpbReport {
    enum EpbReleasedState {
        epbStateReleased @0;
        epbStateNotReleased @1;
        epbStateFault @2;
    }
    stateActual @0 :EpbReleasedState;
    epbIntervener @1 :Bool;
}

struct BodyReport {
    enum ChargeState {
        notCharged @0;
        slow @1;
        fast @2;
        fault @3;
    }

    stateFlags @0 :UInt64;
    sunshineIntensity @1 :UInt32;
    lightSignal @2 :UInt32;
    chargeState @3 :ChargeState;
}


struct VehicleCmd {
    # Header include seq, timestamp(last node pub time), and frame_id(ignored)
    header @0 :StdHeader.Header;

    # Autodrive mode command
    mode @1 :ModeCmd;
    # if 'VehicleReport.enable' report disable, canbus will ignore below

    # Drive-by-wire units
    throttle @2 :ThrottleCmd;
    brake @3 :BrakeCmd;
    steering @4 :SteeringCmd;
    gear @5 :GearCmd;

    # Assist devices
    beam @6 :Beam;

    # time stamp hisoty, to be used for latency time evaluation
    timeStampHist @7 :TimestampHist;

    # to send out the longitudinal cmd for decelerate to stop and driveoff 
    longStopDriveOff @8 :LongStopDriveOff;

    # to send out the vehicleState and selectedSpeed for dash board display 
    planningState @9 :PlanningState;    
    acc @10 :AccCmd;
    epb @11 :EpbCmd;    
    apa @12 :ApaCmd;

    # 外后视镜折叠指令
        # 0 : 无请求
        # 1 : 折叠
        # 2 : 打开
        # 3 : 无效值
    apaRearMirrorFoldReq @13 :UInt8;

    # current running func type
    funcType @14 :AvpMainFuncTypeDef.AvpMainFuncType;
    # current running status
    mainStatus @15 :AvpMainStatusDef.AvpMainStatus;

    # 智能泊车功能开启反馈
    apaOnOffDisp @16 :Bool;

    # 记忆泊车功能开启反馈
    lapaFunModeReq @17 :Bool;

    # 循迹倒车功能开启反馈
    apaTRActiveSt @18 :Bool;

    # 泊车语音播报模式反馈
        # 0 : 无输入
        # 1 : 标准
        # 2 : 简洁
        # 3 : 预留
    apaVoiceModeSt @19 :UInt8;

    # 智能泊车车速设置反馈
        # 0 : 无输入
        # 1 : 较慢
        # 2 : 适中
        # 3 : 较快
    apaSpeedModeSt @20 :UInt8;

    # 记忆泊车线路推荐反馈
        # 0 : 无输入
        # 1 : 关闭
        # 2 : 打开
        # 3 : 预留
    apaSuggestSwSt @21 :UInt8;

    # 泊入位置设置反馈
        # 0 : 无输入
        # 1 : 关闭
        # 2 : 打开
        # 3 : 预留
    apaSetSwSt @22 :UInt8;

    # 智能泊车功能指示灯设置反馈
        # 0 : 熄灭
        # 1 : 灰色
        # 2 : 蓝色
        # 3 : 红色
        # 4 : 黄色
    apaStIconDisp @23 :UInt8;

    # 记忆泊车功能指示灯设置反馈
        # 0 : 熄灭
        # 1 : 灰色
        # 2 : 蓝色
        # 3 : 红色
        # 4 : 黄色
    lapaStIconDisp @24 :UInt8;

    # 循迹倒车功能指示灯设置反馈
        # 0 : 熄灭
        # 1 : 灰色
        # 2 : 蓝色
        # 3 : 红色
        # 4 : 黄色
    traStIconDisp @25 :UInt8;

    # 记忆泊车巡航最大车速显示
    lapaSetSpeed @26 :UInt8;

    # 记忆泊车设定车速状态显示
        # 0 : 不显示
        # 1 : 带有删除线的白色数值
        # 2 : 没有删除线的白色数值
        # 3 : 仅显示短虚线，不显示数值
        # 4 : 绿色数值闪烁
        # 5 : 没有删除线的绿色数值
    lapaSetSpdDisp @27 :UInt8;

    # 智能泊车文言提醒
    apaAutoParkProcessRem @28 :UInt8;

    # 记忆泊车文言提醒
    lapaTextRem @29 :UInt8;

    # 循迹倒车文言提醒
    apaTRRem @30 :UInt8;
}

struct VehicleReport {
    # Header include seq, timestamp, and frame_id(vehicle_type)
    header @0 :StdHeader.Header;

    # Autodrive mode report
    mode @1 :ModeReport;

    # Drive-by-wire units report
    throttle @2 :ThrottleReport;
    brake @3 :BrakeReport;
    steering @4 :SteeringReport;
    gear @5 :GearReport;

    # Assist devices report
    chassis @6 :Chassis;
    beam @7 :Beam;
    imu @8 :Imu;
    navi @9 :Navi;
    sonar @10 :Sonar;
    misc @11 :Misc;
    acc @12 :AccReport;
    epb @13 :EpbReport;

    # Vehicle body report
    body @14 :BodyReport;
}

struct RpaVehicleCmd {
    # Header include seq, timestamp(last node pub time), and frame_id(ignored)
    header @0 :StdHeader.Header;

    # 外后视镜折叠指令
        # 0 : 无请求
        # 1 : 折叠
        # 2 : 打开
        # 3 : 无效值
    rpaRearMirrorFoldReq @1 :UInt8;

    # RPA文言提示
    rpaNoticeDisp @2 :UInt8;

    # RPA进度条
    rpaPercentage @3 :UInt8;

    # RPA自检状态
    rpaStFeedbackSt @4 :Bool;

    # 离车泊入是否可用
    rpaLeaveParkStatus @5 :Bool;

    # 直进支持是否可用
    rpaStraightReadyFbSt @6 :Bool;

    # 遥控泊入/泊出是否可用
    rpaReadyFbSt @7 :Bool;

    # 遥控车位类型
        # 0 : 无效值
        # 1 : 平行车位
        # 2 : 垂直车位
        # 3 : 斜列车位
    rpaPrkgslotType @8 :UInt8;

    # 遥控指令状态
        # 0 : 无遥控连接
        # 1 : 手机APP连接
        # 2 : 钥匙RKE连接
    rpaRemCtrlSts @9 :UInt8;

    # 遥控泊车关天窗
    rpaSunroofReq @10 :Bool;

    # 遥控泊车关车窗
    rpaWinReq @11 :Bool;

    # 遥控解锁四门
        # 0 : 无请求
        # 1 : 落锁
        # 2 : 解锁
        # 3 : 无效值
    rpaDoorloclReq @12 :UInt8;

    # 请求开启/关闭发动机
        # 0 : 无请求
        # 1 : 请求开启发动机
        # 2 : 请求关闭发动机
        # 3 : 请求关闭发动机并落锁
    rpaPEPSReq @13 :UInt8;

    # 左后垂直方向可用状态
    rpaRearLeftVerticalDirectionSt @14 :Bool;

    # 右后垂直方向可用状态
    rpaRearRightVerticalDirectionSt @15 :Bool;

    # 左前垂直方向可用状态
    rpaFrontLeftVerticalDirectionSt @16 :Bool;

    # 右前垂直方向可用状态
    rpaFrontRightVerticalDirectionSt @17 :Bool;

    # 左前水平方向可用状态
    rpaFrontLeftParrallelDirectionSt @18 :Bool;

    # 右前水平方向可用状态
    rpaFrontRightParrallelDirectionSt @19 :Bool;

    # 前向直出可用状态
    rpaFrontStraightDirectionSt @20 :Bool;

    # 后向直出可用状态
    rpaRearStraightDirectionSt @21 :Bool;

    # 推荐泊出方向
        # 0 : 无推荐
        # 1 : 左前垂直
        # 2 : 左前水平
        # 3 : 前向直出
        # 4 : 右前垂直
        # 5 : 右前水平
        # 6 : 后向直出
        # 7 : 左后垂直
        # 8 : 右后垂直
    rpaSuggestBestDirSt @22 :UInt8;

    # 蓝牙短按泊车按键显示状态
        # 0 : 开始
        # 1 : 暂停
        # 2 : 暂停(置灰)
        # 3 : 继续
    rpaParkButtonDispSt @23 :UInt8;
}

struct ParkingHmiReport {
    # Header include seq, timestamp, and frame_id(vehicle_type)
    header @0 :StdHeader.Header;

    # 智能泊车开关信号
        # 0 : 无请求
        # 1 : 请求进入APA
        # 2 : 请求退出APA
        # 3 : 预留
    iviApaSwReq @1 :UInt8;

    # 记忆泊车开关信号
        # 0 : 无请求
        # 1 : 请求进入HPP
        # 2 : 请求退出HPP
        # 3 : 预留
    iviApaSwitchAppReq @2 :UInt8;

    # 循迹倒车开关信号
        # 0 : 无请求
        # 1 : 请求进入TR
        # 2 : 请求退出TR
        # 3 : 预留
    iviTrSwReq @3 :UInt8;

    # 泊车语音播报模式
        # 0 : 无输入
        # 1 : 标准
        # 2 : 简洁
        # 3 : 预留
    iviApaVoiceModeReq @4 :UInt8;

    # 智能泊车车速设置
        # 0 : 无输入
        # 1 : 较慢
        # 2 : 适中
        # 3 : 较快
    iviApaSpeedModeReq @5 :UInt8;

    # 记忆泊车路线推荐开关信号
        # 0 : 无输入
        # 1 : 关闭
        # 2 : 打开
        # 3 : 预留
    iviApaSuggestSwReq @6 :UInt8;

    # 泊入位置设置开关信号
        # 0 : 无输入
        # 1 : 关闭
        # 2 : 打开
        # 3 : 预留
    iviApaSetSwReq @7 :UInt8;

    # APA用户授权考试通过信号
        # 0 : 无输入
        # 1 : 通过
        # 2 : 未通过
        # 3 : 预留
    iviApaUserExamStatus @8 :UInt8;

    # HPA用户授权考试通过信号
        # 0 : 无输入
        # 1 : 通过
        # 2 : 未通过
        # 3 : 预留
    iviHpaUserExamStatus @9 :UInt8;

    # 自定义泊车开关
        # 0 : 无输入
        # 1 : 打开
        # 2 : 关闭
        # 3 : 预留
    iviFreeParkSwReq @10 :UInt8;

    # 记忆泊车开关
        # 0 : 无输入
        # 1 : 关闭
        # 2 : 打开
        # 3 : 预留
    iviLapaSetSwReq @11 :UInt8;

    # 静默建图开关
        # 0 : 无输入
        # 1 : 关闭
        # 2 : 打开
        # 3 : 预留
    iviSilentMappingSetSwReq @12 :UInt8;

    # 语音调节巡航车速开关
        # 0 : 默认
        # 1 : 慢
        # 2 : 标准
        # 3 : 快
    iviVoiceAdjCruiseVehSpdReq @13 :UInt8;
}

# 线控错误码
enum WireErrorCode {
    # 无错误
    none @0;

    # 用户点击退出按钮
    userSelectExit @1;

    # 用户干预epb
    userTakeOverEPB @2;
    # 用户干预油门
    userTakeOverThrottle @3;
    # 用户干预刹车
    userTakeOverBrake @4;
    # 用户干预方向盘
    userTakeOverSteering @5;
    # 用户干预档位
    userTakeOverGear @6;

    # 车门打开
    doorOpened @7;
    # 前盖打开
    frontLidOpened @8;
    # 后盖打开
    rearLidOpened @9;
    # 主驾安全带打开
    driverSeatBeltOpened @10;
    # 后视镜折叠
    rearMirrorFold @11;

    # ESC相关功能激活
    escActive @12;
    # 关联件错误
    associateError @13;
    # 传感器错误
    sensorError @14;
    # 硬件错误
    hardwareError @15;
    # 软件错误
    hoftwareError @16;

    # RPA遥控信号异常
    isAbortRPASignalLost @17;

    # 控制模块触发异常退出
    controlAbort @18;
    # 控制误差过大
    controlError @19;
    # 超速
    overSpeed @20;

    # MEB触发
    mebCollision @21;
    # 其他错误1
    otherError1 @22;
    # 其他错误2
    otherError2 @23;
}

# 线控状态码
enum WireStatus {
    # 未执行握手
    init @0;
    # 申请握手过程中
    isConnectingWire @1;
    # 在线控, 执行指令中
    isInWire @2;
    # 在线控, 暂停执行指令中
    isPending @3;
    # 线控异常退出
    isAbort @4;
}

# 线控状态
struct WireStatusReport {
    # 线控状态
    wireStatus @0 :WireStatus;
    # 错误码
    errorCode @1 :WireErrorCode;
}
