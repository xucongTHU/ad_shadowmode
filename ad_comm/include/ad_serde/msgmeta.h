/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <mutex>
#include <unordered_map>

#include "ad_serde/wrapped_type.h"

namespace senseAD {

namespace serde {

/**
 * @brief Contain all required meta info for message reflection
 *
 */
struct MsgMeta {
    std::string msg_type;
    std::string msg_descriptor;

    template <typename T>
    static MsgMeta From() {
        return MsgMeta{
            senseAD::serde::MessageTypeMeta<T>::GetMessageTypename(),
            senseAD::serde::MessageTypeMeta<T>::GetMessageDescriptor()};
    }
    bool IsEmpty() const { return msg_type.empty(); }
};

}  // namespace serde
}  // namespace senseAD
