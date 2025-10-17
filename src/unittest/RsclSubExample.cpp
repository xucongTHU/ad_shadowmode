//
// Created by xucong on 25-7-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "RsclSubExample.h"
#include "common/logger/Logger.h"

using namespace shadow;
using namespace shadow::logger;


static const char* LOG_TAG = "Example";

bool RsclSubExample::OnInit() {
    printf("[ExampleSubComponent] OnInit\n");
    suber_ = InnerNode()->CreateSubscriber<senseAD::rscl::comm::RawMessage>(
        "/caic_pub_test/nopquit", [this](auto& msg) {
            printf("[RsclSubExample] Received /canbus/vehicle_report\n");
            NewMessageCallback(msg);
            
        });

    LOG_INFO("Init ok");

    return true;

}


bool RsclSubExample::OnProc() {
    printf("[ExampleSubComponent] OnProc\n");
    // suber_ = InnerNode()->CreateSubscriber<senseAD::rscl::comm::RawMessage>(
    //     "/canbus/vehicle_report", [this](auto& msg) {
    //         printf("[RsclSubExample] Received /canbus/vehicle_report\n");
    //         NewMessageCallback(msg);
            
    //     });
    std::cout << "chassisVehicleMps_dsfdsfsdfsf: " << chassisVehicleMps_ << std::endl;
    return true; 
}

bool RsclSubExample::OnReset() { return true; }
void RsclSubExample::OnShutdown() {
    printf("[ExampleSubComponent] OnShutdown\n");
}

void RsclSubExample::NewMessageCallback(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>> msg) {
    auto cur_clock_mode = senseAD::base::time::ClockMode::SYSTEM_TIME;
    uint64_t message_time = senseAD::base::time::Time::Now(&cur_clock_mode).ToMillisecond();
    auto header = msg->Header();
    if (header.is_enabled)
    {
        message_time = msg->Header().stamp;
    }
    else
    {
        LOG_ERROR("[CNOPTrigger] header parse error");   
        return;
    }

    // in start of replay mode rscl timestamp is zero
    if (message_time == 0) {
        LOG_ERROR("message_time is zero");
        return;
    }

    LOG_INFO("[RsclSubExample] update vehicle info");   
    UpdateVehicleInfo(msg);

}

void RsclSubExample::UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg) {
    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
            (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    // auto schema = capnp::Schema::from<senseAD::msg::vehicle::VehicleReport>();
    // auto vehicleReportReader = reader.getRoot<capnp::DynamicStruct>(schema);
    auto vehicleReportReader = reader.getRoot<senseAD::msg::vehicle::VehicleReport>();
   
    // senseAD::rscl::common::dynamicPrintValue(vehicleReportReader);
    UpdateGear(vehicleReportReader);
    UpdateAutoModeEnable(vehicleReportReader);
    UpdateChassisVehicleMps(vehicleReportReader);
}



void RsclSubExample::UpdateGear(const senseAD::msg::vehicle::VehicleReport::Reader& report) {
    gear_.store(report.getGear().getActual());
    LOG_INFO("[RsclSubExample], gear :%d", static_cast<int32_t>(gear_.load()));
}

void RsclSubExample::UpdateAutoModeEnable(const senseAD::msg::vehicle::VehicleReport::Reader& report) {
    autoModeEnable_.store(report.getMode().getEnable());
    LOG_INFO("[RsclSubExample], autoModeEnable :%d", static_cast<int32_t>(autoModeEnable_.load()));
}

void RsclSubExample::UpdateChassisVehicleMps(const senseAD::msg::vehicle::VehicleReport::Reader& report) {
    // chassisVehicleMps_.store(report.getChassis().getVehicleMps());
    // LOG_INFO("[RsclSubExample], chassisVehicleMps :%f", chassisVehicleMps_.load());
     chassisVehicleMps_ = 10.0;
}



int main(int argc, char* argv[]) {
    std::string log_dir = "/tmp/shadow_mode/log.txt";
    std::string log_csv = "/tmp/shadow_mode/log.csv";
    Logger::instance()->Init(LOG_TO_CONSOLE | LOG_TO_FILE, LOG_LEVEL_INFO,
        log_dir.c_str(), log_csv.c_str());

    auto rt = senseAD::rscl::GetCurRuntime();
    printf("Init Runtime\n");
    rt->Init(argc, argv);

    printf("Init Component\n");
    senseAD::rscl::component::ComponentConfig cfg;
    cfg.timer_conf.set_name("example_sub");
    cfg.timer_conf.set_interval_ms(100);
    RsclSubExample e;
    e.Initialize(cfg);

    printf("Wait For Shutdown\n");
    while (rt->OK()) {
        rt->WaitForShutdown(std::chrono::milliseconds(10));
    }

    if (!rt->OK()) {
        printf("Shutdown\n");
        e.Shutdown();
        printf("Finish Shutdown");
    }

    return 0;
}
