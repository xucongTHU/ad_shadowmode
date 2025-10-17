/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <atomic>
#include <string>
#include <exception>
namespace senseAD {
namespace base {
namespace common {

class TrackTag {
 public:
    TrackTag(const std::string& track_class, const std::string& tag_detail);
    ~TrackTag();

    void TTLog(const char* behaviour) const;
    void UpdateTTLogDetail(const std::string& tag_detail);

    std::string GetTTLogPrefix() const;

 private:
    static std::atomic<uint64_t> g_uid;
    uint64_t uid_{0UL};
    std::string track_class_{};
    std::string log_prefix_{};
};

/**
 * @brief
 *
 */
class Shutdownable : public TrackTag {
 public:
    Shutdownable(const std::string& track_class,
                 const std::string& tag_detail = "");
    virtual ~Shutdownable();

    bool Shutdown();
    inline bool IsShutdown() const { return is_shutdown_.load(); }

 protected:
    virtual void ShutdownImpl();

 private:
    std::atomic<bool> is_shutdown_;
};

class NoShutdownImplException : public std::exception {
 public:
    explicit NoShutdownImplException(std::string const& message)
        : msg_(message) {}
    virtual char const* what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

template <typename T>
void ShutdownIfInstanceExist() {
    auto instance = T::Instance(false);
    if (instance) {
        instance->Shutdown();
    }
}

}  // namespace common
}  // namespace base
}  // namespace senseAD

using Shutdownable = senseAD::base::common::Shutdownable;
