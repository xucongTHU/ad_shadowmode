/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file bag_file_writer.h
 * @brief Writing the data into the rsclbag file.
 * A low-level interface without validation and handle the inter-relationship
 * Handle the header creation, and the actual data format in binary format
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>

#include "ad_bag/schema/schema.h"
#include "ad_bag/bag_flags.h"
#include "ad_bag/io/common_struct.h"

namespace senseAD {
namespace bag {

/**
 * @brief Attribute used to control bag file writer behaviour
 * Buffer unit is bytes
 */
struct BagFileWriterAttribute {
    BagFlags flags;
    size_t chunk_write_buffer_size = 1 << 24;
};

/**
 * @brief Class for writing raw data format into single bag file
 * with low-level api
 *
 */
class BagFileWriter {
 public:
    BagFileWriter(const std::string& out_bag,
                  const BagFileWriterAttribute attr);
    ~BagFileWriter();

    // @TODO(lichao) Support write message buffer and its header in two step
    /**
     * @brief Write chunk data in to bag
     *! Assumed the message_headers is sorted by timestamp
     *
     * @param message_headers
     * @param channel_tbls
     * @param message_buffers
     * @param message_buffers_size
     * @param force_sync
     * @param out_bag_size
     * @return true
     * @return false
     */
    bool WriteChunk(
        const std::vector<MessageHeader>& message_headers,
        const std::vector<std::shared_ptr<ChannelInfoItem>>& channel_tbls,
        const char* message_buffers,
        const size_t message_buffers_size,
        bool force_sync = false,
        uint64_t* out_bag_size = nullptr);
    /**
     * @brief Ensure the chunk data in memory is flush into disk
     *
     */
    void FlushChunk();

    bool CloseBag(
        const std::vector<std::shared_ptr<ChannelInfoItem>>& channel_tbls,
        const ExtraBagInfo& extra_bag_info,
        const std::vector<UserDefineItem>& user_defines);

    void DroppedMessageCountInc();

 private:
    bool WriteChannelInfoItem(const ChannelInfoItem& item);
    bool WriteBagHeader(const BagHeader& bag_headers);
    bool WriteExtraBagInfo(uint64_t offset,
                           const ExtraBagInfo& extra_bag_info,
                           const std::vector<UserDefineItem>& user_defines);
    bool WriteEndAndCloseFile();

    BagFileWriterAttribute attr_;
    std::string active_bag_name_;
    std::string final_bag_name_;
    FILE* fptr_;

    // For global header
    BagHeader bag_header_;

    // For message data section
    size_t used_buffer_size_ = 0;
    std::vector<char> chunk_write_buffer_;

    uint64_t chunk_uncompress_size_{0UL};
    uint64_t chunk_compressed_size_{0UL};

    // For Info Table
    std::vector<ChunkInfoTableIndex> chunk_tbls_;
    std::vector<uint64_t> channel_info_offsets_;

    bool closed_ = false;
    std::vector<char> compress_bag_data_;
};

}  // namespace bag
}  // namespace senseAD
