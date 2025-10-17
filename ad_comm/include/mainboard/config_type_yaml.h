/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Zhang Yi <zhangyi15@senseauto.com>
 */
#pragma once

#include <vector>
#include <memory>
#include <string>

#include "mainboard/config_type.h"

namespace senseAD {
namespace rscl {
namespace mainboard {

template <typename Base, typename Derived>
void LoadConfigList(YAML::Node parent,
                    const std::string &key,
                    std::vector<std::shared_ptr<Base>> *list) {
    if (parent[key]) {
        const auto &child = parent[key];
        if (child.IsSequence()) {
            for (const auto &child_node : child) {
                list->push_back(std::make_shared<Derived>(child_node));
            }
        } else {
            list->push_back(std::make_shared<Derived>(child));
        }
    }
}

class SyncedConfigConfigYamlImpl : public SyncedConfigConfig {
 public:
    explicit SyncedConfigConfigYamlImpl(YAML::Node synced_config);

 private:
};

class TimerConfigConfigYamlImpl : public TimerConfigConfig {
 public:
    explicit TimerConfigConfigYamlImpl(YAML::Node timer_config);

 private:
};

class SyncedComponentConfigYamlImpl : public SyncedComponentConfig {
 public:
    explicit SyncedComponentConfigYamlImpl(YAML::Node synced_component);

 private:
};

class TimerComponentConfigYamlImpl : public TimerComponentConfig {
 public:
    explicit TimerComponentConfigYamlImpl(YAML::Node timer_component);

 private:
};

class ModuleConfigYamlImpl : public ModuleConfig {
 public:
    explicit ModuleConfigYamlImpl(YAML::Node module_config);

 private:
};

class BashScriptConfigYamlImpl : public BashScriptConfig {
 public:
    explicit BashScriptConfigYamlImpl(YAML::Node bash_script_config);

 private:
};

class AdLogConfigYamlImpl : public AdLogConfig {
 public:
    explicit AdLogConfigYamlImpl(YAML::Node adlog_config);

 private:
};

class MainConfigYamlImpl : public MainConfig {
 public:
    explicit MainConfigYamlImpl(YAML::Node yaml);

 private:
};

class ConfigParserYamlImpl : public ConfigParser {
 public:
    explicit ConfigParserYamlImpl(const std::string &config_file);

 private:
};

}  // namespace mainboard
}  // namespace rscl
}  // namespace senseAD
