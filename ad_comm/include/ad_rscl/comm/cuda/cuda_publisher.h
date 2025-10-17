/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>

#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/cuda/topic.h"
#include "ad_rscl/comm/publisher_base.h"
#include "ad_rscl/idl/cuda_message_conf.pb.h"
#include "ad_rscl/idl/cuda_message.capnp.h"
#include "ad_rscl/common/errcode.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::rscl::idl::CudaShmNewMessageNotification;

struct PreallocatedMemblock {
 public:
    PreallocatedMemblock(std::shared_ptr<Memblock> block,
                         std::weak_ptr<Topic> topic);
    ~PreallocatedMemblock();
    std::shared_ptr<Memblock> GetMemBlock() const;

 private:
    std::shared_ptr<Memblock> block_;
    std::weak_ptr<Topic> topic_;
};

/**
 * @brief An specialized publisher sent message through nvidia gpu.
 * Only allow sending Memblock which is actually a continous memory block
 * Support device with nvidia gpu or nvidia jetson series
 *
 * Its inner mechanism is spliting the actual message sharing and signaling into
 * two part.
 * 1. Write the data into a cuda shared memory for sharing
 * 2. Let the normal cpu publisher sent the notification signal to receiver. Let
 * the receiver to get the data from shared location
 */
class CudaPublisher : public PublisherBase {
 public:
    using Ptr = std::shared_ptr<CudaPublisher>;

    CudaPublisher(Node* node_owner, const CudaPublisherConf& conf);

    virtual ~CudaPublisher();

    // New function
    virtual std::error_code Publish(const Memblock& msg);
    virtual std::error_code Publish(const std::shared_ptr<Memblock>& msg);
    virtual std::error_code Publish(
        const std::shared_ptr<PreallocatedMemblock>& msg);

    /**
     * @brief Take a memory block ownership from the shm
     * You have two way to use the block
     * 1. Reuse the created block
     * 2. Create a block -> assign value -> Publish -> release
     * !IMPORTANT: Cannot mix this two way. Otherwise, undefined behaviour
     *
     * @param block_byte size of the request block
     * @return std::shared_ptr<PreallocatedMemblock>
     */
    std::shared_ptr<PreallocatedMemblock> NewMemblockInplace(
        const uint64_t block_byte);

    const CudaPublisherConf& GetCudaPublisherConf() const;

 private:
    bool WriteToMemory(const Memblock& msg,
                       CudaShmNewMessageNotification::Builder* out_notify);
    void NoticeSubscriber(const Memblock& msg);

    CudaPublisherConf conf_;
    Publisher<CudaShmNewMessageNotification>::Ptr inner_pub_;

    uint64_t pid_;
    std::string uid_;
    std::shared_ptr<Topic> topic_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
