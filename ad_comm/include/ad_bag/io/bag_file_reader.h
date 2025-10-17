/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file bag_file_reader.h
 * @brief Reading the data into the rsclbag file.
 * Handle the header parsing, and the actual data format in binary file
 */
#pragma once

#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <string>
#include <memory>
#include <limits>

#include "ad_bag/schema/schema.h"
#include "ad_bag/bag_flags.h"
#include "ad_bag/io/common_struct.h"

namespace senseAD {
namespace bag {

struct ChunkAccessFilter {
    // Only read the message within those channel
    std::set<std::string> included_channels;
};

/**
 * @brief Attribute used to control bag file reader behaviour
 * All time unit is ns
 */
struct BagFileReaderAttribute {
    uint64_t begin_time = 0;
    uint64_t end_time = std::numeric_limits<uint64_t>::max();

    //! Those filter can only filter chunk level. Cannot filter message level
    ChunkAccessFilter chunk_filter;
};

/**
 * @brief Class for reading raw data format from single bag file
 * with low-level api
 *
 */
class BagFileReader {
 public:
    BagFileReader(const std::string &in_bag,
                  const BagFileReaderAttribute &attr);
    ~BagFileReader();

    /**
     * @brief Whether the input bag is a valid bag
     */
    bool IsValid() const;
    inline uint8_t GetBagVersion() const { return bag_version_; }

    const BagFlags &GetBagFlags() const;
    const BagHeader &GetBagHeader() const;
    const std::shared_ptr<BagExtraInfoData> &GetBagExtraInfo() const;
    const std::vector<ChunkInfoTableIndex> &GetChunkInfoTableIndex() const;
    const std::vector<ChunkHeader> &GetChunkHeader();
    size_t GetCurrentChunkIndex() const;
    bool MoveCurrentChunkIndexTo(const size_t index);
    const std::unordered_map<std::string, std::shared_ptr<ChannelInfoItem>>
        &GetChannelInfoItem() const;

    /**
     * @brief Read the next chunk sequentially. Will move to next index after
     * read current chunk.
     * Will throw exception if meet problem at read
     *
     * @param chunk_header Return the chunk header for current chunk
     * @param message_headers Return the message header(s) for current chunk
     * @param message_buffers Return the message data sections
     * @return whether already read to end
     */
    bool ReadNextChunk(ChunkHeader *chunk_header,
                       std::vector<MessageHeader> *message_headers,
                       std::vector<char> *message_buffers);
    bool ReadNextChunkHeaders(ChunkHeader *chunk_header,
                              std::vector<MessageHeader> *message_headers);

    bool ReadChunk(const ChunkHeader &chunk_header,
                   std::vector<char> *message_buffers);

 private:
    /**
     * @brief Read bag headers, chunk info tables, channel info tables
     */
    bool ReadHeaderOnce();
    /**
     * @brief Move the current index to next chunk without read it content
     */
    bool StepNextChunk();

    void InitFilter();

    bool ReadChunkHeadersAtFileOffset(const size_t &offset,
                                      ChunkHeader *chunk_header);
    bool ReadChunkHeadersAtFileOffset(
        const size_t &offset,
        ChunkHeader *chunk_header,
        std::vector<MessageHeader> *message_headers);
    bool ReadMessageHeaders(const ChunkHeader &chunk_header,
                            std::vector<MessageHeader> *message_headers);
    bool CheckChunkUsable(
        const std::vector<MessageHeader> &message_headers) const;

    BagFileReaderAttribute attr_;
    FILE *fptr_;
    size_t file_size_;

    uint8_t bag_version_;
    BagHeader bag_header_;
    BagFlags bag_flags_;
    std::shared_ptr<BagExtraInfoData> bag_extra_info_ = nullptr;
    std::vector<ChunkInfoTableIndex> chunkinfo_tbls_;
    bool chunk_headers_inited_{false};
    std::vector<ChunkHeader> chunk_headers_;
    std::unordered_map<std::string, std::shared_ptr<ChannelInfoItem>>
        channelinfo_tbls_;
    size_t cur_chunk_index_ = 0;
    std::vector<char> compressed_tmp_buffer_;

    bool is_valid_ = false;

    // if not empty, skip the chunk that disjoint to this set
    std::unordered_set<uint32_t> chunk_filter_channel_uids_;
};
}  // namespace bag
}  // namespace senseAD
