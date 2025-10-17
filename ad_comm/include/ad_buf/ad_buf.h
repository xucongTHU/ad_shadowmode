/*
 * Copyright (C) 2020-2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <system_error>

#include "ad_base/std/optional.h"
#include "ad_buf/buf_meta.h"
#include "ad_buf/mmap_rwlock.h"

namespace senseAD {
namespace ad_buf {

class AdBufManager;
class AdBuf {
 public:
    using Ptr = std::unique_ptr<AdBuf>;
    using ConstPtr = std::unique_ptr<const AdBuf>;
    using TokenT = uint32_t;

    //***************************************
    //*             Destructor
    //***************************************
    virtual ~AdBuf() = default;
    AdBuf(const AdBuf&) = delete;
    AdBuf& operator=(const AdBuf&) = delete;

    //***************************************
    //*     Unsafe RawLevel Operation
    //***************************************
    template <typename T>
    T* GetRawPtr() {
        return static_cast<T*>(GetRawPtrImpl());
    }
    template <typename T>
    const T* GetRawPtr() const {
        return static_cast<T*>(GetRawPtrImpl());
    }
    template <typename T>
    void SetRawPtr(T* external_buf) {
        auto ptr_ref = GetRawPtrImplRef();
        if (ptr_ref) {
            *ptr_ref = external_buf;
            if (extbuf_import_notify_) {
                extbuf_import_notify_();
            }
        }
    }

    virtual base::optional<size_t> GetDataSize() const = 0;
    virtual char* GetDeviceDataPtr() = 0;
    virtual const char* GetDeviceDataPtr() const = 0;
    virtual char* GetHostDataPtr() = 0;
    virtual const char* GetHostDataPtr() const = 0;

    //***************************************
    //*      Cross Porcess Operation
    //***************************************
    inline TokenT Token() const { return token_; }
    inline std::string MgrKey() const { return mgr_key_; }
    inline std::weak_ptr<AdBufManager> GetMgr() const { return mgr_; }
    virtual uint32_t LockIndex() const = 0;

    //***************************************
    //*      Meta info
    //***************************************
    /**
     * @brief Count the edit version of this adbuf
     * If this buf is edited, this value will auto increment by 1
     *
     * @return uint64_t
     */
    inline uint64_t revision() const { return revision_; }
    inline ImgMeta img_meta() const { return img_meta_; }

    //***************************************
    //*      Debug Print
    //***************************************
    virtual std::ostream& DebugOstream(std::ostream& os) const = 0;
    inline friend std::ostream& operator<<(std::ostream& os, AdBuf const& m) {
        return m.DebugOstream(os);
    }

 protected:
    explicit AdBuf(TokenT token) : token_(token) {}
    void SetMgrKey(const std::string& mgr_key) { mgr_key_ = mgr_key; }
    void SetMgr(const std::shared_ptr<AdBufManager>& mgr) { mgr_ = mgr; }
    virtual void* GetRawPtrImpl() const = 0;
    virtual void** GetRawPtrImplRef() { return nullptr; }

    TokenT token_ = 0;
    std::string mgr_key_;
    std::weak_ptr<AdBufManager> mgr_;
    std::function<void()> extbuf_import_notify_;

 private:
    inline void SetupMeta(const BufMeta& meta) {
        revision_ = meta.revision;
        img_meta_ = meta.img_meta;
    }

    uint64_t revision_ = 0;
    ImgMeta img_meta_;

    friend class AdBufManager;
};

bool Unlock(AdBuf::Ptr buf);
bool Unlock(AdBuf::ConstPtr buf);
bool Release(AdBuf::Ptr buf);
bool Release(AdBuf::ConstPtr buf);
bool UnlockAndRelease(AdBuf::Ptr buf);
bool UnlockAndRelease(AdBuf::ConstPtr buf);
/**
 * Data Layout:
 * mmap shm for mgr:
 *  |AdBufManagerMmapInfo|Specific ManagerMmapInfo|
 *  |lock A             |lock B             |lock C             |
 *  |serialized handle A|serialized handle B|serialized handle C|
 *  |Buf Meta A         |Buf Meta C         |Buf Meta C         |
 *
 * device memory:
 * data:   |buf A      |buf B|buf C                      |
 */

/**
 * @brief
 * Q: Can Preallocate and Self-Allocate Buf exist at same manager
 * A: No. Otherwise, it is tool difficult to manage the device memory size limit
 *
 * Q: Whether AdBufManager own one continous memory or multiple memory
 * A: platform depended
 *
 * Q: Who can free memory?
 * A: platform depended
 *
 * Q: Who can allocate memory?
 * A: Only the AdBufManager Master (no other way when consider all possible
 * case)
 *
 * Q: Whether the size of the total device memory is fixed or dynamic
 * A: fixed
 *
 * Q: Whether the number of block is fixed or dynamic
 * A: platform depended
 *
 * Q: When happen if AdBufManager Master crash
 * A: The entire system status become undefined (since the aliveness of device
 * memory block is underfined). Must restart all related process
 *
 * Q: When is the memory allocated?
 * A: When AdBufManager Master Init
 *
 * Q: How ADBuf reuse block
 * A: platform depended.
 */

/**
 * @brief
 *
 * 1. [func] Create AdBuf or Wrap preallocated ptr to AdBuf
 * 2. [func] Free AdBuf
 * 3. [property] Share AdBuf across process
 * 4. [property] Ensure the cross-process read-write safety for ADBuf
 * 5. [property] Each AdBuf can have different size
 * 6. [property] AdBufs may not allocated in a continous spaces
 * 7. [property] AdBufManager support different type of Adbuf Impl, but all the
 * underly Adbuf may have the same type
 */

