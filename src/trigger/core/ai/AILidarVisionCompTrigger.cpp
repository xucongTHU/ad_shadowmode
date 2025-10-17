#include "trigger/core/ai/AILidarVisionCompTrigger.h"

#include <math.h>

#include "common/environment.h"
#include "common/time/Timer.h"

namespace shadow::trigger {

AILidarVisionCompTrigger::AILidarVisionCompTrigger(stoic::cm::NodeHandle& nh, 
                                                   bool fromConfig) {
  if (fromConfig) {
    // 默认配置文件路径
    std::string configPath = std::string(common::getInstallRootPath())
                            + "/config/ai_shadow_trigger_conf.yaml";
    LOG_INFO("[AILidarVisionCompTrigger] Init load configPath=%s", 
            configPath.c_str());
    
    // 加载配置文件
    YAML::Node configNode;
    try {
      configNode = YAML::LoadFile(configPath);
    } catch (std::exception &e) {
      LOG_ERROR("[AILidarVisionCompTrigger] Fail to load file %s, error code=%s",
                configPath.c_str(), e.what());
      exit(-1);
    }

    // debug标志
    common::loadNode<bool>(configNode, debug, "debug");

    // 读取当前算子的配置参数
    if (configNode[triggerName].IsDefined()) {
      auto triggerNode = configNode[triggerName];
      common::loadNode<std::string>(triggerNode, subVisionTopic, "subVisionTopic");
      common::loadNode<std::string>(triggerNode, subLidarTopic, "subLidarTopic");
      common::loadNode<int>(triggerNode, pubRate, "pubRate");
      common::loadNode<int>(triggerNode, frameSyncThresh, "frameSyncThresh");
      frameSyncThresh *= AI_TIME_CONVERSION;  // 统一为us
      int priority = 0;
      common::loadNode<int>(triggerNode, priority, "priority");
      triggerPriority = static_cast<int8_t>(priority);
    } else {
      LOG_WARN("[AILidarVisionCompTrigger] load config error, use default!");
    }
  }

  // debug 信息
  if (debug) {
    LOG_INFO("================ AILidarVisionCompTrigger Config =============");
    LOG_INFO("subVisionTopic:  %s", subVisionTopic.c_str());
    LOG_INFO("subLidarTopic:   %s", subLidarTopic.c_str());
    LOG_INFO("pubRate:         %d", pubRate);
    LOG_INFO("frameSyncThresh: %d", frameSyncThresh);
    LOG_INFO("triggerPriority: %d", triggerPriority);
    LOG_INFO("==============================================================");
  }

  // 定于视觉感知结果
  synxai::cm::idl::SubscriberConf conf_;
  conf_.set_pending_queue_size(1);
  AdapterManager<AIVisionModelOutput>::SubscriberBuilder(nh)
     .topic(subVisionTopic)
     .subConf(conf_)
     .subscribe(std::bind(&AILidarVisionCompTrigger::VisionCallback, this,
                std::placeholders::_1, std::placeholders::_2));

  // 订阅雷达感知结果
  AdapterManager<AILidarModelOutput>::SubscriberBuilder(nh)
     .topic(subLidarTopic)
     .subConf(conf_)
     .subscribe(std::bind(&AILidarVisionCompTrigger::LidarCallback, this,
                std::placeholders::_1, std::placeholders::_2));

  // 条件判断初始化
  conditionChecker = std::make_unique<TriggerConditionChecker>();
}

AILidarVisionCompTrigger::~AILidarVisionCompTrigger() {}

bool AILidarVisionCompTrigger::Proc() {
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
          LOG_INFO("[AILidarVisionCompTrigger] trigger status: TRUE");
        }
      }
    }
    rate.sleep();
  }

  return true;
}

