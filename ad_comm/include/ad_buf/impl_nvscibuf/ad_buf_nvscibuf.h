/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <type_traits>
#include <array>
#include <memory>
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#include "nvscibuf.h"
#include "nvsciipc.h"

#ifdef __cplusplus
}
#endif

#include "ad_buf/ad_buf.h"
#include "ad_buf/impl_factory.h"
#include "ad_buf/strategy/fixed_size_arena.h"

namespace senseAD {
namespace ad_buf {

// Used for memory allocation
typedef FixedSizeArenaBlock NvscibufBlock;

static constexpr uint32_t kBufAttrDescSize = 2048;
static constexpr uint8_t kMaxEndpoints = 8;
static constexpr uint32_t kSingleExportDescSize =
    sizeof(NvSciBufObjIpcExportDescriptor);
static constexpr uint32_t kBufExportDescSize =
    kSingleExportDescSize * kMaxEndpoints;

struct NvscibufAdBufManagerMmapInfo {
    static constexpr uint8_t kStateEmpty = 0;
    static constexpr uint8_t kStateReadAttrReconciled = 1;
    static constexpr uint8_t kStateCombineAttrReconciled = 2;

    key_t shm_key = 0;
    size_t shm_size = 0;

    // Per-endpoint states - completely replace global state
    uint8_t endpoint_states[kMaxEndpoints] = {kStateEmpty};

    // Per-endpoint consumer read attributes
    size_t consumer_read_attrs_size[kMaxEndpoints] = {0};
    char consumer_read_attrs_desc[kMaxEndpoints][kBufAttrDescSize] = {{0}};

    // Per-endpoint combined attributes
    size_t combined_attrs_size[kMaxEndpoints] = {0};
    char combined_attrs_desc[kMaxEndpoints][kBufAttrDescSize] = {{0}};

    // For descriptor sharing
    uint32_t allocated_block_count = 0;
    uint32_t allocated_size = 0;
    AdBuf::TokenT next_uusable_token = 1;

    uint32_t block_count = 3;

    friend std::ostream& operator<<(std::ostream& os,
                                    NvscibufAdBufManagerMmapInfo const& m) {
        return os << "( shm_key: " << m.shm_key << ", endpoint_states[0]: "
                  << static_cast<int>(m.endpoint_states[0])
                  << ", consumer_read_attrs_size[0]: "
                  << m.consumer_read_attrs_size[0]
                  << ", combined_attrs_size[0]: " << m.combined_attrs_size[0]
                  << ", allocated_size: " << m.allocated_size
                  << ", allocated_block_count: " << m.allocated_block_count
                  << ", next_uusable_token: " << m.next_uusable_token
                  << ", block_count: " << m.block_count << ")";
    }
};

static_assert(std::is_trivially_copyable<NvscibufAdBufManagerMmapInfo>::value,
              "ShmAdBufManagerMmapInfo must be a POD type.");

namespace {
template <typename T>
void ValidatePtr(T* input_ptr, const char* msg) {
    if (input_ptr == NULL || input_ptr == nullptr) {
        throw std::runtime_error(msg);
    }
}
}  // namespace

class NvsciBufCfg : public BufCfg {
 public:
    constexpr static char const* kTypeStr = "nvscibuf";
    NvsciBufCfg() {
        buf_type(kTypeStr);
        specific_info_size(sizeof(NvscibufAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(NvscibufBlock));
    }
    explicit NvsciBufCfg(const BufCfg& base_cfg) : BufCfg(base_cfg) {
        buf_type(kTypeStr);
        specific_info_size(sizeof(NvscibufAdBufManagerMmapInfo));
        max_bufhandle_size(sizeof(NvscibufBlock));
    }

    inline key_t GetShmkey() const { return shm_key_; }
    inline NvsciBufCfg& SetShmkey(key_t shm_key) {
        shm_key_ = shm_key;
        return *this;
    }

    enum class BufEndpointType { PRODUCER, CONSUMER };
    inline BufEndpointType GetType() const { return type_; }
    inline NvsciBufCfg& SetType(BufEndpointType type) {
        type_ = type;
        return *this;
    }

    inline uint32_t GetChannelCount() const { return channel_count_; }
    inline NvsciBufCfg& SetChannelCount(uint32_t count) {
        channel_count_ = count;
        return *this;
    }

    inline NvSciBufModule GetBufModule() const { return buf_module_; }
    inline NvsciBufCfg& SetBufModule(NvSciBufModule module_ptr) {
        ValidatePtr(module_ptr, "module_ptr is null!");
        buf_module_ = module_ptr;
        return *this;
    }

    // deprecated
    // inline NvSciBufAttrList GetBufAttrList() const { return attr_; }
    inline NvsciBufCfg& SetBufAttrList(NvSciBufAttrList attr_ptr) {
        ValidatePtr(attr_ptr, "attr_ptr is null!");
        attrs_.push_back(attr_ptr);
        return *this;
    }

    inline std::vector<NvSciBufAttrList> GetBufAttrLists() const {
        return attrs_;
    }
    inline NvsciBufCfg& SetBufAttrLists(const NvSciBufAttrList attrs_ptr[],
                                        int attrs_num) {
        for (int i = 0; i < attrs_num; i++) {
            ValidatePtr(attrs_ptr[i], "attr_ptr is null!");
            attrs_.push_back(attrs_ptr[i]);
        }
        return *this;
    }

    inline void FillFromInfo(const NvscibufAdBufManagerMmapInfo& info) {
        shm_key_ = info.shm_key;
    }

    friend std::ostream& operator<<(std::ostream& os, NvsciBufCfg const& m) {
        return os << *dynamic_cast<const BufCfg*>(&m)
                  << "( type: " << static_cast<int>(m.type_)
                  << " channel_count: " << m.channel_count_
                  << " buf_module status: " << (m.buf_module_ != nullptr)
                  << " , attrs_ size: " << m.attrs_.size() << ")";
    }

 private:
    BufEndpointType type_ = BufEndpointType::PRODUCER;
    uint32_t channel_count_ = 4;
    NvSciBufModule buf_module_ = nullptr;
    std::vector<NvSciBufAttrList> attrs_;

    key_t shm_key_ = 0;
};

class NvscibufAdBufManager;

/**
 * @brief Wrapper for a Nvscibuf section
 * It will not allocate memory, it take a given shm memory
 *
 */
class NvscibufAdBuf : public AdBuf {
 public:
    using Ptr = std::unique_ptr<NvscibufAdBuf>;
    using ConstPtr = std::unique_ptr<const NvscibufAdBuf>;

