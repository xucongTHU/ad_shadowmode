//
// Created by xucong on 25-7-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "ad_rscl/ad_rscl.h"
#include "ad_msg_idl/ad_planning/planning.capnp.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"



class RsclSubExample : public senseAD::rscl::component::TimerComponent {
public:
    RsclSubExample() = default;
    ~RsclSubExample() = default;

    bool OnInit() override;
    bool OnProc() override;
    bool OnReset() override;
    void OnShutdown() override;

private:
    void NewMessageCallback(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>> msg);
    void UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg);
    void UpdateGear(const senseAD::msg::vehicle::VehicleReport::Reader& report);
    void UpdateAutoModeEnable(const senseAD::msg::vehicle::VehicleReport::Reader& report);
    void UpdateChassisVehicleMps(const senseAD::msg::vehicle::VehicleReport::Reader& report);

private:
    senseAD::rscl::comm::SubscriberBase::Ptr suber_;

    std::atomic<senseAD::msg::vehicle::GearCommand> gear_{senseAD::msg::vehicle::GearCommand::GEAR_NONE};
    std::atomic<bool> autoModeEnable_{false};
    std::atomic<double> chassisVehicleMps_{0.0};

};
    