bool AILidarVisionCompTrigger::CheckCondition() {
  // 判断视觉和雷达感知结果
  JudgeLidarVisionComp();

  // 如果没有要判断的条件，则直接范围（一般是消息还没同步）
  if (vars.empty()) {
    return triggerStatus;
  }

  // 条件判断
  triggerStatus = conditionChecker->check(vars);

  // 清空算子判断条件
  vars.clear();

  return triggerStatus;
}

void AILidarVisionCompTrigger::NotifyTriggerContext(TriggerContext context) {
    TriggerFactory::Instance().InvokeTriggerCallback(context);
    LOG_INFO("Trigger notified: %s (ID: %s, Time: %ld)",
             context.triggerName.c_str(), context.triggerId.c_str(), context.timeStamp);
}

std::string AILidarVisionCompTrigger::GetTriggerName() const {
  return triggerName;
}

int8_t AILidarVisionCompTrigger::GetPriority() const {
  return  triggerPriority;
}

bool  AILidarVisionCompTrigger::GetStatus() {
  std::unique_lock<std::mutex> lock(statusMtx);
  return triggerStatus;
}

void AILidarVisionCompTrigger::UpdateStatus(bool status) {
  std::unique_lock<std::mutex> lock(statusMtx);
  triggerStatus = status;
}

void AILidarVisionCompTrigger::UpdateTriggerEnable(int enable) {
  triggerEnable = enable;
}

void AILidarVisionCompTrigger::VisionCallback(const AIVisionModelOutput& msg, const std::string& topic) {
  if (debug) {
    LOG_INFO("[AILidarVisionCompTrigger] get perception msg (%s), stamp=%ld",
             topic.c_str(), msg.stamp);
  }
  
  // 更新消息
  std::unique_lock<std::mutex> lck(msgMtx);
  visionMsg->stamp = msg.stamp;
  visionMsg->sensorStamp = msg.sensorStamp;
  visionMsg->validCnt = msg.validCnt;
  memcpy(visionMsg->visionRes, msg.visionRes, sizeof(AIVisionTrackObject) * AI_MAX_TRACK_OBJ);
}

void AILidarVisionCompTrigger::LidarCallback(const AILidarModelOutput& msg, const std::string& topic) {
  if (debug) {
    LOG_INFO("[AILidarVisionCompTrigger] get lidar msg (%s), stamp=%ld",
             topic.c_str(), msg.stamp);
  }
  
  // 更新消息
  std::unique_lock<std::mutex> lck(msgMtx);
  lidarMsg->stamp = msg.stamp;
  lidarMsg->sensorStamp = msg.sensorStamp;
  lidarMsg->validCnt = msg.validCnt;
  memcpy(lidarMsg->result, msg.result, sizeof(AIDetectionLidar) * AI_MAX_TRACK_OBJ);
}

