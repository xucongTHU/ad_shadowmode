/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>

#include "ad_mw/comm.h"
#include "ad_mw/visibility_control.h"
#include "cyber/cyber.h"

namespace senseAD {
namespace mw {
namespace comm {

class SubscriberCallbackObserverStore;

using CyberNodeHandleT = apollo::cyber::Node;
template <typename T>
using CyberPublisherHandleT = apollo::cyber::Writer<T>;

struct CyberSubscriberHandleBase {
 public:
    CyberSubscriberHandleBase();
    virtual apollo::cyber::ReaderBase* GetCyberReaderBase() = 0;

    void SetSubStoreToken(const uint32_t token);
    uint32_t GetSubStoreToken() const;

    std::shared_ptr<SubscriberCallbackObserverStore> store();

 private:
    uint32_t sub_store_token_ = 0;
    std::shared_ptr<SubscriberCallbackObserverStore> store_;
};

template <typename T>
struct CyberSubscriberHandleT : public CyberSubscriberHandleBase {
 public:
    explicit CyberSubscriberHandleT(
        std::shared_ptr<apollo::cyber::Reader<T>> _reader)
        : reader(_reader) {}

    CyberSubscriberHandleT(
        std::shared_ptr<apollo::cyber::Reader<T>> _reader,
        const std::function<void(const std::shared_ptr<T>&)>& _cb)
        : reader(_reader), cb(_cb) {}

    virtual ~CyberSubscriberHandleT() = default;

    apollo::cyber::ReaderBase* GetCyberReaderBase() override {
        return reader.get();
    }
    std::shared_ptr<apollo::cyber::Reader<T>> reader;
    std::function<void(const std::shared_ptr<T>&)> cb;
};

template <typename T0, typename T1>
using CyberServiceHandleT = apollo::cyber::Service<T0, T1>;
template <typename T0, typename T1>
using CyberClientHandleT = apollo::cyber::Client<T0, T1>;

inline CyberNodeHandleT* ToNodeHandle(void* ptr) {
    return reinterpret_cast<CyberNodeHandleT*>(ptr);
}
template <typename T>
inline CyberPublisherHandleT<T>* ToPublisherHandle(void* ptr) {
    return reinterpret_cast<CyberPublisherHandleT<T>*>(ptr);
}
inline CyberSubscriberHandleBase* ToSubscriberBaseHandle(void* ptr) {
    return reinterpret_cast<CyberSubscriberHandleBase*>(ptr);
}
template <typename T>
inline CyberSubscriberHandleT<T>* ToSubscriberHandle(void* ptr) {
    return reinterpret_cast<CyberSubscriberHandleT<T>*>(ptr);
}
template <typename T0, typename T1>
inline CyberServiceHandleT<T0, T1>* ToServiceHandle(void* ptr) {
    return reinterpret_cast<CyberServiceHandleT<T0, T1>*>(ptr);
}
template <typename T0, typename T1>
inline CyberClientHandleT<T0, T1>* ToClientHandle(void* ptr) {
    return reinterpret_cast<CyberClientHandleT<T0, T1>*>(ptr);
}

ADMW_PUBLIC apollo::cyber::proto::RoleAttributes RoleAttributesFromConf(
    const PublisherConf& conf);
ADMW_PUBLIC apollo::cyber::ReaderConfig ReaderConfigFromConf(
    const SubscriberConf& conf);

}  // namespace comm
}  // namespace mw
}  // namespace senseAD
