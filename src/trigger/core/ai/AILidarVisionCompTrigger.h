/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file AILidarVisionCompTrigger.h
* @brief 雷达与视觉的结果进行对比
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
#include "trigger/core/ai/HungarianOptimizer.h"

namespace shadow::trigger {

enum EMAILidarVIsionCompTigger {
  TRIGGER_PERCEPTION_CNT = 1 << 0,  // 雷达和激光感知结果对比
  TRIGGER_POSITION_DIFF = 1 << 1,   // 匹配目标位置差异
};

/**
 * @brief 雷达和视觉感知结果对比算子
 */
class AILidarVisionCompTrigger : public TriggerBase {
  /*
    算子触发条件：
      1.视觉和雷达感知结果目标个数不一致
      2.匹配上的视觉和雷达结果距离超过阈值
  */
  

 public:
  using VisionPtr = std::shared_ptr<AIVisionModelOutput>;
  using LidarPtr = std::shared_ptr<AILidarModelOutput>;

  /**
   * @brief 算子构造函数
   * @param nh 通信节点
   * @param fromConfig 是否从配置文件中初始化算子（单元测试的时候，不需要从配置文件中更新）
   */
  explicit AILidarVisionCompTrigger(stoic::cm::NodeHandle& nh, bool fromConfig=true);
  ~AILidarVisionCompTrigger();

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
   * @brief 视觉感知结果回调函数
   * @param msg 视觉感知消息
   * @param topic 话题名
   */
  void VisionCallback(const AIVisionModelOutput&, const std::string&);

  /**
   * @brief 雷达感知结果回调函数
   * @param msg 雷达感知消息
   * @param topic 话题名
   */
  void LidarCallback(const AILidarModelOutput&, const std::string&);

  /**
   * @brief 是否开启debug模式
   * @param _debug 标志位
   */
  void Debug(bool _debug=true) { debug = _debug; }

 private:
  /**
   * @brief 比较雷达和视觉感知结果
   */
  void JudgeLidarVisionComp();

  /**
   * @brief 判断匹配上的雷达和视觉感知结果之间的距离
   * @param visionMsg 视觉感知结果
   * @param lidarMsg 雷达感知结果
   * @param thresh 距离阈值
   * @return 匹配目标最大的距离
   */
  double JudgeMatchPairDistance(
      const VisionPtr &visionMsg, const LidarPtr &lidarMsg);

  /**
   * @brief 更新hungarian优化器的代价矩阵
   * @param graph 雷达和视觉距离map
   * @param costs hungarian优化器的代价矩阵
   */
  void UpdateCosts(
      const std::vector<std::vector<float> >& graph,
      SecureMat<float>* costs);

  /**
   * @brief 雷达和视觉结果匹配
   * @param visionMsg 视觉感知结果
   * @param lidarMsg 雷达感知结果
   * @param distanceGraph 距离map
   * @param matched 匹配结果
   */
  void MatchVisionLidar(
      const VisionPtr &visionMsg, const LidarPtr &lidarMsg,
      std::vector<std::vector<float> > &distanceGraph,
      std::vector<std::pair<size_t, size_t> > &matched);

  /**
   * @brief 当前算子使能功能初始化
   */
  void EnableFlag();

 private:
  // 订阅话题名，默认值仅为单元测试使用
  std::string subVisionTopic = "/test_ai/vision_frame";
  std::string subLidarTopic = "/test_ai/lidar_frame";
  // 算子判断帧率
  int pubRate = 10;

  // 算子配置信息
  std::string triggerName = "AILidarVisionCompTrigger";
  bool debug = false;
  int frameSyncThresh = 50'000; // us, 帧同步最大时间差
  bool enableFlag = false;
  int triggerEnable = TRIGGER_PERCEPTION_CNT | TRIGGER_POSITION_DIFF;
  int8_t triggerPriority = 0;
  
  // 帧同步类
  std::mutex msgMtx;
  VisionPtr visionMsg = std::make_shared<AIVisionModelOutput>();
  LidarPtr lidarMsg = std::make_shared<AILidarModelOutput>();

  // 条件判断
  std::unique_ptr<TriggerConditionChecker> conditionChecker = nullptr;
  std::unordered_map<std::string, TriggerConditionChecker::Value> vars;
  std::unordered_map<std::string, EMAILidarVIsionCompTigger> strEnableMap = {
    { "LVObjectCntDiff", TRIGGER_PERCEPTION_CNT },
    { "LVObjectPosDiff", TRIGGER_POSITION_DIFF },
  };

  // hungarian优化
  HungarianOptimizer<float> optimizer;

  // 当前算子状态
  bool triggerStatus = false;
  std::mutex statusMtx;
};

REGISTER_TRIGGER(AILidarVisionCompTrigger)

}  // namespace shadow::trigger