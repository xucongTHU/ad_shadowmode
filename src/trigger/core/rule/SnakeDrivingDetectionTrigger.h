//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 T3CAIC. All rights reserved.
//

#ifndef SNAKEDRIVE_TRIGGER_H
#define SNAKEDRIVE_TRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"
static float snake_steering_angle_ = 0.0;
static float snake_vehiclespeed_ = 0.0;
namespace shadow {
namespace trigger {

struct SnakeDriveData {
    double timestamp;  // 时间戳
    double steering_angle;  // 转向角度（度），用于计算蛇行驾驶的速度和方向。
    int direction; // 驾驶方向（1:右转，-1:左转,0:直行或停止）。用于确定蛇行的行驶路径和速度计算方式.
    double speed;
};

class SnakeDrivingDetectionTrigger : public TriggerBase{
public:
    SnakeDrivingDetectionTrigger(const std::shared_ptr<senseAD::rscl::comm::Node>& node)
        : node_ptr_(node), trigger_name_("SnakeDrivingDetectionTrigger") {}
    ~SnakeDrivingDetectionTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
    void NotifyTriggerContext(const TriggerContext& context) override;
    std::string GetTriggerName() const override { return trigger_name_; }
    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override;

private:
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void UpdateVehicleReport(const senseAD::msg::vehicle::VehicleReport::Reader& report);

    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_{nullptr};
    senseAD::rscl::comm::SubscriberBase::Ptr suber_;
    std::mutex mutex_;
    std::string trigger_name_;
    TriggerConditionChecker trigger_checker_;
    bool check_event_trigger_;
     bool EvaluateTrigger();
    std::deque<SnakeDriveData> data_window;  // 数据滑动窗口
    const size_t max_window_size = 1000;  // 最大缓存数据量
    double snake_check_condition_ = false;
    bool debug = true;
    bool triggerStatus = false;
    float steering_angle_ = 0.0;
    float vehiclespeed_ = 0.0;
};

REGISTER_TRIGGER(SnakeDrivingDetectionTrigger)

} // namespace trigger
} // namespace shadow
#endif //SNAKEDRIVE_TRIGGER_H
