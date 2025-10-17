/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include "ad_base/common/macros.h"
#include "ad_mw/ad_mw.h"
#include "ad_mw/proto/runtime_conf.pb.h"
#include "ad_rscl/idl/global_config.pb.h"
#include "ad_rscl/common/interface.h"
namespace senseAD {
namespace rscl {
namespace comm {
class Node;
class NodeImpl;
class SubscriberBase;
}  // namespace comm

namespace runtime {
class NodeManagement;
class ProcessManagement;
}  // namespace runtime

namespace comm_cs3 {
class RemoteEndpointStore;
}

using senseAD::rscl::comm::Node;
using senseAD::rscl::comm::NodeImpl;
using senseAD::rscl::comm::SubscriberBase;
using RuntimeParam = senseAD::mw::RuntimeParam;
using senseAD::rscl::common::Suspendable;

struct RuntimeConfig {
    // load from rscl.pb.txt file
    std::shared_ptr<idl::GlobalConfig> pj_cfg;
    // dynamic init
    RuntimeParam rt_param;

    // For specific tools to disable receive sim time
    bool allow_sim_mode = true;

    bool rename_process_name{true};
};
// should not cover inner linux exit code
enum ExitCode : int {
    // exit code when node name duplicate
    NODE_DUPLICATION = 12,
    RESUME_FAILED = 13,
};

/**
 * @brief State of the entire runtime system
 *
 */
enum State : std::uint8_t {
    STATE_UNINITIALIZED = 0,
    STATE_INITIALIZED,
    STATE_SHUTTING_DOWN,
    STATE_SHUTDOWN,
    STATE_SUSPENDED,

    // Below is unexpected state.
    // Need debug if such state happen
    STATE_SUSPEND_FAILED,
    STATE_RESUME_FAILED,
};

/**
 * @brief Contain all variable and function to interact to the system
 *
 */
class Runtime {
 public:
    ~Runtime();
    /**********************************************************
     *                  Init/Exit Support
     ***********************************************************/

    /**
     * @brief Setup global config
     * @param argc From main
     * @param argv From main
     * @param rt_conf Config
     */
    void SetupGlobalConfig(int argc,
                           char* argv[],
                           const RuntimeConfig& rt_conf = RuntimeConfig());

    /**
     * @brief Init the runtime with process_name
     *
     * @param argc From main
     * @param argv From main
     * @param conf Config
     */
    void Init(int argc,
              char* argv[],
              const RuntimeConfig& rt_conf = RuntimeConfig());

    // TODO(kitfung)
    //    void Reset();

    /**
     * @brief Get the runtime current state
     *
     * @return State
     */
    inline State GetState() const { return state_.load(); }

    /**
     * @brief Set the runtime current state
     *
     * @param state state to be set
     */
    inline void SetState(const State& state) { state_.store(state); }

    /**
     * @brief Whether the current system is running normally
     *
     * @return true
     * @return false
     */
    inline bool OK() const {
        return GetState() == STATE_INITIALIZED || GetState() == STATE_SUSPENDED;
    }
    /**
     * @brief Whether the system is shut down
     *
     * @return true
     * @return false
     */
    bool IsShutdown() const {
        return GetState() == STATE_SHUTDOWN ||
               GetState() == STATE_SHUTTING_DOWN;
    }
    /**
     * @brief Loop until the system is turn down. Return whether it is shutdown
     *
     * @param timeout_ms Maximun wait time
     * @return true
     * @return false
     */
    bool WaitForShutdown(std::chrono::milliseconds timeout_ms =
                             std::chrono::milliseconds(-1)) const;

    /**
     * @brief Turn down the system.
     *
     */
    void Shutdown();

    /**********************************************************
     *                  Node Support
     ***********************************************************/

    /**
     * @brief Create a Node handle pointer
     *
     * @param node_name name of the node
     * @param node_namespace
     * @return std::unique_ptr<Node>
     */
    std::unique_ptr<Node> CreateNode(const std::string& node_name,
                                     const std::string& node_namespace = "");

    /**********************************************************
     *                  Cross System Support
     ***********************************************************/
    comm_cs3::RemoteEndpointStore* GetRemoteEndpointStore();

    /**********************************************************
     *                  Suspend/Resume Support
     ***********************************************************/
    void EnableProcessManagement();

    void RegisterGlobalSuspendable(Suspendable* instance);
    void UnRegisterGlobalSuspendable(Suspendable* instance);
    void RegisterComponent(Suspendable* instance);
    void UnRegisterComponent(Suspendable* instance);
    bool Suspend();
    void Resume();

    void InitExternalIPC() const;

    void DeinitExternalIPC() const;

    mw::KeyValue GetKeyValue() const;

    mw::KeyValues GetKeyValues() const;

 private:
    void InitSimTime();
    void UpdateState(const State& state);

    std::atomic<State> state_;
    std::unique_ptr<Node> rt_node_;
    // Cross system Related
    std::mutex remote_endpoint_store_mutex_;
    std::shared_ptr<comm_cs3::RemoteEndpointStore> remote_endpoint_store_{
        nullptr};
    // Sim-Time Related
    std::shared_ptr<SubscriberBase> sim_clock_sub_;
    bool has_setup_global_config_ = false;
    // Support Node, Suspend/Resume
    std::unique_ptr<runtime::NodeManagement> node_mgt_;
    std::unique_ptr<runtime::ProcessManagement> process_mgt_;

    std::mutex state_mutex_;

    std::vector<std::shared_ptr<void>> ref_holders_;
    DECLARE_AD_SINGLETON(Runtime)
};

inline Runtime* GetCurRuntime() { return Runtime::Instance(); }

void SetProcName(const std::string& process_name,
                 const std::pair<int, char**>& main_arg);
}  // namespace rscl
}  // namespace senseAD
