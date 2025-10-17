/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file schema_parse.h
 * @brief Provide function to load schema from buffer
 */
#pragma once

#include <vector>

#include "ad_bag/schema/schema.h"
#include "ad_bag/io/buffer_reader.h"
#include "ad_bag/bag_flags.h"

namespace senseAD {
namespace bag {

using senseAD::bag::BufferReader;

bool ParseSectionInfo(const uint8_t vers,
                      BufferReader* reader,
                      SectionInfo* info);

// @TODO(lichao) If bag_version_ is not equal to current version, need to
// use the old struct to load. Then convert to the new struct

bool ParseSchema(const uint8_t vers,
                 BufferReader* reader,
                 BagHeaderV2* out,
                 bool with_section_info = true);
bool ParseSchema(const uint8_t vers,
                 BufferReader* reader,
                 BagHeaderV3* out,
                 bool with_section_info = true);
bool ParseChunkData(const uint8_t,
                    BufferReader* reader,
                    std::vector<char>* out);
bool ParseChunkData(const uint8_t,
                    BufferReader* reader,
                    size_t size,
                    std::vector<char>* out,
                    bool with_section_info = true);

bool ParseSchema(const uint8_t vers,
                 BufferReader* reader,
                 ChunkHeader* out,
                 bool with_section_info = true);
bool ParseSchema(const uint8_t vers,
                 BufferReader* reader,
                 MessageHeader* out,
                 bool with_section_info = true);
bool ParseSchema(const uint8_t vers,
                 BufferReader* reader,
                 ChunkInfoTableIndex* out,
                 bool with_section_info = true);

bool ParseSchema(const uint8_t vers,
                 BufferReader* reader,
                 ChannelInfoItem* out,
                 bool with_section_info = true);

bool ParseSchema(const uint8_t,
                 BufferReader* reader,
                 ChannelInfoTableIndex* out,
                 bool with_section_info = true);

bool ParseSchema(const uint8_t,
                 BufferReader* reader,
                 ExtraBagInfo* out,
                 bool with_section_info = true);

bool ParseUserDefineItem(const uint8_t,
                         BufferReader* reader,
                         UserDefineItem* out,
                         bool with_section_info = true);

bool DecompressWith(senseAD::bag::CompressionMode mode,
                    const uint64_t compressed_size,
                    const uint64_t expected_decompressed_size,
                    const char* compresssed_message_buffers,
                    std::vector<char>* decompresssed_message_buffers);
void DecompressWithLz4(const uint64_t compressed_size,
                       const uint64_t expected_decompressed_size,
                       const char* compresssed_message_buffers,
                       std::vector<char>* decompresssed_message_buffers);
void DecompressWithZstd(const uint64_t compressed_size,
                        const uint64_t expected_decompressed_size,
                        const char* compresssed_message_buffers,
                        std::vector<char>* decompresssed_message_buffers);
}  // namespace bag
}  // namespace senseAD
