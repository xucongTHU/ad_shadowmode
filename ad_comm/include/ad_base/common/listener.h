/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <string>

#include "ad_base/common/shutdownable.h"

namespace senseAD {
namespace base {
namespace common {

struct ListenerHandle : public ::Shutdownable {
    using Ptr = std::shared_ptr<ListenerHandle>;

    explicit ListenerHandle(std::function<void()> listener_unregister_fn);
    ListenerHandle(std::function<void()> listener_unregister_fn,
                   const std::string &debug_tag);
    virtual ~ListenerHandle();
    void ShutdownImpl() override;

    std::function<void()> unregister_fn;
};

}  // namespace common
}  // namespace base
}  // namespace senseAD
