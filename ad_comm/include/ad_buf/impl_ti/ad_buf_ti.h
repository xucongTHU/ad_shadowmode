/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * zhangyi <zhangyi15@senseauto.com>
 */

#pragma once

#include <type_traits>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "ad_buf/ad_buf.h"
#include "ad_buf/impl_factory.h"
#include "ad_buf/impl_ti/ti_common.h"

namespace senseAD {
namespace ad_buf {

struct TiAdBufSlaveInfo {
    int fd = 0;
    void* buffer_handle = nullptr;
    void* phyaddr = nullptr;
    TiAdBufSlaveInfo() {}
    TiAdBufSlaveInfo(int _fd, void* _buffer_handle, void* _phyaddr)
        : fd(_fd), buffer_handle(_buffer_handle), phyaddr(_phyaddr) {}
};

// First layer key is the translated fd for master process's pid, second layer
// key is the translated fd for the buf_handle
using TiAdBufSlaveInfoMap =
    std::unordered_map<int, std::unordered_map<int, TiAdBufSlaveInfo>>;

struct TiBlock {
    static constexpr uint8_t kStateEmpty = 0;
    static constexpr uint8_t kStateAllocated = 1;

    pid_t master_pid = 0;
    uint64_t master_fd = 0;
    void* master_buf_handle = nullptr;
    void* phyaddr = nullptr;
    vx_enum mem_heap_region = TIVX_MEM_EXTERNAL;

    vx_uint32 size = 0;
    uint8_t state = kStateEmpty;
    AdBuf::TokenT token;

    // Clear all status and free the buffer
    void Clear();
    // Clear all status but not free the buffer
    void Release();

    friend std::ostream& operator<<(std::ostream& os, TiBlock const& m) {
        return os << "( master_pid: " << m.master_pid
                  << ", master_fd: " << m.master_fd
                  << ", master_buf_handle: " << m.master_buf_handle
                  << ", phyaddr: " << m.phyaddr << ", mem_heap_region: "
                  << static_cast<int>(m.mem_heap_region) << ", size: " << m.size
                  << ", state: " << m.state << ", token: " << m.token << ")";
    }
};

struct TiAdBufManagerMmapInfo {
    // Run time info
    uint32_t allocated_block_count = 0;
    AdBuf::TokenT next_uusable_token = 1;

    friend std::ostream& operator<<(std::ostream& os,
                                    TiAdBufManagerMmapInfo const& m) {
        return os << "(allocated_block_count: " << m.allocated_block_count
                  << ", next_uusable_token: " << m.next_uusable_token << ")";
    }
};

static_assert(std::is_trivially_copyable<TiAdBufManagerMmapInfo>::value,
              "TiAdBufManagerMmapInfo must be a POD type.");

class TiBufCfg : public BufCfg {
 public:
    constexpr static char const* kTypeStr = "ti";
    TiBufCfg() {
        buf_type(kTypeStr);
        specific_info_size(sizeof(TiAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(TiBlock));
    }
    explicit TiBufCfg(const BufCfg& base_cfg) : BufCfg(base_cfg) {
        buf_type(kTypeStr);
        specific_info_size(sizeof(TiAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(TiBlock));
    }

    friend std::ostream& operator<<(std::ostream& os, TiBufCfg const& m) {
        return os << *dynamic_cast<const BufCfg*>(&m);
    }

 private:
};

struct TiStruct {
    uint64_t fd_ = 0;
    explicit TiStruct(uint64_t fd) : fd_(fd) {}
};

class TiAdBuf : public AdBuf {
 public:
    using Ptr = std::unique_ptr<TiAdBuf>;
    using ConstPtr = std::unique_ptr<const TiAdBuf>;

    ~TiAdBuf() override;
    TiAdBuf(const TiAdBuf&) = delete;
    TiAdBuf& operator=(const TiAdBuf&) = delete;

    base::optional<size_t> GetDataSize() const override;
    char* GetDeviceDataPtr() override;
    const char* GetDeviceDataPtr() const override;
    char* GetHostDataPtr() override;
    const char* GetHostDataPtr() const override;

    uint32_t LockIndex() const override;
    std::ostream& DebugOstream(std::ostream& os) const override;

 private:
    TiAdBuf(const TokenT token,
            vx_uint32 size,
            const uint32_t lock_index,
            uint64_t fd,
            void* buf_handle,
            void* phyaddr,
            vx_enum mem_heap_region,
            bool need_free = false,
            int master_pidfd = 0,
            int master_fd = 0,
            std::shared_ptr<TiAdBufSlaveInfoMap> slave_info_map = nullptr);

    void* GetRawPtrImpl() const override;

    vx_uint32 size_;
    uint32_t lock_index_;

    uint64_t fd_;
    void* buf_handle_;
    void* phyaddr_;
    vx_enum mem_heap_region_;

    bool need_free_;
    int master_pidfd_;
    int master_fd_;
    std::shared_ptr<TiAdBufSlaveInfoMap>
        slave_info_map_;  // the map stored in buf manager

    friend class TiAdBufManager;
};

class TiAdBufManager : public AdBufManager {
 public:
    ~TiAdBufManager() override;

    static std::shared_ptr<TiAdBufManager> ConstructFn(
        const std::string& mmaped_file, const BufCfg* base_cfg);
    static std::shared_ptr<TiAdBufManager> ReConstructFn(
        const std::string& mmaped_file);

    size_t WasExistingBufCount() const override;
    bool WasAbleToNewBuf() const override;
    std::vector<AdBuf::ConstPtr> WasAllBufs() override;

    inline const TiAdBufManagerMmapInfo& mmaped_specific_info() const {
        return *mmaped_specific_info_;
    }

 private:
    TiAdBufManager() = default;
    bool InitFromConstruct(const std::string& mmaped_file, const TiBufCfg& cfg);
    bool InitFromReConstruct(const std::string& mmaped_file);

    int32_t GetTiBlockIndex(AdBuf::TokenT token) const;

    AdBuf::Ptr OwnBuf() override;
    AdBuf::Ptr OwnBufWithSize(const size_t size) override;

    AdBuf::Ptr OpenBuf(AdBuf::TokenT token) override;
    bool ReleaseBufImpl(AdBuf::ConstPtr buf) override;

    AdBuf::Ptr CreateAdBufFromBlock(const TiBlock& block, int block_index);
    AdBuf::Ptr CreateMasterAdBufFromBlock(const TiBlock& block,
                                          int block_index);
    AdBuf::Ptr CreateSlaveAdBufFromBlock(const TiBlock& block, int block_index);

 private:
    // multiple managers share 1 [master pid]->[master pid's fd] map(e.g. all
    // cameras in the same slave process use 1 fd for the same master process)
    static std::mutex pidfd_map_mtx_;
    static std::unordered_map<pid_t, int> pidfd_map_;
    // each manager maintain it's own [master pid's fd]->[translated
    // fd]->[slave info map]
    std::shared_ptr<TiAdBufSlaveInfoMap> slave_info_map_;
    TiInitilizer initializer_;

    TiBufCfg cfg_;
    TiAdBufManagerMmapInfo* mmaped_specific_info_ = nullptr;
    TiBlock* handles_ = nullptr;

    friend class TiAdBuf;
};

void RegisterTiAdBuf();

}  // namespace ad_buf
}  // namespace senseAD

REG_ADBUF_IMPL(senseAD::ad_buf::RegisterTiAdBuf)
