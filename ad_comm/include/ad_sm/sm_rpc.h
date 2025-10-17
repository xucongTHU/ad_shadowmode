/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Wang Kun <wangkun2@senseauto.com>
 */

#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include "sm_base.h"  // NOLINT
#include "ad_rscl/ad_rscl.h"
#include "ad_base/common/rpc.h"
#include "ad_log/ad_log.hpp"

using senseAD::base::common::RpcClient;
using senseAD::base::common::RpcServer;
using senseAD::base::common::RpcStatus;

namespace senseAD {
namespace ad_sm {

enum class SMStatusKey : int {
    RecordStatus_is_start_record = 0,
    RecordStatus_is_recording = 1,
};

class SMRpc {
 public:
    SMRpc() = default;
    virtual ~SMRpc() = default;

 protected:
    std::string rpc_ip_;
    uint32_t rpc_port_;
};

class SMRpcServer : public SMRpc {
 public:
    bool Init(const std::string &ip, uint32_t port);
    SMStatus *GetSMStatus();

 private:
    std::shared_ptr<RpcServer> server_{nullptr};
    SMStatus sm_status_;

 private:
    RpcStatus serverFunc(const char *in_data_ptr,
                         const size_t in_data_size,
                         std::vector<char> *out_data);
};

class SMRpcClient : public SMClient, public SMRpc {
 public:
    SMRpcClient();
    bool Init(const std::string &ip,
              uint32_t port,
              uint64_t check_period_ms = 1000);

    void RegisterStateChangeFunc(StateChangeFunc func) override;
    SMStatus GetSMStatus() override;

    // Check the content of special fields,
    // only trigger the callback function when the content of the field changes
    // if not set， will trigger cb while any segment of sm_status changes
    void AddStateChangeCondition(const std::vector<SMStatusKey> &conditions);

 private:
    // sm_conf proto keys
    const std::map<SMStatusKey, std::string> CHECK_KEYS;

    std::shared_ptr<RpcClient> client_{nullptr};
    StateChangeFunc state_change_cb_func_ = nullptr;
    uint64_t check_period_ms_{1000};
    std::unique_ptr<rscl::task::TimerTask> check_timer_{nullptr};
    std::vector<std::string> special_segments_{};

    void checkStatus();
};

}  // namespace ad_sm
}  // namespace senseAD
