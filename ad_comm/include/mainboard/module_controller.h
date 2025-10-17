/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */
/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include "mainboard/module_argument.h"
#include "mainboard/config_type.h"
#include "ad_base/class_loader/class_loader_manager.h"
#include "ad_rscl/component/types.h"

namespace senseAD {
namespace rscl {
namespace component {
class ComponentBase;
}  // namespace component

namespace mainboard {

static constexpr const char* kDefaultVersion = "v0.0.0";
class MainConfig;

class ModuleController {
 public:
    explicit ModuleController(const ModuleArgument& args);
    virtual ~ModuleController() = default;

    bool Init();
    bool Run();
    void Clear();
    std::string GetVersion();

 private:
    bool LoadAll();
    bool LoadModule(const std::string& path, const std::string& param_key);
    void MergeCustomParam(std::shared_ptr<MainConfig> main_config,
                          const std::string& param_key);
    void MergeCustomParamFromArgs(
        const std::string& param_key,
        std::unordered_map<std::string, std::string>& param_map);  // NOLINT
    bool LoadConfig(std::shared_ptr<MainConfig> main_config);
    bool LoadModuleConfig(std::shared_ptr<MainConfig> main_config);
    bool LoadModuleLibrary(std::shared_ptr<ModuleConfig> module_config);
    bool LoadSyncedComponents(std::shared_ptr<ModuleConfig> module_config);
    bool LoadTimerComponents(std::shared_ptr<ModuleConfig> module_config);
    bool CreateComponentBase(
        const std::string& class_name,
        std::shared_ptr<component::ComponentBase>& base);  // NOLINT
    component::ComponentConfig CreateSyncedComponent(
        std::shared_ptr<SyncedConfigConfig> synced_config);
    component::ComponentConfig CreateTimerComponent(
        std::shared_ptr<TimerConfigConfig> timer_config);
    bool InitComponent(std::shared_ptr<component::ComponentBase> base,
                       const component::ComponentConfig& comp_config);
    void LoadBashScriptConfig(std::shared_ptr<MainConfig> main_config);
    void LoadAdLogConfig(std::shared_ptr<MainConfig> main_config);

    ModuleArgument args_;
    base::class_loader::ClassLoaderManager class_loader_manager_;
    std::vector<std::shared_ptr<component::ComponentBase>> component_list_;
    std::string version_;
};

inline ModuleController::ModuleController(const ModuleArgument& args)
    : args_(args) {
    args.PrintInfo();
}

inline bool ModuleController::Init() { return LoadAll(); }

}  // namespace mainboard
}  // namespace rscl
}  // namespace senseAD
