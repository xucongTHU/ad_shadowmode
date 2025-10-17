/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once
#include <unistd.h>
#include <capnp/serialize-packed.h>
#include <capnp/message.h>
#include <cstring>
#include <string>
#include "ad_base/time/time.h"

static constexpr size_t kBufferSize = 32;
static constexpr uint64_t kHeaderVersion = 3;
static constexpr uint64_t kHeaderFlag = 0x1234aaaa;

// Layout of Message v1, v2
// | Raw Message | Header |
// Layout of Message v3
// | Raw Message | checksome(optional, 64bit) | Header |
#pragma pack(8)
struct RsclMsgHeader {
    enum FeatureFlag : uint8_t {
        CHECKSOME64 = 1 << 0,
    };

    char unique_node_id[kBufferSize];
    uint64_t stamp{0};
    uint64_t seq{0};
    char pub_node_name[kBufferSize];
    // Is not actually transfered, just used for receive side to check header is
    // usable or not
    bool is_enabled = false;
    uint8_t feature_flag{0};
    //  header_flag and version MUST put at tail
    uint64_t header_flag;
    uint64_t version = kHeaderVersion;
};

static constexpr size_t kHeaderSize = sizeof(RsclMsgHeader);
#pragma pack()

bool ParseHeaderFromBufferTail(const char* source,
                               const uint64_t len,
                               RsclMsgHeader* header);

bool GetHostInfoForHeader(std::string* hostinfo);

struct RsclMsgHeaderOptions {
    bool enable_checksome{false};
};

void FillHeader(RsclMsgHeader* ptr,
                const uint64_t seq,
                const std::string& pub_node_name,
                const RsclMsgHeaderOptions& option,
                void* data_section_ptr,
                const size_t data_section_size);
void FillHeader(RsclMsgHeader* ptr,
                const uint64_t seq,
                const std::string& pub_node_name);

namespace capnp {

size_t computeSerializedSizeInWordsWithHeader(
    kj::ArrayPtr<const kj::ArrayPtr<const word>> segments,
    const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions());

/**
 * @brief Flatten the capnp msgs inner segment into one segment with our Header
 * The header will place at the tail of the segment
 *
 * @param segments
 * @param seq
 * @return kj::Array<word>
 */
kj::Array<word> messageToFlatArrayWithHeader(
    kj::ArrayPtr<const kj::ArrayPtr<const word>> segments,
    uint64_t seq,
    const std::string& pub_node_name,
    const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions());

/**
 * @brief The Same as messageToFlatArrayWithHeader, but will serialize the
 * msg to a target buffer.
 * @param segments
 * @param seq
 * @param target_buffer: The buffer to serialize to
 */
void messageToBufferWithHeader(
    kj::ArrayPtr<const kj::ArrayPtr<const word>> segments,
    uint64_t seq,
    uint8_t* target_buffer,
    const std::string& pub_node_name,
    const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions());

}  // namespace capnp
