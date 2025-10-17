/*
 * Copyright (C) 2020-2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <type_traits>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "ad_buf/ad_buf.h"
#include "ad_buf/impl_factory.h"
#include "ad_buf/strategy/fixed_size_arena.h"

namespace senseAD {
namespace ad_buf {

// Used for memory allocation
typedef FixedSizeArenaBlock ShmBlock;

struct ShmAdBufManagerMmapInfo {
    // From user or auto
    key_t shm_key = 0;
    size_t shm_size = 0;

    // Run time info
    uint32_t allocated_block_count = 0;
    uint32_t allocated_size = 0;
    AdBuf::TokenT next_uusable_token = 1;

    uint32_t block_count = 1;

    friend std::ostream& operator<<(std::ostream& os,
                                    ShmAdBufManagerMmapInfo const& m) {
        return os << "(shm_key: " << m.shm_key << ", shm_size: " << m.shm_size
                  << ", allocated_block_count: " << m.allocated_block_count
                  << ", allocated_size: " << m.allocated_size
                  << ", next_uusable_token: " << m.next_uusable_token
                  << ", block_count: " << m.block_count << ")";
    }
};

static_assert(std::is_trivially_copyable<ShmAdBufManagerMmapInfo>::value,
              "ShmAdBufManagerMmapInfo must be a POD type.");

class ShmBufCfg : public BufCfg {
 public:
    constexpr static char const* kTypeStr = "shm";
    ShmBufCfg() {
        buf_type(kTypeStr);
        specific_info_size(sizeof(ShmAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(ShmBlock));
    }
    explicit ShmBufCfg(const BufCfg& base_cfg) : BufCfg(base_cfg) {
        buf_type(kTypeStr);
        specific_info_size(sizeof(ShmAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(ShmBlock));
    }

    inline key_t shmkey() const { return shm_key_; }
    inline ShmBufCfg& shmkey(key_t shm_key) {
        shm_key_ = shm_key;
        return *this;
    }

    inline uint32_t total_buf_size() const { return total_buf_size_; }
    inline ShmBufCfg& total_buf_size(const uint32_t total_buf_size) {
        total_buf_size_ = total_buf_size;
        return *this;
    }

    inline void FillFromInfo(const ShmAdBufManagerMmapInfo& info) {
        shm_key_ = info.shm_key;
        total_buf_size_ = info.shm_size;
    }

    friend std::ostream& operator<<(std::ostream& os, ShmBufCfg const& m) {
        return os << *dynamic_cast<const BufCfg*>(&m)
                  << "(shm_key: " << m.shmkey()
                  << " , total_buf_size: " << m.total_buf_size() << ")";
    }

 private:
    key_t shm_key_ = 0;
    uint32_t total_buf_size_ = 4096;
};

class ShmAdBufManager;

/**
 * @brief Wrapper for a shm buffer section
 * It will not allocate memory, it take a given shm memory
 *
 */
class ShmAdBuf : public AdBuf {
 public:
    using Ptr = std::unique_ptr<ShmAdBuf>;
    using ConstPtr = std::unique_ptr<const ShmAdBuf>;

    ~ShmAdBuf() override = default;
    ShmAdBuf(const ShmAdBuf&) = delete;
    ShmAdBuf& operator=(const ShmAdBuf&) = delete;

    base::optional<size_t> GetDataSize() const override;
    char* GetDeviceDataPtr() override;
    const char* GetDeviceDataPtr() const override;
    char* GetHostDataPtr() override;
    const char* GetHostDataPtr() const override;

    uint32_t LockIndex() const override;
    std::ostream& DebugOstream(std::ostream& os) const override;

 private:
    ShmAdBuf(const TokenT token,
             char* shm_base_ptr,
             const uint32_t start_offset,
             const size_t size,
             const uint32_t lock_index);
    void* GetRawPtrImpl() const override;

    char* shm_base_ptr_ = nullptr;
    char* shm_data_ptr_ = nullptr;

    uint32_t start_offset_ = 0;
    size_t size_ = 0;
    uint32_t lock_index_ = 0;

    friend class ShmAdBufManager;
};

class ShmAdBufManager : public AdBufManager {
 public:
    ~ShmAdBufManager() override;

    static std::shared_ptr<ShmAdBufManager> ConstructFn(
        const std::string& mmaped_file, const BufCfg* base_cfg);
    static std::shared_ptr<ShmAdBufManager> ReConstructFn(
        const std::string& mmaped_file);

    size_t WasExistingBufCount() const override;
    bool WasAbleToNewBuf() const override;
    std::vector<AdBuf::ConstPtr> WasAllBufs() override;

    inline const ShmAdBufManagerMmapInfo& mmaped_specific_info() const {
        return *mmaped_specific_info_;
    }

 private:
    ShmAdBufManager() = default;
    bool InitFromConstruct(const std::string& mmaped_file,
                           const ShmBufCfg& cfg);
    bool InitFromReConstruct(const std::string& mmaped_file);
    void Defragment();

    char* GetShmBasePtr();
    int32_t GetShmBlockIndex(AdBuf::TokenT token) const;

    AdBuf::Ptr OwnBuf() override;
    AdBuf::Ptr OwnBufWithSize(const size_t size) override;

    AdBuf::Ptr OpenBuf(AdBuf::TokenT token) override;
    bool ReleaseBufImpl(AdBuf::ConstPtr buf) override;

 private:
    bool RemoveShmArena();
    bool CreateShmArena();
    bool OpenShmArena();
    void* MapOpenedShmArenaToMemory();

    int shmid_ = -1;
    char* shm_memory_ = nullptr;

    ShmBufCfg cfg_;
    ShmAdBufManagerMmapInfo* mmaped_specific_info_ = nullptr;
    ShmBlock* handles_ = nullptr;

    friend class ShmAdBuf;
};

void RegisterShmAdBuf();

}  // namespace ad_buf
}  // namespace senseAD

REG_ADBUF_IMPL(senseAD::ad_buf::RegisterShmAdBuf)
