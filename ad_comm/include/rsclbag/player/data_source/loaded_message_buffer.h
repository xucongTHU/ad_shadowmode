/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <memory>
#include <deque>
#include <string>
#include <mutex>

#include "ad_rscl/ad_rscl.h"
#include "rsclbag/player/data_source/frame_loader.h"

class LoadedMessageBuffer {
 public:
    LoadedMessageBuffer() = default;
    ~LoadedMessageBuffer() = default;

    size_t Size() const;
    bool Empty() const;

    void PushBack(const std::shared_ptr<LoadedMessage>& msg);
    void PopFront();
    std::shared_ptr<LoadedMessage> Front() const;
    std::shared_ptr<LoadedMessage> Back() const;
    int64_t InBufferTimeRange() const;
    uint64_t GetFrontSeq() const;

 private:
    std::deque<std::shared_ptr<LoadedMessage>> tasks_;
    mutable std::mutex mtx_;
};
