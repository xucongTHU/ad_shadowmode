//
// Created by xucong on 25-4-25.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#pragma once

#include <sys/inotify.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <cerrno>

#include "StrategyParser.h"
#include "StrategyConfig.h"
#include "common/logger/Logger.h"

namespace shadow {
namespace strategy {
static const char* LOG_TAG = "Strategy";

class ConfigReloader {
public:
    using Callback = std::function<void(const StrategyConfig&)>;

    explicit ConfigReloader(const std::string& path)
        : file_path_(path), inotify_fd_(-1), watch_fd_(-1), running_(false) {}

    ~ConfigReloader() {
        Stop();
    }

    bool Init() {
        parser_ = std::make_shared<StrategyConfigParser>();
        if (!ReloadConfig()) {
            LOG_ERROR("Initial config load failed: %s", file_path_.c_str());
            return false;
        }

        inotify_fd_ = inotify_init1(IN_NONBLOCK);
        if (inotify_fd_ < 0) {
            perror("inotify_init1 failed");
            return false;
        }

        AddWatch();

        running_ = true;
        watcher_thread_ = std::thread([this]() { WatchLoop(); });

        return true;
    }

    void Stop() {
        running_ = false;
        if (watcher_thread_.joinable()) {
            watcher_thread_.join();
        }

        if (watch_fd_ >= 0) {
            inotify_rm_watch(inotify_fd_, watch_fd_);
            watch_fd_ = -1;
        }
        if (inotify_fd_ >= 0) {
            close(inotify_fd_);
            inotify_fd_ = -1;
        }
    }

    StrategyConfig GetConfig() {
        std::lock_guard<std::mutex> lock(mutex_);
        return strategy_config_;
    }

    void SetCallback(Callback cb) {
        std::lock_guard<std::mutex> lock(mutex_);
        callback_ = std::move(cb);
    }

private:
    void AddWatch() {
        watch_fd_ = inotify_add_watch(
            inotify_fd_,
            file_path_.c_str(),
            IN_MODIFY | IN_CLOSE_WRITE | IN_MOVED_TO | IN_DELETE_SELF | IN_MOVE_SELF
        );
        if (watch_fd_ < 0) {
            perror("inotify_add_watch failed");
        }
    }

    bool ReloadConfig() {
        StrategyConfig new_cfg;
        if (!parser_->LoadConfigFromFile(file_path_, new_cfg)) {
            LOG_ERROR("Failed to load config file: %s", file_path_.c_str());
            return false;
        }

        {
            std::lock_guard<std::mutex> lock(mutex_);
            strategy_config_ = std::move(new_cfg);
            if (callback_) {
                callback_(strategy_config_);
            }
        }

        LOG_INFO("Config loaded/reloaded successfully: %s", file_path_.c_str());
        return true;
    }

    void WatchLoop() {
        constexpr size_t buf_size = 1024 * (sizeof(struct inotify_event) + 16);
        char buffer[buf_size];

        while (running_) {
            int length = read(inotify_fd_, buffer, buf_size);
            if (length < 0) {
                if (errno == EAGAIN || errno == EINTR) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                } else {
                    perror("read inotify_fd_ failed");
                    break;
                }
            }

            int i = 0;
            while (i < length) {
                auto* event = reinterpret_cast<struct inotify_event*>(&buffer[i]);

                if (event->mask & (IN_MODIFY | IN_CLOSE_WRITE | IN_MOVED_TO)) {
                    LOG_INFO("Config file modified, reloading...");
                    ReloadConfig();
                }

                if (event->mask & (IN_DELETE_SELF | IN_MOVE_SELF)) {
                    LOG_INFO("Config file replaced, re-adding watch and reloading...");
                    if (watch_fd_ >= 0) {
                        inotify_rm_watch(inotify_fd_, watch_fd_);
                        watch_fd_ = -1;
                    }
                    AddWatch();
                    ReloadConfig();
                }

                i += sizeof(struct inotify_event) + event->len;
            }
        }
    }

private:
    std::string file_path_;
    std::shared_ptr<StrategyConfigParser> parser_;
    StrategyConfig strategy_config_;

    std::mutex mutex_;
    std::thread watcher_thread_;
    std::atomic<bool> running_;

    int inotify_fd_;
    int watch_fd_;
    Callback callback_;
};

} // namespace strategy
} // namespace shadow