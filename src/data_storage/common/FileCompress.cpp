#include "FileCompress.h"
#include <iostream>
#include <fstream>
#include <lz4frame.h>
#include <cstdio>
#include <filesystem>
#include "microtar/microtar.h"

namespace fs = std::filesystem;

namespace shadow {
namespace data_storage{

// 压缩多个目录和文件
FileCompress::ErrorCode FileCompress::CompressFiles(
    const std::vector<std::string>& inputFiles,
    const std::string& outputFile) {
    std::vector<std::string> allFiles;

    for (const auto& path : inputFiles) {
        if (fs::is_regular_file(path)) {
            allFiles.push_back(path);
        } else {
            std::cerr << "Warning: Invalid path: " << path << std::endl;
            return ErrorCode::InvalidInputPath;
        }
    }

    std::string tempTarFile = outputFile + ".tmp.tar";

    mtar_t tar;
    if (mtar_open(&tar, tempTarFile.c_str(), "w") != 0) {
        std::cerr << "Error: Failed to open tar file with microtar." << std::endl;
        return ErrorCode::FailedToCreateTarFile;
    }

    for (const auto& path : allFiles) {
        std::ifstream inFile(path, std::ios::binary | std::ios::ate);
        if (!inFile) {
            std::cerr << "Warning: Failed to open file: " << path << std::endl;
            mtar_close(&tar);
            return ErrorCode::FailedToOpenFile;
        }

        std::streamsize fileSize = inFile.tellg();
        inFile.seekg(0);

        std::vector<char> buffer(fileSize);
        inFile.read(buffer.data(), fileSize);
        inFile.close();

        std::string archiveName = fs::path(path).filename().string();

        if (mtar_write_file_header(&tar, archiveName.c_str(), fileSize) != 0) {
            std::cerr << "Error: Failed to write tar header for " << archiveName << std::endl;
            mtar_close(&tar);
            return ErrorCode::FailedToCreateTarFile;
        }
        if (mtar_write_data(&tar, buffer.data(), fileSize) != 0) {
            std::cerr << "Error: Failed to write tar data for " << archiveName << std::endl;
            mtar_close(&tar);
            return ErrorCode::FailedToCreateTarFile;
        }
    }

    mtar_finalize(&tar);
    mtar_close(&tar);

    std::ifstream tarFile(tempTarFile, std::ios::binary);
    if (!tarFile) {
        std::cerr << "Error: Open temp tar file failed: " << tempTarFile << std::endl;
        std::remove(tempTarFile.c_str());
        return ErrorCode::FailedToOpenFile;
    }
    std::vector<char> tarData((std::istreambuf_iterator<char>(tarFile)), {});
    tarFile.close();

    std::vector<char> compressedData;
    ErrorCode compressError = CompressData(tarData, compressedData);
    if (compressError != ErrorCode::Success) {
        std::cerr << "Error: LZ4 compression failed" << std::endl;
        std::remove(tempTarFile.c_str());
        return compressError;
    }

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Create output file failed: " << outputFile << std::endl;
        std::remove(tempTarFile.c_str());
        return ErrorCode::FailedToCreateTarFile;
    }
    outFile.write(compressedData.data(), compressedData.size());
    outFile.close();

    std::remove(tempTarFile.c_str());

    std::cout << "Compression completed (standard tar.lz4): " << outputFile << std::endl;
    return ErrorCode::Success;
}

FileCompress::ErrorCode FileCompress::CompressSingleFileToLz4(
    const std::string& inputFile, const std::string& outputFile) {
    if (!fs::is_regular_file(inputFile)) {
        std::cerr << "Error: " << inputFile << " is not a regular file." << std::endl;
        return ErrorCode::InvalidInputPath;
    }
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Failed to create output file: " << outputFile << std::endl;
        return ErrorCode::FailedToCreateOutput;
    }
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Warning: Failed to open file: " << inputFile << std::endl;
        return ErrorCode::FailedToOpenFile;
    }
    std::vector<char> fileData(
        (std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>()
    );

    std::vector<char> compressedData;
    ErrorCode error = CompressData(fileData, compressedData);
    if (error != ErrorCode::Success) {
        return error;
    }
    outFile.close();
    return ErrorCode::Success;
}


FileCompress::ErrorCode FileCompress::CompressData(const std::vector<char>& input, std::vector<char>& compressedData) {
    size_t srcSize = input.size();

    LZ4F_cctx* cctx;
    size_t err = LZ4F_createCompressionContext(&cctx, LZ4F_VERSION);
    if (LZ4F_isError(err)) {
        std::cerr << "LZ4F_createCompressionContext error: " << LZ4F_getErrorName(err) << std::endl;
        return ErrorCode::CompressionFailed;
    }

    LZ4F_preferences_t prefs = {};
    prefs.frameInfo.blockSizeID = LZ4F_max64KB;
    prefs.frameInfo.blockMode = LZ4F_blockIndependent;
    prefs.compressionLevel = 1;

    size_t maxDstSize = LZ4F_compressFrameBound(srcSize, &prefs);
    compressedData.resize(maxDstSize);

    size_t compressedSize = LZ4F_compressFrame(
        compressedData.data(), maxDstSize,
        input.data(), srcSize,
        &prefs
    );

    if (LZ4F_isError(compressedSize)) {
        std::cerr << "LZ4F_compressFrame error: " << LZ4F_getErrorName(compressedSize) << std::endl;
        LZ4F_freeCompressionContext(cctx);
        return ErrorCode::CompressionFailed;
    }

    compressedData.resize(compressedSize);
    LZ4F_freeCompressionContext(cctx);
    return ErrorCode::Success;

}

// 递归获取目录中的所有文件
FileCompress::ErrorCode FileCompress::GetFilesInDirectory(const std::string& directory, std::vector<std::string>& fileList) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            if (fs::is_regular_file(entry)) {
                fileList.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to traverse directory: " << directory << " - " << e.what() << std::endl;
        return ErrorCode::InvalidInputPath;
    }
    return ErrorCode::Success;
}

}
}

