//
// Created by xucong on 25-5-8.
// Copyright (c) 2025 T3CAIC. All rights reserved.
//

#ifndef SHADOW_MODE_EMERGENCYEVASIONTRIGGER_H
#define SHADOW_MODE_EMERGENCYEVASIONTRIGGER_H

#include "ad_rscl/ad_rscl.h"

#include "trigger/base/TriggerFactory.hpp"
#include "trigger/common/TriggerConditionChecker.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"

static float evasion_steering_rate_ = 0.0;
static float evasion_yaw_rate_ = 0.0;
static float evasion_mastercylinderpressure_ = 0.0;
namespace shadow {
namespace trigger {

struct EmergencyEvasionData {
    double timestamp;  // 时间戳
    double steeringspeed;
    double yawrate;
    double mastercylinderpressure;
};

static EmergencyEvasionData trigger_data_;
class EmergencyEvasionTrigger : public TriggerBase {
public:
    EmergencyEvasionTrigger(const std::shared_ptr<senseAD::rscl::comm::Node>& node):
        node_ptr_(node),trigger_name_("EmergencyEvasionTrigger"){}
    ~EmergencyEvasionTrigger() override = default;

    bool Proc() override;
    bool CheckCondition() override;
    void NotifyTriggerContext(const TriggerContext& context) override;
    std::string GetTriggerName() const override { return trigger_name_; }
    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override;

private:
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void UpdateChassisReport(const gac::rscl::ChassisReportMsg::Reader& reader);

    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_{nullptr};
    senseAD::rscl::comm::SubscriberBase::Ptr suber_;
    std::mutex mutex_;
    std::string trigger_name_;
    TriggerConditionChecker trigger_checker_;
    // float steering_rate_ = 0.0;
    // float yaw_rate_ = 0.0;
    // float mastercylinderpressure_ = 0.0;
    bool debug = false;
    bool triggerStatus = false;
    EmergencyEvasionData trigger_data_;

};

REGISTER_TRIGGER(EmergencyEvasionTrigger)

} // namespace trigger
} // namespace shadow


#endif //SHADOW_MODE_EMERGENCYEVASIONTRIGGER_H
