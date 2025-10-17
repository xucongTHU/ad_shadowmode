/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>

#include "ad_rscl/comm/cuda/base.h"

namespace senseAD {
namespace rscl {
namespace comm {

class SegmentCreator {
 public:
    explicit SegmentCreator(const MemoryPrivateConfig_t cfg) : cfg_(cfg) {}
    virtual ~SegmentCreator() = default;
    virtual uint8_t *CreateShmSegment(const std::string &name,
                                      const size_t size,
                                      const size_t alignment,
                                      bool *new_segment) const;

    virtual size_t SegmentCudaPartByteSize(
        const size_t data_buffer_size) const = 0;
    virtual uint8_t *InitializeSegmentCudaPart(
        uint8_t *shm_addr,
        const size_t data_buffer_size,
        const bool new_segment,
        const size_t alignment,
        const uint32_t device_id,
        std::shared_ptr<uint8_t> *cleanup_holder) const = 0;

 protected:
    MemoryPrivateConfig_t cfg_;
};

class CudaIpcSegmentCreator : public SegmentCreator {
 public:
    explicit CudaIpcSegmentCreator(const MemoryPrivateConfig_t cfg)
        : SegmentCreator(cfg) {}

    size_t SegmentCudaPartByteSize(
        const size_t data_buffer_size) const override;
    uint8_t *InitializeSegmentCudaPart(
        uint8_t *shm_addr,
        const size_t data_buffer_size,
        const bool new_segment,
        const size_t alignment,
        const uint32_t device_id,
        std::shared_ptr<uint8_t> *cleanup_holder) const override;
};

class NoGpuMemSegmentCreator : public SegmentCreator {
 public:
    explicit NoGpuMemSegmentCreator(const MemoryPrivateConfig_t cfg)
        : SegmentCreator(cfg) {}

    size_t SegmentCudaPartByteSize(
        const size_t data_buffer_size) const override;
    uint8_t *InitializeSegmentCudaPart(
        uint8_t *shm_addr,
        const size_t data_buffer_size,
        const bool new_segment,
        const size_t alignment,
        const uint32_t device_id,
        std::shared_ptr<uint8_t> *cleanup_holder) const override;
};

class IntegratedGpuMemSegmentCreator : public SegmentCreator {
 public:
    explicit IntegratedGpuMemSegmentCreator(const MemoryPrivateConfig_t cfg)
        : SegmentCreator(cfg) {}

    size_t SegmentCudaPartByteSize(
        const size_t data_buffer_size) const override;
    uint8_t *InitializeSegmentCudaPart(
        uint8_t *shm_addr,
        const size_t data_buffer_size,
        const bool new_segment,
        const size_t alignment,
        const uint32_t device_id,
        std::shared_ptr<uint8_t> *cleanup_holder) const override;
};

class SegmentCreatorFactory {
 public:
    static std::unique_ptr<SegmentCreator> Create(
        const MemoryPrivateConfig_t cfg);
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
