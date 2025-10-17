/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "ad_base/common/shutdownable.h"
#include "ad_rscl/global_config.h"
#include "ad_rscl/idl/global_config.pb.h"
#include "ad_rscl/idl/hetero_channel_config.pb.h"
#include "ad_rscl/comm/hetero_channel/basic_type.h"
#include "ad_rscl/comm/hetero_channel/channel_factory.h"
#include "ad_rscl/comm/subscriber_factory.h"
#include "ad_rscl/comm/publisher_factory.h"
#include "ad_rscl/idl/hetero_channel_control.capnp.h"
#include "ad_rscl/comm/subscriber_base.h"
#include "ad_rscl/comm/publisher_base.h"
#include "ad_rscl/comm/hetero_channel/zmq_forward/zmq_forward_sub.h"
#include "ad_rscl/comm/hetero_channel/zmq_forward/zmq_forward_session_manager.h"
#include "ad_rscl/common/internal_topics.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

void Shutdown();

class ChannelManager : public ::Shutdownable {
 public:
    ~ChannelManager();
    void ShutdownImpl() override;
    void Init(NodeOwnerBase* const node_owner);
    void InitControlListener(NodeOwnerBase* const node_owner);
    std::vector<HeteroChannelConfig> GetPublishersHeteroConfigsForTopic(
        const std::string& node_name, const std::string& topic) const;
    std::vector<HeteroChannelConfig> GetSubscribersHeteroConfigsForTopic(
        const std::string& node_name, const std::string& topic) const;
    void AddChannel(const std::shared_ptr<HeteroPubSubBase>& channel_handler);
    void AddChannel(
        const std::vector<std::shared_ptr<PublisherBase>>& channel_handlers);
    void AddChannel(
        const std::vector<std::shared_ptr<SubscriberBase>>& channel_handlers);

    /**
     * @brief Stop sending messages on all hetero channels created by this
     * process.
     */
    void StopAllChannels() const;
    /**
     * @brief Start sending messages on all hetero channels created by this
     * process.
     */
    void StartAllChannels() const;

    bool IsEnabled() const;
    DECLARE_AD_SINGLETON(ChannelManager)

 private:
    std::vector<std::shared_ptr<HeteroPubSubBase>> channels_;
    bool is_inited_{false};
    std::atomic<bool> is_enabled_{false};
    std::shared_ptr<Subscriber<senseAD::rscl::idl::ControlMessage>>
        ctrl_listener_;
    std::shared_ptr<Publisher<senseAD::rscl::idl::ControlMessage>>
        ctrl_publisher_;

    bool check_is_running_{false};
    std::vector<std::shared_ptr<void>>
        managed_objects_;  // used for managing object lifecycles
    mutable std::mutex channels_mutex_;
};

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
