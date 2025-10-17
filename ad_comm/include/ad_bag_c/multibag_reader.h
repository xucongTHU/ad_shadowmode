/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * fengzhongjie <fengzhongjie@senseauto.com>
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "ad_bag_c/common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MultiBagReaderAttribute {
    uint64_t begin_time;
    uint64_t end_time;
    char** excluded_channels;
    size_t excluded_channels_count;
    char** included_channels;
    size_t included_channels_count;
} MultiBagReaderAttribute;

typedef struct MultiBagReader MultiBagReader;

MultiBagReaderAttribute* MultiBagReaderAttribute_Create();
void MultiBagReaderAttribute_Destroy(MultiBagReaderAttribute* attr);
void MultiBagReaderAttribute_SetBeginTime(MultiBagReaderAttribute* attr,
                                          uint64_t begin_time);
void MultiBagReaderAttribute_SetEndTime(MultiBagReaderAttribute* attr,
                                        uint64_t end_time);
void MultiBagReaderAttribute_AddExcludedChannel(MultiBagReaderAttribute* attr,
                                                const char* channel);
void MultiBagReaderAttribute_AddIncludedChannel(MultiBagReaderAttribute* attr,
                                                const char* channel);

MultiBagReader* MultiBagReader_Create(const char** bag_abspaths,
                                      size_t bag_count);
MultiBagReader* MultiBagReader_CreateWithAttribute(
    const char** bag_abspaths,
    size_t bag_count,
    const MultiBagReaderAttribute* attr);
void MultiBagReader_Destroy(MultiBagReader* reader);

bool MultiBagReader_IsValid(const MultiBagReader* reader);
bool MultiBagReader_ReadNextMessage(MultiBagReader* reader, ReadedMessage* out);
bool MultiBagReader_MoveReaderHeadTo(MultiBagReader* reader,
                                     uint64_t min_msg_timestamp);
void MultiBagReader_Interrupt(MultiBagReader* reader);
void MultiBagReader_Reset(MultiBagReader* reader);

uint64_t MultiBagReader_GetMessageNumber(const MultiBagReader* reader,
                                         const char* channel_name);
char* MultiBagReader_GetMessageType(const MultiBagReader* reader,
                                    const char* channel_name);
char* MultiBagReader_GetDescriptor(const MultiBagReader* reader,
                                   const char* channel_name,
                                   size_t* descriptor_size);
char** MultiBagReader_GetChannelList(const MultiBagReader* reader,
                                     size_t* count);

uint64_t MultiBagReader_BeginTime(const MultiBagReader* reader);
uint64_t MultiBagReader_EndTime(const MultiBagReader* reader);

#ifdef __cplusplus
}
#endif
