/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "ad_mw/proto/runtime_conf.pb.h"
#include "ad_rscl/idl/global_config.pb.h"
#include "ad_base/common/macros.h"
#include "ad_msg_bridge/manager/bridge_config.h"
#include "ad_base/std/optional.h"

namespace senseAD {

namespace rscl {

class Runtime;

/**
 * @brief Global Configuration that init from user config and environment
 *
 */
class GlobalConfig {
 public:
    void InitFromGlobalConfFile(const std::string& process_name);
    void InitFromGlobalConfFile(const std::string& process_name,
                                const idl::GlobalConfig& conf);

    std::string ProcessName() const;

    std::string HostName() const;
    std::string HostIp() const;
    std::string HostPlatform() const;
    const std::unordered_map<std::string, std::string>& Envs() const;

    const idl::GlobalConfig& RawIdlConf() const;
    const senseAD::mw::proto::RuntimeConfig GetMwConf() const;

    //***************************
    //      Utils Functions
    //***************************

    // For hetero topic
    std::vector<idl::HeteroChannelConfig> GetHeteroConfigsForTopic(
        const std::string& node_name,
        const std::string& topic,
        const std::set<idl::TopicHeteroChannelConfig_EnabledSide>&
            target_sides);
    bool IsHeteroOnly(const std::string& node_name,
                      const std::string& topic) const;

    // For cross system
    bool IsTopicCrossSystem(const std::string& topic) const;

 private:
    void TrimConfigByConditionCheck(const std::string& process_name,
                                    idl::GlobalConfig* conf);
    void ExpandGlobalConfig(const std::string& process_name,
                            idl::GlobalConfig* conf);
    bool LoadExtendConfFiles(const std::string& process_name,
                             idl::GlobalConfig* conf);
    void SetEnvironmentVariableFromConfig(const std::string& process_name,
                                          const idl::GlobalConfig& conf);
    void InitHostInfo();

    void SetMwConf(const senseAD::mw::proto::RuntimeConfig& cfg);
    idl::GlobalConfig conf_;

    std::string process_name_;
    bool is_user_targetd_process_name_ = false;

    std::string host_name_;
    std::string host_ip_;
    std::unordered_map<std::string, std::string> envs_;
    senseAD::mw::proto::RuntimeConfig mw_runtime_cfg_;

    friend class Runtime;
    DECLARE_AD_SINGLETON(GlobalConfig)
};

void VerifyGlobalConfig(const idl::GlobalConfig& conf);

}  // namespace rscl
}  // namespace senseAD
