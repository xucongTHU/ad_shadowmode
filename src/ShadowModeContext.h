//
// Created by xucong on 25-7-10.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#pragma once

#include "ad_rscl/ad_rscl.h"

#include "common/utils/utils.h"
#include "data_upload/DataUploader.h"
#include "data_upload/DataReporter.h"
#include "data_storage/DataStorage.h"
#include "strategy/StrategyParser.h"
#include "strategy/ConfigFileWatcher.hpp"
#include "trigger/base/TriggerFactory.hpp"
#include "channel/RsclChannelManager.h"


using namespace shadow;
using namespace shadow::common;
using namespace shadow::logger;
using namespace shadow::data_encryption;
using namespace shadow::data_upload;
using namespace shadow::data_storage;
using namespace shadow::data_report;
using namespace shadow::strategy;
using namespace shadow::trigger;
using namespace shadow::config;

namespace shadow {

//TODO
enum class FsmState {
    IDLE,        // 初始空闲状态
    TRIGGERED,   // 已触发状态
    COLLECTING,  // 数据采集状态
    UPLOADING,   // 数据上传状态
    COMPLETED,   // 完成状态
    ERROR        // 错误状态
};

class ShadowModeContext : public senseAD::rscl::component::TimerComponent {
public: 
    ShadowModeContext() = default;

    ~ShadowModeContext() = default;

    bool OnInit() override;
    bool PostInit() override;
    bool OnProc() override;
    void OnShutdown() override;

    // bool Init();
    bool Start();
    bool Stop();
    void RunTrigger();

private:
    bool InitLogger();
    bool InitConfig();
    bool Selfcheck();
    bool InitStrategey();
    bool InitTrigger();
    bool InitDataCollection();
    bool InitDataUploader();
    bool InitDataReporter();
    void OnTrigger(const trigger::TriggerContext& context);

    bool FsmTransmission(FsmState new_state);

    void UpdateStateMachine();

private:
    // 状态机
    std::atomic<FsmState> fsm_state_{FsmState::IDLE};
    std::mutex state_mutex_;
    std::condition_variable state_cv_;

    std::shared_ptr<senseAD::rscl::comm::Node> local_node_{nullptr};
    std::shared_ptr<strategy::StrategyConfigParser> strategy_config_parser_{nullptr};
    std::shared_ptr<data_upload::DataUploader> data_uploader_{nullptr};
    std::shared_ptr<data_storage::DataStorage> data_storage_{nullptr};
    std::shared_ptr<data_report::DataReporter> data_reporter_{nullptr};
    std::shared_ptr<trigger::TriggerFactory> trigger_factory_{nullptr};
    std::shared_ptr<channel::RsclChannelManager> channel_manager_{nullptr};

    strategy::StrategyConfig strategy_config_;
    strategy::StrategyConfig new_config_;
    CacheMode mode;
    config::AppConfigData app_config_;
    std::filesystem::path watch_directory_;
    std::atomic<bool> running_{false};
    std::atomic<bool> stop{false};
    std::shared_ptr<ThreadPool> thread_pool_;

    nlohmann::json appconfig;
    std::unique_ptr<strategy::ConfigReloader> config_reloader_;
    std::mutex config_mutex_;

};

}

