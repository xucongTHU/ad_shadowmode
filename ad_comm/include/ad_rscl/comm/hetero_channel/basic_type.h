/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cstddef>
#include <string>
#include <memory>
#include <functional>

#include "ad_base/common/shutdownable.h"
#include "ad_rscl/comm/types.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

class Buffer {
 public:
    virtual ~Buffer() = default;
    virtual size_t ByteSize() const = 0;
    virtual const char* Bytes() const = 0;
};

class HeteroPubSubBase : public SwitchableNodeMemberBase,
                         public ::Shutdownable {
 public:
    HeteroPubSubBase() : ::Shutdownable("HeteroPubSubBase") {}

    virtual std::string Type() const = 0;
    virtual bool Close() = 0;
    virtual bool IsEstablished() const = 0;
};

class PublisherBase : public HeteroPubSubBase {
 public:
    virtual ~PublisherBase() = default;
    virtual bool Publish(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg) = 0;

    virtual bool Establish() = 0;

    void ToggleSwitch(const bool enabled) final;
};

class SubscriberBase : public HeteroPubSubBase {
 public:
    using CallbackFunc = std::function<void(const std::shared_ptr<Buffer>&)>;
    explicit SubscriberBase(const CallbackFunc& fn);
    explicit SubscriberBase(CallbackFunc&& fn);
    virtual ~SubscriberBase() = default;
    virtual bool Establish() = 0;

    void ToggleSwitch(const bool enabled) final;

 protected:
    const CallbackFunc fn_;
};

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
