//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 Synaptix AI. All rights reserved.
//

#include "SnakeDrivingDetectionTrigger.h"

#include "common/time/Timer.h"

namespace shadow {
namespace trigger {
bool SnakeDrivingDetectionTrigger::Proc() {
    bool ok = CheckCondition();
    CHECK_AND_RETURN(ok, SnakeDrivingDetectionTrigger, "CheckCondition failed", false);

    TriggerContext context;
    context.timeStamp = common::Timer::now();
    context.triggerId = trigger_id_;
    context.triggerName = trigger_name_;
    context.businessType = business_type_;
    context.triggerStatus = TriggerContext::TriggerState::Triggered;
    NotifyTriggerContext(context);

    return ok;
}

void SnakeDrivingDetectionTrigger::OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) {
    if (topic == "/ad_pub_test/snake") {
        UpdateVehicleInfo(msg);
    }

}

void SnakeDrivingDetectionTrigger::UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg) {
    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
            (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto vehicleReportReader = reader.getRoot<senseAD::msg::vehicle::VehicleReport>();
    UpdateVehicleReport(vehicleReportReader);
}

void SnakeDrivingDetectionTrigger::UpdateVehicleReport(const senseAD::msg::vehicle::VehicleReport::Reader& report) {
   snake_steering_angle_ = report.getSteering().getAngleActual();
   snake_vehiclespeed_ = report.getChassis().getVehicleMps(); 
}


bool SnakeDrivingDetectionTrigger::CheckCondition() {
    if (!trigger_checker_.parse(trigger_condition_)) {
        LOG_ERROR("[CNOPTrigger]: %s", trigger_checker_.last_error().c_str());
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
        vars["steering_angle"] = std::abs(snake_steering_angle_);
    }

    bool ok = trigger_checker_.check(vars);
     if (check_event_trigger_ && EvaluateTrigger()) {
        check_event_trigger_ = true;
    } else {
        check_event_trigger_ = false;
    }
    return check_event_trigger_;
}
bool SnakeDrivingDetectionTrigger::EvaluateTrigger() {
    check_event_trigger_ = false;
    SnakeDriveData snake_data;
    snake_data.steering_angle = steering_angle_;
    snake_data.direction = steering_angle_ > 0 ? 1 : -1;
    snake_data.speed = vehiclespeed_;
    if (data_window.size() >= max_window_size) {
        data_window.pop_front();
    }

    data_window.push_back(snake_data);

   

    if (data_window.size() < max_window_size) return false;  // 数据不足
    int drection_changes = 0;
    int over_threshold_count = 0;
    int last_direction=data_window[0].direction ;
    float sum_speed=0;
    for (const auto& data : data_window) {
        if(data.direction != last_direction){
            drection_changes++;
            last_direction = data.direction;
        }
        if(snake_check_condition_){
            over_threshold_count++ ;
        }
        sum_speed +=data.speed ;
    }
    LOG_INFO("SnakeDrivingDetectionTrigger drection_changes: %d, over_threshold_count :%d , avg_speed: %f",
             drection_changes,over_threshold_count, sum_speed/data_window.size());
    if(sum_speed/data_window.size() > 5.0) {
        double direction_change_freq = static_cast<double>(drection_changes)/data_window.size();
        double over_threshold_freq = static_cast<double>(over_threshold_count)/data_window.size();
        // return  (direction_change_freq -0.004 >= 1e-9) && (over_threshold_freq>0.4);
        if ((direction_change_freq -0.003 >= 1e-9) && (over_threshold_freq>0.4)) {
            check_event_trigger_ = true;
        } else {
            check_event_trigger_ = false;
        }

    return check_event_trigger_;
    }
}
void SnakeDrivingDetectionTrigger::NotifyTriggerContext(const TriggerContext& context) {
    if (factoryPtr_) {
        factoryPtr_->OnTriggerContext(context);
    }
    // LOG_INFO("Trigger notified: %s (ID: %s, Time: %ld)",
    //          context.triggerName.c_str(), context.triggerId.c_str(), context.timeStamp);
}



} // namespace trigger
} // namespace shadow
