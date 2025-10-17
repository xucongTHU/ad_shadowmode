/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/cuda/topic.h"
#include "ad_rscl/comm/subscriber_base.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::rscl::idl::CudaShmNewMessageNotification;
using senseAD::rscl::idl::CudaSubscriberConf;

struct CudaInfo {
    int device_id;
};

/**
 * @brief An specialized subscriber received message from CudaPublisher
 * Support device with nvidia gpu or nvidia jetson series
 */
class CudaSubscriber : public SubscriberBase {
 public:
    using Ptr = std::shared_ptr<CudaSubscriber>;
    using CallbackFunc = std::function<void(const std::shared_ptr<Memblock>&)>;
    using CallbackFuncWithInfo =
        std::function<void(const std::shared_ptr<Memblock>&, const CudaInfo&)>;

    CudaSubscriber(Node* const node_owner,
                   const CudaSubscriber::CallbackFunc& cb,
                   const CudaSubscriberConf& conf);
    CudaSubscriber(Node* const node_owner,
                   const CudaSubscriber::CallbackFuncWithInfo& cb,
                   const CudaSubscriberConf& conf);

    virtual ~CudaSubscriber();
    // void Observe() const override;
    // std::shared_ptr<Memblock> GetLatestObserved() const override;

    const CudaSubscriberConf& GetCudaSubscriberConf() const;

 private:
    Topic* GetUidTopic(const uint64_t pid, const std::string& uid) const;
    std::shared_ptr<Memblock> MemblockFromNotification(
        const CudaShmNewMessageNotification::Reader& notification,
        CudaInfo* info) const;
    void InnerProcess(const CudaShmNewMessageNotification::Reader& msg) const;

    CudaSubscriberConf conf_;
    const CudaSubscriber::CallbackFuncWithInfo inner_cb_;
    Subscriber<CudaShmNewMessageNotification>::Ptr inner_sub_;

    mutable std::unordered_map<std::string, std::unique_ptr<Topic>>
        uid_topic_map_;
    mutable std::unordered_map<std::string, uint64_t> uid_pid_map_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
