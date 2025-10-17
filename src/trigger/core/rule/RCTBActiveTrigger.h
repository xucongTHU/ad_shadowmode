//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 Synaptix AI. All rights reserved.
//

#ifndef RCTBACTIVE_TRIGGER_H
#define RCTBACTIVE_TRIGGER_H


#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"
#include "ad_msg_idl/ad_tap/as_command.capnp.h"

static bool rctb_active_ = false;
namespace shadow {
namespace trigger {

class RCTBActiveTrigger : public TriggerBase {
public:
    RCTBActiveTrigger(const std::shared_ptr<senseAD::rscl::comm::Node>& node)
        :node_ptr_(node), trigger_name_("RCTBActiveTrigger") {}
    ~RCTBActiveTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
    void NotifyTriggerContext(const TriggerContext& context) override;
    std::string GetTriggerName() const override { return trigger_name_; }
    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override;

private:
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void UpdateASStateMachine(const senseAD::msg::tap::AsCmdLgSafe::Reader& report);

    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_{nullptr};
    senseAD::rscl::comm::SubscriberBase::Ptr suber_;
    std::mutex mutex_;
    std::string trigger_name_;
    TriggerConditionChecker trigger_checker_;
    bool debug = true;
    bool triggerStatus = false;
    // bool rctb_active_ = false;

};

REGISTER_TRIGGER(RCTBActiveTrigger)

} // namespace trigger
} // namespace shadow
#endif //HNOPTakeoverTrigger_TRIGGER_H
