//
// Created by ymm on 25-7-11.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "statusUtils.h"

namespace shadow {
namespace common {

CPUData readCPUData() {
    std::ifstream statFile("/proc/stat");
    if (!statFile.is_open()) {
        throw std::runtime_error("Could not open /proc/stat file");
    }

    CPUData data;
    std::string line;
    while (std::getline(statFile, line)) {
        if (line.find("cpu") == 0) { // Look for the first "cpu" line
            std::istringstream iss(line.substr(4));
            iss >> data.user >> data.nice >> data.system >> data.idle >> data.iowait >> data.irq >> data.softirq;
            break;
        }
    }

    statFile.close();
    return data;
}

double calculateCPUPercentage(const CPUData& oldData, const CPUData& newData) {  
    long oldTotal = oldData.user + oldData.nice + oldData.system + oldData.idle + oldData.iowait + oldData.irq + oldData.softirq;
    long newTotal = newData.user + newData.nice + newData.system + newData.idle + newData.iowait + newData.irq + newData.softirq;

    long totalDiff = newTotal - oldTotal;
    long idleDiff = newData.idle - oldData.idle;

    std::cout << "Total Diff: " <<  totalDiff  << "\n";   
    std::cout  << "Idle Diff:   " <<  idleDiff << "\n";    

    return (totalDiff - idleDiff) / static_cast<double>(totalDiff);
}

void getMemData(SysInfo& sys_info) {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        std::cerr << "Failed to open /proc/meminfo" << std::endl;
        return;
    }

    std::string line;
    long totalMem = 0, freeMem = 0, buffers = 0, cached = 0;

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal") != std::string::npos) {
            sscanf(line.c_str(), "MemTotal: %ld kB", &totalMem);
        } else if (line.find("MemFree") != std::string::npos) {
            sscanf(line.c_str(), "MemFree: %ld kB", &freeMem);
        } else if (line.find("Buffers") != std::string::npos) {
            sscanf(line.c_str(), "Buffers: %ld kB", &buffers);
        } else if (line.find("Cached") != std::string::npos) {
            sscanf(line.c_str(), "Cached: %ld kB", &cached);
        }
    }
    meminfo.close();

    long usedMem = totalMem - freeMem - buffers - cached;
    sys_info.memUsage = static_cast<double>(usedMem) / totalMem;
    // sys_info.memUsage = std::round(static_cast<double>(usedMem) / totalMem * 100) / 100; 
    std::cout << "sys_info.memUsage" << sys_info.memUsage << std::endl;
}

void getSpaceData(SysInfo& sys_info, const char *path) { 
    struct statvfs fsinfo;
    if (statvfs(path, &fsinfo) == 0) {
        // Total number of blocks in the filesystem.
        uint64_t totalBlocks = fsinfo.f_blocks;
        // Number of free blocks available to non-superuser.
        uint64_t freeBlocks = fsinfo.f_bfree;
        // Block size (in bytes).
        uint64_t blockSize = fsinfo.f_frsize;

        uint64_t totalSpace = totalBlocks * blockSize;
        uint64_t freeSpace = freeBlocks * blockSize;

        std::cout << "Path: " << path << std::endl;
        std::cout << "Total space: " << (totalSpace / (1024 * 1024)) << " MB" << std::endl;
        std::cout << "Free space: " << (freeSpace / (1024 * 1024)) << " MB" << std::endl;
        sys_info.harddriveUsage = static_cast<double>(freeSpace) / totalSpace;   
        // sys_info.harddriveUsage = std::round(static_cast<double>(freeSpace) / totalSpace * 100) / 100; 

    } else {
        throw std::runtime_error("statvfs error");
    }

}

} // namespace common
} // namespace shadow