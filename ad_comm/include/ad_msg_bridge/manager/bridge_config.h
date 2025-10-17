/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */
#ifndef AD_MSG_BRIDGE_MANAGER_BRIDGE_CONFIG_H_
#define AD_MSG_BRIDGE_MANAGER_BRIDGE_CONFIG_H_

#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>
#include <set>
#include <map>

#include "ad_mw/proto/comm_conf.pb.h"
#include "ad_base/common/project_env.h"

namespace senseAD {
namespace ad_msg_bridge {

using PublisherConf = senseAD::mw::proto::PublisherConf;
using SubscriberConf = senseAD::mw::proto::SubscriberConf;

// Topic status and direction
// topic configures in yaml file
// like ros queue_size and namespace
class Topic {
 public:
    Topic()
        : topic_key_(""),
          topic_name_(""),
          msg_type_(""),
          node_name_(""),
          node_(""),
          ns_(""),
          queue_size_(0),
          used_(false) {}
    Topic(const Topic &) = default;
    Topic(Topic &&) = default;
    Topic &operator=(const Topic &) = default;

    // Builders
    Topic &SetTopicKey(std::string topic_key) {
        if (!topic_key.empty()) {
            topic_key_ = topic_key;
        }
        return *this;
    }
    Topic &SetTopicName(std::string topic_name) {
        if (!topic_name.empty()) {
            topic_name_ = topic_name;
        }
        return *this;
    }
    Topic &SetMsgType(std::string msg_type) {
        if (!msg_type.empty()) {
            msg_type_ = msg_type;
        }
        return *this;
    }
    Topic &SetNs(std::string ns) {
        if (!ns.empty()) {
            ns_ = ns;
        }
        return *this;
    }
    Topic &SetNodeName(std::string node_name) {
        if (!node_name.empty()) {
            node_name_ = node_name;
        }
        return *this;
    }
    Topic &SetNode(std::string node) {
        if (!node.empty()) {
            node_ = node;
        }
        return *this;
    }
    Topic &SetQueueSize(uint32_t queue_size) {
        queue_size_ = queue_size;
        return *this;
    }
    Topic &SetUsed(bool used) {
        used_ = used;
        return *this;
    }

    std::string topic_key() const { return topic_key_; }
    std::string msg_type() const { return msg_type_; }
    std::string topic_name() const { return topic_name_; }
    std::string ns() const { return ns_; }
    std::string node_name() const { return node_name_; }
    std::string node() const { return node_; }
    uint32_t queue_size() const { return queue_size_; }
    bool used() const { return used_; }
    void set_used(bool used) { used_ = used; }

    std::string DebugString() const;

    PublisherConf ToPublisherConf() const;
    SubscriberConf ToSubscriberConf() const;

 private:
    // Required
    // topic key in yaml file, index in map
    std::string topic_key_;
    std::string topic_name_;
    // msg_type: name to load class from dynamic library
    std::string msg_type_;
    // node_name: node name of publisher or subscriber
    std::string node_name_;
    // name: local node
    std::string node_;
    // ns: namespace of publisher or subscriber
    std::string ns_;
    // queue_size: queue size of publisher or subscriber
    uint32_t queue_size_;

    bool used_;
};

class MsgBridgeConfig {
 public:
    explicit MsgBridgeConfig(const std::string &file_name);
    explicit MsgBridgeConfig(
        const std::map<std::string, Topic> &topics,
        const std::set<std::string> &dynamic_libs,
        const std::string &conf_path =
            senseAD::base::common::kMsgBridgeDefaultConfPath)
        : dynamic_libs_(dynamic_libs),
          topics_(topics),
          config_path_(conf_path) {}
    MsgBridgeConfig()
        : dynamic_libs_(),
          topics_(),
          config_path_(senseAD::base::common::kMsgBridgeDefaultConfPath) {}

    void SetTopicPrefix(const std::string topic_prefix);
    void SetBridgeConfig(const YAML::Node &node, const std::string &name = "");
    bool IsBridgeEnabled() const;
    bool HasMsgBridge(const std::string &topic_key) const;
    std::string GetConfigPath() const { return config_path_; }

    // key: prefix + topic_name
    const std::map<std::string, Topic> &topics() const;

    std::set<std::string> rscl_to_bridge_topics() const;
    std::set<std::string> bridge_to_rscl_topics() const;

    // return prefix + topic_name
    std::string GetTopicWithPrefix(const std::string &topic_name) const;

    const std::set<std::string> &dynamic_libs() const { return dynamic_libs_; }

    friend std::ostream &operator<<(std::ostream &os,
                                    const MsgBridgeConfig &conf);

 private:
    std::string topic_prefix_;
    std::set<std::string> dynamic_libs_;
    std::map<std::string, Topic> otopics_;
    std::map<std::string, Topic> topics_;
    std::string config_path_;

    std::set<std::string> rscl_to_bridge_topics_;
    std::set<std::string> bridge_to_rscl_topics_;
};

}  // namespace ad_msg_bridge
}  // namespace senseAD
#endif /* AD_MSG_BRIDGE_MANAGER_BRIDGE_CONFIG_H_ */
