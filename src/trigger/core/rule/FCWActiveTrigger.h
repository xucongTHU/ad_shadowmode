//
// Created by xucong on 24-11-27.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef FCWACTIVE_TRIGGER_H
#define FCWACTIVE_TRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"
#include "ad_msg_idl/ad_tap/as_control.capnp.h"
#include "ad_msg_idl/ad_planning/nop_hmi.capnp.h"
#include "ad_msg_idl/ad_tap/as_command.capnp.h"

static bool fcw_active = false;
namespace shadow {
namespace trigger {

class FCWActiveTrigger : public TriggerBase {
public:
     FCWActiveTrigger(): trigger_name_("FCWActiveTrigger") {}
    ~FCWActiveTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
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
    // bool fcw_active = false;

};

REGISTER_TRIGGER(FCWActiveTrigger)

} // namespace trigger
} // namespace shadow
#endif //HNOPTakeoverTrigger_TRIGGER_H
