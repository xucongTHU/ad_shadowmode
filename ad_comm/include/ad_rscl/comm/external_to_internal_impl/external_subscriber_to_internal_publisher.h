/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhangyi <zhangyi15@senseauto.com>
 */

#pragma once

#include <memory>

#include "ad_rscl/comm/internal_impl/internal_publisher.h"
#include "ad_rscl/comm/external_impl/external_subscriber.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename MessageT>
class ExternalSubscriberToInternalPublisher {
 public:
    using Ptr =
        std::shared_ptr<ExternalSubscriberToInternalPublisher<MessageT>>;

    ExternalSubscriberToInternalPublisher(NodeOwnerBase* const node_owner,
                                          const CallbackFunc<MessageT>& cb,
                                          const SubscriberConf& sub_conf,
                                          const PublisherConf& pub_conf) {
        internal_pub_ =
            std::make_shared<InternalPublisher<MessageT>>(node_owner, pub_conf);

        external_sub_ = std::make_shared<ExternalSubscriber<MessageT>>(
            node_owner,
            [this, cb](const std::shared_ptr<ReceivedMsg<MessageT>>& recv_msg) {
                if (recv_msg) {
                    if (internal_pub_) {
                        auto send_msg = std::make_shared<SendMsg<MessageT>>(
                            recv_msg->Msg());
                        internal_pub_->Publish(send_msg);
                    }
                    cb(recv_msg);
                }
            },
            sub_conf);
    }

    std::shared_ptr<Publisher<MessageT>> GetInternalPublisher() const {
        return internal_pub_;
    }

    std::shared_ptr<Subscriber<MessageT>> GetExternalSubscriber() const {
        return external_sub_;
    }

 private:
    std::shared_ptr<Publisher<MessageT>> internal_pub_;
    std::shared_ptr<Subscriber<MessageT>> external_sub_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
