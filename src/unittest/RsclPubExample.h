//
// Created by xucong on 25-7-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "ad_rscl/ad_rscl.h"
#include "ad_msg_idl/ad_planning/planning.capnp.h"
#include "ad_msg_idl/ad_vehicle/vehicle.capnp.h"
#include "ad_msg_idl/ad_gac/chassis.capnp.h"
#include "ad_msg_idl/ad_planning/planning.capnp.h"
#include "ad_msg_idl/ad_tap/as_command.capnp.h"
// #include "as_msg_idl/"
#include "ad_msg_idl/avp_parking_cmd/parking_cmd.capnp.h"


namespace dcl {

class RsclPubExample : public senseAD::rscl::component::TimerComponent {
public:
    RsclPubExample() = default;
    ~RsclPubExample() = default;

    bool OnInit() override;
    bool OnProc() override;
    bool OnReset() override;
    void OnShutdown() override;

private:
    senseAD::rscl::comm::PublisherBase::Ptr pub_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_acceleration_emergency_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_deceleration_emergency_;
    // senseAD::rscl::comm::PublisherBase::Ptr pub_deceleration_emergency_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_nop_quit_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_acc_quit_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_snake_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_slope_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_curve_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_statemachine_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_meb_;
    senseAD::rscl::comm::PublisherBase::Ptr pub_parking_;

    std::atomic<senseAD::msg::vehicle::GearCommand> gear_{senseAD::msg::vehicle::GearCommand::GEAR_NONE};
    std::atomic<bool> autoModeEnable_{false};
    std::atomic<double> chassisVehicleMps_{0.0};

};
    
    
} // dcl
