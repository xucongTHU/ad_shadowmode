#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "common/logger/Logger.h"

namespace shadow{
namespace data_upload{


//读取指定路径文件并对文件进行切片
class FileSplitter {
public:
    char* LOG_TAG = "FileSplitter";
    // 错误码枚举
    enum ErrorCode {
        SUCCESS = 0,      // 成功
        FILE_OPEN_FAILED, // 文件打开失败
        INVALID_CHUNK,    // 分片号无效
        FILE_SEEK_FAILED, // 文件定位失败
        FILE_READ_FAILED  // 文件读取失败
    };

    //chunksize每个分片的大小
    FileSplitter(const std::string& filePath_, size_t chunkSize_)
        : filePath(filePath_), chunkSize(chunkSize_) {
        chunkSize = chunkSize * 1024 * 1024;
        // 打开文件以计算分片总数
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file) {
            errorCode = FILE_OPEN_FAILED; // 文件打开失败
            LOG_ERROR("File %s open failed.", filePath.c_str());
            return;
        }
        fileSize = file.tellg();//返回当前文件流的读写位置，配合ate标志，当前读写位置为文件总大小
        file.close();//关闭文件流，释放资源
        chunkCount = (fileSize + chunkSize - 1) / chunkSize; // 计算分片总数，向上取整
        LOG_INFO("fileSize:%d chunkSize:%d chunkCount:%d", (int)fileSize , (int)chunkSize,(int)chunkCount); 
        errorCode = SUCCESS; // 初始化成功
    }

    // 获取错误码
    ErrorCode getErrorCode() const {
        return errorCode;
    }

    // 获取分片总数
    int getChunkCount() const {
        return chunkCount;
    }

     // 获取文件大小
    size_t getFileSize() const {
        return fileSize;
    }

    // 根据分片号获取分片数据
    ErrorCode getChunkData(int chunkNumber, std::vector<char>& chunkData) const {
        chunkNumber-- ; 
        LOG_INFO("Chunk number after: %d, Chunk count: %d", chunkNumber, chunkCount); 
        if (chunkNumber < 0 || chunkNumber >= chunkCount) {
            LOG_ERROR("Invalid chunk number");
            return INVALID_CHUNK; // 分片号无效
        }

        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            LOG_ERROR("File %s open failed.", filePath.c_str());
            return FILE_OPEN_FAILED; // 文件打开失败
        }

        // 计算分片的起始位置和大小
        size_t offset = chunkNumber * chunkSize;
        size_t size = (chunkNumber == chunkCount - 1) ? (fileSize - offset) : chunkSize;
        LOG_INFO("offset: %d, size: %d", (int)offset, (int)size); 

        // 定位到分片的起始位置
        file.seekg(offset, std::ios::beg);
        if (!file) {
            LOG_ERROR("Seek to chunk %d failed.", (chunkNumber + 1));
            return FILE_SEEK_FAILED; // 文件定位失败
        }

        // 读取分片数据
        chunkData.resize(size);
        file.read(chunkData.data(), size);
        if (!file && !file.eof()) {
            LOG_ERROR("Read chunk %d failed.", (chunkNumber + 1));
            return FILE_READ_FAILED; // 文件读取失败
        }

        return SUCCESS; // 成功
    }

    // 根据分片号获取分片名
    ErrorCode getChunkName(int chunkNumber, std::string& chunkName) const {
        if (chunkNumber < 0 || chunkNumber >= chunkCount) {
            LOG_ERROR("Invalid chunk number");
            return INVALID_CHUNK; // 分片号无效
        }
        chunkName = generateChunkFileName(chunkNumber);
        return SUCCESS; // 成功
    }

private:
    std::string filePath;       // 原文件路径
    size_t chunkSize;           // 分片大小
    size_t fileSize;            // 文件总大小
    int chunkCount;             // 分片总数
    ErrorCode errorCode;        // 错误码

    // 生成分片文件名
    std::string generateChunkFileName(int chunkNumber) const {
        std::ostringstream oss;//输出字符串流
        //setw(3)表示输出字段宽度为3，setfill('0')表示不够3位时用0填充
        oss << filePath << "." << std::setw(3) << std::setfill('0') << (chunkNumber + 1);
        return oss.str();
    }
};


}
}

