/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <memory>
#include <string>

#include "rsclbag/rec_replayer/common.h"
#include "rsclbag/rec_replayer/player.h"
#include "rsclbag/rec_replayer/cluster.h"

class ReplayController {
 public:
    virtual ~ReplayController() {}
    virtual void Init() = 0;
    virtual bool Start() = 0;
    virtual void Stop() = 0;
    virtual bool IsEnd() = 0;
};

class ProgressBar {
 public:
    inline void SetMessage(const std::string& msg) {
        std::lock_guard<std::mutex> lg(mtx);
        message_ = msg;
    }
    inline std::string GetMessage() {
        std::lock_guard<std::mutex> lg(mtx);
        std::string msg{message_};
        return msg;
    }

 private:
    std::string message_;
    std::mutex mtx;
};

/**
 * @brief own all the execution
 * Suppose to have 3 thread
 * - logic thread
 *  - do the init and tsync
 *  - handle the data loading and ticket update
 * - tui thread
 *  - do the tui display and user interaction
 * - replay thread
 *  - do data publishing and control the play time
 */
class ReplayServerController : public ReplayController {
 public:
    explicit ReplayServerController(const RecReplayGlobalOption& option);
    virtual ~ReplayServerController();

    // Init all member variable and thread
    virtual void Init();

    /**
     * @brief Execute all necessary preparation work in blocking mode. Then
     * launch all following data loading & cmd sync in new thread;
     * - Step 1. Prepration
     *  - load bag header
     *  - sync channels & tsync
     * - Step 2. Start TUI Thread
     * - Step 3. Start Main Thread
     */
    virtual bool Start();

    /**
     * @brief Stop all thread and Release member
     *
     */
    virtual void Stop();

    virtual bool IsEnd();

 private:
    void TuiThread();
    void MainLogicThread();

 private:
    RecReplayGlobalOption option_;
    LoadedMessageBuffer loaded_message_buffer_;

    bool running_{false};

    std::unique_ptr<ReplayServer> cluster_server_;
    std::unique_ptr<ReplayPlayer> player_;
    std::unique_ptr<rscl::Node> node_;
    PlayRuntimeSetting runtime_setting_;

    std::thread tui_thread_;
    std::thread main_logic_thread_;

    ProgressBar progress_bar_;
};

class ReplayClientController : public ReplayController {
 public:
    explicit ReplayClientController(const RecReplayGlobalOption& option);
    virtual ~ReplayClientController();

    virtual void Init();
    virtual bool Start();
    virtual void Stop();
    virtual bool IsEnd();

 private:
    void TuiThread();

 private:
    RecReplayGlobalOption option_;
    LoadedMessageBuffer loaded_message_buffer_;

    bool running_{false};

    std::unique_ptr<ReplayClient> cluster_cli_;
    std::unique_ptr<ReplayPlayer> player_;
    std::unique_ptr<rscl::Node> node_;

    PlayRuntimeSetting runtime_setting_;

    std::thread tui_thread_;

    ProgressBar progress_bar_;
};

class ReplayMainController {
 public:
    explicit ReplayMainController(const RecReplayGlobalOption& option);
    void Start();
    void Stop();
    void WaitUntilEnd();

 private:
    bool start_ok_{false};
    std::unique_ptr<ReplayController> controller_impl_;
};