class AdBufManager : public std::enable_shared_from_this<AdBufManager> {
 public:
    template <typename T>
    static std::shared_ptr<AdBufManager> Create(const std::string& key, T cfg) {
        static_assert(std::is_base_of<BufCfg, T>::value,
                      "cfg must be a subclass of BufCfg");
        T m = cfg;
        m.key(key);
        return AdBufManager::CreateP(key, &m);
    }

    static std::shared_ptr<AdBufManager> CreateP(const std::string& key,
                                                 const BufCfg* cfg);
    static std::shared_ptr<AdBufManager> Open(const std::string& key);

    virtual ~AdBufManager();

    //******* [Master-Only(Platform Depended)] WLock With Allocate Buf *********
    AdBuf::Ptr TryWLockNewBuf();
    // img buf is the most common usecase, specialize a api for it.
    AdBuf::Ptr TryWLockNewBuf(const ImgMeta& img_bufmeta);

    // If block size is fixed, call this function return nullptr
    AdBuf::Ptr TryWLockNewBufWithSize(const size_t size);
    // img buf is the most common usecase, specialize a api for it.
    AdBuf::Ptr TryWLockNewBufWithSize(const size_t size,
                                      const ImgMeta& img_bufmeta);

    //******* WLock With Existing Buf *********
    // In case, want to reuse target buf
    AdBuf::Ptr TryWLockBuf(AdBuf::TokenT token, bool incre_revision = false);
    AdBuf::Ptr TryWLockBuf(AdBuf::TokenT token,
                           const ImgMeta& img_bufmeta,
                           bool incre_revision = false);

    //******* RLock With Existing Buf *********
    AdBuf::ConstPtr TryRLockBuf(AdBuf::TokenT token);
    AdBuf::ConstPtr TryRLockBuf(AdBuf::TokenT token,
                                const uint64_t target_revision);

    //******* UnLock Existing Buf *********
    // TODO(kit) need change this signature for the return value
    // How to handle if unlock fail
    bool UnlockBuf(AdBuf::TokenT buf);
    bool UnlockBuf(const AdBuf::Ptr& buf);
    bool UnlockBuf(const AdBuf::ConstPtr& buf);

    //******* Release Buf *********
    // Only Master can use this
    bool ReleaseBuf(AdBuf::TokenT token);
    bool ReleaseBuf(AdBuf::Ptr buf);
    bool ReleaseBuf(AdBuf::ConstPtr buf);

    bool UnlockAndReleaseBuf(AdBuf::Ptr buf);
    bool UnlockAndReleaseBuf(AdBuf::ConstPtr buf);
    size_t ReleaseAllUnlockedBuf();

    //******* Utilies *************
    virtual size_t WasExistingBufCount() const = 0;
    virtual bool WasAbleToNewBuf() const = 0;
    virtual std::vector<AdBuf::ConstPtr> WasAllBufs() = 0;
    virtual void SyncBufMeta(const std::shared_ptr<BufCfg>& cfg) {
        static_cast<void>(cfg);
    }

    //******* developer info *************
    inline const AdBufManagerMmapInfo& mmaped_base_info() const {
        return *mmaped_base_info_;
    }

    //******* validation check *************
    bool IsMasterDead() const;
    bool IsManagerUsable() const;
    bool IsMasterStillSame() const;

 protected:
    AdBufManager() = default;

    //******* size calculation *************
    uint32_t InfoSectionOffset() const;
    uint32_t SpecificInfoSectionOffset() const;
    uint32_t LocksSectionOffset() const;
    uint32_t HandlesSectionOffset() const;
    uint32_t BufMetasSectionOffset() const;

    uint32_t ExpectedMmapSize(const BufCfg& base_cfg) const;
    bool IsMaster() const;
    std::string Key() const;

    //******* mmap init *************
    static base::optional<AdBufManagerMmapInfo>
    GetAdBufManagerMmapInfoFromMmapedFile(const std::string& mmaped_file);

    bool CheckMmapedFileStillValid(const std::string& mmaped_file) const;
    bool InitAdBufManagerForCreate(const std::string& mmaped_file,
                                   const BufCfg& base_cfg);
    bool InitAdBufManagerForOpen(const std::string& mmaped_file);

    //******* init utilies for specific mmap info and buf handle *************
    template <typename T>
    T* InitSpecificMmapInfo() const {
        return new (static_cast<char*>(mmap_ptr_) + SpecificInfoSectionOffset())
            T();
    }
    template <typename T>
    T* GetSpecificMmapInfo() const {
        return reinterpret_cast<T*>(static_cast<char*>(mmap_ptr_) +
                                    SpecificInfoSectionOffset());
    }

    template <typename T>
    T* GetBufHandles() const {
        return reinterpret_cast<T*>(static_cast<char*>(mmap_ptr_) +
                                    HandlesSectionOffset());
    }

    //******* Impl by underly impl *************
    virtual AdBuf::Ptr OwnBuf() = 0;
    virtual AdBuf::Ptr OwnBufWithSize(const size_t size) = 0;

    virtual AdBuf::Ptr OpenBuf(AdBuf::TokenT token) = 0;
    virtual bool ReleaseBufImpl(AdBuf::ConstPtr buf) = 0;
    virtual bool IsBufValidAfterMasterDead() const { return true; }

    std::string key_;
    int mmaped_file_fd_ = 0;
    std::string mmaped_file_;
    BufCfg base_cfg_;

    void* mmap_ptr_ = nullptr;
    uint32_t mmap_size_ = 0;
    AdBufManagerMmapInfo* mmaped_base_info_ = nullptr;
    std::string master_gen_id_;

    std::shared_ptr<MmapRwlocks> buf_locks_;

    BufMeta* buf_metas_;

    bool is_master_ = false;

 private:
    friend class AdBuf;
};

}  // namespace ad_buf
}  // namespace senseAD