void AILidarVisionCompTrigger::EnableFlag() {
  // 初次处理，初始化条件判断，根据输入条件决定算子内部使能功能
  triggerEnable = 0;
  if (!conditionChecker->parse(trigger_obj_->triggerCondition)) {
    LOG_ERROR("AILidarVisionCompTrigger: %s", conditionChecker->last_error().c_str());
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
  
  // LVObjectCntDiff, LVObjectCntDiff -> 更新当前算子内部使能状态
  for (auto iter = strEnableMap.begin(); iter != strEnableMap.end(); iter++) {
    if (trigger_obj_->triggerCondition.find(iter->first) != std::string::npos) {
      triggerEnable |= iter->second;
    }
  }

  LOG_INFO("[AISkipFrameTrigger] triggerEnable = %d", triggerEnable);
  
  enableFlag = true;
}

void AILidarVisionCompTrigger::JudgeLidarVisionComp() {
  // 算子条件解析
  EnableFlag();

  std::unique_lock<std::mutex> lck(msgMtx);
  // 判断消息是否同步
  if (visionMsg->stamp != -1 && lidarMsg->stamp != -1 
    && abs(visionMsg->stamp - lidarMsg->stamp) < frameSyncThresh) {
    // 感知个数
    if (triggerEnable & TRIGGER_PERCEPTION_CNT) {
      double cntDiff = static_cast<double>(visionMsg->validCnt != lidarMsg->validCnt);
      vars["LVObjectCntDiff"] = cntDiff;
      if (debug) {
        LOG_INFO("[AILidarVisionCompTrigger] LVObjectCntDiff = %f", cntDiff);
      }
    }

    // 位置差异
    if (triggerEnable & TRIGGER_POSITION_DIFF) {
      auto dist = JudgeMatchPairDistance(visionMsg, lidarMsg);
      vars["LVObjectPosDiff"] = dist;
      if (debug) {
        LOG_INFO("[AILidarVisionCompTrigger] LVObjectPosDiff = %f", dist);
      }
    }

    // 清空同步缓存，开始同步下一次
    visionMsg->stamp = -1;
    lidarMsg->stamp = -1;
  } else {
    LOG_WARN("[AILidarVisionCompTrigger] wait sync ...");
  }
}

double AILidarVisionCompTrigger::JudgeMatchPairDistance(const VisionPtr &visionMsg,
                                                        const LidarPtr &lidarMsg) {
  // 匹配视觉和雷达感知结果
  std::vector<std::vector<float> > graph;
  std::vector<std::pair<size_t, size_t> > matched;
  MatchVisionLidar(visionMsg, lidarMsg, graph, matched);

  if (debug) {
    LOG_INFO("  ---- lv compare result ----");
    for (size_t i = 0; i < matched.size(); ++i) {
      size_t vIdx = matched[i].first;
      size_t lIdx = matched[i].second;
      LOG_INFO("  vIdx=%zu, lIdx=%zu  dist: %f ", vIdx, lIdx, graph[vIdx][lIdx]);
    }
  }

  float maxDist = 0.0f;
  // 找到匹配上的目标的位置距离最大值
  for (size_t i = 0; i < matched.size(); ++i) {
    size_t vIdx = matched[i].first;
    size_t lIdx = matched[i].second;

    if (graph[vIdx][lIdx] > maxDist) {
      maxDist = graph[vIdx][lIdx];
    }
  }
  return static_cast<double>(maxDist);
}

void AILidarVisionCompTrigger::UpdateCosts(
    const std::vector<std::vector<float>>& graph,
    SecureMat<float>* costs) {
  size_t rows_size = graph.size();
  size_t cols_size = rows_size > 0 ? graph.at(0).size() : 0;

  costs->Resize(rows_size, cols_size);

  // 计算每两个目标之前的距离
  for (size_t row_idx = 0; row_idx < rows_size; ++row_idx) {
    for (size_t col_idx = 0; col_idx < cols_size; ++col_idx) {
      (*costs)(row_idx, col_idx) = graph.at(row_idx).at(col_idx);
    }
  }
}

void AILidarVisionCompTrigger::MatchVisionLidar(
    const VisionPtr &visionMsg, const LidarPtr &lidarMsg,
    std::vector<std::vector<float> > &distanceGraph,
    std::vector<std::pair<size_t, size_t> > &matched) {
  // 初始化距离矩阵
  distanceGraph.clear();
  distanceGraph.resize(visionMsg->validCnt);
  for (int i = 0; i < static_cast<int>(visionMsg->validCnt); ++i) {
    auto vPos = visionMsg->visionRes[i].position;
    distanceGraph[i].resize(lidarMsg->validCnt);
    for (int j = 0; j < static_cast<int>(lidarMsg->validCnt); ++j) {
      auto lPos = lidarMsg->result[j].position;
      // L2车体
      float dist = std::sqrt(
          std::pow(vPos[0] - lPos[0], 2) + std::pow(vPos[1] - lPos[1], 2));
      distanceGraph[i][j] = dist;
    }
  }
  // 获取雷达和视觉匹配结果
  matched.clear();
  UpdateCosts(distanceGraph, optimizer.costs());
  optimizer.Minimize(&matched);  // 按照全局最小距离cost来匹配目标
}

}  // namespace shadow::trigger