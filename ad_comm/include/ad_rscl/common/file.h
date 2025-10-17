/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <mutex>
#include <new>
#include <string>
#include <vector>

#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"
#include "yaml-cpp/yaml.h"

#include "ad_log/ad_log.hpp"
#include "ad_base/common/file.h"
#include "ad_rscl/common/config.h"

namespace senseAD {
namespace rscl {
namespace common {
struct FileDescripter {
    FileDescripter(const char *path, int flags) { fd_ = open(path, flags); }
    ~FileDescripter() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }
    inline int GetFd() const noexcept { return fd_; }
    inline bool Opened() const noexcept { return fd_ >= 0; }
    int fd_ = -1;
};

bool GetProtoFromASCIIFileWithEnvSub(const std::string &file_name,
                                     google::protobuf::Message *message);
bool GetProtoFromASCIIFile(const std::string &file_name,
                           google::protobuf::Message *message);
bool GetYamlFromASCIIFile(const std::string &file_name, YAML::Node *node);
YAML::Node MergeYamlNode(const YAML::Node &base_node,
                         const YAML::Node &append_node);

bool SetProtoToASCIIFile(const google::protobuf::Message &message,
                         int file_descriptor);
/**
 * @brief Sets the content of the file specified by the file_name to be the
 *        ascii representation of the input protobuf.
 * @param message The proto to output to the specified file.
 * @param file_name The name of the target file to set the content.
 * @return If the action is successful.
 */
bool SetProtoToASCIIFile(const google::protobuf::Message &message,
                         const std::string &file_name);

/**
 * @brief Parses the content of the file specified by the file_name as ascii
 *        representation of protobufs, and merges the parsed content to the
 *        proto.
 * @param file_name The name of the file to parse whose content.
 * @param message The proto to carry the parsed content in the specified file.
 * @return If the action is successful.
 */
bool GetProtoFromASCIIFile(const std::string &file_name,
                           google::protobuf::Message *message);

/**
 * @brief Sets the content of the file specified by the file_name to be the
 *        binary representation of the input protobuf.
 * @param message The proto to output to the specified file.
 * @param file_name The name of the target file to set the content.
 * @return If the action is successful.
 */
bool SetProtoToBinaryFile(const google::protobuf::Message &message,
                          const std::string &file_name);

/**
 * @brief Parses the content of the file specified by the file_name as binary
 *        representation of protobufs, and merges the parsed content to the
 *        proto.
 * @param file_name The name of the file to parse whose content.
 * @param message The proto to carry the parsed content in the specified file.
 * @return If the action is successful.
 */
bool GetProtoFromBinaryFile(const std::string &file_name,
                            google::protobuf::Message *message);

/**
 * @brief Parses the content of the file specified by the file_name as a
 *        representation of protobufs, and merges the parsed content to the
 *        proto.
 * @param file_name The name of the file to parse whose content.
 * @param message The proto to carry the parsed content in the specified file.
 * @return If the action is successful.
 */
bool GetProtoFromFile(const std::string &file_name,
                      google::protobuf::Message *message);

using senseAD::base::common::Copy;
using senseAD::base::common::CopyDir;
using senseAD::base::common::CopyFile;
using senseAD::base::common::CreateDir;
using senseAD::base::common::DeleteFile;
using senseAD::base::common::DirectoryExists;
using senseAD::base::common::EnsureDirectory;
using senseAD::base::common::FileType;
using senseAD::base::common::GetAbsolutePath;
using senseAD::base::common::GetContent;
using senseAD::base::common::GetCurrentPath;
using senseAD::base::common::GetFileName;
using senseAD::base::common::GetType;
using senseAD::base::common::ListSubPaths;
using senseAD::base::common::PathExists;
using senseAD::base::common::RemoveAllFiles;
using senseAD::base::common::SearchFile;
}  // namespace common
}  // namespace rscl
}  // namespace senseAD
