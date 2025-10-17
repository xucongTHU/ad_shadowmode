/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <exception>
#include <string>
#include <memory>
#include <vector>

#include "rsclbag/player/msg_transform/interface.h"
#include "rsclbag/player/data_source/frame_loader.h"
#include "ad_base/class_loader/class_loader_manager.h"

enum class MsgTransformMode { MSG_TIME_REWRITE_FOR_LOOP };

std::string DefaultModeSharedLibraryPath(const MsgTransformMode& mode);

class MsgTransformLogicLoadingException : public std::exception {
 public:
    explicit MsgTransformLogicLoadingException(std::string const& message)
        : msg_(message) {}
    virtual char const* what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class MsgTransformer {
 public:
    explicit MsgTransformer(const std::string& impl_so_path);
    ~MsgTransformer();
    void Process(const uint64_t& expected_play_timestamp_ns,
                 const senseAD::base::optional<RsclMsgHeader>& rscl_header,
                 LoadedMessage* msg);
    bool ProcessData(const std::string& msg_channel,
                     const uint64_t& arrived_timestamp_ns,
                     const senseAD::base::optional<RsclMsgHeader>& rscl_header,
                     const char* msg_buffer,
                     const size_t msg_size,
                     uint64_t* out_timestamp,
                     std::vector<char>* out_msg_buffer_with_header,
                     bool* msg_changed);

 private:
    std::vector<char> buffer_;
    senseAD::base::class_loader::ClassLoaderManager loader_mgr_;
    std::shared_ptr<MsgTransformLogic> loaded_logic_;
};
