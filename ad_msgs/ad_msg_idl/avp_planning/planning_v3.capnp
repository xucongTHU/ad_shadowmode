@0xb5220b972a2b50e9;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_planning");

using PlanningV2 = import "/ad_msg_idl/avp_planning/planning.capnp";
using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using AvpStateV3Def = import "/ad_msg_idl/avp_state_v3_def/avp_state_v3_def.capnp";

using import "/ad_msg_idl/avp_fusion_map/avp_fusion_map.capnp".FusionSlotOutArray;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingCMD;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingOutDirection;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingInDirection;
using import "/ad_msg_idl/avp_parking_cmd/parking_cmd.capnp".ParkingInAlignGroup;

struct PlanningWarnningRouteMapping {
    unavailableInput @0  :Bool;
    routeFail @1  :Bool;
    preRouteFail @2  :Bool;
    dataSaveFail @3  :Bool;
    processFail @4  :Bool;
    invalidFreespaceSource @5  :Bool;
    invalidVehicleposeSource @6  :Bool;
    invalidSematicmapSource @7  :Bool;
    failEndslot @8  :Bool;
    timeout @9  :Bool;
}

struct PlanningNodeWarning {
    makeApaTaskFailed @0  :Bool;
    makeHppTaskFailed @1  :Bool;
}

struct PlanningHpaWarning {
    planFailedForCollision @0  :Bool;
    planFailedForTrajLenTooShort @1  :Bool;
}

struct PlanningApaWarning {
    planFailedForLong @0  :Bool;
    planFailedForLAT @1  :Bool;
}

struct PlanningRaRouteInfo {
    mileage @0  :Int64;
}

struct PlanningWarningOutput {
    # 规划路径上有障碍物
    obstaclesInPath @0 :Bool;
    # 泊车把数过多
    abortWithGearShiftOverTimes @1 :Bool;
    # 泊车时间过长
    abortWithParkingTimeTooLong @2 :Bool;
    # 学习时间过长
    abortWithLearningTimeTooLong @3 :Bool;
    # 学习距离过长
    abortWithLearningDistanceTooLong @4 :Bool;
    # 倒挡距离过长
    abortWithReverseDistanceTooLong @5 :Bool;
    # 循迹总时长过长
    cruiseTotalTimeTooLong @6 :Bool;
    # 循迹无可泊车位
    cruiseNoAvailableSlot @7 :Bool;
    # RouteMap 计算和存储异常
    routeMappingWarnning @8 :PlanningWarnningRouteMapping;
    # node层错误信息提示
    nodeWarnning @9 :PlanningNodeWarning;
    # HPA错误信息提示
    hpaWarnning @10 :PlanningHpaWarning;
    # APA错误信息提示
    apaWarnning @11 :PlanningApaWarning;
}

# 规划结果
enum AvpPlanFeedBack {
    # 计算中
    none @0;
    # 计算完成，结果是成功
    ok @1;
    # 计算完成，结果是失败
    fail @2;
    # 计算暂停
    pause @3;
    # 紧急刹车
    emergencyBrake @4;
    # 紧急刹车解除
    emergencyBrakeRelease @5;
    # 泊车结束
    trajFinish @6;
    # 匹配轨迹成功
    routeTracking @7;
    # 匹配轨迹失败
    routeTrackLost @8;
    # 找到空闲车位
    foundIdleSlot @9;
    # 召唤泊出结束
    summonParkout @10;
    # 建图失败
    hpmFail @11;
    # 规划状态异常，对应算法侧实时全局路径搜索失败
    routeOnceFailed @12;
    # 规划状态正常，对应算法侧在车位附近未启用全局搜索功能
    routeNearby @13;
    # 规划状态正常，对应算法侧自车姿态与全局路径相反
    routeReverse @14;
    # 漫游规划成功
    roamingStartRelease @15;
    # 匹配漫游参考线失败，需要调整自车姿态
    roamingMatchFailed @16;
    # APA内部等待
    apaWait @17;
}

