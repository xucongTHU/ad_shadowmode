/**
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include "ad_log/ad_log.hpp"
#include "ad_rscl/component/lifecycle/fsm.h"

namespace senseAD {

/**
 * @brief Define the lifecycle of a module. User override the function to create
 * their module
 *
 */
class LifeCycle {
 public:
    virtual ~LifeCycle() {}

    // Trigger by Component Automatically
    /**
     * @brief Call when system is initialzed and going to init the user variable
     * STATE: StateStage::INITING
     *
     * @return true
     * @return false
     */
    virtual bool OnInit() = 0;
    /**
     * @brief Call before OnInit
     */
    virtual bool PreInit() = 0;
    /**
     * @brief Call after OnInit
     * STATE: StateStage::INITING and StateChangeStage::POST_CHANGE
     */
    virtual bool PostInit() = 0;
    /**
     * @brief Call when the system is shuting down
     * STATE::STOPPED_NORMAL or StateStage::STOPPED_ABNORMAL
     */
    virtual void OnShutdown() = 0;

    // Trigger by monitor
    /**
     * @brief Call when the auto driving mode is ready
     * STATE: StateStage::READY
     *
     * @return true
     * @return false
     */
    virtual bool OnReady() = 0;
    /**
     * @brief Call when reset
     * STATE: StateStage::RESETTING
     *
     * @return true
     * @return false
     */
    virtual bool OnReset() = 0;
    /**
     * @brief Call when pause the running
     * STATE: StateStage::SUSPENDED
     *
     * @return true
     * @return false
     */
    virtual bool OnSuspend() = 0;
    /**
     * @brief Call when resume running from paused
     * STATE: StateStage::RESUMING
     *
     * @return true
     * @return false
     */
    virtual bool OnResume() = 0;

    /**
     * @brief Call when mode switching
     * STATE: StateStage::MODE_SWITCHING
     *
     * @return true
     * @return false
     */
    virtual bool OnModeSwitching(const RunningMode) = 0;

    // Trigger when proc
    /**
     * @brief Call before OnProc
     *
     */
    virtual void PreProc() = 0;
    /**
     * @brief Call after OnProc
     *
     */
    virtual void PostProc() = 0;
};

/**
 * @brief Using a fsm to maintain the lifecycle
 *
 */
class ManagementableLifeCycle : public LifeCycle {
 public:
    ManagementableLifeCycle() { this->InitFsm(); }
    ~ManagementableLifeCycle() override {}

    bool OnInit() override { return true; }
    bool PreInit() override { return true; }
    bool PostInit() override { return true; }
    void OnShutdown() override {}

    bool OnReady() override { return true; }
    bool OnReset() override { return true; }
    bool OnSuspend() override { return true; }
    bool OnResume() override { return true; }
    bool OnModeSwitching(const RunningMode) override { return true; }

    void PreProc() override {}
    void PostProc() override {}

 protected:
    inline bool EmitEvent(const Event e) { return fsm_.EmitEvent(e); }
    inline State GetState() const { return fsm_.GetState(); }
    /**
     * @brief The internal state machine. Not expected to be called by the user
     *
     */
    Fsm fsm_;

 private:
    void InitFsm();
};

}  // namespace senseAD
