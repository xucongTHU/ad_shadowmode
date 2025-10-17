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

#include "pmem_client/pmem.h"
#include "pmem_client/pmem_id.h"

#include "ad_buf/ad_buf.h"
#include "ad_buf/impl_factory.h"

namespace senseAD {
namespace ad_buf {

struct QcommPmemBlock {
    static constexpr uint8_t kStateEmpty = 0;
    static constexpr uint8_t kStateAllocated = 1;

    pmem_handle_t buf_handle;
    uint32_t size = 0;

    uint8_t state = kStateEmpty;

    AdBuf::TokenT token;

    // Clear all status and free the buffer
    void Clear();
    // Clear all status but not free the buffer
    void Release();

    friend std::ostream& operator<<(std::ostream& os, QcommPmemBlock const& m) {
        return os << "(buf_handle: " << static_cast<void*>(m.buf_handle)
                  << ", size: " << m.size
                  << ", state: " << static_cast<int>(m.state)
                  << ", token: " << m.token << ")";
    }
};

struct QcommPmemAdBufManagerMmapInfo {
    // Run time info
    uint32_t allocated_block_count = 0;
    uint32_t allocated_size = 0;
    AdBuf::TokenT next_uusable_token = 1;

    friend std::ostream& operator<<(std::ostream& os,
                                    QcommPmemAdBufManagerMmapInfo const& m) {
        return os << "(allocated_block_count: " << m.allocated_block_count
                  << ", allocated_size: " << m.allocated_size
                  << ", next_uusable_token: " << m.next_uusable_token << ")";
    }
};

static_assert(std::is_trivially_copyable<QcommPmemAdBufManagerMmapInfo>::value,
              "QcommPmemAdBufManagerMmapInfo must be a POD type.");

class QcommPmemBufCfg : public BufCfg {
 public:
    constexpr static char const* kTypeStr = "qcomm_pmem";
    QcommPmemBufCfg() {
        buf_type(kTypeStr);
        specific_info_size(sizeof(QcommPmemAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(QcommPmemBlock));
    }
    explicit QcommPmemBufCfg(const BufCfg& base_cfg) : BufCfg(base_cfg) {
        buf_type(kTypeStr);
        specific_info_size(sizeof(QcommPmemAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(QcommPmemBlock));
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    QcommPmemBufCfg const& m) {
        return os << *dynamic_cast<const BufCfg*>(&m);
    }

 private:
};

class QcommPmemAdBufManager;

/**
 * @brief Wrapper for a shm buffer section
 * It will not allocate memory, it take a given shm memory
 *
 */
class QcommPmemAdBuf : public AdBuf {
 public:
    using Ptr = std::unique_ptr<QcommPmemAdBuf>;
    using ConstPtr = std::unique_ptr<const QcommPmemAdBuf>;

    ~QcommPmemAdBuf() override;
    QcommPmemAdBuf(const QcommPmemAdBuf&) = delete;
    QcommPmemAdBuf& operator=(const QcommPmemAdBuf&) = delete;

    base::optional<size_t> GetDataSize() const override;
    char* GetDeviceDataPtr() override;
    const char* GetDeviceDataPtr() const override;
    char* GetHostDataPtr() override;
    const char* GetHostDataPtr() const override;

    uint32_t LockIndex() const override;
    std::ostream& DebugOstream(std::ostream& os) const override;

 private:
    QcommPmemAdBuf(const TokenT token,
                   pmem_handle_t buf_handle,
                   const size_t size,
                   const uint32_t lock_index);

    QcommPmemAdBuf(const TokenT token,
                   pmem_handle_t buf_handle,
                   char* buf_addr,
                   const size_t size,
                   const uint32_t lock_index);

    void* GetRawPtrImpl() const override;

    pmem_handle_t buf_handle_;
    size_t size_ = 0;
    uint32_t lock_index_ = 0;

    char* buf_addr_ = nullptr;
    bool need_unmap_ = false;

    friend class QcommPmemAdBufManager;
};

class QcommPmemAdBufManager : public AdBufManager {
 public:
    ~QcommPmemAdBufManager() override;

    static std::shared_ptr<QcommPmemAdBufManager> ConstructFn(
        const std::string& mmaped_file, const BufCfg* base_cfg);
    static std::shared_ptr<QcommPmemAdBufManager> ReConstructFn(
        const std::string& mmaped_file);

    size_t WasExistingBufCount() const override;
    bool WasAbleToNewBuf() const override;
    std::vector<AdBuf::ConstPtr> WasAllBufs() override;

    inline const QcommPmemAdBufManagerMmapInfo& mmaped_specific_info() const {
        return *mmaped_specific_info_;
    }

 private:
    QcommPmemAdBufManager() = default;
    bool InitFromConstruct(const std::string& mmaped_file,
                           const QcommPmemBufCfg& cfg);
    bool InitFromReConstruct(const std::string& mmaped_file);

    int32_t GetQcommPmemBlockIndex(AdBuf::TokenT token) const;

    AdBuf::Ptr OwnBuf() override;
    AdBuf::Ptr OwnBufWithSize(const size_t size) override;

    AdBuf::Ptr OpenBuf(AdBuf::TokenT token) override;
    bool ReleaseBufImpl(AdBuf::ConstPtr buf) override;

 private:
    QcommPmemBufCfg cfg_;
    QcommPmemAdBufManagerMmapInfo* mmaped_specific_info_ = nullptr;
    QcommPmemBlock* handles_ = nullptr;

    friend class QcommPmemAdBuf;
};

void RegisterQcommPmemAdBuf();

}  // namespace ad_buf
}  // namespace senseAD

REG_ADBUF_IMPL(senseAD::ad_buf::RegisterQcommPmemAdBuf)
