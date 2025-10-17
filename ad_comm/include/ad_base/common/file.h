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
#include <cstdint>  // Added for uint64_t
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

namespace senseAD {
namespace base {
namespace common {
/**
 * @brief Get file content as string.
 * @param file_name The name of the file to read content.
 * @param content The file content.
 * @return If the action is successful.
 */
bool GetContent(const std::string &file_name, std::string *content);

/**
 * @brief Get absolute path by concatenating prefix and relative_path.
 * @return The absolute path.
 */
std::string GetAbsolutePath(const std::string &prefix,
                            const std::string &relative_path);

std::vector<std::string> SearchFile(const std::string &directory_path,
                                    const std::string &file_name,
                                    int max_search_level,
                                    std::vector<std::string> exclude_dirs = {});
std::vector<std::string> SearchFileByRegex(
    const std::string &directory_path,
    const std::string &regex_string,
    int max_search_level,
    std::vector<std::string> exclude_dirs = {});
/**
 * @brief Check if the path exists.
 * @param path a file name, such as /a/b/c.txt
 * @return If the path exists.
 */
bool PathExists(const std::string &path);

/**
 * @brief Check if the directory specified by directory_path exists
 *        and is indeed a directory.
 * @param directory_path Directory path.
 * @return If the directory specified by directory_path exists
 *         and is indeed a directory.
 */
bool DirectoryExists(const std::string &directory_path);

/**
 * @brief Get directory name of the give path
 *
 * @param file_path
 * @return directory path
 */
std::string DirectoryPath(const std::string &file_path);

/**
 * @brief Copy a file.
 * @param from The file path to copy from.
 * @param to The file path to copy to.
 * @return If the action is successful.
 */
bool CopyFile(const std::string &from, const std::string &to);

/**
 * @brief Copy a directory.
 * @param from The path to copy from.
 * @param to The path to copy to.
 * @return If the action is successful.
 */
bool CopyDir(const std::string &from, const std::string &to);

/**
 * @brief Copy a file or directory.
 * @param from The path to copy from.
 * @param to The path to copy to.
 * @return If the action is successful.
 */
bool Copy(const std::string &from, const std::string &to);

/**
 * @brief Check if a specified directory specified by directory_path exists.
 *        If not, recursively create the directory (and its parents).
 * @param directory_path Directory path.
 * @return If the directory does exist or its creation is successful.
 */
bool EnsureDirectory(const std::string &directory_path);

/**
 * @brief Remove all the files under a specified directory. Note that
 *        sub-directories are NOT affected.
 * @param directory_path Directory path.
 * @return If the action is successful.
 */
bool RemoveAllFiles(const std::string &directory_path);

/**
 * @brief List sub-paths.
 * @param directory_path Directory path.
 * @return A vector of sub-paths, without the directory_path prefix.
 */
std::vector<std::string> ListSubPaths(const std::string &directory_path);

std::string GetFileName(const std::string &path,
                        const bool remove_extension = false);

std::string GetCurrentPath();

// delete file including file or directory
bool DeleteFile(const std::string &filename);

enum FileType { TYPE_FILE, TYPE_DIR };
bool GetType(const std::string &filename, FileType *type);

bool CreateDir(const std::string &dir);

bool GetPidFd(pid_t pid, int *pidfd);  // NOLINT

bool GetCorrectFd(int pidfd, int fd, int *new_fd);  // NOLINT

bool ProcessNameFromPid(int pid, std::string *process_name);

std::vector<std::string> GetBlockDevices();
bool SetCurrentProcessBlkioWriteLimit(const std::string &cgroup_name,
                                      uint64_t limit);

}  // namespace common
}  // namespace base
}  // namespace senseAD
