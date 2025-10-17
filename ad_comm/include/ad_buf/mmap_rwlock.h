/*
 * Copyright (C) 2020-2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <pthread.h>

#include <memory>

namespace senseAD {
namespace ad_buf {

class MmapRwlocks;

struct MmapRwlock {
 public:
    // TODO(kit) design whether need to use mutex to impl rwlock?
    pthread_rwlockattr_t rwlock_attr_;
    pthread_rwlock_t rwlock_;
    bool alive_ = false;

    void InitOnceCreated();
    void UnInitOnce();

    inline bool try_r_lock() {
        return alive_ && pthread_rwlock_tryrdlock(&rwlock_) == 0;
    }
    inline bool try_w_lock() {
        return alive_ && pthread_rwlock_trywrlock(&rwlock_) == 0;
    }
    inline bool w_lock() {
        return alive_ && pthread_rwlock_wrlock(&rwlock_) == 0;
    }
    inline bool unlock() { return pthread_rwlock_unlock(&rwlock_) == 0; }
    inline bool alive() { return alive_; }

 private:
    MmapRwlock() = default;
    friend class MmapRwlocks;
};

class MmapRwlocks {
 public:
    static std::shared_ptr<MmapRwlocks> Create(char *mmaped_ptr,
                                               const uint32_t lock_count);
    static std::shared_ptr<MmapRwlocks> Open(char *mmaped_ptr,
                                             const uint32_t lock_count);

    ~MmapRwlocks();
    MmapRwlock *operator[](size_t index) const;
    MmapRwlock *at(size_t index) const;
    uint32_t lock_count() const;

    static size_t ExpectedMmapByteSize(const uint32_t lock_count);

 private:
    MmapRwlocks(char *mmaped_ptr, const uint32_t lock_count);

    uint32_t *GetLockCountSectionFromMmap(void *mmap_ptr) const;
    MmapRwlock *GetLockArrSectionFromMmap(void *mmap_ptr) const;

    void InitFormExistingMmap();
    void InitFormNewMmap();

    bool is_creator_ = false;
    MmapRwlock *lock_arr_;
    char *mmaped_ptr_ = nullptr;
    uint32_t lock_count_ = 0;
};

}  // namespace ad_buf
}  // namespace senseAD
