//
// Created by xucong on 25-5-8.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_SLOPEDETECTIONTRIGGER_H
#define SHADOW_MODE_SLOPEDETECTIONTRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"

static float slope_throttle_ = 0.0;
static float slope_pitch_angle_ = 0.0;
namespace shadow {
namespace trigger {
class SlopeDetectionTrigger : public TriggerBase {
public:
    SlopeDetectionTrigger(): trigger_name_("SlopeDetectionTrigger") {}
    ~SlopeDetectionTrigger() = default;

    bool Proc() override;
    bool CheckCondition() override;
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
    bool debug = false;
    bool triggerStatus = false;
    // float throttle_ = 0.0;
    // float pitch_angle_ = 0.0;

};

REGISTER_TRIGGER(SlopeDetectionTrigger)

} // namespace trigger
} // namespace shadow

#endif //SHADOW_MODE_SLOPEDETECTIONTRIGGER_H
