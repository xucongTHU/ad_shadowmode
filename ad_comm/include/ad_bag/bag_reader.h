/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file bag_reader.h
 * @brief Interface for reading data from rsclbag. Unit used here is `Message`
 */

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <set>
#include <vector>
#include <limits>
#include <functional>
#include <queue>
#include <utility>

#include "ad_bag/io/bag_file_reader.h"
#include "ad_bag/schema/schema.h"
#include "ad_bag/bag_flags.h"
#include "ad_bag/random_access_helper.h"

namespace senseAD {
namespace bag {

/**
 * @brief An untyped Message from bag reader
 */
struct ReadedMessage {
    // When was the message recorded .Unit: ns
    uint64_t timestamp;
    std::string channel_name;
    // Serialized data of the message
    std::vector<char> message_buffer;

    inline void swap(ReadedMessage& other) {  // NOLINT
        std::swap(timestamp, other.timestamp);
        std::swap(channel_name, other.channel_name);
        std::swap(message_buffer, other.message_buffer);
    }
};

struct ReadedMessageGreater {
    bool operator()(const ReadedMessage& a, const ReadedMessage& b) {
        return a.timestamp > b.timestamp;
    }
};

struct ReadedMessageLess {
    bool operator()(const ReadedMessage& a, const ReadedMessage& b) {
        return a.timestamp < b.timestamp;
    }
};

// Enumeration for the types of iterators in the bag
enum class BagIteratorType {
    READ_NEXT_BY_SEQUENCE,        // Iterates over the bag by sequence number
    READ_NEXT_BY_TIMESTAMP_ORDER  // Iterates over the bag by timestamp order
};

/**
 * @brief Attribute for bag reader to control reading beheviour
 *
 */
struct BagReaderAttribute {
    // Argument:
    // char*: base address of the message
    // size_t: size of the message
    // ReadedMessage*: Final output struct. The header part is prefilled
    // Return bool: Mean the read failed or this message should be skipped
    using MessageBufferReadFn =
        std::function<bool(const char*, size_t, ReadedMessage*)>;

    // Read Time Range. Unit: ns
    uint64_t begin_time = 0;
    // Read Time Range. Unit: ns
    uint64_t end_time = std::numeric_limits<uint64_t>::max();

    // Don't read the message within those channel
    std::set<std::string> excluded_channels;
    // Only read the message within those channel
    std::set<std::string> included_channels;

    // Custom fn for reading message buffer for target topics
    std::unordered_map<std::string, MessageBufferReadFn> custom_buffer_read_fns;

    // The type of iterator
    BagIteratorType iterator_type{
        senseAD::bag::BagIteratorType::READ_NEXT_BY_SEQUENCE};
};

struct VerificationResult {
    bool is_bag_valid;
    bool need_recover;
    uint64_t total_message_size = 0;
};

/**
 * @brief Reader to read message from rsclbag with high-level api
 *
 */
class BagReader {
 public:
    BagReader(const std::string& bag_abspath,
              const BagReaderAttribute& attr = BagReaderAttribute());
    ~BagReader() = default;

    /**
     * @brief Whether this bag is a valid bag
     */
    bool IsValid() const;

    void Reset();

    /**
     * @brief Get the version of bag
     *
     * @return bag version
     */
    uint8_t GetBagVersion() const;
    const BagFlags& GetBagFlags() const;
    const BagHeader& GetBagHeader() const;
    const std::shared_ptr<BagExtraInfoData>& GetBagExtraInfo() const;

    /**
     * @brief Get the Number of Message for specific channel
     * !IMPORTANT: Not affected by the filter in attribute
     * @param channel_name Channel Name
     * @return uint64_t Number of Message
     */
    uint64_t GetMessageNumber(const std::string& channel_name) const;
    /**
     * @brief Get the Message Type for specific channel
     *
     * @param channel_name Channel Name
     * @return std::string message type
     */
    std::string GetMessageType(const std::string& channel_name) const;
    /**
     * @brief Get the Message Descriptor for the channel
     *
     * @param channel_name Channel Name
     * @return std::string Message Descriptor
     */
    std::string GetDescriptor(const std::string& channel_name) const;
    /**
     * @brief Get the Raw Channel Info which is directly stored in bag
     *
     * @param channel_name Channel Name
     * @return std::shared_ptr<ChannelInfoItem>
     */
    std::shared_ptr<ChannelInfoItem> GetRawChannelInfo(
        const std::string& channel_name) const;
    /**
     * @brief Get the all channel names in bag
     * !IMPORTANT: Not affected by the filter in attribute
     * @return std::set<std::string> channel names
     */
    std::set<std::string> GetChannelList() const;
    std::set<std::string> GetIncludedChannels() const;
    std::set<std::string> GetExcludedChannels() const;

