/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>

#include "ad_rscl/comm/node.h"
#include "ad_buf/ad_buf.h"
#include "ad_buf/impl_shm/ad_buf_shm.h"
#include "ad_rscl/idl/buf_comm_conf.pb.h"
#include "ad_rscl/idl/buf_comm_message.capnp.h"
#include "ad_rscl/comm/publisher_base.h"
#include "ad_rscl/common/errcode.h"
#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::ad_buf::AdBuf;
using senseAD::ad_buf::AdBufManager;
using senseAD::ad_buf::ShmBufCfg;
using senseAD::rscl::idl::BufNewMessageNotification;
using senseAD::rscl::idl::BufPublisherConf;

class BufPublisher : public PublisherBase {
 public:
    using Ptr = std::shared_ptr<BufPublisher>;
    BufPublisher(Node* node_owner, const BufPublisherConf& conf);
    virtual std::error_code Publish(const AdBuf& msg);
    virtual std::error_code Publish(const AdBuf::Ptr& msg);
    std::error_code PublishWithWUnlcok(const AdBuf& msg);
    std::error_code PublishWithWUnlcok(const AdBuf::Ptr& msg);

    const BufPublisherConf& GetBufPublisherConf() const;

    virtual ~BufPublisher() {}

 private:
    std::error_code NoticeSubscriber(const AdBuf& msg);
    PublisherConf MockPublisherConf() const;
    PublisherConf GenPublisherConf(const BufPublisherConf& in);
    const Publisher<BufNewMessageNotification>::Ptr& GetInnerPublisher() const;

 private:
    BufPublisherConf conf_;
    Publisher<BufNewMessageNotification>::Ptr inner_pub_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
