#include "trigger/core/ai/AISkipFrameTrigger.h"

#include "common/environment.h"
#include "common/time/Timer.h"

namespace shadow::trigger {

AISkipFrameTrigger::AISkipFrameTrigger(bool fromConfig) {
  if (fromConfig) {
    // 默认配置文件路径
    std::string configPath = std::string(common::getInstallRootPath())
                            + "/config/ai_shadow_trigger_conf.yaml";
    LOG_INFO("[AISkipFrameTrigger] Init load configPath=%s",
            configPath.c_str());

    // 加载配置文件
    YAML::Node configNode;
    try {
      configNode = YAML::LoadFile(configPath);
    } catch (std::exception &e) {
      LOG_ERROR("[AISkipFrameTrigger] Fail to load file %s, error code=%s",
                configPath.c_str(), e.what());
      exit(-1);
    }

    // debug标志
    AILoadNode<bool>(configNode, debug, "debug");

    // 读取当前算子的配置参数
    if (configNode[triggerName].IsDefined()) {
      auto triggerNode = configNode[triggerName];
      AILoadNode<std::string>(triggerNode, subTopic, "subTopic");
      AILoadNode<int>(triggerNode, pubRate, "pubRate");
      AILoadNode<int>(triggerNode, lostAgeThresh, "lostAgeThresh");
      AILoadNode<int64_t>(triggerNode, visionTimeDuration, "visionTimeDuration");
      visionTimeDuration *= AI_TIME_CONVERSION;
      AILoadNode<int>(triggerNode, triggerEnable, "triggerEnable");
      int priority = 0;
      AILoadNode<int>(triggerNode, priority, "priority");
      triggerPriority = static_cast<int8_t>(priority);
    } else {
      LOG_WARN("[AISkipFrameTrigger] load config error, use default!");
    }
  }

  // debug 信息
  if (debug) {
    LOG_INFO("================ AISkipFrameTrigger Config =============");
    LOG_INFO("subTopic:           %s", subTopic.c_str());
    LOG_INFO("pubRate:            %d", pubRate);
    LOG_INFO("lostAgeThresh:      %d", lostAgeThresh);
    LOG_INFO("visionTimeDuration: %d", visionTimeDuration);
    LOG_INFO("triggerEnable:      %d", triggerEnable);
    LOG_INFO("triggerPriority:    %d", triggerPriority);
    LOG_INFO("=========================================================");
  }

  // 条件判断初始化
  conditionChecker = std::make_unique<TriggerConditionChecker>();
}

AISkipFrameTrigger::~AISkipFrameTrigger() { }

bool AISkipFrameTrigger::Proc() {
  bool ok = CheckCondition();
  CHECK_AND_RETURN(ok, AISkipFrameTrigger, "CheckCondition failed!", false);

  TriggerContext context;
  context.timeStamp = common::Timer::now();
  context.triggerId = trigger_id_;
  context.triggerName = GetTriggerName();
  context.businessType = business_type_;
  context.triggerStatus = TriggerContext::TriggerState::Triggered;
  NotifyTriggerContext(context);

  return ok;
}

bool AISkipFrameTrigger::CheckCondition() {
  // 如果没有要判断的条件，则直接范围（一般是消息还没同步）
  if (vars.empty()) {
    LOG_WARN("[AISkipFrameTrigger] inputIds is empty!");
    return false;
  }

  // 条件判断
  bool triggerStatus = conditionChecker->check(vars);

  // 清空当前需要判断的条件，等待下次
  vars.clear();

  if (debug) {
    LOG_INFO("[AISkipFrameTrigger] CheckCondition = %d", triggerStatus);
  }

  return triggerStatus;
}

std::string AISkipFrameTrigger::GetTriggerName() const {
  return triggerName;
}

void AISkipFrameTrigger::OnMessageReceived(
    const std::string& topic, const TRawMessagePtr& msg) {
  if (topic == subTopic) {
    if (debug) {
      LOG_INFO("[AISkipFrameTrigger] get perception msg (%s)", topic.c_str());
    }

    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
        (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto objectFrameReader = reader.getRoot<senseAD::msg::perception::ObjectFrame>();

    AIVisionModelOutput visionMsg;
    visionMsg.stamp = objectFrameReader.getHeader().getTime().getNanoSec();
    visionMsg.sensorStamp = objectFrameReader.getFrameTimestampNs();
    auto objectList = objectFrameReader.getPerceptionObjectList();
    visionMsg.validCnt = std::min(static_cast<int>(objectList.size()), AI_MAX_TRACK_OBJ);

    int index = 0;
    for (auto obj : objectList) {
      if (index >= AI_MAX_TRACK_OBJ) {
        break;
      }

      AIVisionTrackObject objInfo;
      objInfo.trackId = obj.getTrackId();
      objInfo.updateTime = obj.getLastMeasureUpdateTimestampNs();
      visionMsg.visionRes[index] = objInfo;
      index += 1;
    }

    if (debug) {
      LOG_INFO("[AISkipFrameTrigger] msg info : stamp=%u, validCnt=%d", visionMsg.stamp, visionMsg.validCnt);
    }

    JudgeSkipFrame(visionMsg);
  }
}

void AISkipFrameTrigger::EnableFlag() {
  // 初次处理，初始化条件判断，根据输入条件决定算子内部使能功能
  triggerEnable = 0;
  if (!conditionChecker->parse(trigger_obj_->triggerCondition)) {
    LOG_ERROR("AISkipFrameTrigger: %s", conditionChecker->last_error().c_str());
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

  // lostAge. trackTypeDiff, frameStampDiff -> 更新当前算子内部使能状态
  for (auto iter = strEnableMap.begin(); iter != strEnableMap.end(); iter++) {
    if (trigger_obj_->triggerCondition.find(iter->first) != std::string::npos) {
      triggerEnable |= iter->second;
    }
  }
  LOG_INFO("[AISkipFrameTrigger] triggerEnable = %d", triggerEnable);

  enableFlag = true;
}

void AISkipFrameTrigger::JudgeSkipFrame(const AIVisionModelOutput &msg) {
  // 算子条件更新
  EnableFlag();

  // 目标信息检测
  {
    // lostAge
    if (triggerEnable & TRIGGER_LOST_AGE) {
      double lost = static_cast<double>(JudgePercetpionLost(msg));
      vars["lostAge"] = lost;
      if (debug) {
        LOG_INFO("[AISkipFrameTrigger] lostAge = %f", lost);
      }
    }

    // trackTypeDiff
    if (triggerEnable & TRIGGER_OBJECT_INFO) {
      double typeDiff = static_cast<double>(JudgePercetpionType(msg));
      vars["trackTypeDiff"] = typeDiff;
      if (debug) {
        LOG_INFO("[AISkipFrameTrigger] trackTypeDiff = %f", typeDiff);
      }
    }

    // frameStampDiff
    if (triggerEnable & TRIGGER_TIME_DURATION) {
      // ms
      double timeDiff = static_cast<double>((msg.stamp - latestStamp) / AI_TIME_CONVERSION);
      timeDiff = latestStamp == 0 ? 0.0 : latestStamp;
      vars["frameStampDiff"] = timeDiff;
      if (debug) {
        LOG_INFO("[AISkipFrameTrigger] frameStampDiff = %f", timeDiff);
      }
    }
  }

  // 更新所有算子状态
  updateTrackStatus(msg);
  latestStamp = msg.stamp;
}

int AISkipFrameTrigger::JudgePercetpionLost(const AIVisionModelOutput &msg) {
  for (int idx = 0; idx < static_cast<int>(msg.validCnt); ++idx) {
    auto curObj = msg.visionRes[idx];
    int trackId = curObj.trackId;
    // 单条轨迹校验
    if (trackStatus.find(trackId) != trackStatus.end()) {
      // 时间跨度过大，或者 已经lost的轨迹，现在又重新跟踪上了
      auto track = trackStatus[trackId];
      if (track.latestStamp - msg.stamp > visionTimeDuration || track.lostAge > 0) {
        return 1;
      }
    }
  }

  return 0;
}

int AISkipFrameTrigger::JudgePercetpionType(const AIVisionModelOutput &msg) {
  for (int idx = 0; idx < static_cast<int>(msg.validCnt); ++idx) {
    auto curObj = msg.visionRes[idx];
    int trackId = curObj.trackId;
    // 单条轨迹判断
    if (trackStatus.find(trackId) != trackStatus.end()) {
      auto track = trackStatus[trackId];
      // 类型不一致
      if (track.classId != curObj.det2d.classId) {
        return 1;
      }
    }
  }

  return 0;
}

void AISkipFrameTrigger::updateTrackStatus(const AIVisionModelOutput &msg) {
  std::vector<int> updatedTrackId;
  for (int idx = 0; idx < static_cast<int>(msg.validCnt); ++idx) {
    auto curObj = msg.visionRes[idx];
    int trackId = curObj.trackId;

    if (trackStatus.find(trackId) != trackStatus.end()) {
      // 如果类型不一致，则不更新
      if (trackStatus[trackId].classId != curObj.det2d.classId) {
        trackStatus[trackId].latestStamp = msg.stamp;
        trackStatus[trackId].trackAge += 1;
        trackStatus[trackId].lostAge = 0;
      }
    } else {
      TrackStaus newState;
      newState.trackId = trackId;
      newState.latestStamp = msg.stamp;
      newState.beginStamp = msg.stamp;          // 仅初始化赋值
      newState.trackAge = 1;
      newState.lostAge = 0;
      newState.trackId = curObj.det2d.classId;  // 仅初始化赋值
      trackStatus[trackId] = newState;
    }

    updatedTrackId.push_back(trackId);
  }

  // 更新丢失轨迹状态
  for (auto iter = trackStatus.begin(); iter != trackStatus.end(); ++iter) {
    // 如果当前帧，轨迹已经更新，则跳过
    auto findIter = std::find(updatedTrackId.begin(), updatedTrackId.end(), iter->first);
    if (findIter != updatedTrackId.end()) {
      continue;
    }

    // 更新丢失轨迹信息
    iter->second.trackAge = 0;
    iter->second.lostAge += 1;
  }

  // 删除永久消失轨迹（超过阈值）
  for (auto iter = trackStatus.begin(); iter != trackStatus.end();) {
    if (iter->second.lostAge > lostAgeThresh) {
      printf("remove lost track: %d\n", iter->first);
      iter = trackStatus.erase(iter);
    } else {
      ++iter;
    }
  }
}

}  // namespace shadow::trigger