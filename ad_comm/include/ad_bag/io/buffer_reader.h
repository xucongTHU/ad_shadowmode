/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file buffer_reader.h
 * @brief Wrapping the input source as a reader.
 */
#pragma once

#include <cstdio>
#include <cstdlib>
#include <limits>

namespace senseAD {
namespace bag {

/**
 * @brief Abstract class used for reading data from destination
 *
 */
class BufferReader {
 public:
    virtual ~BufferReader() = default;

    virtual size_t CurrentPosition() const = 0;
    virtual size_t RemainBufferSize() const = 0;
    virtual size_t TotalBufferSize() const = 0;

    virtual bool Read(const size_t nbytes, void* out_pos) = 0;

    virtual void Reset() = 0;
};

class FileSegmentReader : public BufferReader {
 public:
    FileSegmentReader(FILE* f,
                      size_t segment_size = std::numeric_limits<size_t>::max());
    virtual ~FileSegmentReader() = default;

    size_t CurrentPosition() const override;
    size_t RemainBufferSize() const override;
    size_t TotalBufferSize() const override;

    bool Read(const size_t nbytes, void* out_pos) override;

    void Reset() override;

 private:
    FILE* f_;
    size_t used_size_;
    size_t segment_size_;
};

class LoadedBufferReader : public BufferReader {
 public:
    LoadedBufferReader(const char* buffer, size_t buffer_size);
    virtual ~LoadedBufferReader() = default;

    size_t CurrentPosition() const override;
    size_t RemainBufferSize() const override;
    size_t TotalBufferSize() const override;

    bool Read(const size_t nbytes, void* out_pos) override;

    void Reset() override;

 private:
    const char* buffer_;
    size_t used_size_;
    size_t buffer_size_;
};

}  // namespace bag
}  // namespace senseAD
