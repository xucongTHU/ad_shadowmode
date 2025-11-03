//
// Created by xucong on 24-11-27.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef MEB_TRIGGER_H
#define MEB_TRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"

static bool meb_active_ = false;
namespace shadow {
namespace trigger {


class MEBActiveTrigger : public TriggerBase{
public:
    MEBActiveTrigger(): trigger_name_("MEBActiveTrigger") {}
    ~MEBActiveTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
    std::string GetTriggerName() const override { return trigger_name_; }
    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override;

private:
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void UpdateVehicleCmd(const senseAD::msg::vehicle::VehicleCmd::Reader& report);

    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_{nullptr};
    senseAD::rscl::comm::SubscriberBase::Ptr suber_;
    std::mutex mutex_;
    std::string trigger_name_;
    TriggerConditionChecker trigger_checker_;
    bool check_event_trigger_;
    bool debug = true;
    bool triggerStatus = false;
    // bool meb_active_ = false;
};

REGISTER_TRIGGER(MEBActiveTrigger)

} // namespace trigger
} // namespace shadow
#endif //SNAKEDRIVE_TRIGGER_H
