/*
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Mu Chunpeng <muchunpeng@sensetime.com>
 */

#pragma once

#include <memory>
#include <string>

#include "ad_rscl/comm/cuda/allocator.h"
#include "ad_rscl/comm/cuda/base.h"
#include "ad_rscl/comm/cuda/segment_creator.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <class ElemT>
class Memory {
    static_assert(std::is_base_of<Element, ElemT>::value,
                  "ElemT must be a subclass of Element");

 public:
    explicit Memory(const MemorySharedConfig_t &shared_config,
                    const MemoryPrivateConfig_t &private_config);

    void init_shared_queue();
    size_t queue_size() const;

    uint8_t *create_memory_segment(const std::string &name,
                                   size_t size,
                                   bool *new_segment,
                                   size_t alignment);

    uint8_t *create_cuda_memory_segment(uint8_t *shm_addr,
                                        size_t size,
                                        const bool &new_segment,
                                        size_t alignment);

    bool check_shm_segment_exist(std::string name);
    bool get_shm_status();
    const MemorySharedConfig_t &get_memory_config() const;

    bool unlink(std::string name, void *addr, size_t len);
    ~Memory();

    std::string name_;
    PIDSet *pid_set_;
    Allocator *allocator_;
    SharedQueue<ElemT> *shared_queue_;

    MemorySharedConfig_t shared_config_;
    MemoryPrivateConfig_t private_config_;

    sharedMemory_t shm_;
    const size_t alignment_size_ = 32;

    std::unique_ptr<SegmentCreator> segment_creator_;
    std::shared_ptr<uint8_t> cuda_cleanup_holder_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
