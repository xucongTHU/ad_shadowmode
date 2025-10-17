//
// Created by xucong on 25-7-10.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "ShadowModeContext.h"

#include "common/environment.h"
#include "common/time/Timer.h"
#include "common/logger/Logger.h"
#include "config/AppConfig.h"
#include "trigger/scheduler/PriorityScheduler.h"
#include "trigger/core/rule/ACCQuitTrigger.h"
#include "trigger/core/rule/AEBActiveTrigger.h"
#include "trigger/core/rule/AVPTakeoverTrigger.h"       
#include "trigger/core/rule/CNOPQuitTrigger.h"
#include "trigger/core/rule/CNOPTakeoverTrigger.h"
#include "trigger/core/rule/CurveDynamicDetectionTrigger.h"
#include "trigger/core/rule/DriveWheelSlipTrigger.h"
#include "trigger/core/rule/EmergencyAccTrigger.h"
#include "trigger/core/rule/EmergencyBrakingTrigger.h"
#include "trigger/core/rule/EmergencyEvasionTrigger.h"
#include "trigger/core/rule/FCTBActiveTrigger.h"
#include "trigger/core/rule/FCWActiveTrigger.h"
#include "trigger/core/rule/HPATakeoverTrigger.h"
#include "trigger/core/rule/MEBActiveTrigger.h"
#include "trigger/core/rule/RCTBActiveTrigger.h"
#include "trigger/core/rule/SlopeDetectionTrigger.h"
#include "trigger/core/rule/SnakeDrivingDetectionTrigger.h"

