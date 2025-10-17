/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file AISkipFrameTrigger.h
* @brief 感知跳帧算子
*
* @author maqiang
* @date 2025-06
*******************************************************************/
#pragma once

// 中间件，消息接口头文件
#include "caic_interface.h"
#include "cm/cm.h"
#include "pattern/AdapterManager.hpp"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "trigger/core/ai/AIMsgBase.h"

namespace shadow::trigger {

enum EMAISkipFrameTrigger {
  TRIGGER_LOST_AGE = 1 << 0,             // 同一个trackid漏帧
  TRIGGER_OBJECT_INFO = 1 << 1,          // 同一个trackid目标类型不一致
  TRIGGER_TIME_DURATION = 1 << 2,        // 感知结果前后帧时间差过大
};

/**
 * @brief 单条轨迹状态
 */
struct TrackStaus {
  int trackId = -1;
  int64_t latestStamp = -1;
  int64_t beginStamp = -1;
  int trackAge = 0;
  int lostAge = 0;

  // 目标信息
  int classId;
};

/**
 * @brief ai模型，感知跳帧
 */
class AISkipFrameTrigger : public TriggerBase {
  /*
    算子触发:
      1. 同一个trackid漏帧
      2. 同一个trackid目标类型不一致
      3. 感知结果前后帧时间差过大
  */

 public:

  /**
   * @brief 构造函数
   * @param nh 通信节点
   * @param fromConfig 是否从配置文件中初始化算子（单元测试的时候，不需要从配置文件中更新）
   */
  explicit AISkipFrameTrigger(stoic::cm::NodeHandle& nh, bool fromConfig=true);
  ~AISkipFrameTrigger();

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
   * @brief 触发通知
   */
  void NotifyTriggerContext(TriggerContext context) override;

  /**
   * @brief 获取当前算子名字
   */
  std::string GetTriggerName() const override;

  /**
   * @brief 获取算子优先级
   */
  int8_t GetPriority() const override;

  /**
   * @brief 获取算子状态（仅用于单元测试）
   * @return 算子当前状态
   */
  bool GetStatus();

  /**
   * @brief 更新算子状态（仅用于单元测试）
   * @param status 要设置的状态，true或false
   */
  void UpdateStatus(bool status);

  /**
   * @brief 更新算子内部功能
   * @param enable 算子内部功能
   */
  void UpdateTriggerEnable(int enable);
  
  /**
   * @brief 视觉检测结果回调函数
   * @param msg 视觉感知消息
   * @param topic 话题名
   */
 void VisionCallback(const AIVisionModelOutput&, const std::string&);

  /**
   * @brief 是否开启debug模式
   * @param _debug 标志位
   */
  void Debug(bool _debug=true) { debug = _debug; }

 private:
  /**
   * @brief 判断同一个trackid丢帧
   * @param msg 当前消息
   * @return 当前条件
   */
  int JudgePercetpionLost(const AIVisionModelOutput &msg);

  /**
   * @brief 判断同一个trackid类型
   * @param msg 当前消息
   * @return 当前条件
   */
  int JudgePercetpionType(const AIVisionModelOutput &msg);

  /**
   * @brief 判断感知跳帧
   * @param msg 当前消息
   */
  void JudgeSkipFrame(const AIVisionModelOutput &msg);

  /**
   * @brief 更新轨迹状态
   * @param msg 感知消息
   */
  void updateTrackStatus(const AIVisionModelOutput &msg);

  /**
   * @brief 当前算子使能功能初始化
   */
  void EnableFlag();
  
 private:
  // 订阅话题名，默认值仅为单元测试使用
  std::string subTopic = "/test_ai/skip_frame";
  // 算子判断帧率
  int pubRate = 10;

  // 感知结果轨迹管理
  int64_t latestStamp = -1;
  std::unordered_map<int, TrackStaus> trackStatus;

  // 算子配置文件
  std::string triggerName = "AISkipFrameTrigger";
  bool debug = false;
  int lostAgeThresh = 10;
  int64_t visionTimeDuration = 100'000;  // us
  bool enableFlag = false;
  int triggerEnable = TRIGGER_LOST_AGE | TRIGGER_OBJECT_INFO | TRIGGER_TIME_DURATION;
  int8_t triggerPriority = 0;

  // 条件判断
  std::unique_ptr<TriggerConditionChecker> conditionChecker = nullptr;
  std::unordered_map<std::string, TriggerConditionChecker::Value> vars;
  std::unordered_map<std::string, EMAISkipFrameTrigger> strEnableMap = {
    { "lostAge", TRIGGER_LOST_AGE },
    { "trackTypeDiff", TRIGGER_OBJECT_INFO },
    { "frameStampDiff", TRIGGER_TIME_DURATION },
  };

  // 当前算子状态
  bool triggerStatus = false;
  std::mutex statusMtx;
  std::mutex triggerEnableMtx;
};

REGISTER_TRIGGER(AISkipFrameTrigger)

}  // namespace shadow::trigger