struct PlanningStatus {
    # 初始化成功
    initSuccessed @0 :Bool;
    # 初始化失败
    initFailed @1 :Bool;
    # 泊出
    parkingOut @2 :Bool;
    # 泊入
    parkingIn @3 :Bool;
    # APA规划成功
    apaPlanningSuccess @4 :Bool;
    # APA规划失败
    apaPlanningFail @5 :Bool;
    # plan模块触发暂停
    planningSuspend @6 :Bool;
    # plan模块触发异常退出
    planningAbort @7 :Bool;
    # 停车指令
    planCmdStop @8 :Bool;
    # 打方向盘指令
    planCmdSteer @9 :Bool;
    # 行车指令
    planCmdDrive @10 :Bool;
    # 泊车结束指令
    planCmdEnd @11 :Bool;
    # 泊车完成指令
    planCmdFinish @12 :Bool;
    # 反馈车辆可控状态指令
    planCmdPrepare @13 :Bool;
    # 紧急刹车指令
    planCmdEmergencyBrake @14 :Bool;
    # 循迹
    planHpa @15 :Bool;
    # 召唤
    planHps @16 :Bool;
    # 加载轨迹成功
    hpmPlanningSuccess @17 :Bool;
    # 加载轨迹失败
    hpmPlanningFail @18 :Bool;
    # mapping中路径保存成功
    hpmRouteSaveSuccess @19 :Bool;
    # mapping中路径保存失败
    hpmRouteSaveFail @20 :Bool;
    # 到达循迹终点
    cruiseReachEnd @21 :Bool;
    # 循迹阶段停车
    cruisePlanCmdStop @22 :Bool;
    # 循迹阶段暂停
    cruisePlanCmdPause @23 :Bool;
    # 沿途泊车开启中
    cruiseAlongParking @24 :Bool;
    # 会根据是否为最后循迹阶段等逻辑判断是否需要更平滑的定位效果，提供给SLAM做参考
    isShortTrajectoryLocalizationNeeded @25 :Bool;
    # 停车指令
    planCmdStopDone @26 :Bool;
    # 规划结果
    planFeedBack @27 :AvpPlanFeedBack;
    # 车辆是否在车位内
    isVehicleInSlot @28 :Bool;
    # 循迹剩余距离
    cruiseLeftDistance @29 :Float32;
    # 漫游开启中
    roamingParking @30 :Bool;
    # 正在循迹召唤建图
    planRaMapping @31 :Bool;
    # 输出循迹召唤全局规划结果
    planRaRouting @32 :Bool;
    # 当前循迹召唤生成参考线可用
    planRaRoutingSuccess @33 :Bool;
}

# 车位状态
enum SlotState {
    # 未知
    unknown @0;
    # 可泊
    available @1;
    # 不可泊
    occupied @2;
}

struct PlanningSlotDisplayActor {
    isTargetSlot @0 :Bool;
    isSelectable @1 :Bool;

    # 车位状态
    slotState @2 :SlotState;
}

# topic: /parking/plan/status_info
struct PlanningStatusInfo {
    # Header include seq, timestamp(last node pub time), and frame_id
    header @0 :StdHeader.Header;

    # msg id
    msgId @1 :UInt64;

    # avp fsm status & uid
    avpStatus @2 :AvpStateV3Def.AvpMainStatus;
    avpStatusUid @3 :UInt64;

    # planning status code
    planningStatus @4 :PlanningStatus;

    # planning warnning code
    planningWarnning @5 :PlanningWarningOutput;

    # reference line lane line
    referenceLineLaneLine @6 :PlanningV2.ReferenceLineAndLaneLine;

    # scenario model debug
    scenarioModelDebug @7 :PlanningV2.ScenarioModelDebug;

    # turn reminder
    turnReminder @8 :PlanningV2.TurnReminder;

    # trajectory
    trajectory @9 :PlanningV2.Trajectory;

    # parking slot
    parkingSlots @10 :FusionSlotOutArray;

    # target slot
    targetSlot @11 :ParkingCMD;

    # 推荐泊出方向(车头)
    recommendationDirections @12 :List(ParkingOutDirection);

    parkingSlotsDisplayActors @13 :List(PlanningSlotDisplayActor);

    parkingSlotsOptDisplayActors @14 :List(PlanningSlotDisplayActor);

    # 循迹召唤建图当前缓存下来的轨迹信息
    planRaRouteInfo @15 :PlanningRaRouteInfo;

    # 推荐泊出方向(车尾),  recommendationDirections 和 recommendationDirectionsTail 同一时刻最多有且只有一个不为空
    recommendationDirectionsTail @16 :List(ParkingOutDirection);

    # 推荐泊入方向
    recommendationParkingInDirections @17 :ParkingInDirection;

    # odom trajectory
    odomTrajectory @18 :PlanningV2.Trajectory;

    # 泊车入库位姿选项集
    parkingInAlignSet @19 :ParkingInAlignGroup;
}
