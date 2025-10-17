/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <mutex>
#include <map>
#include <limits>
#include <memory>
#include <atomic>
#include <utility>
#include <sstream>

namespace senseAD {
namespace rscl {
namespace comm {
namespace someip {

constexpr uint16_t kE2EDataSize = 12;

enum class event_type_e : uint8_t {
    ET_EVENT = 0x00,
    ET_SELECTIVE_EVENT = 0x01,
    ET_FIELD = 0x02,
    ET_UNKNOWN = 0xFF
};
struct SomeipConfig {
    uint16_t service_id;
    uint16_t instance_id;
    uint8_t major_version{1};
    uint32_t minor_version{0};
    bool e2e_enabled{false};  // Added missing member variable

    // Constructor with default values
    SomeipConfig() = default;

    // Constructor with required fields
    SomeipConfig(uint16_t service_id_,
                 uint16_t instance_id_,
                 bool e2e_enabled_ = false)
        : service_id(service_id_),
          instance_id(instance_id_),
          e2e_enabled(e2e_enabled_) {}

    // Validate configuration
    bool IsValid() const {
        // Check required fields are set
        if (service_id == 0 || instance_id == 0) {
            return false;
        }
        return true;
    }

    // e2e enable
    bool IsE2EEnabled() const { return e2e_enabled; }

    // Clone this config
    SomeipConfig Clone() const { return *this; }

    // Create string representation for logging/debugging
    std::string ToString() const {
        std::stringstream ss;
        ss << "SomeipConfig{"
           << "service_id:" << service_id << ", instance_id:" << instance_id
           << ", major_version:" << static_cast<int>(major_version)
           << ", minor_version:" << minor_version << "}";
        return ss.str();
    }

    // Reset config to default values
    void Reset() {
        service_id = 0;
        instance_id = 0;
        major_version = 1;
        minor_version = 0;
    }

    // Compare two configs
    bool operator==(const SomeipConfig& other) const {
        return service_id == other.service_id &&
               instance_id == other.instance_id &&
               major_version == other.major_version &&
               minor_version == other.minor_version;
    }

    bool operator!=(const SomeipConfig& other) const {
        return !(*this == other);
    }

    // Setter methods with chaining support
    SomeipConfig& SetServiceId(uint16_t service_id_) {
        service_id = service_id_;
        return *this;
    }

    SomeipConfig& SetInstanceId(uint16_t instance_id_) {
        instance_id = instance_id_;
        return *this;
    }

    SomeipConfig& SetMajorVersion(uint8_t major_version_) {
        major_version = major_version_;
        return *this;
    }

    SomeipConfig& SetMinorVersion(uint32_t minor_version_) {
        minor_version = minor_version_;
        return *this;
    }

    SomeipConfig& SetE2EEnabled(bool e2e_enabled_) {
        e2e_enabled = e2e_enabled_;
        return *this;
    }
};

struct SomeipEventConfig {
    SomeipConfig config;
    uint16_t event_id;
    uint16_t event_group_id;
    event_type_e event_type;

    SomeipEventConfig() = default;

    SomeipEventConfig(uint16_t service_id_,
                      uint16_t instance_id_,
                      uint16_t event_id_,
                      uint16_t event_group_id_,
                      bool e2e_enabled_ = false,
                      event_type_e event_type_ = event_type_e::ET_EVENT)
        : config(service_id_, instance_id_, e2e_enabled_),
          event_id(event_id_),
          event_group_id(event_group_id_),
          event_type(event_type_) {}

    uint16_t service_id() const { return config.service_id; }
    uint16_t instance_id() const { return config.instance_id; }
    bool IsE2EEnabled() const { return config.IsE2EEnabled(); }
    uint8_t major_version() const { return config.major_version; }
    uint32_t minor_version() const { return config.minor_version; }

    bool IsValid() const {
        if (!config.IsValid() || event_id == 0) {
            return false;
        }
        return true;
    }

    SomeipEventConfig Clone() const { return *this; }

    std::string ToString() const {
        std::stringstream ss;
        ss << "SomeipEventConfig{"
           << "service_id:" << config.service_id
           << ", instance_id:" << config.instance_id
           << ", event_id:" << event_id << ", event_group_id:" << event_group_id
           << ", major_version:" << static_cast<int>(config.major_version)
           << ", minor_version:" << config.minor_version << "}";
        return ss.str();
    }

    void Reset() {
        config.Reset();
        event_id = 0;
        event_group_id = 0;
        event_type = event_type_e::ET_EVENT;
    }

    bool operator==(const SomeipEventConfig& other) const {
        return config == other.config && event_id == other.event_id &&
               event_group_id == other.event_group_id &&
               event_type == other.event_type;
    }

    bool operator!=(const SomeipEventConfig& other) const {
        return !(*this == other);
    }

    // Setter methods with chaining support
    SomeipEventConfig& SetConfig(const SomeipConfig& config_) {
        config = config_;
        return *this;
    }

    SomeipEventConfig& SetEventId(uint16_t event_id_) {
        event_id = event_id_;
        return *this;
    }

