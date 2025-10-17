/*
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Mu Chunpeng <muchunpeng@sensetime.com>
 */

#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <atomic>
#include <cerrno>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#include "ad_serde/ad_serde.h"

#include "ad_rscl/comm/cuda/allocator.h"
#include "ad_rscl/idl/cuda_message_conf.pb.h"
#include "ad_rscl/idl/cuda_message.capnp.h"
#include "concurrency/lockless_set.h"
#include "concurrency/robust_lock.h"
#include "concurrency/scope.h"

namespace senseAD {
namespace rscl {
namespace comm {
using senseAD::ad_shm::concurrent::LocklessSet;
using senseAD::ad_shm::concurrent::RobustLock;
using senseAD::rscl::idl::CudaPublisherConf;

constexpr size_t kGap = 1024;  // 1kb safety gap

struct sharedMemory_t {
    uint8_t *base_addr;
    uint8_t *data_addr;
    size_t header_size;
    size_t buffer_size;
    int fd;
};

struct MemorySharedConfig_t {
    MemorySharedConfig_t() {}
    MemorySharedConfig_t(const std::string &n, const CudaPublisherConf &q)
        : buffer_size(q.buffer_size()),
          queue_size(q.depth()),
          cuda_device_id(q.device_id()) {
        std::snprintf(name, sizeof(name), "%s", n.c_str());
    }

    explicit MemorySharedConfig_t(const std::string &n)
        : buffer_size((1U << 24)), queue_size(1), cuda_device_id(0) {
        std::snprintf(name, sizeof(name), "%s", n.c_str());
    }

    char name[128];
    size_t buffer_size;
    size_t queue_size;

    int cuda_device_id;
};

struct MemoryPrivateConfig_t {
    bool read_only = false;
    bool ipc_handle_enable = true;
};

class PIDSet {
 public:
    bool any_alive();
    bool insert(uint32_t pid);

    void lock();
    void unlock();

 private:
    LocklessSet<32> pid_set;
    RobustLock lck;
};

struct Element {
    size_t size;
    Allocator::handle address_handle;
    bool empty;
    uint64_t seq;
    bool owned_by_user;

    Element()
        : size(0),
          address_handle(0),
          empty(true),
          seq(0),
          owned_by_user(false) {}
};

struct LockableElement : public Element {
    using LockT = senseAD::ad_shm::concurrent::PthreadReadWriteLock;
    senseAD::ad_shm::concurrent::PthreadReadWriteLock mutex;

    LockableElement() : Element(), mutex() {}

    LockableElement(const LockableElement &topic_elem) : Element(topic_elem) {
        mutex = topic_elem.mutex;
    }
};

struct Memblock {
    void *ptr;  // pointer for user read/write
    size_t size;
    bool free;
    bool preallocated;
    void *preallocated_addr;  // pointer for management
    std::shared_ptr<uint8_t> sub_counter_holder_;

    Memblock() : ptr(nullptr), size(0), free(true),
        preallocated(false), sub_counter_holder_(nullptr) {}
    Memblock(void *ptr, size_t size): ptr(ptr), size(size), free(true),
        preallocated(false), sub_counter_holder_(nullptr) {}
    ~Memblock();
    void no_delete() { free = false; }

    bool is_empty() const { return ptr == nullptr && size == 0; }
};

template <class ElemT>
class SharedQueue {
 public:
    std::atomic<uint32_t> counter;
    std::atomic<uint64_t> buffer_size;
    std::atomic<uint32_t> queue_size;
    std::array<ElemT, kMaxQueueSize> elements;

    MemorySharedConfig_t config;
};
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD

template <>
struct SendMsg<senseAD::rscl::comm::Memblock> {
 public:
    SendMsg() {}

    senseAD::rscl::comm::Memblock &MutData() { return data; }
    const senseAD::rscl::comm::Memblock &Data() const { return data; }

 private:
    senseAD::rscl::comm::Memblock data;
};
