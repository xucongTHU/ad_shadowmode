/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 */

#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

namespace senseAD {
namespace sensor {

class SimpleSharedMem {
 public:
    SimpleSharedMem(const std::string& name, const int32_t& size) {
        size_ = std::min(size, 1024);
        name_ = name;

        // 创建共享内存
        shm_fd_ = shm_open(name.c_str(), O_RDWR | O_CREAT, 0666);
        if (shm_fd_ < 0) {
            return;
        }

        int ret = ftruncate(shm_fd_, size_);
        if (-1 == ret) {
            close(shm_fd_);
            return;
        }

        data_ = reinterpret_cast<uint64_t*>(mmap(
            nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
        close(shm_fd_);

        // 创建或打开信号量
        std::string sem_name = "/" + name_ + "_sem";
        sem_ = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
        if (sem_ == SEM_FAILED) {
            munmap(data_, size_);
            data_ = nullptr;
        }
    }

    ~SimpleSharedMem() {
        if (sem_) {
            sem_close(sem_);
        }
        munmap(data_, size_);
        // 可选：删除信号量
        sem_unlink(("/" + name_ + "_sem").c_str());
    }

    uint64_t GetData() {
        sem_wait(sem_);
        uint64_t result = *data_;
        sem_post(sem_);
        return result;
    }

    int32_t SetData(const uint64_t* data, const int32_t& size) {
        if (size > size_) {
            return -1;
        }

        sem_wait(sem_);
        memcpy(data_, data, size);
        sem_post(sem_);

        return 0;
    }

 private:
    int32_t size_;
    std::string name_;
    int shm_fd_;
    uint64_t* data_;
    sem_t* sem_;  // 信号量指针
};

}  // namespace sensor
}  // namespace senseAD
