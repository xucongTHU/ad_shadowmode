/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file schema.h
 * @brief Define the bag scheme here.
 * Specific the bytes size for each field and how the data is organized here
 *
 * !IMPORTANT: Don't put anything that isn't schema into this file
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace senseAD {

namespace bag {

// rsclbag version
// 1: the original version, normal rsclbag
// 2: ****
// 3: add extra bag info, locate after Header info
const uint8_t kCurVersion = 3;

#pragma pack(push, 1)

/**
 * @brief An enum used for specific the following section type
 *
 */
enum class SectionIdentifier : uint8_t {
    kBagHeader = 0,

    kChunkData = 1,

    kChunkHeader = 2,
    kMessageHeader = 3,

    kChunkInfoTableIndex = 4,

    kChannelInfoItem = 5,
    kChannelInfoTableIndex = 6,

    kExtraBagInfo = 7,

    kUserDefineItem = 8
};  // namespace bag

/**
 * @brief meta info the following section
 *
 */
struct SectionInfo {
    SectionIdentifier identifier;
    uint64_t size = 0;
};

/**
 * @brief The overall header for the current bag
 *
 */
struct BagHeaderV3 {
    /**
     * @brief Begin time of the bag message
     *
     */
    uint64_t begin_time;
    /**
     * @brief Begin time of the bag message
     *
     */
    uint64_t end_time;
    /**
     * @brief Position of the channel info table
     */
    int64_t channelinfo_tbl_offset;
    /**
     * @brief Number of channel in this bag
     */
    uint32_t channel_count;

    /**
     * @brief Position of the chunk info table
     */
    int64_t chunkinfo_tbl_offset;
    /**
     * @brief Position of the first chunk
     */
    int64_t chunk_offset;
    /**
     * @brief Number of chunk in this bag
     */
    uint32_t chunk_count = 0;

    /**
     * @brief Flag to indicate global modes.
     * For example: is it compressed? compressed mode? etc
     */
    uint64_t flags = 0;

    /**
     * @brief Number of bytes used for the optional part
     */
    uint8_t optional_count = 0;

    uint64_t dropped_count = 0;

    //! V3 prams
    /**
     * @brief V3 params: Position of the extra bag info
     * 0 mean invalid
     */
    int64_t extra_bag_info_offset;
    /**
     * @brief V3 params: (uncompression_size / compression_size) * 1000
     * 0 mean invalid
     */
    uint64_t chunk_compress_ratio_1000 = 1000;
};

/**
 * @brief One chunk header per chunk. One chunk contain multiple message
 *
 */
struct ChunkHeader {
    /**
     * @brief First timestamp in this chunk
     */
    uint64_t start_time;
    /**
     * @brief Last timestamp in this chunk
     */
    uint64_t end_time;

    /**
     * @brief Offset to the message header table
     */
    uint64_t message_header_tbl_offset;
    /**
     * @brief Offset to the message data section
     */
    uint64_t message_data_section_offset;
    /**
     * @brief Number of message in this chunk
     */
    uint32_t message_count;
    /**
     * !IMPORTANT: introduced in version 2
     * @brief Size of message data in this chunk before compress
     */
    uint64_t message_ori_size;
};

/**
 * @brief One message header per message data
 *
 */
struct MessageHeader {
    inline std::string Key() const {
        return std::to_string(channel_uid) + "_" + std::to_string(timestamp);
    }
    /**
     * @brief Uid of the channel
     */
    uint32_t channel_uid;
    /**
     * @brief Size of this message data
     */
    uint32_t message_size;
    /**
     * @brief timestamp
     */
    uint64_t timestamp;
};

/**
 * @brief An index item at the chunk info index table.
 * Will generate when the bag is going to close
 *
 */
struct ChunkInfoTableIndex {
    /**
     * @brief Offset of the cooresponding chuck header
     */
    int64_t chunk_header_offset;
    /**
     * @brief First message time
     */
    uint64_t chunk_start_time;
};

#pragma pack(pop)

// Below struct is not POD, need manually handle the loading/saving
struct InBagString {
    InBagString() = default;
    InBagString(InBagString&& other);
    InBagString(const InBagString& other) = delete;
    InBagString& operator=(InBagString&& other);
    InBagString& operator=(const InBagString& other) = delete;
    ~InBagString();
    std::string ToString() const;
    inline size_t ByteSize() const { return content_len + sizeof(content_len); }

    uint32_t content_len{0UL};
    char* content{nullptr};
};

/**
 * @brief Information for a channel
 *
 */
struct ChannelInfoItem {
    /**
     * @brief Channel name
     */
    InBagString channel_name;
    /**
     * @brief A Shorten Name
     */
    InBagString type;
    /**
     * @brief IDL descriptor
     */
    InBagString descriptor;
    /**
     * @brief UID
     */
    uint32_t uid;
    /**
     * @brief Total number of message of this channel in this bag
     */
    uint32_t total_message;
    /**
     * @brief First message time offset of this channel in this bag
     */
    uint64_t first_message_time;
    /**
     * @brief Last message time offset of this channel in this bag
     */
    uint64_t last_message_time;

    inline size_t ByteSize() const {
        return channel_name.ByteSize() + type.ByteSize() +
               descriptor.ByteSize() + sizeof(uid) + sizeof(total_message) +
               sizeof(first_message_time) + sizeof(last_message_time);
    }
};

/**
 * @brief An index item at the channel info index table.
 * Will generate when the bag is going to close
 *
 */
struct ChannelInfoTableIndex {
    /**
     * @brief Offset of the cooresponding channel header
     */
    uint64_t channel_info_item_offset;
};

/**
 * @brief extra info for bag, like platform or vehicle etc..
 *
 */
struct ExtraBagInfo {
    /**
     * @brief platform of rsclbag, like mdc, had etc
     */
    char platform[128];
    /**
     * @brief the vehicle for rsclbag record
     */
    char vehicle[128];

    /**
     * @brief Offset to the message data section
     */
    uint64_t user_define_offset = 0;
    /**
     * @brief user define item count
     */
    uint32_t user_define_count = 0;
};

struct UserDefineItem {
    UserDefineItem() = default;
    UserDefineItem(UserDefineItem&& other) = default;
    UserDefineItem(const UserDefineItem& other) = delete;
    UserDefineItem& operator=(UserDefineItem&& other) = default;
    UserDefineItem& operator=(const UserDefineItem& other) = delete;

    InBagString key;
    InBagString value;
    inline size_t ByteSize() const { return key.ByteSize() + value.ByteSize(); }
};

/**
 * @brief Magic number to indicate bag end
 *
 */
constexpr uint32_t kMagicEndNumer = 0x1234;

// alias for BagHeader
using BagHeader = BagHeaderV3;

/******************************************
 *
 *    Below Code Keep for Compatiable Only
 *
 *******************************************
 */
#pragma pack(push, 1)

struct BagHeaderV2 {
    /**
     * @brief Begin time of the bag message
     *
     */
    uint64_t begin_time;
    /**
     * @brief Begin time of the bag message
     *
     */
    uint64_t end_time;
    /**
     * @brief Position of the channel info table
     */
    int64_t channelinfo_tbl_offset;
    /**
     * @brief Number of channel in this bag
     */
    uint32_t channel_count;

    /**
     * @brief Position of the chunk info table
     */
    int64_t chunkinfo_tbl_offset;
    /**
     * @brief Position of the first chunk
     */
    int64_t chunk_offset;
    /**
     * @brief Number of chunk in this bag
     */
    uint32_t chunk_count = 0;

    /**
     * @brief Flag to indicate global modes.
     * For example: is it compressed? compressed mode? etc
     */
    uint64_t flags = 0;

    /**
     * @brief Number of bytes used for the optional part
     */
    uint8_t optional_count = 0;

    uint64_t dropped_count = 0;

    BagHeaderV3 ToLatestVersion() const;
};

#pragma pack(pop)

}  // namespace bag
}  // namespace senseAD
