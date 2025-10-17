//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 T3CAIC. All rights reserved.
//

#ifndef CURVEDETECTOR_TRIGGER_H
#define CURVEDETECTOR_TRIGGER_H

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"
static float curve_acceleraton_y_ = 0.0;
static float curve_yaw_rate_ = 0.0;
namespace shadow {
namespace trigger {

class CurveDynamicDetectionTrigger : public TriggerBase {
public:
    CurveDynamicDetectionTrigger(const std::shared_ptr<senseAD::rscl::comm::Node>& node)
        : node_ptr_(node), trigger_name_("CurveDynamicDetectionTrigger"){};
    ~CurveDynamicDetectionTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
    void NotifyTriggerContext(const TriggerContext& context) override;
    std::string GetTriggerName() const override { return trigger_name_; }
    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override;

private:
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void UpdateChassisReport(const gac::rscl::ChassisReportMsg::Reader& reader);
    void UpdateVehicleReport(const senseAD::msg::vehicle::VehicleReport::Reader& report);


    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_{nullptr};
    std::mutex mutex_;
    std::string trigger_name_;
    TriggerConditionChecker trigger_checker_;
    bool debug = false;
    bool triggerStatus = false;
    // float curve_acceleraton_y_ = 0.0;
    // float curve_yaw_rate_ = 0.0;

};

REGISTER_TRIGGER(CurveDynamicDetectionTrigger)

} // namespace trigger
} // namespace shadow
#endif //CURVEDETECTOR_TRIGGER_H
