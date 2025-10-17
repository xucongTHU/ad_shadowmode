/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file bag_reader.h
 * @brief Interface for reading data from multiple splited rsclbag
 */

#pragma once

#include <vector>
#include <string>
#include <limits>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>
#include <deque>

#include "ad_bag/bag_reader.h"
#include "ad_bag/random_access_helper.h"

namespace senseAD {
namespace bag {

struct MultiBagReaderAttribute {
    // Argument:
    // char*: base address of the message
    // size_t: size of the message
    // ReadedMessage*: Final output struct. The header part is prefilled
    // Return bool: Mean the read failed or this message should be skipped
    using MessageBufferReadFn = BagReaderAttribute::MessageBufferReadFn;

    // Read time range. Unit: ns
    uint64_t begin_time = 0;
    // Read time range. Unit: ns
    uint64_t end_time = std::numeric_limits<uint64_t>::max();
    // Don't read message within those channel
    std::set<std::string> excluded_channels;
    // Only read message within those channel
    std::set<std::string> included_channels;

    // Custom fn for reading message buffer for target topics
    std::unordered_map<std::string, MessageBufferReadFn> custom_buffer_read_fns;

    BagReaderAttribute ToBagReaderAttribute() const;
};

/**
 * @brief Allow Read Multiple Bag at the same time.
 *
 */
class MultiBagReader {
 public:
    MultiBagReader(
        const std::vector<std::string>& bag_abspaths,
        const MultiBagReaderAttribute& attr = MultiBagReaderAttribute());
    MultiBagReader(
        const std::vector<std::shared_ptr<BagReader>>& readers,
        const MultiBagReaderAttribute& attr = MultiBagReaderAttribute());
    ~MultiBagReader();

    /**
     * @brief Whether all bags is a valid bag
     */
    bool IsValid() const;

    /**
     * @brief Get the next message that fit the filter in attr from bags
     * Move current head to next message after read.
     *
     * !IMPORTANT: This will keep searching the bag until find the next valid
     * message or no more possible candinate
     *
     * @param out Message Content
     * @return success or not
     */
    bool ReadNextMessage(ReadedMessage* out);
    bool MoveReaderHeadTo(const uint64_t min_msg_timestamp);

    /**
     * @brief Interrupt the search in ReadNextMessage
     */
    void Interrupt();
    void Reset();

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
     * @return std::vector<std::shared_ptr<ChannelInfoItem>>
     */
    std::vector<std::shared_ptr<ChannelInfoItem>> GetRawChannelInfo(
        const std::string& channel_name) const;
    /**
     * @brief Get the all channel names in bag
     * !IMPORTANT: Not affected by the filter in attribute
     * @return std::set<std::string> channel names
     */
    std::set<std::string> GetChannelList() const;
    std::set<std::string> GetIncludedChannels() const;
    std::set<std::string> GetExcludedChannels() const;
    /**
     * @brief First message time among those bags
     */
    uint64_t BeginTime() const;
    /**
     * @brief Last message time among those bags
     */
    uint64_t EndTime() const;

    std::vector<BagFlags> GetBagFlags() const;

 private:
    void InitInternal(const std::vector<std::shared_ptr<BagReader>>& readers);
    void InitBuffer();

    friend class Iterator;

    MultiBagReaderAttribute attr_;
    std::vector<std::shared_ptr<BagReader>> readers_;

    bool is_valid_ = false;
    bool interrupted_ = false;
    uint64_t bags_begin_time_ = 0;
    uint64_t bags_end_time_ = std::numeric_limits<uint64_t>::max();

    std::set<std::string> all_channels_;
    std::unordered_map<std::string, uint64_t> channel_message_numbers_;
    std::unordered_map<std::string, std::string> channel_message_types_;
    std::unordered_map<std::string, std::string> channel_message_descriptors_;

    uint64_t end_timestamp_ = std::numeric_limits<uint64_t>::max();

    struct BufferItem {
        BagReader* reader_ptr;
        std::shared_ptr<ReadedMessage> msg;
    };
    std::multimap<uint64_t, BufferItem> msg_buffer_;

    std::unique_ptr<MultiBagReaderRandomAccessHelper> random_access_helper_;

    friend class MultiBagReaderRandomAccessHelper;
};

}  // namespace bag
}  // namespace senseAD
