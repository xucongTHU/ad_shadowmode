//
// Created by xucong on 25-7-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "RsclPubExample.h"
#include "common/logger/Logger.h"

using namespace shadow;
using namespace shadow::logger;

namespace dcl {

static const char* LOG_TAG = "Example";

bool RsclPubExample::OnInit() {
    printf("[ExamplePubComponent] OnInit\n");
    pub_ = InnerNode()->CreatePublisher<gac::rscl::ChassisReportMsg>("/caic_pub_test/EmergencyEvasionTrigger");
    pub_acceleration_emergency_ = InnerNode()->CreatePublisher<senseAD::msg::vehicle::VehicleReport>("/caic_pub_test/EmergencyAcceleration");
    pub_deceleration_emergency_ = InnerNode()->CreatePublisher<senseAD::msg::vehicle::VehicleReport>("/caic_pub_test/EmergencyDeceleration");
    pub_nop_quit_ = InnerNode()->CreatePublisher<senseAD::msg::planning::MCUStateMachineInfo>("/caic_pub_test/nopquit");
    pub_acc_quit_ = InnerNode()->CreatePublisher<senseAD::msg::tap::AsCmdLgSafe>("/caic_pub_test/accquit");
    pub_snake_ = InnerNode()->CreatePublisher<senseAD::msg::vehicle::VehicleReport>("/caic_pub_test/snake");
    pub_slope_ = InnerNode()->CreatePublisher<senseAD::msg::vehicle::VehicleReport>("/caic_pub_test/slope");
    pub_curve_ = InnerNode()->CreatePublisher<senseAD::msg::vehicle::VehicleReport>("/caic_pub_test/curve");
    pub_statemachine_ = InnerNode()->CreatePublisher<senseAD::msg::tap::AsCmdLgSafe>("/caic_pub_test/statemachine");
    
    pub_meb_ =  InnerNode()->CreatePublisher<senseAD::msg::vehicle::VehicleCmd>("/caic_pub_test/meb");
    pub_parking_ =  InnerNode()->CreatePublisher<senseAD::msg::avp_state_v3::UicFsmCmd>("/caic_pub_te st/parking");
    return true;

}


bool RsclPubExample::OnProc() {
    // printf("[ExamplePubComponent] OnProc\n");

     auto msg = pub_->NewSendMsg<gac::rscl::ChassisReportMsg>();
    auto chssisreport = msg->MsgBuilder();
    chssisreport.getSteer().getSteerAngleRate().setValue(150.0);
    //  report.getEgoMotion().getYawRate().getValue();
    chssisreport.getEgoMotion().getYawRate().setValue(17.0);
    // mastercylinderpressure_ = report.getBrake().getMastCylinderPressure().getValue();
    chssisreport.getBrake().getMastCylinderPressure().setValue(90.0);
    pub_->Publish(msg);

    //acceleration emergency
    auto msg_acceleration_emergecy = pub_acceleration_emergency_->NewSendMsg<senseAD::msg::vehicle::VehicleReport>();
    auto vehiclereport_accemergency = msg_acceleration_emergecy->MsgBuilder();
    vehiclereport_accemergency.getAcc().setValueActual(4.0);
    pub_acceleration_emergency_->Publish(msg_acceleration_emergecy);


    //deceleration emergency
     auto msg_deceleration_emergecy = pub_deceleration_emergency_->NewSendMsg<senseAD::msg::vehicle::VehicleReport>();
    auto vehiclereport_decemergency = msg_deceleration_emergecy->MsgBuilder();
    vehiclereport_decemergency.getAcc().setValueActual(-8.0);
    pub_deceleration_emergency_->Publish(msg_deceleration_emergecy);

    //nop quit
    auto msg_planning_statemachine = pub_nop_quit_->NewSendMsg<senseAD::msg::planning::MCUStateMachineInfo>();
    auto planningstate = msg_planning_statemachine->MsgBuilder();
    planningstate.setMcuStateMachene(senseAD::msg::planning::MCUStateMachineInfo::StateMachine::NOP_ACITVE);
    planningstate.setMcuDrvOverride(senseAD::msg::planning::MCUStateMachineInfo::Override::BOTH_OVERRIDE);
    pub_nop_quit_->Publish(msg_planning_statemachine);

    //acc quit
    auto msg_acc_statemachine = pub_acc_quit_->NewSendMsg<senseAD::msg::planning::MCUStateMachineInfo>();
    auto accstate = msg_acc_statemachine->MsgBuilder();
    accstate.setMcuStateMachene(senseAD::msg::planning::MCUStateMachineInfo::StateMachine::ACC_ACITVE);
    accstate.setMcuDrvOverride(senseAD::msg::planning::MCUStateMachineInfo::Override::BOTH_OVERRIDE);
    pub_acc_quit_->Publish(msg_acc_statemachine);
    //snake trigger
    auto msg_snake_trigger = pub_snake_->NewSendMsg<senseAD::msg::vehicle::VehicleReport>();
    auto snaketrigger = msg_snake_trigger->MsgBuilder();
    snaketrigger.getSteering().setAngleActual(30.0);
    snaketrigger.getChassis().setVehicleMps(20.0);
    snaketrigger.getChassis().setWheelSpeedFl(30.0);
    snaketrigger.getChassis().setWheelSpeedFr(30.0);
    snaketrigger.getChassis().setWheelSpeedRl(50.0);
    snaketrigger.getChassis().setWheelSpeedRr(50.0);
    pub_snake_->Publish(msg_snake_trigger);

    //curve trigger
    auto msg_curve_trigger = pub_snake_->NewSendMsg<senseAD::msg::vehicle::VehicleReport>();
    auto curvetrigger = msg_curve_trigger->MsgBuilder();
    curvetrigger.getImu().setAccelY(0.5);
    pub_curve_->Publish(msg_curve_trigger);

    //slope 
     //snake trigger
    auto msg_slope_trigger = pub_snake_->NewSendMsg<senseAD::msg::vehicle::VehicleReport>();
    auto slopetrigger = msg_slope_trigger->MsgBuilder();
    slopetrigger.getThrottle().setPedalActual(60.0);
    pub_slope_->Publish(msg_slope_trigger);

    //statemachine
    auto msg_statemachine = pub_statemachine_->NewSendMsg<senseAD::msg::tap::AsCmdLgSafe>();
    auto statemachine = msg_statemachine->MsgBuilder();
    //AEB
    statemachine.setShadowMode(3);
    //fcw
    // statemachine.setShadowMode(9);
    pub_statemachine_->Publish(msg_statemachine);

    //MEB
    auto msg_meb = pub_meb_->NewSendMsg<senseAD::msg::vehicle::VehicleCmd>();
    auto meb_builder = msg_meb->MsgBuilder();
    meb_builder.getBrake().setBrakeMode(senseAD::msg::vehicle::BrakeCmd::BrakeMode::EMERGENCY);
    pub_meb_->Publish(msg_meb);

    //parking
    auto msg_parking = pub_parking_->NewSendMsg<senseAD::msg::avp_state_v3::UicFsmCmd>();
    auto parking_builder = msg_parking->MsgBuilder();
    //avp
    parking_builder.setAvpStatus(senseAD::msg::avp_state_v3::AvpMainStatus::APA_ABORT);
    //hpa
    parking_builder.setAvpStatus(senseAD::msg::avp_state_v3::AvpMainStatus::HPP_ABORT);

    pub_parking_->Publish(msg_parking);
    return true; 
}

bool RsclPubExample::OnReset() { return true; }
void RsclPubExample::OnShutdown() {
    printf("[ExamplePubComponent] OnShutdown\n");
}

} // dcl


int main(int argc, char* argv[]) {
    std::string log_dir = "/tmp/shadowmode/log.txt";
    std::string log_csv = "/tmp/shadowmode/log.csv";
    Logger::instance()->Init(LOG_TO_CONSOLE | LOG_TO_FILE, LOG_LEVEL_INFO,
        log_dir.c_str(), log_csv.c_str());

    auto rt = senseAD::rscl::GetCurRuntime();
    printf("Init Runtime\n");
    rt->Init(argc, argv);

    printf("Init Component\n");
    senseAD::rscl::component::ComponentConfig cfg;
    cfg.timer_conf.set_name("example_pub");
    cfg.timer_conf.set_interval_ms(10);
    dcl::RsclPubExample e;
    e.Initialize(cfg);

    printf("Wait For Shutdown\n");
    while (rt->OK()) {
        rt->WaitForShutdown(std::chrono::seconds(1));
    }

    if (!rt->OK()) {
        printf("Shutdown\n");
        e.Shutdown();
        printf("Finish Shutdown");
    }

    return 0;
}
