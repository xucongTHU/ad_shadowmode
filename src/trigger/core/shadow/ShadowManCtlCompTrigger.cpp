#include "trigger/core/shadow/ShadowManCtlCompTrigger.h"

#include "common/environment.h"
#include "common/time/Timer.h"

namespace shadow::trigger {

ShadowManCtlCompTrigger::ShadowManCtlCompTrigger(stoic::cm::NodeHandle& nh, bool fromConfig) {
  if (fromConfig) {
    // 默认配置文件路径
    std::string configPath = std::string(common::getInstallRootPath())
                            + "/config/ai_shadow_trigger_conf.yaml";
    LOG_INFO("[ShadowManCtlCompTrigger] Init load configPath=%s", 
             configPath.c_str());

    // 加载配置yaml文件
    YAML::Node configNode;
    try {
      configNode = YAML::LoadFile(configPath);
    } catch (std::exception &e) {
      LOG_ERROR("[ShadowManCtlCompTrigger] Fail to load file %s, error code=%s",
                configPath.c_str(), e.what());
      exit(-1);
    }

    // debug标志位
    common::loadNode<bool>(configNode, debug, "debug");

    // 读取当前算子的配置参数
    if (configNode[triggerName].IsDefined()) {
      auto triggerNode = configNode[triggerName];
      common::loadNode<std::string>(triggerNode, subCtlTopic, "subCtlTopic");
      common::loadNode<std::string>(triggerNode, subCanTopic, "subCanTopic");
      common::loadNode<int>(triggerNode, pubRate, "pubRate");
      common::loadNode<int64_t>(triggerNode, compTimeDuration, 
                                "compTimeDuration");
      compTimeDuration *= SHADOW_TIME_CONVERSION;
      common::loadNode<int>(triggerNode, ctlRate, "ctlRate");
      common::loadNode<int>(triggerNode, canRate, "canRate");
      common::loadNode<float>(triggerNode, bufferExtendRatio, 
                              "bufferExtendRatio");
      ctlMinMax.clear();
      common::loadNode<std::vector<float> >(triggerNode, ctlMinMax, "ctlMinMax");
      int priority = 0;
      common::loadNode<int>(triggerNode, priority, "priority");
      triggerPriority = static_cast<int8_t>(priority);
    } else {
      LOG_WARN("[ShadowManCtlCompTrigger] load config error, use default!");
    }
  }

  // 订阅规划消息
  caicAD::cm::idl::SubscriberConf conf_;
  conf_.set_pending_queue_size(1);
  AdapterManager<ShadowModelCtlOutput>::SubscriberBuilder(nh)
     .topic(subCtlTopic)
     .subConf(conf_)
     .subscribe(std::bind(&ShadowManCtlCompTrigger::CtlCallback, this,
                std::placeholders::_1, std::placeholders::_2));

  // 订阅定位消息
  ChassisAdapter::SubscriberBuilder(nh)
     .topic(subCanTopic)
     .subConf(conf_)
     .subscribe(std::bind(&ShadowManCtlCompTrigger::CanCallback, this,
                std::placeholders::_1, std::placeholders::_2));

  // 初始化循环队列
  float timeDurationSeconds = static_cast<float>(
      compTimeDuration) / SHADOW_TIME_CONVERSION / SHADOW_TIME_CONVERSION;
  ctlDurationCnt = static_cast<size_t>(timeDurationSeconds * ctlRate);
  canDurationCnt = static_cast<size_t>(timeDurationSeconds * canRate);
  ctlBufferSize = static_cast<size_t>(ctlDurationCnt * bufferExtendRatio);

  // debug 信息
  if (debug) {
    LOG_INFO("============== ShadowManCtlCompTrigger Config ===========");
    LOG_INFO("subCtlTopic:          %s", subCtlTopic.c_str());
    LOG_INFO("subCanTopic:          %s", subCanTopic.c_str());
    LOG_INFO("pubRate:              %d", pubRate);
    LOG_INFO("compTimeDuration:     %d", compTimeDuration);
    LOG_INFO("ctlRate:              %d", ctlRate);
    LOG_INFO("canRate:              %d", canRate);
    LOG_INFO("bufferExtendRatio:    %f", bufferExtendRatio);
    LOG_INFO("triggerPriority:      %d", triggerPriority);
    LOG_INFO("timeDurationSeconds:  %f", timeDurationSeconds);
    LOG_INFO("ctlDurationCnt:       %zu", ctlDurationCnt);
    LOG_INFO("canDurationCnt:       %zu", canDurationCnt);
    LOG_INFO("=========================================================");
  }

  // 控制消息循环队列
  ctlBufferPtr = std::make_unique<SeriesBuffer>(
    ctlBufferSize,
    [](const MsgPtr& frame) { return frame->stamp; }
  );

  // canbus循环队列
  canBufferSize = static_cast<size_t>(canDurationCnt * bufferExtendRatio);
  canBufferPtr = std::make_unique<SeriesBuffer>(
    canBufferSize,
    [](const MsgPtr& frame) { return frame->stamp; }
  );

  // 条件判断初始化
  conditionChecker = std::make_unique<TriggerConditionChecker>();
}

ShadowManCtlCompTrigger::~ShadowManCtlCompTrigger() { }

bool ShadowManCtlCompTrigger::Proc() {
  common::Rate rate(pubRate);
  while (stoic::cm::ok()) {
    std::unique_lock<std::mutex> lck(statusMtx);

    // 判断算子状态
    CheckCondition();

    {
      // 如果满足条件，则触发算子，发送消息  
      if (triggerStatus) {
        TriggerContext context;
        context.timeStamp = common::Timer::now();
        context.triggerId = trigger_obj_ ? trigger_obj_->triggerId : "10";
        context.triggerName = GetTriggerName();
        context.triggerStatus = TriggerContext::TriggerState::Triggered;

        NotifyTriggerContext(context); // 调用通知接口
        
        if (debug) {
          LOG_INFO("[ShadowManCtlCompTrigger] trigger status: TRUE");
        }
      }
    }
    rate.sleep();
  }

  return true;
}

bool ShadowManCtlCompTrigger::CheckCondition() {
  // 1.人机操作对比
  JudgeManCtl();

  // 如果没有要判断的条件，则直接范围（一般是消息还没同步）
  if (vars.empty()) {
    return triggerStatus;
  } else {
    Clear();   // 如果有满足的条件，则清空ctl和can的缓存buffer
  }

  // 2. 条件判断
  triggerStatus = conditionChecker->check(vars);

  // 3. 清空当前需要判断的条件，等待下次
  vars.clear();

  return triggerStatus;
}

void ShadowManCtlCompTrigger::NotifyTriggerContext(TriggerContext context) {
    TriggerFactory::Instance().InvokeTriggerCallback(context);
    LOG_INFO("Trigger notified: %s (ID: %s, Time: %ld)",
             context.triggerName.c_str(), context.triggerId.c_str(), context.timeStamp);
}

std::string ShadowManCtlCompTrigger::GetTriggerName() const {
  return triggerName;
}

int8_t ShadowManCtlCompTrigger::GetPriority() const {
  return  triggerPriority;
}

bool ShadowManCtlCompTrigger::GetStatus() {
  // 获取当前算子状态
  std::unique_lock<std::mutex> lock(statusMtx);
  return triggerStatus;
}

void ShadowManCtlCompTrigger::UpdateStatus(bool status) {
  // 更新当前算子状态，用于单元测试
  std::unique_lock<std::mutex> lock(statusMtx);
  triggerStatus = status;
}

void ShadowManCtlCompTrigger::CtlCallback(const ShadowModelCtlOutput &msg, const std::string &topic) {
  if (debug) {
    LOG_INFO("[ShadowManCtlCompTrigger] get ctl msg (%s), stamp=%ld",
              topic.c_str(), msg.stamp);
  }

  {
    // 存入缓存
    std::unique_lock<std::mutex> lock(ctlMtx);
    MsgPtr msgPtr = std::make_shared<ShadowInnerConditions>();
    msgPtr->stamp = msg.stamp;
    msgPtr->acc = msg.acc;
    msgPtr->brake = msg.brake;
    msgPtr->throttle = msg.throttle;
    msgPtr->wheelAngle = msg.wheelAngle;
    msgPtr->wheelAngularVelocity = msg.wheelAngularVelocity;
    ctlBufferPtr->push(msgPtr);
  }
}

void ShadowManCtlCompTrigger::CanCallback(const caic_sensor::Canbus &msg, const std::string &topic) {
  if (debug) {
    LOG_INFO("[ShadowManCtlCompTrigger] get can msg (%s), stamp=%ld",
              topic.c_str(), msg.header.stamp);
  }

  {
    // 存入缓存
    std::unique_lock<std::mutex> lock(canMtx);
    MsgPtr msgPtr = std::make_shared<ShadowInnerConditions>();
    msgPtr->stamp = msg.header.stamp;
    msgPtr->acc = msg.chassis_info.yrs.acceleration_x;
    msgPtr->brake = msg.chassis_info.esp.master_cylinder_pressure;
    msgPtr->throttle = msg.chassis_info.esp.acceleration_pedal_position;
    msgPtr->wheelAngle = msg.chassis_info.eps.steering_wheel_info.angle;
    msgPtr->wheelAngularVelocity = msg.chassis_info.eps.steering_wheel_info.speed;
    canBufferPtr->push(msgPtr);
  }
}

void ShadowManCtlCompTrigger::Clear() {
  {
    // 清空缓存结果
    std::unique_lock<std::mutex> lock(ctlMtx);
    ctlBufferPtr->clear();
  }
  {
    // 清空缓存结果
    std::unique_lock<std::mutex> lock(canMtx);
    canBufferPtr->clear();
  }
  if (debug) {
    LOG_INFO("[ShadowManCtlCompTrigger] buffer cleared!");
  }
}

void ShadowManCtlCompTrigger::GetAverageConds(
    ShadowInnerConditions &conds, const std::unique_ptr<SeriesBuffer> &buffer,
    int64_t startTime, int64_t endTime) {
  // 从后往前进行遍历
  int validCnt = 0;
  for (int i = static_cast<int>(buffer->size()) - 1; i >= 0; i--) {
    auto curMsg = buffer->at(i);
    if (curMsg->stamp >= startTime && curMsg->stamp <= endTime) {
      conds.acc += curMsg->acc;
      conds.brake += curMsg->brake;
      conds.throttle += curMsg->throttle;
      conds.wheelAngle += curMsg->wheelAngle;
      conds.wheelAngularVelocity += curMsg->wheelAngularVelocity;
      validCnt += 1;
    }

    // 提前退出
    if (curMsg->stamp < startTime) {
      break;
    }
  }

  // 计算均值
  if (validCnt > 0) { 
    conds.acc /= validCnt;
    conds.brake /= validCnt;
    conds.throttle /= validCnt;
    conds.wheelAngle /= validCnt;
    conds.wheelAngularVelocity /= validCnt;
  }
}

float ShadowManCtlCompTrigger::GetRatio(float input, int index) {
  if (index < CTL_INDEX_ACC || index > CTL_INDEX_ANGULAR_V) {
    LOG_WARN("[ShadowManCtlCompTrigger] input index=%d out of range", index);
    return 0.0f;
  }

  auto min = ctlMinMax[index * 2];
  auto max = ctlMinMax[index * 2 + 1];

  return abs(input) / (max - min);
}

void ShadowManCtlCompTrigger::JudgeManCtl() {
  // 算子条件解析
  EnableFlag();

  // lock初始化，但是先不锁
  std::unique_lock<std::mutex> lock1(ctlMtx, std::defer_lock);
  std::unique_lock<std::mutex> lock2(canMtx, std::defer_lock);

  // 使用 std::lock 同时枷锁
  std::lock(lock1, lock2);

  // 判断两个buffer中，是否达到指定帧数    
  if (ctlBufferPtr->size() < ctlDurationCnt 
      || canBufferPtr->size() < canDurationCnt) {
    LOG_WARN("[ShadowManCtlCompTrigger] can or ctl buffer size is not enough!");
    return;
  }

  // 取指定时间范围内的变量
  auto endTime = std::min(ctlBufferPtr->backTime(), canBufferPtr->backTime());
  auto startTime = endTime - compTimeDuration;  // 获取开始截止i时间
  if (debug) {
    LOG_INFO("[ShadowManCtlCompTrigger] JudgeManCtl startTime=%ld, endTime=%ld",
             startTime, endTime);
  }

  ShadowInnerConditions averCtlConds, averCanConds;
  // 统计时间范围内的各项指标的均值
  GetAverageConds(averCtlConds, ctlBufferPtr, startTime, endTime);
  GetAverageConds(averCanConds, canBufferPtr, startTime, endTime);
  if (debug) {
    LOG_INFO("[ShadowManCtlCompTrigger] averCtlConds average value");
    LOG_INFO("    acc: %f", averCtlConds.acc);
    LOG_INFO("    brake: %f", averCtlConds.brake);
    LOG_INFO("    throttle: %f", averCtlConds.throttle);
    LOG_INFO("    wheelAngle: %f", averCtlConds.wheelAngle);
    LOG_INFO("    wheelAngularVelocity: %f", averCtlConds.wheelAngularVelocity);
    LOG_INFO("[ShadowManCtlCompTrigger] averCanConds average value");
    LOG_INFO("    acc: %f", averCanConds.acc);
    LOG_INFO("    brake: %f", averCanConds.brake);
    LOG_INFO("    throttle: %f", averCanConds.throttle);
    LOG_INFO("    wheelAngle: %f", averCanConds.wheelAngle);
    LOG_INFO("    wheelAngularVelocity: %f", averCanConds.wheelAngularVelocity);
  }

  // ACC差值
  if (triggerEnable & TRIGGER_ACC_COMP) {
    auto acc_diff = averCtlConds.acc - averCanConds.acc;
    float acc_diff_ratio = GetRatio(acc_diff, CTL_INDEX_ACC);
    vars["ctlAccDiff"] = static_cast<double>(acc_diff_ratio);
    if (debug) {
      LOG_INFO("[ShadowManCtlCompTrigger] acc_diff=%f, ctlAccDiff=%f", 
                acc_diff, acc_diff_ratio);
    }
  }

  // brake差值
  if (triggerEnable & TRIGGER_BRAKE_COMP) {
    auto brake_diff = averCtlConds.brake - averCanConds.brake;
    float brake_diff_ratio = GetRatio(brake_diff, CTL_INDEX_BRAKE);
    vars["ctlBrakeDiff"] = static_cast<double>(brake_diff_ratio);
    if (debug) {
      LOG_INFO("[ShadowManCtlCompTrigger] brake_diff=%f, ctlBrakeDiff=%f", 
               brake_diff, brake_diff_ratio);
    }
  }

  // throttle差值
  if (triggerEnable & TRIGGER_THROTTLE_COMP) {
    auto throttle_diff = averCtlConds.throttle - averCanConds.throttle;
    float throttle_diff_ratio = GetRatio(throttle_diff, CTL_INDEX_THROTTLE);
    vars["ctlThrottleDiff"] = static_cast<double>(throttle_diff_ratio);
    if (debug) {
      LOG_INFO("[ShadowManCtlCompTrigger] throttle_diff=%f, ctlThrottleDiff=%f", 
               throttle_diff, throttle_diff_ratio);
    }
  }

  // wheelAngle差值
  if (triggerEnable & TRIGGER_WHEEL_ANGLE_COMP) {
    auto wheelAngle_diff = averCtlConds.wheelAngle - averCanConds.wheelAngle;
    float wheelAngle_diff_ratio = GetRatio(wheelAngle_diff, CTL_INDEX_ANGLE);
    vars["ctlWheelAngleDiff"] = static_cast<double>(wheelAngle_diff_ratio);
    if (debug) {
      LOG_INFO("[ShadowManCtlCompTrigger] wheelAngle_diff=%f, ctlWheelAngleDiff=%f", 
               wheelAngle_diff, wheelAngle_diff_ratio);
    }
  }

  // wheelAngularVelocity差值
  if (triggerEnable & TRIGGER_WHEEL_ANGULAR_VELOCITY_COMP) {
    auto wheelAngularVelocity_diff = 
        averCtlConds.wheelAngularVelocity - averCanConds.wheelAngularVelocity;
    float wheelAngularVelocity_diff_ratio = 
        GetRatio(wheelAngularVelocity_diff, CTL_INDEX_ANGULAR_V);
    vars["ctlWheelAngularVelocityDiff"] = 
        static_cast<double>(wheelAngularVelocity_diff_ratio);
    if (debug) {
      LOG_INFO("[ShadowManCtlCompTrigger] wheelAngularVelocity_diff=%f, ctlWheelAngularVelocityDiff=%f", 
               wheelAngularVelocity_diff, wheelAngularVelocity_diff_ratio);
    }
  }
}

void ShadowManCtlCompTrigger::EnableFlag() {
  // 初次处理，初始化条件判断，根据输入条件决定算子内部使能功能
  triggerEnable = 0;
  if (!conditionChecker->parse(trigger_obj_->triggerCondition)) {
    LOG_ERROR("ShadowManCtlCompTrigger: %s", conditionChecker->last_error().c_str());
    return;
  }

  if (debug) {
    auto elmes = conditionChecker->get_elements();
    for (auto & elem : elmes) {
      LOG_INFO("Variable: %s | Operator: %s | Threshold: %s | Logic: %s", 
                elem.variable.c_str(), elem.comparison_op.c_str(), 
                elem.threshold_str().c_str(), 
                (elem.logical_op.empty() ? "Empty" : elem.logical_op).c_str());
    }
  }

  // ctlAccDiff | ctlBrakeDiff | ctlThrottleDiff | ctlWheelAngleDiff | ctlWheelAngularVelocityDiff -> 更新当前算子内部使能状态
  for (auto iter = strEnableMap.begin(); iter != strEnableMap.end(); iter++) {
    if (trigger_obj_->triggerCondition.find(iter->first) != std::string::npos) {
      triggerEnable |= iter->second;
    }
  }
  LOG_INFO("[ShadowManCtlCompTrigger] triggerEnable = %d", triggerEnable);
}
}