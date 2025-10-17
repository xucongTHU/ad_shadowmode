/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * fengzhongjie <fengzhongjie@senseauto.com>
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "ad_bag_c/common.h"

typedef struct BagReader BagReader;
typedef struct BagReaderAttribute BagReaderAttribute;
typedef struct BagFlags BagFlags;

typedef struct BagHeader {
    uint64_t begin_time;
    uint64_t end_time;
    int64_t channelinfo_tbl_offset;
    uint32_t channel_count;
    int64_t chunkinfo_tbl_offset;
    int64_t chunk_offset;
    uint32_t chunk_count;
    uint64_t flags;
    uint8_t optional_count;
    uint64_t dropped_count;
    int64_t extra_bag_info_offset;
    uint64_t chunk_compress_ratio_1000;
} BagHeader;

struct BagReaderAttribute {
    uint64_t begin_time;
    uint64_t end_time;
    char** excluded_channels;
    size_t excluded_channels_count;
    char** included_channels;
    size_t included_channels_count;
    // Custom buffer read functions are not supported in C API
    // BagIteratorType iterator_type;
};

struct BagFlags {
    char compress_mode[32];
    char time_mode[128];
    int32_t gmt_offset_sec;
};

BagReaderAttribute* BagReaderAttribute_Create();
void BagReaderAttribute_Destroy(BagReaderAttribute* attr);
void BagReaderAttribute_SetBeginTime(BagReaderAttribute* attr,
                                     uint64_t begin_time);
void BagReaderAttribute_SetEndTime(BagReaderAttribute* attr, uint64_t end_time);
void BagReaderAttribute_AddExcludedChannel(BagReaderAttribute* attr,
                                           const char* channel);
void BagReaderAttribute_AddIncludedChannel(BagReaderAttribute* attr,
                                           const char* channel);

BagReader* BagReader_Create(const char* bag_abspath);
BagReader* BagReader_CreateWithAttribute(const char* bag_abspath,
                                         const BagReaderAttribute* attr);
void BagReader_Destroy(BagReader* reader);

bool BagReader_IsValid(const BagReader* reader);
void BagReader_Reset(BagReader* reader);

uint8_t BagReader_GetBagVersion(const BagReader* reader);
BagFlags BagReader_GetBagFlags(const BagReader* reader);
BagHeader BagReader_GetBagHeader(const BagReader* reader);

uint64_t BagReader_GetMessageNumber(const BagReader* reader,
                                    const char* channel_name);
char* BagReader_GetMessageType(const BagReader* reader,
                               const char* channel_name);
char* BagReader_GetDescriptor(const BagReader* reader,
                              const char* channel_name,
                              size_t* descriptor_size);
char** BagReader_GetChannelList(const BagReader* reader, size_t* count);

bool BagReader_VerifyData(const BagReader* reader,
                          bool* is_bag_valid,
                          bool* need_recover,
                          uint64_t* total_message_size);
bool BagReader_ReadNextMessage(BagReader* reader, ReadedMessage* out);
bool BagReader_MoveReaderHeadTo(BagReader* reader, uint64_t min_msg_timestamp);

#ifdef __cplusplus
}
#endif
