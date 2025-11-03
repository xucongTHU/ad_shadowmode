//
// Created by xucong on 24-11-27.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef ACCQuit_TRIGGER_H
#define ACCQuit_TRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_planning/planning.capnp.h"

namespace shadow {
namespace trigger {


class ACCQuitTrigger : public TriggerBase {
public:
    ACCQuitTrigger(): trigger_name_("ACCQuitTrigger"){};
    ~ACCQuitTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
    std::string GetTriggerName() const override { return trigger_name_; }
    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override;

private:
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void Updateplanning(const senseAD::msg::planning::MCUStateMachineInfo::Reader& report);
    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_{nullptr};
    std::mutex mutex_;
    std::string trigger_name_;
    TriggerConditionChecker trigger_checker_;
    bool debug = false;
    bool triggerStatus = false;
    std::atomic<bool> acc_flag_{false};
    // bool accquit_ = false;
};

REGISTER_TRIGGER(ACCQuitTrigger)

} // namespace trigger
} // namespace shadow
#endif //ACCQuit_TRIGGER_H
