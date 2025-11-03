/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file ShadowManCtlCompTrigger.h
* @brief 人机对比算子
*
* @author maqiang
* @date 2025-07
*******************************************************************/
#pragma once

// 中间件，消息接口头文件
#include "ad_rscl/ad_rscl.h"

// trigger base
#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"

// 消息类型
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_planning/planning.capnp.h"
#include "common/time/TimeSeriesBuffer.hpp"
#include "trigger/core/shadow/ShadowMsgBase.h"

namespace shadow::trigger {

enum EMShadowManCtlCompTrigger {
  TRIGGER_ACC_COMP = 1 << 0,                      // ctlAccDiff
  TRIGGER_BRAKE_COMP = 1 << 1,                    // ctlBrakeDiff
  TRIGGER_THROTTLE_COMP = 1 << 2,                 // ctlThrottleDiff
  TRIGGER_WHEEL_ANGLE_COMP = 1 << 3,              // ctlWheelAngleDiff
  TRIGGER_WHEEL_ANGULAR_VELOCITY_COMP = 1 << 4,   // ctlWheelAngularVelocityDiff
};

// 用于保存接受到消息中有效的数据，减少缓存需要的空间占用
struct ShadowInnerConditions {
  uint64_t stamp = 0;
  float acc = 0.0f;                    // acc控制值
  float brake = 0.0f;                  // 制动板控制值
  float throttle = 0.0f;               // 油门控制值
  float wheelAngle = 0.0f;             // 方向盘转角
  float wheelAngularVelocity = 0.0f;   // 方向盘角速度
};

constexpr int CTL_INDEX_ACC = 0;
constexpr int CTL_INDEX_BRAKE = 1;
constexpr int CTL_INDEX_THROTTLE = 2;
constexpr int CTL_INDEX_ANGLE = 3;
constexpr int CTL_INDEX_ANGULAR_V = 4;

/**
 * @brief 人机对比算子
 */
class ShadowManCtlCompTrigger : public TriggerBase {
/*
    触发条件：
      1. 100ms内，Acc的平均值差异超过10%
      2. 100ms内，Brake的平均值差异超过10%
      3. 100ms内，Throttle的平均值差异超过10%
      4. 100ms内，WheelAngle的平均值差异超过10%
      5. 100ms内，WheelAngularVelocity的平均值差异超过10%
  */
 public:
  using MsgPtr = std::shared_ptr<ShadowInnerConditions>;
  using SeriesBuffer = common::TimeSeriesBuffer<MsgPtr>;

  /**
   * @brief 构造函数
   * @param nh 通信节点
   * @param fromConfig 是否从配置文件中初始化算子（单元测试的时候，不需要从配置文件中更新）
   */
  explicit ShadowManCtlCompTrigger(bool fromConfig=true);
  ~ShadowManCtlCompTrigger();

  /**
   * @brief 算子执行，根据算子状态，如果触发则发送消息
   * @return 状态
   */
  bool Proc() override;

  /**
   * @brief 条件判断
   */
  bool CheckCondition() override;

  /**
   * @brief 获取当前算子名字
   */
  std::string GetTriggerName() const override;

   /**
   * @brief 视觉感知结果回调函数
   * @param topic 话题名
   * @param msg raw消息
   */
  void OnMessageReceived(const std::string& topic,
                         const TRawMessagePtr& msg) override;

  /**
   * @brief 清空已经保存的buffer
   */
  void Clear();

  /**
   * @brief 是否开启debug模式
   * @param _debug 标志位
   */
  void Debug(bool _debug=true) { debug = _debug; }

 private:

  /**
   * @brief 获取当前buffer中指定时间范围的数据
   * @param conds 输出数据
   * @param buffer 消息buffer
   * @param startTime 开始时间
   * @param endTime 结束时间
  */
  void GetAverageConds(ShadowInnerConditions &conds,
                       const std::unique_ptr<SeriesBuffer> &buffer,
                       int64_t startTime, int64_t endTime);

  /**
   * @brief 获取当前差值占比
   * @param input 当前差值
   * @param index 索引，比较的是哪一项
   */
  float GetRatio(float input, int index);

  /**
   * @brief 条件判断
   */
  void JudgeManCtl();

  /**
   * @brief 当前算子使能功能初始化
   */
  void EnableFlag();

 private:
  // 订阅话题名，默认值仅为单元测试使用
  std::string subTopic = "/test_shadow/canbus_topic";
  // 算子判断帧率
  int pubRate = 10;

  // 算子配置文件
  std::string triggerName = "ShadowManCtlCompTrigger";
  bool debug = false;
  int64_t compTimeDuration = 100'000;   // compare time range 比较的时间范围 us
  int ctlRate = 50;
  int canRate = 100;
  float bufferExtendRatio = 1.2f;
  int8_t triggerPriority = 0;

  // 标定的参数范围
  // 默认值用于单元测试
  std::vector<float> ctlMinMax = {
      -10.0f, 10.0f, 0.0f, 200.0f, 0.0f, 1.0f, -600.0f, 600.0f, -600.0f, 600.0f};

  bool enableFlag = false;
  int triggerEnable = TRIGGER_ACC_COMP
                    | TRIGGER_BRAKE_COMP
                    | TRIGGER_THROTTLE_COMP
                    | TRIGGER_WHEEL_ANGLE_COMP
                    | TRIGGER_WHEEL_ANGULAR_VELOCITY_COMP;

  // 控制消息与can消息缓存队列
  std::mutex ctlMtx, canMtx;
  size_t ctlBufferSize, canBufferSize;   // 缓存队列大小
                                         // 考虑到时间对齐，通常需要略大于指定比较时间范围
  size_t ctlDurationCnt, canDurationCnt; // 满足时间范围的最小帧数要求
  std::unique_ptr<SeriesBuffer> ctlBufferPtr = nullptr;
  std::unique_ptr<SeriesBuffer> canBufferPtr = nullptr;

  // 条件判断
  std::unique_ptr<TriggerConditionChecker> conditionChecker = nullptr;
  std::unordered_map<std::string, TriggerConditionChecker::Value> vars;

  std::unordered_map<std::string, EMShadowManCtlCompTrigger> strEnableMap = {
    { "ctlAccDiff", TRIGGER_ACC_COMP },
    { "ctlBrakeDiff", TRIGGER_BRAKE_COMP },
    { "ctlThrottleDiff", TRIGGER_THROTTLE_COMP },
    { "ctlWheelAngleDiff", TRIGGER_WHEEL_ANGLE_COMP },
    { "ctlWheelAngularVelocityDiff", TRIGGER_WHEEL_ANGULAR_VELOCITY_COMP },
  };
};

REGISTER_TRIGGER(ShadowManCtlCompTrigger)

}  // namespace shadow::trigger