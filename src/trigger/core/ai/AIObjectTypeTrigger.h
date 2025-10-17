#pragma once

// 中间件，消息接口头文件
#include "ad_interface.h"
#include "cm/cm.h"
#include "pattern/AdapterManager.hpp"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "trigger/core/ai/AIMsgBase.h"

namespace shadow::trigger {

/**
 * @brief ai模型：目标类型算子
 */
class AIObjectTypeTrigger : public TriggerBase {
  /*
    触发条件 (contain_any & contain_all):
      1. contain_any: 存在目标与指定target类型一致，则触发
      2. contain_all: 所有的目标类型与target完全符合，则触发
  */

 public:
  using DetectPtr = std::shared_ptr<AIDetectModelOutput>;
  using RoadPtr = std::shared_ptr<AIRoadConditionOutput>;

  /**
   * @brief 目标类型算子构造函数
   * @param nh 通信阶段
   * @param fromConfig 是否从配置文件中初始化参数（仅在单元测试的时候为false）
   */
  explicit AIObjectTypeTrigger(stoic::cm::NodeHandle& nh, bool fromConfig=true);
  ~AIObjectTypeTrigger();

  /**
   * @brief 算子处理
   * @return 算子状态
   */
  bool Proc() override;

  /**
   * @brief 算子判断
   * @return 算子当前状态
   */
  bool CheckCondition();
    
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
   * @brief 获取算子状态
   * @return 算子状态
   */
  bool GetStatus();

   /**
   * @brief 更新算子状态
   * @param status 带设定的状态
   */
  void UpdateStatus(bool status);

  /**
   * @brief 视觉检测回调函数（为单元测试调整为public）
   * @param msg 视觉检测结果消息
   * @param topic 话题名
   */
  void VisionCallback(const AIDetectModelOutput& msg, const std::string& topic);
  
  /**
   * @brief 路况消息回调函数（为单元测试调整为public）
   * @param msg 路况消息
   * @param topic 话题名
   */
  void RoadConditionCallback(const AIRoadConditionOutput &msg,
                             const std::string &topic);

  /**
   * @brief 是否开启debug模式
   * @param _debug 标志位
   */
  void Debug(bool _debug=true) { debug = _debug; }

 private:
  /**
   * @brief 判断是否满足
   */
  void JudgeObjectType();
  
  /**
   * @brief 当前算子使能功能初始化
   */
  void EnableFlag();

 private:
  // 话题名，默认值为单元测试
  std::string subTopic = "/test_ai/object_type";
  std::string subRoadConditionTopic  = "/test_ai/road_condition";
  // 算子判断帧率
  int pubRate = 10;

  // 算子配置信息
  std::string triggerName = "AIObjectTypeTrigger";
  bool debug = false;
  int frameSyncThresh = 50'000; // us, 帧同步最大时间差
  bool containAny = true;
  bool enableFlag = false;
  int8_t triggerPriority = 0;

  // 条件判断
  std::vector<int> inputIds;
  std::unique_ptr<TriggerConditionChecker> conditionChecker = nullptr;
  std::unordered_map<std::string, TriggerConditionChecker::Value> vars;

  // 算子状态
  bool triggerStatus = false;
  std::mutex statusMtx;
  int targetTypeIdx = 0;

  // 存放算子消息变量
  std::mutex msgMtx;
  DetectPtr visionMsg = std::make_shared<AIDetectModelOutput>();
  RoadPtr roadMsg = std::make_shared<AIRoadConditionOutput>();

  // 数据类型以及对应name映射
  std::unordered_map<uint8_t, int> classTypeMap = {
      { 0,   1 },  // 人
      { 1,   2 },  // 自行车
      { 2,   3 },  // 轿车
      { 3,   4 },  // 摩托车
      { 5,   5 },  // 公交车
      { 7,   6 },  // 货车
      { 9,   7 },  // 交通灯
      { 10,  8 },  // 消防栓
      { 11,  9 },  // 停止标志牌
      { 16,  10 },  // 狗
  };

  std::unordered_map<EMWeatherType, int> weatherTypeMap = {
      { EMWeatherType::WEATHER_SUNNY,    11 },
      { EMWeatherType::WEATHER_RAINY,    12 },
      { EMWeatherType::WEATHER_OVERCAST, 13 },
  };

  std::unordered_map<EMRoadType, int> roadTypeMap = {
      { EMRoadType::ROAD_URBAN,   14 },
      { EMRoadType::ROAD_HIGHWAY, 15 },
      { EMRoadType::ROAD_BRIDGE,  16 },
      { EMRoadType::ROAD_TUNNEL,  17 }
  };

  std::unordered_map<EMTimeType, int> timeTypeMap = {
      { EMTimeType::TIME_DAY,   18 },
      { EMTimeType::TIME_NIGHT, 19 },
  };
};

REGISTER_TRIGGER(AIObjectTypeTrigger)

}  // namespace shadow::trigger
