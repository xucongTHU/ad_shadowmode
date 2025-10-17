/*
std::unique_ptr<senseAD::base::common::ThreadPool> tp_;
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <atomic>

#include "cyber/cyber.h"
#include "ad_mw/comm.h"
#include "ad_mw/visibility_control.h"
#include "ad_mw_cyber/comm_handle.h"
#include "ad_base/common/thread_pool.h"

namespace senseAD {
namespace mw {
namespace comm {

/**
 * @brief An workaround for cyber not support multiple subscribe for same
 * channel in a node
 *
 */
class ADMW_PUBLIC SubscriberCallbackObserverStore {
 public:
    uint32_t AddObserver(const std::string& key,
                         const std::shared_ptr<CyberSubscriberHandleT<
                             apollo::cyber::message::RawMessage>> observer);
    void RemoveObserver(const uint32_t token);

    void Notify(const std::string& key,
                const std::shared_ptr<apollo::cyber::message::RawMessage>& msg);

    static std::shared_ptr<SubscriberCallbackObserverStore> Instance();
    ~SubscriberCallbackObserverStore();

 private:
    std::mutex gmtx_;
    std::atomic<uint32_t> token_{1};
    std::unordered_map<std::string, std::mutex> c_mtxs_;
    std::unordered_map<std::string, std::unordered_set<uint32_t>>
        observerd_tokens_;

    std::unordered_map<uint32_t,
                       std::shared_ptr<CyberSubscriberHandleT<
                           apollo::cyber::message::RawMessage>>>
        token_cands_;
    std::unordered_map<uint32_t, std::string> token_keys_;
    std::unique_ptr<senseAD::base::common::ThreadPool> tp_;
};
}  // namespace comm
}  // namespace mw
}  // namespace senseAD
