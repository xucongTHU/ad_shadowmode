/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file bag_writer.h
 * @brief Interface for writing data into rsclbag. Unit used here is `Message`
 */

#pragma once

#include <string>
#include <type_traits>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>
#include <shared_mutex>

#include "ad_base/std/optional.h"
#include "ad_bag/bag_flags.h"
#include "ad_bag/io/bag_file_writer.h"

namespace senseAD {
namespace bag {

/**
 * @brief Attribute to control bag writer behaviour
 *
 */
struct BagWriterAttribute {
    // 0 mean don't split by bytes. Otherwise, auto-split bag when message
    // section exceed this size
    size_t bagsplit_bytes = 0;
    // 0 mean don't split by bytes. Otherwise, auto-split bag when message
    // count exceed this size
    size_t bagsplit_msgcnt = 0;

    BagFlags flags;
    uint32_t msg_per_chunk = 512;
};

struct ChannelInfo {
    std::string channel_name;
    // The channel name wrote to bag can be different to the received channel
    // name if a remap setting exist
    std::string write_channel_name;
    // An unique short string to identify the message type
    std::string type;
    // An descriptor for serialization framework to recover message from binary
    // buffer
    std::string descriptor;
};

struct BagWriteExtraInfo {
    std::string platform;
    std::string vehicle;
    std::unordered_map<std::string, std::string> user_defines;

    void ExportToIoFormat(ExtraBagInfo* header_part,
                          std::vector<UserDefineItem>* content_part) const;
    static senseAD::base::optional<BagWriteExtraInfo> FromBagExtraInfoData(
        const BagExtraInfoData& in);
};

/**
 * @brief High level api to write message into rsclbag
 *
 */
class BagWriter {
 public:
    BagWriter(const std::string& out_bag_basename,
              const BagWriterAttribute& attr = BagWriterAttribute());
    ~BagWriter();

    /**
     * @brief Add a serialized message into the bag
     * The actual time of writing to hard disk is not synced
     *
     * @param timestamp Timestamp of the Message
     * @param buf Message Pointer
     * @param buf_len Size of the Message
     * @param info Channel Info
     * @return succes or not
     */
    bool AddSerializedMessage(const uint64_t timestamp,
                              const void* buf,
                              const size_t buf_len,
                              const ChannelInfo& info);
    /*
     * @brief A function to incr the message count when the message is dropped
     */
    void DroppedCountIncr();
    void Close();
    void SetSimTime(bool is_sim_time) { is_sim_time_ = is_sim_time; }
    bool MoveToNextBagSeqFile();

    /**
     * @brief init extra bag info
     * @param data: extra bag content
     *
     */
    bool InitExtraBagInfo(const BagWriteExtraInfo& data);

 private:
    bool NeedMoveToNextSeq(const uint32_t cur_msgcnt,
                           const uint64_t cur_bagsize) const;
    bool CloseCurBagSeqFile();

    std::string BagNameForSeq() const;
    void CleanBagLevelBuffer();

    void AppendToChunk(const uint64_t timestamp,
                       const void* buf,
                       const size_t buf_len,
                       const ChannelInfo& info);
    bool ChunkCompleted() const;
    bool WriteBufferedChunk();
    void CleanChunkLevelBuffer();

    // Global Level
    std::string out_bag_basename_;
    BagWriterAttribute attr_;

    uint32_t cur_seq_ = 0;

    // Bag Level
    uint32_t curbag_msgcnt_ = 0;
    uint64_t curbag_bytes_ = 0;

    std::mutex writer_mtx_;
    std::unique_ptr<BagFileWriter> curbag_writer_;
    // For channel table. Need maintain latest state
    std::unordered_map<std::string, uint32_t> curbag_channel_index_map_;
    std::vector<std::shared_ptr<ChannelInfoItem>> curbag_channel_tbls_;
    // For extra bag info
    std::shared_ptr<ExtraBagInfo> extra_bag_info_ = nullptr;
    std::vector<UserDefineItem> user_define_items_;

    // Chunk level
    std::vector<MessageHeader> message_headers_;
    std::vector<char> chunk_buffer_;
    uint32_t chunk_buffer_size_ = 1 << 25;  // 32MB
    size_t chunk_buffer_used_ = 0;
    bool is_sim_time_{false};
};

}  // namespace bag
}  // namespace senseAD
