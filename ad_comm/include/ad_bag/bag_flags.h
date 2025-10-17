/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 */
#pragma once

#include <cstdint>
#include <string>

#include "ad_bag/schema/schema.h"
#include "ad_bag/exception.h"
namespace senseAD {

namespace bag {

/**
 * @brief Global Option for bag
 *
 */
const uint64_t kCompressFlagMask8 = 0b11111111;
const uint64_t kBitShiftCompressMode = 0;

const uint64_t kBitShiftTimeMode = 8;

const uint64_t kBitShiftGmtOffsetSign = 9;
const uint64_t kGmtOffsetMinuteFlagMask10 = 0b1111111111;
const uint64_t kBitShiftGmtOffsetMinute = 10;

// use 8 bit for it
enum class CompressionMode { NONE = 0, kLz4 = 1 << 0, kZstd = 1 << 1 };
enum class TimeMode { RSCL_TIME = 0, HEADER_TIME = 1 << 0 };

struct BagFlags {
    // Available bit is depend on the flags variable
    static constexpr size_t kTotalBit =
        sizeof(senseAD::bag::BagHeader::flags) * 8;

    // Current Use Bit layout: (start from index 0)
    // |compress mode(8bit)  | time mode(1bit)|
    // |GMT timezone offset (1bit for sign)(10bit for minute offset)|
    // |Unused(remaining bit)|
    CompressionMode compress_mode = CompressionMode::NONE;
    TimeMode time_mode{TimeMode::RSCL_TIME};
    // Assumed the absolute value of gmt_offset_sec/60 will never > 10bit
    int32_t gmt_offset_sec{0};

    inline static BagFlags FromFlags(const uint64_t flags) {
        BagFlags out;
        // Compress part
        uint8_t compress_flag =
            (flags >> kBitShiftCompressMode) & kCompressFlagMask8;
        out.compress_mode = static_cast<CompressionMode>(compress_flag);

        uint8_t time_mode_flag = (flags >> kBitShiftTimeMode) & 0b1;
        out.time_mode = static_cast<TimeMode>(time_mode_flag);

        bool gmt_offset_negative =
            ((flags >> kBitShiftGmtOffsetSign) & 0b1) == 1;
        int32_t gmt_offset_minutes =
            (flags >> kBitShiftGmtOffsetMinute) & kGmtOffsetMinuteFlagMask10;
        out.gmt_offset_sec =
            (gmt_offset_negative ? -1 : 1) * gmt_offset_minutes * 60;

        return out;
    }
    inline uint64_t ToFlags() const {
        uint64_t out =
            (static_cast<uint8_t>(compress_mode) << kBitShiftCompressMode) |
            (static_cast<uint64_t>(time_mode) << kBitShiftTimeMode);

        if (gmt_offset_sec != 0) {
            if (gmt_offset_sec % 60 != 0) {
                throw senseAD::bag::WriteMessageException(
                    "gmt_offset_sec must be minute level. (gmt_offset_sec%60 "
                    "must be 0)");
            }
            int32_t gmt_offset_min{gmt_offset_sec / 60};
            uint8_t gmt_offset_sign_bit = 0;
            if (gmt_offset_min < 0) {
                gmt_offset_min *= -1;
                gmt_offset_sign_bit = 1;
            }
            // Ensure abs of gmt_offset_sec is smaller than 10 bit
            if ((gmt_offset_min >> 10) != 0) {
                throw senseAD::bag::WriteMessageException(
                    "abs(gmt_offset_sec) must be <= 13 hour");
            }

            out |= (gmt_offset_sign_bit << kBitShiftGmtOffsetSign);
            out |= (gmt_offset_min << kBitShiftGmtOffsetMinute);
        }

        return out;
    }
    inline bool IsCompressed() const {
        return compress_mode != CompressionMode::NONE;
    }

    inline bool IsHeaderTimeMode() const {
        return time_mode == TimeMode::HEADER_TIME;
    }

    inline std::string CompressedMethod() const {
        switch (compress_mode) {
            case CompressionMode::NONE:
                return "none";
            case CompressionMode::kLz4:
                return "lz4";
            case CompressionMode::kZstd:
                return "zstd";
            default:
                return "unknown";
        }
    }

    inline std::string HeaderTimeMode() const {
        return time_mode == TimeMode::HEADER_TIME ? "HEADER_TIME" : "RSCL_TIME";
    }
};
}  // namespace bag
}  // namespace senseAD