namespace shadow {

static const char* LOG_TAG = "ShadowMode";
static std::string kAppConfigPath = "/config/app_config.json";
static std::string kStrategyConfigPath = "/config/strategy_config.json";

bool ShadowModeContext::OnInit() {
    // 初始化线程池
    thread_pool_ = std::make_shared<ThreadPool>(4);
    // const auto& debug_config = AppConfig::getInstance().GetConfig().debug;
    
    // 初始化配置
    bool ok = InitConfig();
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitConfig failed", false);
    
    // 初始化日志
    ok = InitLogger();
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitLogger failed", false);
    
    // 系统自检
    ok = Selfcheck();
    CHECK_AND_RETURN(ok, ShadowModeContext, "Selfcheck failed", false);

    // 初始化运行时节点node
    auto rt = senseAD::rscl::GetCurRuntime();
    local_node_ = rt->CreateNode("shadow_" + std::to_string(getpid()),
                           rscl::INTERNAL_NODE_SPACE);
    // 初始化策略解析器
    ok = InitStrategey();
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitStrategey failed", false);
    
    // 初始化触发模块
    ok = InitTrigger();
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitTrigger failed", false);
    
    // 初始化数据上报模块
    ok = InitDataReporter();
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitDataReporter failed", false);

    // 初始化数据采集模块
    ok = InitDataCollection();
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitDataCollection failed", false);
    
    // 初始化channel管理器
    channel_manager_ = std::make_shared<channel::RsclChannelManager>();
    auto recorder = data_storage_->GetRsclRecorder();
    ok = channel_manager_->Init(local_node_, strategy_config_, 
                                trigger_factory_, recorder, data_reporter_);
    CHECK_AND_RETURN(ok, ShadowModeContext, "channel_manager Init failed", false);
    
    // 初始化数据上传模块
    ok = InitDataUploader();
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitDataUploader failed", false);

    LOG_INFO("ShadowModeContext Init ok");

    return true;
}

bool ShadowModeContext::PostInit() {
    printf("[ShadowModeContext] PostInit\n");
    return Start();
}

bool ShadowModeContext::OnProc() {
    // LOG_INFO("ShadowModeContext timecomponent timer tick");
    return true; 
}
void ShadowModeContext::OnShutdown() {
    LOG_INFO("ShadowModeContext OnShutdown");
    (void)Stop();
}

bool ShadowModeContext::InitConfig() {
    std::string AppConfigPath = std::string(getInstallRootPath()) + kAppConfigPath;
    bool ok = AppConfig::getInstance().Init(AppConfigPath);
    CHECK_AND_RETURN(ok, ShadowModeContext, "AppConfig Init failed", false);
    
    return ok;
}


bool ShadowModeContext::InitLogger() {
    auto appconfig = AppConfig::getInstance().GetConfig();
    if (!ForceCreateDirRecursive(appconfig.log.logPath)) {
        LOG_ERROR("Failed to create log directory: %s", appconfig.log.logPath.c_str());
        return false;
    }

    time_t time_s = common::Timer::now_sec();
    char time[80];
    std::strftime(time, 80, appconfig.log.logPattern.c_str(), std::localtime(&time_s));
    std::string log_dir = appconfig.log.logPath + appconfig.log.logBasename + "_" + time + ".txt";
    std::string log_csv = appconfig.log.logPath + appconfig.log.logBasename + "_" + time + ".csv";

    int log_level = LOG_LEVEL_INFO;
    if (appconfig.log.logLevel == "debug") {
        log_level = LOG_LEVEL_DEBUG1;
    } else if (appconfig.log.logLevel == "info") {
        log_level = LOG_LEVEL_INFO;
    } else if (appconfig.log.logLevel == "warn") {
        log_level = LOG_LEVEL_WARNING;
    } else if (appconfig.log.logLevel == "error") {
        log_level = LOG_LEVEL_ERROR;
    }

    bool ok = Logger::instance()->Init(LOG_TO_CONSOLE | LOG_TO_FILE, log_level, log_dir.c_str(), log_csv.c_str());

    return ok;
}

bool ShadowModeContext::Selfcheck() {
    auto appconfig = AppConfig::getInstance().GetConfig();

    std::string vin = Vin();
    if (vin.empty()) {
        LOG_ERROR("ShadowModeContext get vin failed");
        return false;
    }

    if (!IsDirExist(appconfig.log.logPath)) {
        LOG_ERROR("Log directory not exist: %s", appconfig.log.logPath.c_str());
        return false;
    }

    if (!IsDirExist(appconfig.dataStorage.storagePaths["bagPath"])) {
        LOG_ERROR("Data directory not exist: %s", appconfig.dataStorage.storagePaths["bagPath"].c_str());
        return false;
    }

    LOG_INFO("ShadowModeContext selfcheck ok");

    return true;
}

bool ShadowModeContext::InitStrategey() {
    std::string StrategyConfigPath = std::string(getInstallRootPath()) + kStrategyConfigPath;
    static std::string kDefaultFallbackConfigPath = "/config/default_strategy_config.json";
    std::string finalConfigPath = StrategyConfigPath;

    auto appconfig = AppConfig::getInstance().GetConfig();
    auto time_start = Timer::now_ms();
    bool useDefaultConfig = false;

    std::string fallbackConfigPath;
    while (!IsDirExist(StrategyConfigPath)) {
        auto elapsed = Timer::now_ms()- time_start;

        if (elapsed >= appconfig.debug.cloudtimeOutMs) {
            LOG_WARN("Timeout waiting for strategy config file: %s after %lld seconds",
                     StrategyConfigPath.c_str(), elapsed/1000);
            
            // 尝试使用默认配置文件
            fallbackConfigPath = std::string(getInstallRootPath()) + kDefaultFallbackConfigPath;
            if (IsDirExist(fallbackConfigPath)) {
                // LOG_INFO("Using default strategy config file: %s", fallbackConfigPath.c_str());
                finalConfigPath = fallbackConfigPath;
                useDefaultConfig = true;
            } else {
                LOG_ERROR("Fallback default strategy config file not found: %s", fallbackConfigPath.c_str());
                return false;
            }
            break;
        }

        LOG_WARN("Strategy config file not found at: %s, retrying in second... (waited: %llds)",
                 kStrategyConfigPath.c_str(), elapsed/1000);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    config_reloader_ = std::make_unique<ConfigReloader>(finalConfigPath); // 初始化配置文件重新加载器
    if (!config_reloader_->Init()) {
        LOG_ERROR("Failed to init strategy config reloader with file: %s", finalConfigPath.c_str());
        return false;
    }

    // strategy_config_reloader_->SetCallback([this](const StrategyConfig& cfg) {
    //     LOG_INFO("Strategy config updated (reload).");
    //     std::lock_guard<std::mutex> lock(strategy_mutex_);
    //     strategy_config_ = cfg;
    // });
    
    {
        std::lock_guard<std::mutex> lock(config_mutex_); // 加锁，确保线程安全
        strategy_config_ = config_reloader_->GetConfig();
    }

    if (useDefaultConfig) {
        LOG_INFO("Strategy config initialized succ with default from: %s", finalConfigPath.c_str());
    } else {
        LOG_INFO("Strategy config initialized succ from: %s", finalConfigPath.c_str());
    }

    return true;
}

bool ShadowModeContext::InitTrigger() {
    StrategyConfig strategy_cfg;
    {
        std::lock_guard<std::mutex> lock(config_mutex_);
        strategy_cfg = strategy_config_;
    }

    trigger_factory_ = std::make_shared<trigger::TriggerFactory>();
    trigger_factory_->SetNodeHandle(local_node_);
    // RegisterACCQuitTrigger(trigger_factory_, local_node_);
    RegisterAEBActiveTrigger(trigger_factory_, local_node_);
    // RegisterAVPTakeoverTrigger(trigger_factory_, local_node_);
    // RegisterCNOPQuitTrigger(trigger_factory_, local_node_);
    // RegisterCNOPTakeoverTrigger(trigger_factory_, local_node_);
    // RegisterCurveDynamicDetectionTrigger(trigger_factory_, local_node_);
    // RegisterDriveWheelSlipTrigger(trigger_factory_, local_node_);
    // RegisterEmergencyAccTrigger(trigger_factory_, local_node_);
    // RegisterEmergencyBrakingTrigger(trigger_factory_, local_node_);
    // RegisterEmergencyEvasionTrigger(trigger_factory_, local_node_);
    // RegisterFCTBActiveTrigger(trigger_factory_, local_node_);
    // RegisterFCWActiveTrigger(trigger_factory_, local_node_);
    // RegisterHPATakeoverTrigger(trigger_factory_, local_node_);
    // RegisterMEBActiveTrigger(trigger_factory_, local_node_);
    // RegisterRCTBActiveTrigger(trigger_factory_, local_node_);
    // RegisterSlopeDetectionTrigger(trigger_factory_, local_node_);
    // RegisterSnakeDrivingDetectionTrigger(trigger_factory_, local_node_);

    trigger_factory_->SetTriggerConfig(strategy_cfg);
    auto threadPool = std::make_shared<ThreadPool>(trigger_factory_->GetThreadPoolSize());
    auto scheduler = std::make_shared<PriorityScheduler>(threadPool);
    bool ok = trigger_factory_->InitTriggerScheduler(local_node_, scheduler);
    CHECK_AND_RETURN(ok, ShadowModeContext, "InitTriggerScheduler failed", false);

    trigger_factory_->SetTriggerCallback(std::bind(&ShadowModeContext::OnTrigger, this, std::placeholders::_1));

    return ok;
}

bool ShadowModeContext::InitDataCollection() {
    data_storage_ = std::make_shared<data_storage::DataStorage>();
    if (!data_storage_) {
        LOG_ERROR("data_storage_ make shared failed");
        return false;
    }

    auto configData = AppConfig::getInstance().GetConfig();
    bool ok = data_storage_->Init(local_node_, strategy_config_);
    CHECK_AND_RETURN(ok, ShadowModeContext, "data_storage Init failed", false);

    LOG_INFO("data storage Init ok");
    return ok;
}

bool ShadowModeContext::InitDataReporter() {
    if(strategy_config_.strategies.size()> 0){
        mode = strategy_config_.strategies[0].mode.cacheMode;
    }
    data_reporter_ = std::make_shared<data_report::DataReporter>();
    bool ok = data_reporter_->Init(local_node_, mode);
    CHECK_AND_RETURN(ok, ShadowModeContext, "data_reporter Init failed", false);

    return ok;
}

bool ShadowModeContext::InitDataUploader() {
    data_uploader_ = std::make_shared<DataUploader>();
    if (!data_uploader_) {
        LOG_ERROR("data_uploader_ make shared failed");
        return false;
    }

    auto data_upload_config = config::AppConfig::getInstance().GetConfig();
    bool ok = data_uploader_->Init(data_upload_config.dataUpload, data_reporter_);
    CHECK_AND_RETURN(ok, ShadowModeContext, "data_uploader Init failed", false);

    return ok;
}

bool ShadowModeContext::Start() {
    // LOG_INFO("Start...");
    const auto& debug_config = config::AppConfig::getInstance().GetConfig().debug;

    //1. run trigger
    thread_pool_->enqueue(std::bind(&ShadowModeContext::RunTrigger, this));

    //2. run data reporter
    thread_pool_->enqueue(std::bind(&DataReporter::Start, data_reporter_));

    //3. run data storage
    thread_pool_->enqueue(std::bind(&DataStorage::Start, data_storage_));

    //4. run data uploader
    if (!debug_config.closeDataUpload) {
        thread_pool_->enqueue(std::bind(&DataUploader::Start, data_uploader_));
    }

    // LOG_INFO("Start ok");

    return true;
}

bool ShadowModeContext::Stop() {
    LOG_INFO("Stop...");
    stop.store(true);

    bool ok = false;

    // TODO
    const auto& debug_config = config::AppConfig::getInstance().GetConfig().debug;

    if (!debug_config.closeDataUpload) {
        ok = data_uploader_->Stop();
        CHECK_AND_RETURN(ok, ShadowModeContext, "data_uploader stop failed", false);
    }

    LOG_INFO("Stop ok");

    return ok;
}

void ShadowModeContext::RunTrigger() {
    constexpr int32_t preIntervalSec = 15;
    std::this_thread::sleep_for(std::chrono::seconds(preIntervalSec));

    constexpr int32_t LoopIntervalMs = 10;
    while(!stop.load())
    {
        trigger_factory_->PostTriggerProcess();
        std::this_thread::sleep_for(std::chrono::milliseconds(LoopIntervalMs));       
    }

}

void ShadowModeContext::OnTrigger(const trigger::TriggerContext& context) {
    if (context.triggerStatus != trigger::TriggerContext::TriggerState::Triggered) {
        LOG_ERROR("Ignore non-trigger status: %d (trigger ID: %s)",
                  context.triggerStatus, context.triggerId.c_str());
        return;
    }

    if (data_storage_) {
        auto ptr = std::make_shared<trigger::TriggerContext>(context);
        data_storage_->AddTrigger(ptr);
        // LOG_INFO("Add trigger: %s (id: %s, timestamp: %s)", 
        //          context.triggerName.c_str(), context.triggerId.c_str(), UnixSecondsToString(context.timeStamp/1e6).c_str());
    }

}

} //namespace shadow
