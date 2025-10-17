//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 Synaptix AI. All rights reserved.
//

#ifndef HPATakeover_TRIGGER_H
#define HPATakeover_TRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"
#include "ad_msg_idl/avp_parking_cmd/parking_cmd.capnp.h"
static bool hpatakeover_ = false;
namespace shadow {
namespace trigger {


class HPATakeoverTrigger : public TriggerBase {
public:
    HPATakeoverTrigger(const std::shared_ptr<senseAD::rscl::comm::Node>& node)
        :node_ptr_(node), trigger_name_("HPATakeoverTrigger") {}
    ~HPATakeoverTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
    void NotifyTriggerContext(const TriggerContext& context) override;
    std::string GetTriggerName() const override { return trigger_name_; }
    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override;

private:
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void UpdateParkingReport(const senseAD::msg::avp_state_v3::UicFsmCmd::Reader& reader);

    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_{nullptr};
    senseAD::rscl::comm::SubscriberBase::Ptr suber_;
    std::mutex mutex_;
    std::string trigger_name_;
    TriggerConditionChecker trigger_checker_;
    bool debug = true;
    bool triggerStatus = false;
    // bool hpatakeover_ = false;

};

REGISTER_TRIGGER(HPATakeoverTrigger)

} // namespace trigger
} // namespace shadow
#endif //HNOPTakeoverTrigger_TRIGGER_H
