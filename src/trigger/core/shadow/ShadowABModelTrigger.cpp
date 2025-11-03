#include "trigger/core/shadow/ShadowABModelTrigger.h"

#include "common/environment.h"
#include "common/time/Timer.h"

namespace shadow::trigger {

ShadowABModelTrigger::ShadowABModelTrigger(bool fromConfig) {
  if (fromConfig) {
    // 默认配置文件路径
    std::string configPath = std::string(common::getInstallRootPath())
                            + "/config/ai_shadow_trigger_conf.yaml";
    LOG_INFO("[ShadowABModelTrigger] Init load configPath=%s",
            configPath.c_str());

    // 加载配置文件
    YAML::Node configNode;
    try {
      configNode = YAML::LoadFile(configPath);
    } catch (std::exception &e) {
      LOG_ERROR("[ShadowABModelTrigger] Fail to load file %s, error code=%s",
                configPath.c_str(), e.what());
      exit(-1);
    }

    // debug标志
    ShadowLoadNode<bool>(configNode, debug, "debug");

    // 读取当前算子的配置参数
    if (configNode[triggerName].IsDefined()) {
      auto triggerNode = configNode[triggerName];
      ShadowLoadNode<std::string>(triggerNode, subModelATopic, "subModelATopic");
      ShadowLoadNode<std::string>(triggerNode, subModelBTopic, "subModelBTopic");
      ShadowLoadNode<int>(triggerNode, pubRate, "pubRate");
      ShadowLoadNode<int64_t>(triggerNode, frameSyncThresh, "frameSyncThresh");
      frameSyncThresh *= SHADOW_TIME_CONVERSION;
      ShadowLoadNode<float>(triggerNode, bboxIouThresh, "bboxIouThresh");
      int priority = 0;
      ShadowLoadNode<int>(triggerNode, priority, "priority");
      triggerPriority = static_cast<int8_t>(priority);
    } else {
      LOG_WARN("[ShadowABModelTrigger] load config error, use default!");
    }
  }

  // debug 信息
  if (debug) {
    LOG_INFO("============= ShadowABModelTrigger Config ==========");
    LOG_INFO("subModelATopic:  %s", subModelATopic.c_str());
    LOG_INFO("subModelBTopic:  %s", subModelBTopic.c_str());
    LOG_INFO("pubRate:         %d", pubRate);
    LOG_INFO("frameSyncThresh: %d", frameSyncThresh);
    LOG_INFO("bboxIouThresh:   %f", bboxIouThresh);
    LOG_INFO("triggerPriority: %d", triggerPriority);
    LOG_INFO("=========================================================");
  }

  // 条件判断初始化
  conditionChecker = std::make_unique<TriggerConditionChecker>();
}

ShadowABModelTrigger::~ShadowABModelTrigger() { }

bool ShadowABModelTrigger::Proc() {
  bool ok = CheckCondition();
  CHECK_AND_RETURN(ok, ShadowABModelTrigger, "CheckCondition failed!", false);

  TriggerContext context;
  context.timeStamp = common::Timer::now();
  context.triggerId = trigger_id_;
  context.triggerName = GetTriggerName();
  context.businessType = business_type_;
  context.triggerStatus = TriggerContext::TriggerState::Triggered;
  NotifyTriggerContext(context);

  return ok;
}

bool ShadowABModelTrigger::CheckCondition() {
  // 计算AB模型条件
  JudgeABModel();

  // 如果没有要判断的条件，则直接返回
  if (vars.empty()) {
    LOG_WARN("[ShadowABModelTrigger] inputIds is empty!");
    return false;
  }

  // 条件判断
  bool triggerStatus = conditionChecker->check(vars);

  vars.clear();

  if (debug) {
    LOG_INFO("[ShadowABModelTrigger] CheckCondition = %d", triggerStatus);
  }

  return triggerStatus;
}

std::string ShadowABModelTrigger::GetTriggerName() const {
  return triggerName;
}

void ShadowABModelTrigger::OnMessageReceived(
    const std::string& topic, const TRawMessagePtr& msg) {
  if (topic == subModelATopic || topic == subModelBTopic) {
    if (debug) {
      LOG_INFO("[ShadowABModelTrigger] get perception msg (%s)", topic.c_str());
    }

    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
        (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto objectFrameReader = reader.getRoot<senseAD::msg::perception::ObjectFrame>();
    uint64_t stamp = objectFrameReader.getHeader().getTime().getNanoSec();
    uint64_t sensorStamp = objectFrameReader.getFrameTimestampNs();
    auto objectList = objectFrameReader.getPerceptionObjectList();

    ShadowDetection detRes[SHADOW_MAX_DETECT_OBJ];
    int index = 0;
    for (auto obj : objectList) {
      if (index >= SHADOW_MAX_DETECT_OBJ) {
        break;
      }

      // label
      detRes[index].classId = static_cast<uint8_t>(obj.getLabel());

      // bbox
      auto rawDetectionBox = obj.getCameraBboxInfo().getRawDetectionBox();
      detRes[index].bbox[0] = rawDetectionBox.getTopLeftX();
      detRes[index].bbox[1] = rawDetectionBox.getTopLeftY();
      detRes[index].bbox[2] = rawDetectionBox.getBottomRightX();
      detRes[index].bbox[3] = rawDetectionBox.getBottomRightY();
      detRes[index].conf = rawDetectionBox.getConfidence();
      index += 1;
    }

    if (debug) {
      LOG_INFO("[ShadowABModelTrigger] msg info: stamp=%u, validCnt=%d", stamp, index);
    }

    {
      std::unique_lock<std::mutex> lock(dataMtx);
      if (topic == subModelATopic) {
        modelA->stamp = stamp;
        modelA->sensorStamp = sensorStamp;
        modelA->validCnt = static_cast<uint8_t>(index);
        memcpy(modelA->detRes, detRes, SHADOW_MAX_DETECT_OBJ * sizeof(ShadowDetection));
      } else {
        modelB->stamp = stamp;
        modelB->sensorStamp = sensorStamp;
        modelB->validCnt = static_cast<uint8_t>(index);
        memcpy(modelB->detRes, detRes, SHADOW_MAX_DETECT_OBJ * sizeof(ShadowDetection));
      }
    }
  }
}

void ShadowABModelTrigger::EnableFlag() {
  // 初次处理，初始化条件判断，根据输入条件决定算子内部使能功能
  triggerEnable = 0;
  if (!conditionChecker->parse(trigger_obj_->triggerCondition)) {
    LOG_ERROR("ShadowABModelTrigger: %s", conditionChecker->last_error().c_str());
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

  // ObjectCntDiff, ObjectTypeDiff, ObjectIOU -> 更新当前算子内部使能状态
  for (auto iter = strEnableMap.begin(); iter != strEnableMap.end(); iter++) {
    if (trigger_obj_->triggerCondition.find(iter->first) != std::string::npos) {
      triggerEnable |= iter->second;
    }
  }

  LOG_INFO("[ShadowABModelTrigger] triggerEnable = %d", triggerEnable);

  enableFlag = true;
}

void ShadowABModelTrigger::JudgeABModel() {
  // 更新当前算子使能
  EnableFlag();

  // 判断是否同步
  auto timeDiff = (modelA->stamp > modelB->stamp)
                ? (modelA->stamp - modelB->stamp)
                : (modelB->stamp - modelA->stamp);
  if ((modelA->stamp != 0) && (modelB->stamp != 0) && timeDiff < frameSyncThresh) {
    std::unique_lock<std::mutex> lck(dataMtx);
    {
      // AB模型数量
      if (triggerEnable & TRIGGER_OBJECT_CNT) {
        double cntDiff = static_cast<double>(modelA->validCnt != modelB->validCnt);
        vars["objectCntDiff"] = cntDiff;

        if (debug) {
          LOG_INFO("[ShadowABModelTrigger] ObjectCntDiff = %f", cntDiff);
        }
      }

      // 结果匹配
      std::vector<std::vector<float> > graph;
      std::vector<std::pair<size_t, size_t> > matched;
      if (triggerEnable & TRIGGER_OBJECT_TYPE | triggerEnable & TRIGGER_OBJECTIOU) {
        MatchVisionObjects(modelA, modelB, graph, matched);
      }

      // 匹配目标类型
      if (triggerEnable & TRIGGER_OBJECT_TYPE) {
        double typeDiff = static_cast<double>(JudgeMatchPairType(modelA, modelB, matched));
        vars["objectTypeDiff"] = typeDiff;
        if (debug) {
          LOG_INFO("[ShadowABModelTrigger] ObjectTypeDiff = %f", typeDiff);
        }
      }

      // 匹配目标IOU
      if (triggerEnable & TRIGGER_OBJECTIOU) {
        double maxIOU = JudgeMatchPairIOU(modelA, modelB, graph, matched);
        vars["objectIOU"] = maxIOU;
        if (debug) {
          LOG_INFO("[ShadowABModelTrigger] ObjectIOU = %f", maxIOU);
        }
      }
    }

    // 清空同步的消息，开始同步下一帧
    modelA->stamp = 0;
    modelB->stamp = 0;
  } else {
    LOG_WARN("[ShadowABModelTrigger] wait sync ...");
  }
}

bool ShadowABModelTrigger::JudgeMatchPairType(
    const VisionPtr &vModelAMsg, const VisionPtr &vModelBMsg,
    const std::vector<std::pair<size_t, size_t> > &matched) {
  // 判断匹配目标的类型
  for (size_t i = 0; i < matched.size(); ++i) {
    size_t vIdx = matched[i].first;
    size_t lIdx = matched[i].second;
    // 如果存在不一致，则直接返回
    if (vModelAMsg->detRes[vIdx].classId != vModelBMsg->detRes[lIdx].classId) {
      return true;
    }
  }
  return false;
}
double ShadowABModelTrigger::JudgeMatchPairIOU(
    const VisionPtr &vModelAMsg, const VisionPtr &vModelBMsg,
    const std::vector<std::vector<float> > &graph,
    const std::vector<std::pair<size_t, size_t> > &matched) {

  // 计算最小iou
  double minIOU = 1.0;
  for (size_t i = 0; i < matched.size(); ++i) {
    size_t vIdx = matched[i].first;
    size_t lIdx = matched[i].second;
    if (graph[vIdx][lIdx] < minIOU) {
      minIOU = graph[vIdx][lIdx];
    }
  }
  return minIOU;
}

void ShadowABModelTrigger::UpdateCosts(
    const std::vector<std::vector<float> >& graph,
    SecureMat<float>* costs) {
  size_t rows_size = graph.size();
  size_t cols_size = rows_size > 0 ? graph.at(0).size() : 0;

  costs->Resize(rows_size, cols_size);

  // 更新iou矩阵，用于匹配AB模型结果
  for (size_t row_idx = 0; row_idx < rows_size; ++row_idx) {
    for (size_t col_idx = 0; col_idx < cols_size; ++col_idx) {
      (*costs)(row_idx, col_idx) = graph.at(row_idx).at(col_idx);
    }
  }
}

float ShadowABModelTrigger::CalcIou(const float (&bbox1)[4],
                                    const float (&bbox2)[4]) {
  // 没有相交区域直接返回
  if (bbox1[SHADOW_BBOX_L_INDEX] > bbox2[SHADOW_BBOX_R_INDEX]
      || bbox1[SHADOW_BBOX_R_INDEX] < bbox2[SHADOW_BBOX_L_INDEX]
      || bbox1[SHADOW_BBOX_T_INDEX] > bbox2[SHADOW_BBOX_B_INDEX]
      || bbox1[SHADOW_BBOX_B_INDEX] < bbox2[SHADOW_BBOX_T_INDEX]) {
    return 0.0f;
  }

  // 计算交集
  float iouLeft = std::max(bbox1[SHADOW_BBOX_L_INDEX],
                           bbox2[SHADOW_BBOX_L_INDEX]);
  float iouRight = std::min(bbox1[SHADOW_BBOX_R_INDEX],
                            bbox2[SHADOW_BBOX_R_INDEX]);
  float iouTop = std::max(bbox1[SHADOW_BBOX_T_INDEX],
                          bbox2[SHADOW_BBOX_T_INDEX]);
  float iouBottom = std::min(bbox1[SHADOW_BBOX_B_INDEX],
                             bbox2[SHADOW_BBOX_B_INDEX]);
  // 计算两个矩形面积
  float areaA = (bbox1[SHADOW_BBOX_R_INDEX] - bbox1[SHADOW_BBOX_L_INDEX])
              * (bbox1[SHADOW_BBOX_B_INDEX] - bbox1[SHADOW_BBOX_T_INDEX]);
  float areaB = (bbox2[SHADOW_BBOX_R_INDEX] - bbox2[SHADOW_BBOX_L_INDEX])
              * (bbox2[SHADOW_BBOX_B_INDEX] - bbox2[SHADOW_BBOX_T_INDEX]);

  // 计算iou
  float areaIou = (iouRight - iouLeft) * (iouBottom - iouTop);
  return areaIou / (areaA + areaB - areaIou);
}

void ShadowABModelTrigger::MatchVisionObjects(
    const VisionPtr &vModelAMsg, const VisionPtr &vModelBMsg,
    std::vector<std::vector<float> > &distanceGraph,
    std::vector<std::pair<size_t, size_t> > &matched) {
  distanceGraph.resize(vModelAMsg->validCnt);
  for (int i = 0; i < static_cast<int>(vModelAMsg->validCnt); ++i) {
    distanceGraph[i].resize(vModelBMsg->validCnt);
    for (int j = 0; j < static_cast<int>(vModelBMsg->validCnt); ++j) {
      // 计算iou
      float dist = CalcIou(vModelAMsg->detRes[i].bbox,
                           vModelBMsg->detRes[j].bbox);
      distanceGraph[i][j] = dist;
    }
  }
  // 得到匹配结果
  matched.clear();
  UpdateCosts(distanceGraph, optimizer.costs());
  optimizer.Maximize(&matched);
}
}  // namespace shadow::trigger