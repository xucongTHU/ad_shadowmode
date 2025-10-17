/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/subscriber_base.h"
#include "ad_rscl/idl/buf_comm_conf.pb.h"
#include "ad_rscl/idl/buf_comm_message.capnp.h"
#include "ad_rscl/comm/internal_impl/internal_subscriber.h"
#include "ad_buf/ad_buf.h"
#include "ad_buf/impl_shm/ad_buf_shm.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::ad_buf::AdBuf;
using senseAD::ad_buf::AdBufManager;
using senseAD::ad_buf::BufCfg;
using senseAD::ad_buf::ShmBufCfg;
using senseAD::rscl::idl::BufNewMessageNotification;
using senseAD::rscl::idl::BufSubscriberConf;

class BufSubscriber : public SubscriberBase {
 public:
    using Ptr = std::shared_ptr<BufSubscriber>;
    using CallbackFunc = std::function<void(AdBuf::ConstPtr&)>;
    using TimeCallbackFunc = std::function<void(AdBuf::ConstPtr&, uint64_t)>;

    BufSubscriber(Node* const node_owner,
                  const BufSubscriber::CallbackFunc& cb,
                  const BufSubscriberConf& conf,
                  const std::shared_ptr<BufCfg>& buf_meta = nullptr);

    BufSubscriber(Node* const node_owner,
                  const BufSubscriber::TimeCallbackFunc& cb,
                  const BufSubscriberConf& conf,
                  const std::shared_ptr<BufCfg>& buf_meta = nullptr);

    virtual ~BufSubscriber();

    const BufSubscriberConf& GetBufSubscriberConf() const;

 private:
    AdBuf::ConstPtr AdBufFromNotification(
        const BufNewMessageNotification::Reader& notification) const;
    void InnerProcess(const BufNewMessageNotification::Reader& msg) const;
    void TimeInnerProcess(const BufNewMessageNotification::Reader& msg) const;
    void CheckTransmitTime(uint64_t before, uint64_t after) const;

    const BufSubscriber::CallbackFunc inner_cb_;
    const BufSubscriber::TimeCallbackFunc time_inner_cb_;
    BufSubscriberConf conf_;
    Subscriber<BufNewMessageNotification>::Ptr inner_sub_;
    mutable std::unordered_map<std::string, std::shared_ptr<AdBufManager>>
        mgrs_;
    std::shared_ptr<BufCfg> base_buf_meta_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
