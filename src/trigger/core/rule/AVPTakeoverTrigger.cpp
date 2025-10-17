//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 Synaptix AI. All rights reserved.
//

#include "AVPTakeoverTrigger.h"

#include "common/time/Timer.h"

namespace shadow {
namespace trigger {

bool AVPTakeoverTrigger::Proc() {
    bool ok = CheckCondition();
    CHECK_AND_RETURN(ok, AVPTakeoverTrigger, "CheckCondition failed", false);

    TriggerContext context;
    context.timeStamp = common::Timer::now();
    context.triggerId = trigger_id_;
    context.triggerName = trigger_name_;
    context.businessType = business_type_;
    context.triggerStatus = TriggerContext::TriggerState::Triggered;
    NotifyTriggerContext(context);

    return ok;
}

void AVPTakeoverTrigger::OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) {
    if (topic == "/ad_pub_te st/parking") {
        UpdateVehicleInfo(msg);
    }

}

void AVPTakeoverTrigger::UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg) {
    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
            (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    
    auto parkingcmdReader = reader.getRoot<senseAD::msg::avp_state_v3::UicFsmCmd>();
    UpdateParkingReport(parkingcmdReader);
}

void AVPTakeoverTrigger::UpdateParkingReport(const senseAD::msg::avp_state_v3::UicFsmCmd::Reader& report) {
    avptakeover_ = (report.getAvpStatus() == senseAD::msg::avp_state_v3::AvpMainStatus::APA_ABORT);
}


bool AVPTakeoverTrigger::CheckCondition() {
    if (!trigger_checker_.parse(trigger_condition_)) {
        LOG_ERROR("[AVPTakeoverTrigger]: %s", trigger_checker_.last_error().c_str());
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
        vars["avptakeover"] = avptakeover_;
    }

    bool ok = trigger_checker_.check(vars);
    
    return ok;
}

void AVPTakeoverTrigger::NotifyTriggerContext(const TriggerContext& context) {
    if (factoryPtr_) {
        factoryPtr_->OnTriggerContext(context);
    }
    // LOG_INFO("Trigger notified: %s (ID: %s, Time: %ld)",
    //          context.triggerName.c_str(), context.triggerId.c_str(), context.timeStamp);
}

} // namespace trigger
} // namespace shadow
