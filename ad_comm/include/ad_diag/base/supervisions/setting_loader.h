/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "ad_base/common/macros.h"
#include "ad_base/std/optional.h"

#include "ad_diag/base/supervisions/common.h"
#include "ad_diag/manifest/schema/process_manifest.h"

namespace senseAD {
namespace diag {

std::string ExpectedConfigLocatedDir();

// The setting for the current process
class SupervisionSettingLoader {
 public:
    // For load
    bool LoadFromYamlImpl(const std::string& yaml_path);
    void Reset();

    senseAD::base::optional<SupervisionSetting> GetSetting(
        const SupervisionUniqueId uid) const;
    senseAD::base::optional<SupervisionSetting> GetCommSetting(
        const std::string& topic_name) const;

 private:
    // Actual data
    std::unordered_map<std::string, SupervisionSetting> comm_settings_;
    std::unordered_map<SupervisionUniqueId, SupervisionSetting> settings_;
};

// The setting for the entire project
class SupervisionSettingStore {
 public:
    void Init(const std::string& process_name);

    senseAD::base::optional<SupervisionSetting> GetSetting(
        const SupervisionUniqueId uid) const;
    senseAD::base::optional<SupervisionSetting> GetCommSetting(
        const std::string& topic_name) const;

    std::string ProcessName() const;
    inline std::string Identifier() const { return identifier_; }
    inline std::string PlatformName() const { return platform_name_; }

 private:
    void Load();

    std::string process_name_;
    std::string identifier_;
    std::string platform_name_;

    // project level setting
    std::unique_ptr<SupervisionSettingLoader> global_setting_;

    // process level setting
    std::unique_ptr<SupervisionSettingLoader> process_setting_;

    DECLARE_AD_SINGLETON(SupervisionSettingStore)
};

}  // namespace diag
}  // namespace senseAD