    inline const std::string& BagAbspath() const { return bag_abspath_; }

    /**
     * VerifyData function.
     *
     * This function verifies the data and returns the result.
     *
     * @return The result of the verification process.
     * need_recover: Whether the messages are ordered
     * is_bag_valid: Whether the bag is valid
     */
    VerificationResult VerifyData();
    /**
     * @brief Get the next message that fit the filter in attr from bags.
     * Move current head to next message after read
     *
     * @param out Message
     * @return Able to read?
     */
    bool ReadNextMessage(ReadedMessage* out);
    bool MoveReaderHeadTo(const uint64_t min_msg_timestamp);

    class BagIterator {
     public:
        explicit BagIterator(const BagReader* bag_reader,
                             const BagIteratorType type);

        bool PreviewNextMessageHeader(ReadedMessage*);
        bool ReadNextMessage(ReadedMessage*);

        /**
         * @brief Skip the next message without reading
         *
         * @return Able to skip?
         */
        bool SkipMessage();

     private:
        BagIterator() = delete;
        BagIterator operator=(const BagIterator& other) = delete;

        BagReader* bag_reader_;
        const BagIteratorType type_;
    };

    BagIterator& GetIterator();

 private:
    /**
     * @brief Read the message header without read the content
     *
     * @param out Only content the header data
     * @return Able to read new message?
     */
    bool PreviewNextMessageHeader(ReadedMessage* out);

    bool EnsureNextChunkDataLoaded();
    bool LoadNextChunkData();
    /**
     * @brief Skip the next message without reading
     *
     * @return Able to skip?
     */
    bool SkipMessage();
    void SkipMessageInner(const MessageHeader& header);

    int IsCurMessageFitFilter() const;

    //* ====== Function for READ_NEXT_BY_TIMESTAMP_ORDER mode ======
    /**
     * @brief Preview the header of the next message based on its timestamp.
     *
     * @param out Only content the header data
     * @return Able to read new message?
     */
    bool PreviewNextMessageHeaderByTimestamp(ReadedMessage* out);
    /**
     * Retrieves the next message from the buffer. Differ from the function
     * ReadNextMessage, this function gets the next ordered message (by
     * timestamp) from the priority queue.
     *
     * @param out Message
     * @return Able to read?
     */
    bool ReadNextMessageByTimestamp(ReadedMessage* out);
    /**
     * @brief Preloads data into memory, and sort by timestamp.
     *
     * Priority queue size is limited to 3
     * chunks (BagWriterAttribute.msg_per_chunk).
     */
    bool PreLoadBag();
    void ClearPreLoad();
    bool SkipMessageByTimestamp();

    void UserChannelsToActualChannels();
    //* ============================================================

    std::string bag_abspath_;
    BagReaderAttribute attr_;
    std::unique_ptr<BagFileReader> bag_file_reader_;
    std::unordered_map<uint32_t, std::string> uid_channelinfo_map_;

    struct CurChunkData {
        ChunkHeader header;
        std::vector<MessageHeader> message_headers;
        std::vector<char> message_buffers;
        int readed_index = -1;
        uint64_t readed_message_size = 0;
    };
    CurChunkData cur_chunk_data_;

    bool has_preload_data_ = false;
    // Message buffer: order by timestamp
    std::priority_queue<ReadedMessage,
                        std::vector<ReadedMessage>,
                        ReadedMessageGreater>
        msg_buffer_;
    uint64_t last_played_msg_timestamp_ = 0;

    BagIterator bag_iter_;
    std::unique_ptr<BagReaderRandomAccessHelper> random_access_helper_;

    friend class BagReaderRandomAccessHelper;
};

}  // namespace bag
}  // namespace senseAD
