//
// Created by xucong on 25-5-8.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "SlopeDetectionTrigger.h"

#include "common/time/Timer.h"

namespace shadow {
namespace trigger {
bool SlopeDetectionTrigger::Proc() {
    bool ok = CheckCondition();
    CHECK_AND_RETURN(ok, SlopeDetectionTrigger, "CheckCondition failed", false);

    TriggerContext context;
    context.timeStamp = common::Timer::now();
    context.triggerId = trigger_id_;
    context.triggerName = trigger_name_;
    context.businessType = business_type_;
    context.triggerStatus = TriggerContext::TriggerState::Triggered;
    NotifyTriggerContext(context);

    return ok;
}


void SlopeDetectionTrigger::OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) {
    if (topic == "/caic_pub_test/snake") {
        UpdateVehicleInfo(msg);
    }

}

void SlopeDetectionTrigger::UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg) {
    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
            (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);


    auto vehicleReportReader = reader.getRoot<senseAD::msg::vehicle::VehicleReport>();
    UpdateVehicleReport(vehicleReportReader);
}

void SlopeDetectionTrigger::UpdateVehicleReport(const senseAD::msg::vehicle::VehicleReport::Reader& report) {
    slope_throttle_ = report.getThrottle().getPedalActual();
    slope_pitch_angle_ = 5.0; 
}

bool SlopeDetectionTrigger::CheckCondition() {
    if (!trigger_checker_.parse(trigger_condition_)) {
        LOG_ERROR("[SlopeDetectionTrigger]: %s", trigger_checker_.last_error().c_str());
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
        vars["throttle"] = slope_throttle_;
        vars["pitch_angle"] = slope_pitch_angle_;
    }

    bool ok = trigger_checker_.check(vars);
    
    return ok;
}

} // namespace trigger
} // namespace shadow