    ~NvscibufAdBuf() override = default;
    NvscibufAdBuf(const NvscibufAdBuf&) = delete;
    NvscibufAdBuf& operator=(const NvscibufAdBuf&) = delete;

    base::optional<size_t> GetDataSize() const override;
    char* GetDeviceDataPtr() override;
    const char* GetDeviceDataPtr() const override;
    char* GetHostDataPtr() override;
    const char* GetHostDataPtr() const override;

    void** GetRawPtrImplRef() override {
        return reinterpret_cast<void**>(&sci_buf_obj_);
    }

    std::vector<char> SerializeHandle() const;
    bool InitFromSerializedHandle(void* mgr_ptr,
                                  const char* serialized,
                                  const size_t size);

    uint32_t LockIndex() const override;
    std::ostream& DebugOstream(std::ostream& os) const override;

    void SetAllEndpoints(const std::vector<NvSciIpcEndpoint>& endpoints);

 private:
    NvscibufAdBuf(const TokenT token,
                  char* shm_base_ptr,
                  const uint32_t start_offset,
                  const uint32_t max_lock,
                  NvSciIpcEndpoint ipc_end);
    void* GetRawPtrImpl() const override;

    bool ImportObjFromDesc() const;

    char* shm_base_ptr_ = nullptr;
    char* shm_data_ptr_ = nullptr;

    uint32_t start_offset_ = 0;
    size_t size_ = 0;
    uint32_t max_lock_ = 0;

    mutable NvSciBufObj sci_buf_obj_ = nullptr;
    NvSciIpcEndpoint ipc_endpoint_;
    std::vector<NvSciIpcEndpoint> all_endpoints_;

    friend class NvscibufAdBufManager;
};

class NvscibufAdBufManager : public AdBufManager {
 public:
    ~NvscibufAdBufManager() override;

    static std::shared_ptr<NvscibufAdBufManager> ConstructFn(
        const std::string& mmaped_file, const BufCfg* base_cfg);
    static std::shared_ptr<NvscibufAdBufManager> ReConstructFn(
        const std::string& mmaped_file);

    size_t WasExistingBufCount() const override;
    bool WasAbleToNewBuf() const override;
    std::vector<AdBuf::ConstPtr> WasAllBufs() override;
    void SyncBufMeta(const std::shared_ptr<BufCfg>& cfg) override;

    inline const NvscibufAdBufManagerMmapInfo& mmaped_specific_info() const {
        return *mmaped_specific_info_;
    }

    void AttachPeerBeforeExportObj(NvSciBufObj obj);

    NvSciBufAttrList GetImportAttrForReader() const { return import_attr_; }

 private:
    NvscibufAdBufManager() = default;
    bool InitFromConstruct(const std::string& mmaped_file,
                           const NvsciBufCfg& cfg);
    bool InitFromReConstruct(const std::string& mmaped_file);
    void Defragment();

    char* GetShmBasePtr();
    int32_t GetShmBlockIndex(AdBuf::TokenT token) const;

    AdBuf::Ptr OwnBuf() override;
    AdBuf::Ptr OwnBufWithSize(const size_t size) override;

    AdBuf::Ptr OpenBuf(AdBuf::TokenT token) override;
    bool ReleaseBufImpl(AdBuf::ConstPtr buf) override;

    void AttrListReconcile();

    int shmid_ = 0;
    char* shm_memory_ = nullptr;

    NvsciBufCfg cfg_;
    NvscibufAdBufManagerMmapInfo* mmaped_specific_info_ = nullptr;
    NvscibufBlock* handles_ = nullptr;

    std::vector<NvSciIpcEndpoint> ipc_endpoints_;
    uint32_t active_endpoint_index_ = 0;
    // for producer side
    std::vector<NvSciBufAttrList> consumer_attrs_;
    NvSciBufAttrList consumer_attr_ = NULL;
    // for consumer side
    NvSciBufAttrList import_attr_ = NULL;

    friend class NvscibufAdBuf;
};

void RegisterNvscibufAdBuf();

}  // namespace ad_buf
}  // namespace senseAD

REG_ADBUF_IMPL(senseAD::ad_buf::RegisterNvscibufAdBuf)
