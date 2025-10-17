/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "ad_bag/io/bag_file_reader.h"
#include "ad_bag/schema/schema.h"
#include "ad_bag/bag_flags.h"

namespace senseAD {
namespace bag {

class BagReader;
class MultiBagReader;

/**
 * Helper class for random access in a Bag file.
 *
 * This class provides functionality to move the reader head to a position where
 * the message timestamp is greater than or equal to the given timestamp,
 * allowing for efficient random access within a Bag file.
 */
class BagReaderRandomAccessHelper {
 public:
    explicit BagReaderRandomAccessHelper(BagReader* bag_reader);
    ~BagReaderRandomAccessHelper();

    void Init();
    // Move the reader head to a position where the message timestamp is greater
    // than or equal to `the given timestamp.
    bool MoveReaderHeadTo(const uint64_t min_msg_timestamp);

 private:
    BagReader* bag_reader_;

    struct SortItem {
        senseAD::bag::ChunkHeader chunk_header;
        uint64_t original_index;
    };

    std::vector<SortItem> sorted_chunk_headers_;
};

/**
 * Helper class for random access in a MultiBag file.
 *
 * This class provides functionality to move the reader head to a position where
 * the message timestamp is greater than or equal to the given timestamp,
 * allowing for efficient random access within a MultiBag file.
 */
class MultiBagReaderRandomAccessHelper {
 public:
    explicit MultiBagReaderRandomAccessHelper(MultiBagReader* multibag_reader);
    ~MultiBagReaderRandomAccessHelper();
    void Init();
    // Move the reader head to a position where the message timestamp is
    // greater
    // than or equal to the given timestamp.
    bool MoveReaderHeadTo(const uint64_t min_msg_timestamp);

 private:
    MultiBagReader* multibag_reader_;
};

}  // namespace bag
}  // namespace senseAD
