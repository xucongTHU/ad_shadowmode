/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file AIObjectTypeTrigger.h
* @brief 目标类型算子
*
* @author maqiang
* @date 2025-06
*******************************************************************/
#pragma once

// 中间件，消息接口头文件
#include "ad_rscl/ad_rscl.h"

// trigger base
#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"

// 消息类型
#include "ad_msg_idl/perception/perception.capnp.h"
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
  explicit AIObjectTypeTrigger(bool fromConfig=true);
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

  int64_t targetTypeIdx = 0;

  // 存放算子消息变量
  std::mutex msgMtx;
  DetectPtr visionMsg = std::make_shared<AIDetectModelOutput>();
  RoadPtr roadMsg = std::make_shared<AIRoadConditionOutput>();

  // 数据类型以及对应name映射
  std::unordered_map<uint8_t, int> classTypeMap = {
      { 1,   1 },   // 人
      { 2,   2 },   // 车辆
      { 3,   3 },   // 车辆 car
      { 4,   4 },   // 车辆 suv
      { 5,   5 },   // 车辆 van
      { 6,   6 },   // 车辆 truck
      { 7,   7 },   // 车辆 pickup_truck
      { 8,   8 },   // 车辆 bus
      { 9,   9 },   // 车辆 taxi
      { 10,  10 },  // 车辆 emergency
      { 11,  11 },  // 车辆 school_bus
      { 12,  12 },  // 车辆 others
      { 14,  13 },  // 自行车
      { 15,  14 },  // 无人车
      { 16,  15 },  // 自行车 bicycle
      { 17,  16 },  // 自行车 bikebig
      { 18,  17 },  // 自行车 bikesmall
      { 65,  18 },  // 交通牌
      { 137, 19 },  // GATE
      { 138, 20 },  // TRAIN
      { 139, 21 },  // WARNING PILLAR
      { 140, 22 },  // ROAD CONE
  };

  std::unordered_map<EMWeatherType, int> weatherTypeMap = {
      { EMWeatherType::WEATHER_UNKNOW,    23 },
      { EMWeatherType::WEATHER_OVERCAST,  24 },
      { EMWeatherType::WEATHER_SUNNY,     25 },
      { EMWeatherType::WEATHER_RAINY,     26 },
      { EMWeatherType::WEATHER_FOGGY,     27 },
      { EMWeatherType::WEATHER_SNOWY,     28 },
      { EMWeatherType::WEATHER_SANDY,     29 },
      { EMWeatherType::WEATHER_OTHERS,    30 },
  };

  std::unordered_map<EMRoadType, int> roadTypeMap = {
      { EMRoadType::ROAD_UNKNOW,       31 },
      { EMRoadType::ROAD_OPEN_ROAD,    32 },
      { EMRoadType::ROAD_CLOSED_ROAD,  33 },
      { EMRoadType::ROAD_TUNNEL,       34 }
  };

  std::unordered_map<EMTimeType, int> timeTypeMap = {
      { EMTimeType::TIME_UNKNOW,       35 },
      { EMTimeType::TIME_DAYTIME,      36 },
      { EMTimeType::TIME_NIGHT_BRIGHT, 37 },
  };
};

REGISTER_TRIGGER(AIObjectTypeTrigger)

}  // namespace shadow::trigger
