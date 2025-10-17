//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 T3CAIC. All rights reserved.
//

#ifndef EMERGENCY_BRAKING_TRIGGER_H
#define EMERGENCY_BRAKING_TRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"

static float deceleration_ = 0.0;
namespace shadow {
namespace trigger {

class EmergencyBrakingTrigger : public TriggerBase {
public:
    EmergencyBrakingTrigger(const std::shared_ptr<senseAD::rscl::comm::Node>& node)
        :node_ptr_(node),trigger_name_("EmergencyBrakingTrigger"){}
    ~EmergencyBrakingTrigger() override = default;

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
    bool debug = true;
    bool triggerStatus = false;
    // float deceleration_ = 0.0;

};

REGISTER_TRIGGER(EmergencyBrakingTrigger)

} // namespace trigger
} // namespace shadow
#endif //EMERGENCY_BRAKING_TRIGGER_H
