/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "ad_log/ad_log.hpp"
#include "ad_base/class_loader/class_loader_register_macro.h"
#include "ad_base/common/shutdownable.h"
#include "ad_rscl/comm/node.h"
#include "ad_rscl/common/file.h"
#include "ad_rscl/common/interface.h"
#include "ad_rscl/component/param/param_parser.h"
#include "ad_rscl/component/param/param_loader.h"
#include "ad_rscl/component/impl/env_param_impl.h"
#include "ad_rscl/component/impl/runtime_state_impl.h"
#include "ad_rscl/component/lifecycle/lifecycle.h"
#include "ad_rscl/component/types.h"

namespace senseAD {
namespace rscl {
namespace component {

using senseAD::rscl::comm::Node;

/**
 * @brief Base class for component. Componet is the base wrapper for each
 * function module
 *
 */
class ComponentBase : public std::enable_shared_from_this<ComponentBase>,
                      public senseAD::ManagementableLifeCycle,
                      public ::Shutdownable,
                      public senseAD::rscl::common::Suspendable {
 public:
    // ***** Core Feature *****

    ComponentBase();
    bool LoadConfig(const ComponentConfig& config);
    virtual ~ComponentBase();
    virtual bool Initialize(const ComponentConfig& config) = 0;
    virtual bool Initialize() = 0;
    virtual void SetupCustomParam() {}

    void ShutdownImpl() override {
        EmitEvent(senseAD::Event::SYSTEM_EXIT);
        if (monitor_cli_impl_ != nullptr) {
            monitor_cli_impl_->Shutdown();
            monitor_cli_impl_ = nullptr;
        }
        env_param_impl_ = nullptr;
        if (node_ != nullptr) {
            node_->Shutdown();
            node_ = nullptr;
        }
    }

    // ***** Function provided in impl *****

    inline bool GetEnvParam(const char* param_name,
                            std::string* param_str,
                            bool allow_global = false) const {
        return env_param_impl_->GetEnvParam(param_name, param_str,
                                            allow_global);
    }
    inline void PushFunctionCost(const std::string& func_name,
                                 const uint64_t& time_cost_ns) {
        if (monitor_cli_impl_) {
            monitor_cli_impl_->PushFunctionCost(func_name, time_cost_ns);
        }
    }

    inline void UpdateComponentCost(const uint64_t& time_cost_ns) {
        if (monitor_cli_impl_) {
            monitor_cli_impl_->UpdateComponentCost(time_cost_ns);
        }
    }

    /**
     * @brief Get the Param object according to yaml config file definition.
     * This funtion would iterate all_config_node_(YAML::Node) which loaded all
     * configs. If
     * direct key not found, search key in all_config_node_["node_param"] node.
     * If still search fail, return false.
     *
     * @tparam T Support basic built-in types and std::vector<T>
     * @param key
     * @param container
     * @return true
     * @return false
     */
    template <typename T>
    bool GetParam(const std::string& key, T& container) {  // NOLINT
        return param_loader_->GetParam(key, container);
    }

    template <typename T>
    bool GetParam(const std::string& key,
                  T& container,  // NOLINT
                  T default_val) {
        if (!GetParam(key, container)) {
            container = default_val;
        }
        return true;
    }

    /**
     * @brief Specialition for implicit conversion of std::string
     * and const char*
     *
     */
    inline bool GetParam(const std::string& key,
                         std::string& container,  // NOLINT
                         const char* default_val) {
        if (!GetParam(key, container)) {
            container = std::string(default_val);
        }
        return true;
    }

    void SendComponentStatusImmediately() const;
    bool PreInit() override;
    bool PostInit() override;

    bool Suspend() override;
    bool Resume() override;

 protected:
    /**
     * @brief Utilies Class to handle the status change while OnProc
     *
     */
    class OnProcGuard {
     public:
        explicit OnProcGuard(ComponentBase* c) : c_(c) {
            c_->HandleEventSignal();
            able_to_proc_ = c->GetState().AbleToProc();
            if (able_to_proc_) {
                c_->PreProc();
                c_->EmitEvent(senseAD::Event::TICK);
            }
        }
        ~OnProcGuard() {
            if (able_to_proc_) {
                c_->PostProc();
            }
        }

        bool AbleToProc() const { return able_to_proc_; }

     private:
        ComponentBase* c_;
        bool able_to_proc_;
    };

    void HandleEventSignal();

    void InnerNodeCreate(const std::string& name);
    virtual void PostInnerNodeCreated();
    inline Node* InnerNode() const { return node_.get(); }

    void LoadConfigFiles(const ComponentConfig& config);
    senseAD::mw::comm::Node* GetMwNode() const;

    bool ParseAllConfigYaml();
    bool SetupExternalCommunication(const std::string& node_name);

    std::atomic<bool> need_reset_ = {false};
    std::atomic<int> ctrl_action_ = {-1};

    std::shared_ptr<Node> node_ = nullptr;

    /* config related */
    YAML::Node all_config_node_;
    std::unordered_map<std::string, std::string> custom_param_;
    std::unique_ptr<GeneralParamLoader> param_loader_;
    std::shared_ptr<ComponentConfig> config_;
    // offline_config_path_: rscl_param.yaml/ros_param.yaml in replay data
    std::string offline_config_path_;

    // Splited impl
    std::unique_ptr<EnvParamImpl> env_param_impl_;
    std::unique_ptr<RuntimeStateCliImpl> monitor_cli_impl_;

    std::shared_ptr<senseAD::rscl::Runtime> rt_ptr_;

    friend class EnvParamImpl;
    friend class RuntimeStateCliImpl;
};

/**
 * @brief Register the component class into the global class loader
 *
 */
#define ASCL_REGISTER_COMPONENT(name) \
    CLASS_LOADER_REGISTER_CLASS(name, senseAD::rscl::component::ComponentBase)

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
