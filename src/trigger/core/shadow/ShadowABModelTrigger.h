/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file ShadowABModelTrigger.h
* @brief shadow模式：AB模型
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
#include "trigger/core/ai/HungarianOptimizer.h"
#include "trigger/core/shadow/ShadowMsgBase.h"

namespace shadow::trigger {

enum EMShadowABModelTrigger {
    TRIGGER_OBJECT_CNT = 1 << 0,   // AB模型检测结果个数不一致
    TRIGGER_OBJECT_TYPE = 2 << 0,  // AB模型检测结果类型不一致
    TRIGGER_OBJECTIOU = 3 << 0,    // AB模型检测结果IOU超过阈值
  };

/**
 * @brief AB模型算子
 */
class ShadowABModelTrigger : public TriggerBase {
  /*
    触发条件：
      1. AB模型检测结果个数不一致
      2. AB模型检测结果类型不一致
      3. AB模型检测结果IOU超过阈值
  */

 public:
  using VisionPtr = std::shared_ptr<ShadowDetectModelOutput>;

  /**
   * @brief 构造函数
   * @param nh 通信节点
   * @param fromConfig 是否从配置文件中初始化算子（单元测试的时候，不需要从配置文件中更新）
   */
  explicit ShadowABModelTrigger(stoic::cm::NodeHandle& nh, bool fromConfig=true);
  ~ShadowABModelTrigger();

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
   * @brief 检测结果回调函数
   * @param msg 视觉感知消息
   * @param topic 话题名
   */
  void ModelCallback(const ShadowDetectModelOutput&, const std::string&);

  /**
   * @brief 是否开启debug模式
   * @param _debug 标志位
   */
  void Debug(bool _debug=true) { debug = _debug; }
  
 private:
  /**
   * @brief 判断AB模型结果
   */
  void JudgeABModel();

  /**
   * @brief 判断匹配的目标类型
   * @param vModelAMsg A模型结果
   * @param vModelBMsg B模型结果
   * @param matched 匹配结果
   * @return 是否匹配
   */
  bool JudgeMatchPairType(
      const VisionPtr &vModelAMsg, const VisionPtr &vModelBMsg,
      const std::vector<std::pair<size_t, size_t> > &matched);

  /**
   * @brief 判断匹配的IOU
   * @param vModelAMsg A模型结果
   * @param vModelBMsg B模型结果
   * @param graph IOU map
   * @param matched 匹配结果
   * @return 最小IOU
   */
  double JudgeMatchPairIOU(
      const VisionPtr &vModelAMsg, const VisionPtr &vModelBMsg,
      const std::vector<std::vector<float> > &graph,
      const std::vector<std::pair<size_t, size_t> > &matched);

  /**
   * @brief hungarian优化器更新cost
   * @param graph iou矩阵
   * @param costs costs
   */
  void UpdateCosts(
      const std::vector<std::vector<float> >& graph,
      SecureMat<float>* costs);

  /**
   * @brief 计算两个检测框的IOU
   * @param bbox1 模型A中目标的检测框, x1,y1,x2,y2
   * @param bbox2 模型B中目标的检测框, x1,y1,x2,y2
   * @return iou得分
   */
  float CalcIou(const float (&bbox1)[4],
                const float (&bbox2)[4]);

  /**
   * @brief 比较两个模型结果
   * @param vModelAMsg A模型结果
   * @param vModelBMsg B模型结果
   * @param distanceGraph IOU map
   * @param matched 匹配结果
   */
  void MatchVisionObjects(
      const VisionPtr &vModelAMsg, const VisionPtr &vModelBMsg,
      std::vector<std::vector<float> > &distanceGraph,
      std::vector<std::pair<size_t, size_t> > &matched);

  /**
   * @brief 当前算子使能功能初始化
   */
  void EnableFlag();
  
 private:
  // 订阅话题名，默认值仅为单元测试使用
  std::string subModelATopic = "/test_shadow/modelA_topic";
  std::string subModelBTopic = "/test_shadow/modelB_topic";
  // 算子判断帧率
  int pubRate = 10;

  // 算子配置文件
  std::string triggerName = "ShadowABModelTrigger";
  bool debug = false;
  float bboxIouThresh = 0.8f;  // iou threshold
  int modelSize = 2;
  int64_t frameSyncThresh = 50'000;  // us
  bool enableFlag = false;
  int triggerEnable = TRIGGER_OBJECT_CNT | TRIGGER_OBJECT_TYPE | TRIGGER_OBJECTIOU;
  int8_t triggerPriority = 0;

  // 消息同步类
  std::mutex dataMtx;
  VisionPtr modelA = std::make_shared<ShadowDetectModelOutput>();
  VisionPtr modelB = std::make_shared<ShadowDetectModelOutput>();

  // hungarian优化器
  HungarianOptimizer<float> optimizer;

  // 条件判断
  std::unique_ptr<TriggerConditionChecker> conditionChecker = nullptr;
  std::unordered_map<std::string, TriggerConditionChecker::Value> vars;
  std::unordered_map<std::string, EMShadowABModelTrigger> strEnableMap = {
    { "objectCntDiff", TRIGGER_OBJECT_CNT },
    { "objectTypeDiff", TRIGGER_OBJECT_TYPE },
    { "objectIOU", TRIGGER_OBJECTIOU },
  };

  // 当前算子状态
  bool triggerStatus = false;
  std::mutex statusMtx;
};

REGISTER_TRIGGER(ShadowABModelTrigger)

}  // namespace shadow::trigger