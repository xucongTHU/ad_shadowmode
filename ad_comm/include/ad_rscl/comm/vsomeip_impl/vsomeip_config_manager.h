/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "ad_log/ad_log.hpp"
#include "ad_rscl/comm/vsomeip_impl/common.h"

#include "json11.hpp"

namespace senseAD {
namespace rscl {
namespace comm {
namespace someip {

enum class NodeRole { CLIENT, SERVICE, BOTH };

class VSOMEIPConfigManager {
 public:
    // Get config manager instance (singleton)
    static VSOMEIPConfigManager& GetInstance();

    // Load configuration from file
    bool LoadConfig(const std::string& config_path);

    // Save configuration to file
    bool SaveConfig(const std::string& config_path = "");

    // Update configuration based on role and SomeipEventConfig
    bool UpdateConfig(const SomeipEventConfig& config,
                      const std::string& application_name = "",
                      NodeRole role = NodeRole::BOTH);

    // Get complete configuration
    json11::Json GetConfig() const { return config_; }

    // Set network parameters
    void SetNetworkInfo(const std::string& network_interface,
                        const std::string& unicast_address,
                        const std::string& netmask = "255.255.255.0");

    // One-stop configuration function: load, update, save config and set
    // environment variable Uses standard config path by default, or custom path
    // if provided
    bool SetupAndApplyConfig(const SomeipEventConfig& config,
                             const std::string& application_name,
                             NodeRole role = NodeRole::BOTH,
                             const std::string& config_path = "");
    bool SetupAndApplyConfig(const SomeipMethodConfig& config,
                             const std::string& application_name,
                             NodeRole role = NodeRole::BOTH,
                             const std::string& config_path = "");

    // Set VSOMEIP configuration environment variable
    void SetVsomeipConfigEnv(const std::string& config_file = "");

 private:
    VSOMEIPConfigManager() = default;
    ~VSOMEIPConfigManager() = default;

    // Disable copy and assignment
    VSOMEIPConfigManager(const VSOMEIPConfigManager&) = delete;
    VSOMEIPConfigManager& operator=(const VSOMEIPConfigManager&) = delete;

    // Update client configuration
    bool UpdateClientConfig(const SomeipEventConfig& config);

    // Update service configuration
    bool UpdateServiceConfig(const SomeipEventConfig& config);

    // Update E2E protection configuration
    bool UpdateE2EConfig(const SomeipEventConfig& config);

    // Update application configuration
    bool UpdateApplicationConfig(const std::string& name, uint32_t id);

    // Check if service/instance configuration already exists
    bool ServiceExists(const SomeipEventConfig& config, bool is_client);

    // Convert hex value to string format "0x1234"
    std::string ToHexString(uint32_t value) const;

    // Create empty JSON array
    json11::Json CreateArray() const;

    // Create empty JSON object
    json11::Json CreateObject() const;

    // Format JSON with proper indentation and line breaks
    std::string FormatJson(const json11::Json& json,
                           int indent_level = 0) const;

    // Generate application ID based on application name
    uint16_t GenerateAppIdFromName(const std::string& name) const;

    json11::Json config_;              // Configuration content
    std::string current_config_path_;  // Current configuration file path
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
