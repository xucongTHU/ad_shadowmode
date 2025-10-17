/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <vector>
#include <thread>
#include <memory>

#include "ad_rscl/ad_rscl.h"
#include "ad_bag/multibag_reader.h"
#include "rsclbag/player/common.h"
#include "rsclbag/player/msg_transform/msg_transform.h"
#include "rsclbag/player/data_source/frame_loader.h"

using senseAD::bag::MultiBagReader;
using senseAD::bag::MultiBagReaderAttribute;

class MessageLoader {
 public:
    MessageLoader(std::shared_ptr<MultiBagReader> bags_reader,
                  const PlayerOptions &options);
    ~MessageLoader();

    void Start();
    void Stop();

    bool ReadyToPlay() const;
    std::shared_ptr<LoadedMessageBuffer> GetLoadedBuffer();
    bool IsPlayEnd() const;

    uint64_t BeginTime() const;
    uint64_t EndTime() const;

 private:
    void LoadMessageUntilEnd();

    std::unique_ptr<FrameLoader> frame_loader_;
    PlayerOptions options_;

    bool stopped_ = true;
    std::shared_ptr<LoadedMessageBuffer> loaded_buffer_;
    std::thread loader_thread_;
    bool is_play_end_ = false;
    bool ready_to_play_ = false;

    std::unique_ptr<MsgTransformer> transformer_;
};