    SomeipEventConfig& SetEventGroupId(uint16_t event_group_id_) {
        event_group_id = event_group_id_;
        return *this;
    }

    SomeipEventConfig& SetEventType(event_type_e event_type_) {
        event_type = event_type_;
        return *this;
    }

    // Delegate setters for convenience
    SomeipEventConfig& SetServiceId(uint16_t service_id_) {
        config.service_id = service_id_;
        return *this;
    }

    SomeipEventConfig& SetInstanceId(uint16_t instance_id_) {
        config.instance_id = instance_id_;
        return *this;
    }

    SomeipEventConfig& SetMajorVersion(uint8_t major_version_) {
        config.major_version = major_version_;
        return *this;
    }

    SomeipEventConfig& SetMinorVersion(uint32_t minor_version_) {
        config.minor_version = minor_version_;
        return *this;
    }

    SomeipEventConfig& SetE2EEnabled(bool e2e_enabled_) {
        config.e2e_enabled = e2e_enabled_;
        return *this;
    }
};

struct SomeipMethodConfig {
    SomeipConfig config;  // 使用组合而非继承
    uint16_t method_id;

    SomeipMethodConfig() = default;

    SomeipMethodConfig(uint16_t service_id_,
                       uint16_t instance_id_,
                       uint16_t method_id_,
                       bool e2e_enabled_ = false)
        : config(service_id_, instance_id_, e2e_enabled_),
          method_id(method_id_) {}

    // 委托方法
    uint16_t service_id() const { return config.service_id; }
    uint16_t instance_id() const { return config.instance_id; }
    bool IsE2EEnabled() const { return config.IsE2EEnabled(); }
    uint8_t major_version() const { return config.major_version; }
    uint32_t minor_version() const { return config.minor_version; }

    bool IsValid() const {
        if (!config.IsValid() || method_id == 0) {
            return false;
        }
        return true;
    }

    SomeipMethodConfig Clone() const { return *this; }

    std::string ToString() const {
        std::stringstream ss;
        ss << "SomeipMethodConfig{"
           << "service_id:" << config.service_id
           << ", instance_id:" << config.instance_id
           << ", method_id:" << method_id
           << ", major_version:" << static_cast<int>(config.major_version)
           << ", minor_version:" << config.minor_version << "}";
        return ss.str();
    }

    void Reset() {
        config.Reset();
        method_id = 0;
    }

    bool operator==(const SomeipMethodConfig& other) const {
        return config == other.config && method_id == other.method_id;
    }

    bool operator!=(const SomeipMethodConfig& other) const {
        return !(*this == other);
    }

    // Setter methods with chaining support
    SomeipMethodConfig& SetConfig(const SomeipConfig& config_) {
        config = config_;
        return *this;
    }

    SomeipMethodConfig& SetMethodId(uint16_t method_id_) {
        method_id = method_id_;
        return *this;
    }

    // Delegate setters for convenience
    SomeipMethodConfig& SetServiceId(uint16_t service_id_) {
        config.service_id = service_id_;
        return *this;
    }

    SomeipMethodConfig& SetInstanceId(uint16_t instance_id_) {
        config.instance_id = instance_id_;
        return *this;
    }

    SomeipMethodConfig& SetMajorVersion(uint8_t major_version_) {
        config.major_version = major_version_;
        return *this;
    }

    SomeipMethodConfig& SetMinorVersion(uint32_t minor_version_) {
        config.minor_version = minor_version_;
        return *this;
    }

    SomeipMethodConfig& SetE2EEnabled(bool e2e_enabled_) {
        config.e2e_enabled = e2e_enabled_;
        return *this;
    }
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD

// 在全局命名空间中定义hash特化
namespace std {
template <>
struct hash<senseAD::rscl::comm::someip::SomeipConfig> {
    size_t operator()(
        const senseAD::rscl::comm::someip::SomeipConfig& config) const {
        size_t h1 = std::hash<uint16_t>{}(config.service_id);
        size_t h2 = std::hash<uint16_t>{}(config.instance_id);
        return h1 ^ (h2 << 1);
    }
};

template <>
struct hash<senseAD::rscl::comm::someip::SomeipEventConfig> {
    size_t operator()(
        const senseAD::rscl::comm::someip::SomeipEventConfig& config) const {
        size_t h1 = std::hash<uint16_t>{}(config.service_id());
        size_t h2 = std::hash<uint16_t>{}(config.instance_id());
        size_t h3 = std::hash<uint16_t>{}(config.event_id);
        size_t h4 = std::hash<uint16_t>{}(config.event_group_id);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

template <>
struct hash<senseAD::rscl::comm::someip::SomeipMethodConfig> {
    size_t operator()(
        const senseAD::rscl::comm::someip::SomeipMethodConfig& config) const {
        size_t h1 = std::hash<uint16_t>{}(config.service_id());
        size_t h2 = std::hash<uint16_t>{}(config.instance_id());
        size_t h3 = std::hash<uint16_t>{}(config.method_id);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
}  // namespace std
