/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <atomic>
#include <limits>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

#include "ad_mw/ad_mw.h"
#include "ad_rscl/time/time.h"
#include "ad_rscl/common/interface.h"
#include "ad_rscl/idl/comm_conf.pb.h"
#include "ad_serde/ad_serde.h"

#include "ad_service_discovery/polyfill_service_discovery/sd_service_client.h"

namespace senseAD {
namespace rscl {

static constexpr const char* INTERNAL_NODE_SPACE =
    senseAD::mw::comm::INTERNAL_NODE_SPACE;

namespace comm {
using RawMessage = senseAD::mw::RawMessage;
using BorrowedRawMessage = senseAD::mw::BorrowedRawMessage;

using QosProfile = senseAD::rscl::idl::QosProfile;
using PublisherConf = senseAD::rscl::idl::PublisherConf;
using SubscriberConf = senseAD::rscl::idl::SubscriberConf;
using ServiceConf = senseAD::rscl::idl::ServiceConf;
using ClientConf = senseAD::rscl::idl::ClientConf;

using MwNodeHandle = senseAD::mw::comm::Node;

using MwPublisherHandle = senseAD::mw::comm::Publisher;
using MwSubscriberHandle = senseAD::mw::comm::Subscriber;
using MwServiceHandle = senseAD::mw::comm::Service;
using MwClientHandle = senseAD::mw::comm::Client;

class NodeOwnerBase {
 public:
    NodeOwnerBase() = default;
    ~NodeOwnerBase() = default;
    virtual MwNodeHandle* InnerHandle() const = 0;
    virtual const std::string& GetName() const = 0;
};

/**
 * @brief for pub/sub msg frame ratio currently
 *
 */
class TransmitEval {
 public:
    TransmitEval();
    ~TransmitEval() = default;

    TransmitEval(const TransmitEval& other) noexcept;
    TransmitEval(TransmitEval&& other) noexcept;
    TransmitEval& operator=(TransmitEval&& other) noexcept;

    void Disable();

    TransmitEval& operator+=(const uint64_t byte_size);

    // Used to combine other TransmitEval
    void Add(const TransmitEval& val, const uint32_t number_of_eval);

    double UpdateFrameRatio();

    inline double GetFrameRatio() const { return frame_ratio_.load(); }

    inline uint64_t GetFrameCounter() const { return frame_counter_.load(); }

    inline uint64_t GetMsgTimeNs() const { return msg_time_ns_.load(); }

    inline int64_t GetLatestTimeDiffNs() const {
        return latest_diff_ns_.load();
    }

    inline int64_t GetMaxTimeDiffNs() const { return max_diff_ns_.load(); }
    inline int64_t GetMinTimeDiffNs() const { return min_diff_ns_.load(); }

    inline double GetAvgMsgKb() const { return avg_msg_kb_.load(); }

    void Reset();

 private:
    bool enable_{true};
    std::atomic<uint64_t> frame_counter_;
    std::atomic<uint64_t> msg_time_ns_;
    std::atomic<int64_t> latest_diff_ns_;
    std::atomic<int64_t> max_diff_ns_;
    std::atomic<int64_t> min_diff_ns_;
    std::atomic<double> frame_ratio_;
    std::atomic<double> avg_msg_kb_;
    rscl::time::Time last_time_;
};

class NodeMemberBase {
 public:
    NodeMemberBase() = default;
    ~NodeMemberBase() = default;
    inline MwNodeHandle* GetMwNode(const NodeOwnerBase& node) const {
        return node.InnerHandle();
    }
    TransmitEval transmit_eval_copy() { return transmit_eval_; }

 protected:
    TransmitEval& transmit_eval() { return transmit_eval_; }

    std::shared_ptr<senseAD::service_discovery::SDServiceClient>
        sd_client_sptr_ =
            senseAD::service_discovery::SDServiceClient::SharedInstance();
    bool should_send_sd_msg_{false};

 private:
    TransmitEval transmit_eval_;
};

class SwitchableNodeMemberBase : public NodeMemberBase,
                                 public senseAD::rscl::common::Suspendable {
 public:
    virtual ~SwitchableNodeMemberBase() = default;

    inline void Enable() {
        enabled_ = true;
        ToggleSwitch(enabled_);
    }
    inline void Disable() {
        enabled_ = false;
        ToggleSwitch(enabled_);
    }
    inline bool Suspend() final {
        Disable();
        return true;
    }
    inline bool Resume() final {
        Enable();
        return true;
    }
    /**
     * @brief Need the implement class to check this thread to determine whether
     * it should function
     *
     * @return true
     * @return false
     */
    inline bool IsEnabled() { return enabled_; }

 protected:
    virtual void ToggleSwitch(const bool) {}

 private:
    bool enabled_ = true;
};

senseAD::mw::proto::QosProfile ToMwQosProfile(
    const senseAD::rscl::idl::QosProfile&);
senseAD::mw::proto::PublisherConf ToMwPublisherConf(
    const senseAD::rscl::idl::PublisherConf&);
senseAD::mw::proto::SubscriberConf ToMwSubscriberConf(
    const senseAD::rscl::idl::SubscriberConf&);
senseAD::mw::proto::ServiceConf ToMwServiceConf(
    const senseAD::rscl::idl::ServiceConf&);
senseAD::mw::proto::ClientConf ToMwClientConf(
    const senseAD::rscl::idl::ClientConf&);

struct UniqueTopic {
    std::string topic_name;
    std::string type_name;
    std::string descriptor;
    bool disable_cross_system{false};
};

class CommInstanceCount {
 public:
    enum class SupportedType : uint8_t { PUB = 0, SUB = 1 };

    uint32_t Add(const SupportedType type,
                 const std::string& topic_name,
                 const std::string& type_name,
                 const std::string& descriptor,
                 const bool disable_cross_system = false);
    void Delete(const SupportedType type, const uint32_t token);
    std::vector<UniqueTopic> List(const SupportedType type,
                                  uint64_t* change_cnt = nullptr);
    uint64_t ChangeCnt() const;

 private:
    mutable std::mutex mtx_;

    uint32_t idx_{0};
    std::unordered_map<uint8_t, std::unordered_map<uint32_t, UniqueTopic>>
        store_;
    uint64_t change_cnt_{0};

    DECLARE_AD_SINGLETON(CommInstanceCount)
};

class CommInstanceCountHelper {
 public:
    static std::shared_ptr<CommInstanceCountHelper> New(
        const CommInstanceCount::SupportedType type,
        const std::string& topic_name,
        const std::string& type_name,
        const std::string& descriptor,
        const bool disable_cross_system = false);
    ~CommInstanceCountHelper();

 private:
    CommInstanceCountHelper(const CommInstanceCount::SupportedType type,
                            const uint32_t token);

    std::shared_ptr<CommInstanceCount> ref_{nullptr};
    CommInstanceCount::SupportedType type_;
    uint32_t token_{0};
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
