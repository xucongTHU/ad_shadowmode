//
// Created by xucong on 25-5-8.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "EmergencyEvasionTrigger.h"

#include "common/time/Timer.h"

namespace shadow {
namespace trigger {
bool EmergencyEvasionTrigger::Proc() {
    bool ok = CheckCondition();
    CHECK_AND_RETURN(ok, EmergencyEvasionTrigger, "CheckCondition failed", false);

    TriggerContext context;
    context.timeStamp = common::Timer::now();
    context.triggerId = trigger_id_;
    context.triggerName = trigger_name_;
    context.businessType = business_type_;
    context.triggerStatus = TriggerContext::TriggerState::Triggered;
    NotifyTriggerContext(context);

    return ok;
}

void EmergencyEvasionTrigger::OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) {
    if (topic == "/caic_pub_test/EmergencyEvasionTrigger") {
        UpdateVehicleInfo(msg);
    }

}

void EmergencyEvasionTrigger::UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg) {
    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
            (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto vehicleReportReader = reader.getRoot<senseAD::msg::vehicle::VehicleReport>();
    auto chassisReportReader = reader.getRoot<gac::rscl::ChassisReportMsg>();

    UpdateChassisReport(chassisReportReader);
}

void EmergencyEvasionTrigger::UpdateChassisReport(const gac::rscl::ChassisReportMsg::Reader& report) {
    evasion_steering_rate_ = report.getSteer().getSteerAngleRate().getValue();
    evasion_yaw_rate_ = report.getEgoMotion().getYawRate().getValue();
   evasion_mastercylinderpressure_ = report.getBrake().getMastCylinderPressure().getValue();
}

bool EmergencyEvasionTrigger::CheckCondition() {
    if (!trigger_checker_.parse(trigger_condition_)) {
        LOG_ERROR("[EmergencyEvasionTrigger]: %s", trigger_checker_.last_error().c_str());
        return false;
    }

    if (debug) {
        auto elmes = trigger_checker_.get_elements();
        for (auto & elem : elmes) {
            LOG_INFO("Variable: %s | Operator: %s | Threshold: %s | Logic: %s",
                      elem.variable.c_str(), elem.comparison_op.c_str(),
                      elem.threshold_str().c_str(),
                      (elem.logical_op.empty() ? "Empty" : elem.logical_op).c_str());
        }
    }

    std::unordered_map<std::string, TriggerConditionChecker::Value> vars;
    {
        // std::lock_guard<std::mutex> lock(mutex_);
        vars["steering_rate"] = std::abs(evasion_steering_rate_);
        vars["yaw_rate"] = std::abs(evasion_yaw_rate_);
        vars["brake_pressure"] = evasion_mastercylinderpressure_;
    }

    bool ok = trigger_checker_.check(vars);
    
    return ok;
}


} // namespace trigger
} // namespace shadow

