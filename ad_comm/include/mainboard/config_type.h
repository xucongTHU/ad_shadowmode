/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Zhang Yi <zhangyi15@senseauto.com>
 */
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "yaml-cpp/yaml.h"
#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace rscl {
namespace mainboard {

class SyncedConfigConfig {
 public:
    void Init(const std::string &name,
              const std::unordered_map<std::string, std::string> &custom_param,
              const YAML::Node &config_param);
    const std::string &GetName() const;
    const std::unordered_map<std::string, std::string> &GetCustomParam() const;
    std::unordered_map<std::string, std::string> &GetMutableCustomParam();
    YAML::Node GetConfigParam() const;

 private:
    std::string name_;
    std::unordered_map<std::string, std::string> custom_param_;
    YAML::Node config_param_;
};

class TimerConfigConfig {
 public:
    void Init(const std::string &name,
              const std::unordered_map<std::string, std::string> &custom_param,
              const YAML::Node &config_param,
              uint32_t interval_ms,
              bool sim_time_interval);
    const std::string &GetName() const;
    const std::unordered_map<std::string, std::string> &GetCustomParam() const;
    std::unordered_map<std::string, std::string> &GetMutableCustomParam();
    YAML::Node GetConfigParam() const;
    uint32_t GetIntervalMs() const;
    bool GetSimTimeInterval() const;

 private:
    std::string name_;
    std::unordered_map<std::string, std::string> custom_param_;
    YAML::Node config_param_;
    uint32_t interval_ms_;
    bool sim_time_interval_;
};

class SyncedComponentConfig {
 public:
    void Init(const std::string &class_name,
              const std::shared_ptr<SyncedConfigConfig> &synced_config);
    const std::string &GetClassName() const;
    std::shared_ptr<SyncedConfigConfig> GetSyncedConfig() const;

 private:
    std::string class_name_;
    std::shared_ptr<SyncedConfigConfig> synced_config_;
};

class TimerComponentConfig {
 public:
    void Init(const std::string &class_name,
              const std::shared_ptr<TimerConfigConfig> &timer_config);
    const std::string &GetClassName() const;
    std::shared_ptr<TimerConfigConfig> GetTimerConfig() const;

 private:
    std::string class_name_;
    std::shared_ptr<TimerConfigConfig> timer_config_;
};

class ModuleConfig {
 public:
    void Init(const std::string &module_library,
              const std::vector<std::shared_ptr<SyncedComponentConfig>>
                  &synced_component,
              const std::vector<std::shared_ptr<TimerComponentConfig>>
                  &timer_component);
    const std::string &GetModuleLibrary() const;
    const std::vector<std::shared_ptr<SyncedComponentConfig>>
        &GetSyncedComponents() const;
    std::vector<std::shared_ptr<SyncedComponentConfig>>
        &GetMutableSyncedComponents();
    const std::vector<std::shared_ptr<TimerComponentConfig>>
        &GetTimerComponents() const;
    std::vector<std::shared_ptr<TimerComponentConfig>>
        &GetMutableTimerComponents();

 private:
    std::string module_library_;
    std::vector<std::shared_ptr<SyncedComponentConfig>> synced_component_;
    std::vector<std::shared_ptr<TimerComponentConfig>> timer_component_;
};

class BashScriptConfig {
 public:
    void Init(const std::string &command);
    const std::string &GetCommand() const;

 private:
    std::string command_;
};

class AdLogConfig {
 public:
    void Init(YAML::Node config_param);
    void Update(LogConfig &cfg);
    const YAML::Node &GetConfigParam() const;

    // To ensures the order of priority, keey yaml node before update logconf
    template <typename T>
    static void Parse(const YAML::Node &node,
                      const std::string key,
                      T &target) {
        if (node[key] && node[key].IsDefined()) {
            target = node[key].as<T>();
        }
    }

 private:
    YAML::Node config_param_;
};

class MainConfig {
 public:
    void Init(const std::string &version,
              const std::vector<std::shared_ptr<ModuleConfig>> &module_config,
              const std::vector<std::shared_ptr<BashScriptConfig>>
                  &bash_script_config,
              const std::shared_ptr<AdLogConfig> &adlog_config);
    const std::string &GetVersion() const;
    const std::vector<std::shared_ptr<ModuleConfig>> &GetModuleConfig() const;
    std::vector<std::shared_ptr<ModuleConfig>> &GetMutableModuleConfig();
    const std::vector<std::shared_ptr<BashScriptConfig>> &GetBashScriptConfig()
        const;
    const std::shared_ptr<AdLogConfig> &GetAdLogConfig() const;

 private:
    std::string version_;
    std::vector<std::shared_ptr<ModuleConfig>> module_config_;
    std::vector<std::shared_ptr<BashScriptConfig>> bash_script_config_;
    std::shared_ptr<AdLogConfig> adlog_config_;
};

class ConfigParser {
 public:
    void Init(const std::shared_ptr<MainConfig> &main_config);
    std::shared_ptr<MainConfig> GetMainConfig() const;

 private:
    std::shared_ptr<MainConfig> main_config_;
};

}  // namespace mainboard
}  // namespace rscl
}  // namespace senseAD
