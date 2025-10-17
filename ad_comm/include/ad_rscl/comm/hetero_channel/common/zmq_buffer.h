/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#define ZMQ_HAS_RVALUE_REFS
#include "cppzmq/zmq.hpp"

#include "ad_rscl/comm/hetero_channel/basic_type.h"
#include "ad_base/common/macros.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

class ZmqBuffer : public Buffer {
 public:
    ZmqBuffer() = default;
    explicit ZmqBuffer(zmq::message_t&& msg);

    ~ZmqBuffer() override = default;
    size_t ByteSize() const override;
    const char* Bytes() const override;

 private:
    zmq::message_t inner_msg_;

    NON_COPYABLE_ASSIGNABLE(ZmqBuffer)
};
}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
