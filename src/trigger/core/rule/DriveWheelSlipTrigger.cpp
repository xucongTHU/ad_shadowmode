//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 Synaptix AI. All rights reserved.
//

#include "DriveWheelSlipTrigger.h"

#include "common/time/Timer.h"

namespace shadow {
namespace trigger {

bool DriveWheelSlipTrigger::Proc() {
    bool ok = CheckCondition();
    CHECK_AND_RETURN(ok, DriveWheelSlipTrigger, "CheckCondition failed", false);

    TriggerContext context;
    context.timeStamp = common::Timer::now();
    context.triggerId = trigger_id_;
    context.triggerName = trigger_name_;
    context.businessType = business_type_;
    context.triggerStatus = TriggerContext::TriggerState::Triggered;
    NotifyTriggerContext(context);

    return ok;
}

void DriveWheelSlipTrigger::OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) {
    if (topic == "/ad_pub_test/snake") {
        UpdateVehicleInfo(msg);
    }

}

void DriveWheelSlipTrigger::UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg) {
    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
            (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto vehiclereport = reader.getRoot<senseAD::msg::vehicle::VehicleReport>();
    UpdateVehicleReport(vehiclereport);
}

void DriveWheelSlipTrigger::UpdateVehicleReport(const senseAD::msg::vehicle::VehicleReport::Reader& report) {
    is_slip_ = (report.getChassis().getWheelSpeedRl() > 1.2 * report.getChassis().getWheelSpeedFl() || 
    report.getChassis().getWheelSpeedRr() > report.getChassis().getWheelSpeedFr());
}

bool DriveWheelSlipTrigger::CheckCondition() {
    if (!trigger_checker_.parse(trigger_condition_)) {
        LOG_ERROR("[DriveWheelSlipTrigger]: %s", trigger_checker_.last_error().c_str());
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
    vars["is_slip"] = is_slip_;

    bool ok = trigger_checker_.check(vars);
    
    return ok;
}

void DriveWheelSlipTrigger::NotifyTriggerContext(const TriggerContext& context) {
    if (factoryPtr_) {
        factoryPtr_->OnTriggerContext(context);
    }
    // LOG_INFO("Trigger notified: %s (ID: %s, Time: %ld)",
    //          context.triggerName.c_str(), context.triggerId.c_str(), context.timeStamp);
}

} // namespace trigger
} // namespace shadow
