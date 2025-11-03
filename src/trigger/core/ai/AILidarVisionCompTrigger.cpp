#include "trigger/core/ai/AILidarVisionCompTrigger.h"

#include <math.h>

#include "common/environment.h"
#include "common/time/Timer.h"

namespace shadow::trigger {

AILidarVisionCompTrigger::AILidarVisionCompTrigger(bool fromConfig) {
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
    AILoadNode<bool>(configNode, debug, "debug");

    // 读取当前算子的配置参数
    if (configNode[triggerName].IsDefined()) {
      auto triggerNode = configNode[triggerName];
      AILoadNode<std::string>(triggerNode, subVisionTopic, "subVisionTopic");
      AILoadNode<std::string>(triggerNode, subLidarTopic, "subLidarTopic");
      AILoadNode<int>(triggerNode, pubRate, "pubRate");
      AILoadNode<int>(triggerNode, frameSyncThresh, "frameSyncThresh");
      frameSyncThresh *= AI_TIME_CONVERSION;  // 统一为us
      int priority = 0;
      AILoadNode<int>(triggerNode, priority, "priority");
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

  // 条件判断初始化
  conditionChecker = std::make_unique<TriggerConditionChecker>();
}

AILidarVisionCompTrigger::~AILidarVisionCompTrigger() {}

bool AILidarVisionCompTrigger::Proc() {
  bool ok = CheckCondition();
  CHECK_AND_RETURN(ok, AILidarVisionCompTrigger, "CheckCondition failed!", false);

  TriggerContext context;
  context.timeStamp = common::Timer::now();
  context.triggerId = trigger_id_;
  context.triggerName = GetTriggerName();
  context.businessType = business_type_;
  context.triggerStatus = TriggerContext::TriggerState::Triggered;
  NotifyTriggerContext(context);

  return ok;
}

bool AILidarVisionCompTrigger::CheckCondition() {
  // 判断视觉和雷达感知结果
  JudgeLidarVisionComp();

  // 如果没有要判断的条件，则直接范围（一般是消息还没同步）
  if (vars.empty()) {
    LOG_WARN("[AILidarVisionCompTrigger] inputIds is empty!");
    return false;
  }

  // 条件判断
  bool triggerStatus = conditionChecker->check(vars);

  // 清空算子判断条件
  vars.clear();

  if (debug) {
    LOG_INFO("[AILidarVisionCompTrigger] CheckCondition = %d", triggerStatus);
  }

  return triggerStatus;
}

std::string AILidarVisionCompTrigger::GetTriggerName() const {
  return triggerName;
}

void AILidarVisionCompTrigger::OnMessageReceived(
    const std::string& topic, const TRawMessagePtr& msg) {
  // vision 消息处理
  if (topic == subVisionTopic) {
    if (debug) {
      LOG_INFO("[AILidarVisionCompTrigger] get perception msg (%s)", topic.c_str());
    }

    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
        (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto objectFrameReader = reader.getRoot<senseAD::msg::perception::ObjectFrame>();
    uint64_t stamp = objectFrameReader.getHeader().getTime().getNanoSec();
    uint64_t sensorStamp = objectFrameReader.getFrameTimestampNs();
    auto objectList = objectFrameReader.getPerceptionObjectList();

    AIVisionTrackObject visionRes[AI_MAX_TRACK_OBJ];
    int index = 0;
    for (auto obj : objectList) {
      if (index >= AI_MAX_TRACK_OBJ) {
        break;
      }

      // 仅保留通用障碍物输出
      // 1 行人
      // 2 车辆
      // 14 两轮车
      auto label = obj.getLabel();
      if (label != 1 && label != 2 && label != 14) {
        continue;
      }

      auto centerReader = obj.getMotionInfo().getCenter();
      visionRes[index].position[0] = static_cast<float>(centerReader.getX());
      visionRes[index].position[1] = static_cast<float>(centerReader.getY());
      visionRes[index].position[2] = static_cast<float>(centerReader.getZ());
      index += 1;
    }

    if (debug) {
      LOG_INFO("[AILidarVisionCompTrigger] vision msg info: stamp=%u, validCnt=%d", stamp, index);
    }

    // 更新内部消息
    {
      std::unique_lock<std::mutex> lock(msgMtx);
      visionMsg->stamp = stamp;
      visionMsg->sensorStamp = sensorStamp;
      visionMsg->validCnt = static_cast<uint8_t>(index);
      memcpy(visionMsg->visionRes, visionRes, sizeof(AIVisionTrackObject) * AI_MAX_TRACK_OBJ);
    }
  }

  // lidar 消息处理
  if (topic == subLidarTopic) {
    if (debug) {
      LOG_INFO("[AILidarVisionCompTrigger] get perception msg (%s)", topic.c_str());
    }

    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
        (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto objectFrameReader = reader.getRoot<senseAD::msg::perception::ObjectFrame>();
    uint64_t stamp = objectFrameReader.getHeader().getTime().getNanoSec();
    uint64_t sensorStamp = objectFrameReader.getFrameTimestampNs();
    auto objectList = objectFrameReader.getPerceptionObjectList();

    AIDetectionLidar lidarRes[AI_MAX_TRACK_OBJ];
    int index = 0;
    for (auto obj : objectList) {
      if (index >= AI_MAX_TRACK_OBJ) {
        break;
      }

      // 仅保留通用障碍物输出
      // 1 车辆
      // 2 行人
      // 3 两轮车
      auto label = obj.getLabel();
      if (label != 1 && label != 2 && label != 3) {
        continue;
      }

      auto centerReader = obj.getMotionInfo().getCenter();
      lidarRes[index].position[0] = static_cast<float>(centerReader.getX());
      lidarRes[index].position[1] = static_cast<float>(centerReader.getY());
      lidarRes[index].position[2] = static_cast<float>(centerReader.getZ());
      index += 1;
    }

    if (debug) {
      LOG_INFO("[AILidarVisionCompTrigger] lidar msg info: stamp=%u, validCnt=%d", stamp, index);
    }

    // 更新内部消息
    {
      std::unique_lock<std::mutex> lock(msgMtx);
      lidarMsg->stamp = stamp;
      lidarMsg->sensorStamp = sensorStamp;
      lidarMsg->validCnt = static_cast<uint8_t>(index);
      memcpy(lidarMsg->lidarRes, lidarRes, sizeof(AIVisionTrackObject) * AI_MAX_TRACK_OBJ);
    }
  }
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
  auto timeDiff = (visionMsg->stamp > lidarMsg->stamp)
                ? (visionMsg->stamp - lidarMsg->stamp)
                : (lidarMsg->stamp - visionMsg->stamp);
  if (visionMsg->stamp != 0 && lidarMsg->stamp != 0 && timeDiff < frameSyncThresh) {
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
    visionMsg->stamp = 0;
    lidarMsg->stamp = 0;
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
      auto lPos = lidarMsg->lidarRes[j].position;
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