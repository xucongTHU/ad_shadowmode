#include "trigger/core/ai/AIObjectTypeTrigger.h"

#include "common/environment.h"
#include "common/time/Timer.h"

namespace shadow::trigger {

AIObjectTypeTrigger::AIObjectTypeTrigger(stoic::cm::NodeHandle& nh, bool fromConfig) : TriggerBase() {
  if (fromConfig) {
    // 默认配置文件路径
    std::string configPath = std::string(common::getInstallRootPath())
                            + "/config/ai_shadow_trigger_conf.yaml";
    LOG_INFO("[AIObjectTypeTrigger] Init load configPath=%s", 
            configPath.c_str());
    
    // 加载配置文件
    YAML::Node configNode;
    try {
      configNode = YAML::LoadFile(configPath);
    } catch (std::exception &e) {
      LOG_ERROR("[AIObjectTypeTrigger] Fail to load file %s, error code=%s",
                configPath.c_str(), e.what());
      exit(-1);
    }

    // debug标志
    common::loadNode<bool>(configNode, debug, "debug");

    // 读取当前算子的配置参数
    if (configNode[triggerName].IsDefined()) {
      auto triggerNode = configNode[triggerName];
      common::loadNode<std::string>(triggerNode, subTopic, "subTopic");
      common::loadNode<std::string>(triggerNode, subRoadConditionTopic,   
                                    "subRoadConditionTopic");
      common::loadNode<int>(triggerNode, pubRate, "pubRate");
      common::loadNode<bool>(triggerNode, containAny, "containAny");
      common::loadNode<int>(triggerNode, frameSyncThresh, "frameSyncThresh");
      frameSyncThresh *= AI_TIME_CONVERSION;
      int priority = 0;
      common::loadNode<int>(triggerNode, priority, "priority");
      triggerPriority = static_cast<int8_t>(priority);
    } else {
      LOG_WARN("[AIObjectTypeTrigger] load config error, use default!");
    }
  }
  
  // debug信息
  if (debug) {
    LOG_INFO("================ AIObjectTypeTrigger Config =============");
    LOG_INFO("subTopic:              %s", subTopic.c_str());
    LOG_INFO("subRoadConditionTopic: %s", subRoadConditionTopic.c_str());
    LOG_INFO("pubRate:               %d", pubRate);
    LOG_INFO("containAny:            %d", containAny);
    LOG_INFO("frameSyncThresh:       %d", frameSyncThresh);
    LOG_INFO("triggerPriority:       %d", triggerPriority);
    LOG_INFO("=========================================================");
  }

  // 检测话题订阅
  caicAD::cm::idl::SubscriberConf conf_;
  conf_.set_pending_queue_size(1);
  AdapterManager<AIDetectModelOutput>::SubscriberBuilder(nh)
     .topic(subTopic)
     .subConf(conf_)
     .subscribe(std::bind(&AIObjectTypeTrigger::VisionCallback, this,
                std::placeholders::_1, std::placeholders::_2));

  // 路况消息订阅
  AdapterManager<AIRoadConditionOutput>::SubscriberBuilder(nh)
     .topic(subTopic)
     .subConf(conf_)
     .subscribe(std::bind(&AIObjectTypeTrigger::RoadConditionCallback, this,
                std::placeholders::_1, std::placeholders::_2));

  // 条件判断初始化
  conditionChecker = std::make_unique<TriggerConditionChecker>();
}

AIObjectTypeTrigger::~AIObjectTypeTrigger() {}

bool AIObjectTypeTrigger::Proc() {
  common::Rate rate(pubRate);
  while (stoic::cm::ok()) {
    {
      std::unique_lock<std::mutex> lck(statusMtx);

      // 判断算子状态
      CheckCondition();

      // 如果满足条件，则触发算子，发送消息
      if (triggerStatus) {
        TriggerContext context;
        context.timeStamp = common::Timer::now();
        context.triggerId = trigger_obj_ ? trigger_obj_->triggerId : "10";
        context.triggerName = GetTriggerName();
        context.triggerStatus = TriggerContext::TriggerState::Triggered;

        NotifyTriggerContext(context); // 调用通知接口

        if (debug) {
          LOG_INFO("[AIObjectTypeTrigger] trigger status: TRUE");
        }
      }
    }
    rate.sleep();
  }

  return true;
}

bool AIObjectTypeTrigger::CheckCondition() {
  // 判断同步帧包含类型
  JudgeObjectType();

  // 如果没有要判断的条件，则直接范围（一般是消息还没同步）
  if (inputIds.empty()) {
    LOG_WARN("[AIObjectTypeTrigger] inputIds is empty!");
    triggerStatus = false;
    return triggerStatus;
  }

  // 条件判断
  {
    // 循环check，如果满足条件，提前退出
    int curIdx = 0;
    for (auto id : inputIds) {
      curIdx |= (1 << id);
    }
    if (debug) {
      LOG_INFO("[AIObjectTypeTrigger] current typeIdxs = %d, targetTypeIdx & curIdx=%d", 
          curIdx, targetTypeIdx & curIdx);
    }
    
    int objTag = 0;
    if (containAny) {
      // (targetTypeIdx & curIdx > 0) 存在指定类别
      objTag = (targetTypeIdx & curIdx) > 0 ? targetTypeIdx : 0;
    } else {
      // (targetTypeIdx & curIdx == targetTypeIdx) 指定类别都存在
      curIdx += 1;  // 最后一位置1
      objTag = (targetTypeIdx & curIdx) == targetTypeIdx ? targetTypeIdx : 0;
    }
    
    vars["objTag"] = static_cast<double>(objTag);
    if (debug) {
      LOG_INFO("[AIObjectTypeTrigger] objTag = %lf", vars["objTag"]);
    }
    triggerStatus = conditionChecker->check(vars);  
  }

  // 清空历史结果
  vars.clear();
  inputIds.clear();

  return triggerStatus;
}

void AIObjectTypeTrigger::NotifyTriggerContext(TriggerContext context) {
    TriggerFactory::Instance().InvokeTriggerCallback(context);
    LOG_INFO("Trigger notified: %s (ID: %s, Time: %ld)",
             context.triggerName.c_str(), context.triggerId.c_str(), context.timeStamp);
}

std::string AIObjectTypeTrigger::GetTriggerName() const {
  return triggerName;
}

int8_t AIObjectTypeTrigger::GetPriority() const {
  return  triggerPriority;
}

bool AIObjectTypeTrigger::GetStatus() {
  std::unique_lock<std::mutex> lock(statusMtx);
  return triggerStatus;
}

void AIObjectTypeTrigger::UpdateStatus(bool status) {
  std::unique_lock<std::mutex> lock(statusMtx);
  triggerStatus = status;
}

void AIObjectTypeTrigger::VisionCallback(const AIDetectModelOutput& msg, const std::string& topic) {
  if (debug) {
    LOG_INFO("[AIObjectTypeTrigger] get perception msg (%s), stamp=%ld, valid_cnt=%d",
              topic.c_str(), msg.stamp, msg.validCnt);
  }

  // 帧同步
  {
    std::unique_lock<std::mutex> lck(msgMtx);
    visionMsg->stamp = msg.stamp;
    visionMsg->sensorStamp = msg.sensorStamp;
    visionMsg->validCnt = msg.validCnt;
    memcpy(visionMsg->result, msg.result, sizeof(AIDetection) * AI_MAX_DETECT_OBJ);
  }
}

void AIObjectTypeTrigger::RoadConditionCallback(
    const AIRoadConditionOutput &msg, const std::string &topic) {
  if (debug) {
    LOG_INFO("[AIObjectTypeTrigger] get road condition msg (%s), stamp=%ld",
              topic.c_str(), msg.stamp);
  }

  // 帧同步 
  {
    std::unique_lock<std::mutex> lck(msgMtx);
    roadMsg->stamp = msg.stamp;
    roadMsg->sensorStamp = msg.sensorStamp;
    roadMsg->roadType = msg.roadType;
    roadMsg->weatherType = msg.weatherType;
    roadMsg->timeType = msg.timeType;
  }
}

void AIObjectTypeTrigger::EnableFlag() {
  // 初次处理，初始化条件判断，根据输入条件决定算子内部使能功能
  if (!conditionChecker->parse(trigger_obj_->triggerCondition)) {
    LOG_ERROR("AIObjectTypeTrigger: %s", 
              conditionChecker->last_error().c_str());
    return;
  }

  auto elmes = conditionChecker->get_elements();
  std::string strTypes = "";  // 获取当前算子需要监测的类别
  for (auto & elem : elmes) {
    if (debug) {
      LOG_INFO("Variable: %s | Operator: %s | Threshold: %s | Logic: %s", 
                elem.variable.c_str(), elem.comparison_op.c_str(), 
                elem.threshold_str().c_str(), 
                (elem.logical_op.empty() ? "Empty" : elem.logical_op).c_str());
    }

    if (elem.variable == "objTag") {
      strTypes = elem.threshold_str().c_str();
    }
  }

  if (strTypes == "") {
    LOG_ERROR("[AIObjectTypeTrigger] can not parse target types!");
    exit(-1);
  }

  targetTypeIdx = static_cast<int>(std::stod(strTypes));
  LOG_INFO("[AIObjectTypeTrigger] target types: %d", targetTypeIdx);

  containAny = (targetTypeIdx & 1) == 0 ? true : false;
  LOG_INFO("[AIObjectTypeTrigger] containAny: %d", containAny);

  enableFlag = true;
}

void AIObjectTypeTrigger::JudgeObjectType() {
  // 算子条件解析
  EnableFlag();

  // 判断是否同步完成
  if (visionMsg->stamp == -1 || roadMsg->stamp == -1
    || abs(visionMsg->stamp - roadMsg->stamp) > frameSyncThresh) {
    LOG_WARN("[AIObjectTypeTrigger] wait sync ...");
    return;
  } else {
    std::unique_lock<std::mutex> lck(msgMtx);
    // 统计所有检测类型的名字
    for (int i = 0; i < visionMsg->validCnt; i++) {
      auto curId = visionMsg->result[i].classId;
      if (classTypeMap.find(curId) != classTypeMap.end()) {
        int mapId = classTypeMap[curId];
        if (std::find(inputIds.begin(), inputIds.end(), mapId) == inputIds.end()) {
          inputIds.push_back(mapId);
        }
      }
    }

    // 统计所有路况情况
    if (roadMsg->weatherType != EMWeatherType::WEATHER_UNSET) {
      inputIds.push_back(weatherTypeMap[roadMsg->weatherType]);
    }

    if (roadMsg->roadType != EMRoadType::ROAD_UNSET) {
      inputIds.push_back(roadTypeMap[roadMsg->roadType]);
    }

    if (roadMsg->timeType != EMTimeType::TIME_UNSET) {
      inputIds.push_back(timeTypeMap[roadMsg->timeType]);
    }
    
    if (debug) {
      LOG_INFO("[AIObjectTypeTrigger] inputIds: %zu", inputIds.size());
      for (size_t i = 0; i < inputIds.size(); i++) {
        LOG_INFO("[AIObjectTypeTrigger] get type %d", inputIds[i]);
      }
    }

    // 清空(通过时间戳判断是否同步），等待下一帧同步
    visionMsg->stamp = -1;
    roadMsg->stamp = -1;
  }
}

}  // namespace shadow::trigger