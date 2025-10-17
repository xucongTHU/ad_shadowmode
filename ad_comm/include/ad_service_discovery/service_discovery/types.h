/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <string>
#include <memory>
#include <utility>
#include <functional>

namespace senseAD {
namespace service_discovery {

struct SrvDiscRoleAttr {
    enum class Identity { NONE, NODE, READER, WRITER, CLIENT, SERVER };
    Identity identity;
    std::string node_name;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(identity, node_name);
    }
};

struct SrvDiscEvent {
    enum class Action { NONE, EVENT_JOIN, EVENT_EXIT };
    SrvDiscRoleAttr::Identity identity;
    Action action;
    // TODO(zhoupenghui) this is not using
    uint64_t timestamp;
    std::string node_name;
    std::string channel_name;
    std::string service_name;
    std::string msg_type;
    std::string msg_desc;

    inline SrvDiscEvent& SetIdentity(const SrvDiscRoleAttr::Identity& e) {
        identity = e;
        return *this;
    }
    inline SrvDiscEvent& SetAction(const Action& e) {
        action = e;
        return *this;
    }
    inline SrvDiscEvent& SetTimestamp(const uint64_t& e) {
        timestamp = e;
        return *this;
    }
    inline SrvDiscEvent& SetNodeName(const std::string& e) {
        node_name = e;
        return *this;
    }
    inline SrvDiscEvent& SetChannelName(const std::string& e) {
        channel_name = e;
        return *this;
    }
    inline SrvDiscEvent& SetServiceName(const std::string& e) {
        service_name = e;
        return *this;
    }
    inline SrvDiscEvent& SetMsgType(const std::string& e) {
        msg_type = e;
        return *this;
    }
    inline SrvDiscEvent& SetMsgDesc(const std::string& e) {
        msg_desc = e;
        return *this;
    }

    inline std::string AsKey() const {
        return node_name + "#" + channel_name + "#" + service_name + "#" +
               std::to_string(static_cast<int>(identity));
    }

    inline std::string DebugString() const {
        return AsKey() + "#A:" + std::to_string(static_cast<int>(action)) +
               "#T:" + msg_type + "#DS:" + std::to_string(msg_desc.size()) +
               "#TS:" + std::to_string(timestamp);
    }

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(identity, action, timestamp, node_name, channel_name,
                service_name, msg_type, msg_desc);
    }

    // overide operator ==
    bool operator==(const SrvDiscEvent& other) const {
        return identity == other.identity && action == other.action &&
               node_name == other.node_name &&
               channel_name == other.channel_name &&
               service_name == other.service_name &&
               msg_type == other.msg_type && msg_desc == other.msg_desc;
    }
};

using SrvDiscListenerCallback =
    std::function<void(const std::shared_ptr<SrvDiscEvent>&)>;

}  // namespace service_discovery
}  // namespace senseAD
