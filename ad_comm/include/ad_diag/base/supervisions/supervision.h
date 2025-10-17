/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <queue>
#include <mutex>

#include "ad_base/common/thread_pool.h"
#include "ad_base/std/optional.h"
#include "ad_base/common/macros.h"
#include "ad_diag/base/supervisions/common.h"
#include "ad_diag/base/supervisions/rpc.h"

namespace senseAD {
namespace diag {

/********************************************
 *! FOR SWC TO DO SUPERVISION              !*
 ********************************************/

// Act as the base class of client interface
class SupervisionBase {
 public:
    SupervisionBase();
    explicit SupervisionBase(const SupervisionUniqueId uid);
    explicit SupervisionBase(const SupervisionSetting& setting);
    virtual ~SupervisionBase();

    void Init(const SupervisionSetting& setting);

    const SupervisionSetting& setting() const;
    const SupervisionEntity& entity() const;
    const SupervisionStatus& status() const;

    bool IsValid() const;

 protected:
    // report to monitor
    void ReportSupervision(const SupervisionReport& report,
                           const char* full_report_ptr,
                           const size_t full_report_size);

    // whether should do the logic
    bool IsEnabled() const;
    void HandleModeChange(const SupervisionGroup& active_group,
                          const ImportantLevel& active_level);

    void UpdateStatus(const SupervisionStatus::StatusLevel& next_level,
                      const std::string& problem_description);

    void PrefillSupervisionReport(SupervisionReport* report) const;

    void UpdateStatus(const SupervisionStatus::StatusLevel& next_level,
                      const std::string& problem_description,
                      const std::string& id);
    void PrefillSupervisionReport(SupervisionReport* report,
                                  const std::string& id);

    bool valid_ = false;

    // Load from init
    SupervisionSetting setting_;
    // Update when report
    SupervisionStatus status_;

    std::shared_ptr<SupervisionRPC::Client> rpc_client_;
    Token client_token_;

    std::vector<char> buf_;
    mutable uint64_t seq_ = 0;

    std::mutex mt_;
    std::unordered_map<std::string, SupervisionStatus> map_status_;
};

/**
 * @brief Control the global status of all supervision instance
 *
 */
class SupervisionManager {
 public:
    void Init(const std::string& swc_name);
    // void Reset();

 private:
    std::string SearchConfigFile(const std::string& swc_name);

    std::string swc_name_;
    std::unordered_map<SupervisionUniqueId, SupervisionSetting> settings_;

    DECLARE_AD_SINGLETON(SupervisionManager)
};

/********************************************
 *! FOR MONITOR TO RECEIVE SUPERVISION     !*
 ********************************************/

enum class EndpointConnectionState : uint32_t {
    CONNECTED = 0,
    NOT_CONNECTED = 1,
    PENDING = 2
};

// Act as the base class of server interface
class SupervisionEndpointBase {
 public:
    virtual ~SupervisionEndpointBase();

    void Subscribe(uint32_t buffer_size);
    void UnSubscribe();

    void SetOnNewReportHandler(std::function<void()> handler);
    bool GetNewReport(SupervisionEntity* entity,
                      SupervisionReport* base_report,
                      std::vector<char>* serialized_report);
    size_t GetNewReports(
        const std::function<void(const SupervisionEntity&,
                                 const SupervisionReport&,
                                 const std::vector<char>&)>& handle_fn);

    size_t GetNewReportCount() const;

 protected:
    explicit SupervisionEndpointBase(
        const SupervisionCppType& supervision_type = SupervisionCppType::ALL);

 private:
    std::unique_ptr<senseAD::base::common::ThreadPool> waker_;
    std::shared_ptr<SupervisionRPC::Server> rpc_ser_;

    SupervisionCppType supervision_type_;
    mutable std::mutex gmtx_;
    bool subscribed_ = false;
    uint32_t buffer_size_ = 0;
    struct Report {
        SupervisionEntity entity;
        SupervisionReport base_report;
        std::vector<char> serialized_report;
    };

    std::function<void()> onnew_handler_;

    std::queue<std::shared_ptr<Report>> buffer_;
    std::vector<std::shared_ptr<Report>> mem_pool_;
};

template <typename ReportType, SupervisionCppType CppType>
class SupervisionEndpoint : public SupervisionEndpointBase {
 public:
    SupervisionEndpoint() : SupervisionEndpointBase(CppType) {}
    bool GetNewReport(SupervisionEntity* entity, ReportType* report);
    size_t GetNewReports(
        const std::function<void(const SupervisionEntity&, const ReportType&)>&
            handle_fn);

 private:
};

class SupervisionEndpointManager {
 public:
    std::shared_ptr<SupervisionEndpointBase> GetEndpoint(
        const SupervisionCppType& type);

    void ChangeEnabledMode(const SupervisionGroup target_group,
                           const ImportantLevel target_level);

 private:
    std::mutex gmtx_;
    std::shared_ptr<SupervisionRPC::Server> rpc_ser_;
    std::unordered_map<uint32_t, std::shared_ptr<SupervisionEndpointBase>>
        endpoints_;

    DECLARE_AD_SINGLETON(SupervisionEndpointManager)
};

}  // namespace diag
}  // namespace senseAD
