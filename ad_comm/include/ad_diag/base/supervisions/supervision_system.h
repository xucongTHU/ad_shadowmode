/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "cereal/types/string.hpp"

#include "ad_diag/base/supervisions/common.h"
#include "ad_diag/base/supervisions/supervision.h"

namespace senseAD {
namespace diag {

/***************************************************
 * !Check Point For SystemProfiler                 *
 ***************************************************/

struct Diagnosis {
    SupervisionStatus::StatusLevel level = SupervisionStatus::StatusLevel::NONE;
    uint64_t last_level_change_time_ns = 0;
    uint64_t last_update_time_ns = 0;

    std::string diag_msg;
    std::string class_type;
    std::string sub_class_type;

    DEFINE_SAVE_LOAD_FN(level,
                        last_level_change_time_ns,
                        last_update_time_ns,
                        diag_msg,
                        class_type,
                        sub_class_type)
};
struct CpuStat {
    uint8_t id;
    float user;
    float system;
    float nice;
    float idle;
    float iowait;
    // hardware irq
    float hirq;
    // software irq
    float sirq;
    // Steal time
    float st;

    float temperature;
    float cpu_used;

    float load_avg_1;
    float load_avg_5;
    float load_avg_15;

    Diagnosis diag;
    DEFINE_SAVE_LOAD_FN(id,
                        user,
                        system,
                        nice,
                        idle,
                        iowait,
                        hirq,
                        sirq,
                        st,
                        temperature,
                        cpu_used,
                        load_avg_1,
                        load_avg_5,
                        load_avg_15,
                        diag)
};
struct GpuStat {
    std::string id;

    /// In percentage
    int gpu_util;
    int memory_util;
    int encoder_util;
    int decoder_util;

    // Unit. KB
    int memory_total;
    int memory_used;
    int memory_free;

    int temperature;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(id,
                        gpu_util,
                        memory_util,
                        encoder_util,
                        decoder_util,
                        memory_total,
                        memory_used,
                        memory_free,
                        temperature,
                        diag)
};

struct MemoryStat {
    // Unit. KB
    unsigned total;
    unsigned used;
    unsigned free;
    unsigned buffered;
    unsigned cached;
    unsigned dirty;
    float memory_used;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(
        total, used, free, buffered, cached, dirty, memory_used, diag)
};

struct SwapStat {
    // Unit. KB
    float used;
    float free;
};

struct IoStat {
    // Unit. KB
    std::string device;
    std::string mountdir;
    std::string mountroot;
    /// performance stats
    double tps;
    unsigned kb_read;
    unsigned kb_write;
    float kb_read_per_second;
    float kb_write_per_second;
    unsigned r_await_millisec;
    unsigned w_await_millisec;
    /// storage stats, in KB
    unsigned total;
    unsigned used;
    unsigned free;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(device,
                        mountdir,
                        mountroot,
                        tps,
                        kb_read,
                        kb_write,
                        kb_read_per_second,
                        kb_write_per_second,
                        r_await_millisec,
                        w_await_millisec,
                        total,
                        used,
                        free,
                        diag)
};

struct NetworkStat {
    std::string nic;
    bool enable;

    // Unit. packet
    unsigned rx_bytes_rate;
    unsigned tx_bytes_rate;
    unsigned rx_packets_rate;
    unsigned tx_packets_rate;
    unsigned nr_rx_dropped;
    unsigned nr_tx_dropped;
    unsigned nr_rx_errors;
    unsigned nr_tx_errors;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(nic,
                        enable,
                        rx_bytes_rate,
                        tx_bytes_rate,
                        rx_packets_rate,
                        tx_packets_rate,
                        nr_rx_dropped,
                        nr_tx_dropped,
                        nr_rx_errors,
                        nr_tx_errors,
                        diag)
};

struct NetworkPeerTransportStat {
    unsigned local_port;
    unsigned remote_port;
    unsigned rx_queue;
    unsigned tx_queue;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(local_port, remote_port, rx_queue, tx_queue, diag)
};

struct NetworkPeerStat {
    std::string peer;
    int latency_avg;      // in us
    int latency_max;      // in us
    int latency_min;      // in us
    int latency_timeout;  // in percentage
    uint64_t clock_diff;  // in ms, local - remote

    std::vector<NetworkPeerTransportStat> tcp;
    std::vector<NetworkPeerTransportStat> udp;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(peer,
                        latency_avg,
                        latency_max,
                        latency_min,
                        latency_timeout,
                        tcp,
                        udp,
                        diag)
};

struct ProcessIO {
    unsigned long long rchar;                  // in B NOLINT
    unsigned long long wchar;                  // in B NOLINT
    unsigned long long syscr;                  // NOLINT
    unsigned long long syscw;                  // NOLINT
    unsigned long long read_bytes;             // in B NOLINT
    unsigned long long write_bytes;            // in B NOLINT
    unsigned long long cancelled_write_bytes;  // in B NOLINT

    DEFINE_SAVE_LOAD_FN(rchar,
                        wchar,
                        syscr,
                        syscw,
                        read_bytes,
                        write_bytes,
                        cancelled_write_bytes)
};

struct ProcessStat {
    std::string command;
    int pid;
    float cpu_used;
    uint32_t minor_faults;
    uint32_t major_faults;
    uint32_t utime;
    uint32_t stime;
    int32_t nr_threads;

    uint32_t vsize;  /// in KB
    int32_t rss;     /// in KB
    int memory_used;
    int64_t wait_cpu;
    int32_t pss;       /// in KB
    int32_t uss;       /// in KB
    int32_t sp_alloc;  /// in KB

    unsigned long long ion;  // NOLINT in B

    ProcessIO io;

    unsigned long long vol_ctxt_swtch;      // NOLINT
    unsigned long long non_vol_ctxt_swtch;  // NOLINT

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(command,
                        pid,
                        cpu_used,
                        minor_faults,
                        major_faults,
                        utime,
                        stime,
                        nr_threads,
                        vsize,
                        rss,
                        memory_used,
                        wait_cpu,
                        pss,
                        uss,
                        sp_alloc,
                        ion,
                        io,
                        vol_ctxt_swtch,
                        non_vol_ctxt_swtch,
                        diag)
};

struct AiCoreUsage {
    double used = 0.0;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(used, diag)
};

struct AiCore {
    std::string name;
    float used;
    float hwi;
    float swi;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(name, used, hwi, swi, diag)
};

struct MemBandwidth {
    float mem_bandwidth = 0.0;

    std::vector<float> all_mem_bandwidth;
    std::vector<float> ai_core_mem_bandwidth;

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(mem_bandwidth,
                        all_mem_bandwidth,
                        ai_core_mem_bandwidth,
                        diag)
};

struct Bandwidth {
    std::string mpamid;
    double bw = 0.0;
    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(mpamid, bw, diag)
};

struct DuStat {
    std::string dir;
    unsigned long long rate;  // NOLINT

    Diagnosis diag;

    DEFINE_SAVE_LOAD_FN(dir, rate, diag)
};

struct SystemReport {
    // The hardware have. Ex. j5, x86, xavier
    std::string platform;
    // Ex. vh-1, vh-2,
    std::string identifier;

    bool has_cpus_stat = false;
    std::vector<CpuStat> cpus;

    bool has_gpus_stat = false;
    std::vector<GpuStat> gpus;

    bool has_memory_stat = false;
    MemoryStat memory;

    bool has_ios_stat = false;
    std::vector<IoStat> ios;

    bool has_network_stat = false;
    std::vector<NetworkStat> networks;

    std::vector<NetworkPeerStat> network_peers;

    std::vector<ProcessStat> processes;

    /////////////// < VDC/HAD_TDA specific  ///////////////
    std::string ti_resource_info;
    /////////////// > VDC specific  ///////////////
    /////////////// > VDC/HAD_TDA specific  ///////////////

    /////////////// < VDC/MDC specific  ///////////////
    bool has_ai_core_usages = false;
    std::vector<AiCore> ai_core_usages;
    /////////////// > VDC/MDC specific  ///////////////

    // /////////////// < HAD_J5 specific  ///////////////
    // bool has_hrut_ddr_infos = false;
    // std::vector<std::string> hrut_ddr_infos;
    // bool has_hrut_bpu_infos = false;
    // std::vector<std::string> hrut_bpu_infos;
    // /////////////// > HAD_J5 specific  ///////////////

    /////////////// < J6E specific  ///////////////
    bool has_hrut_ddr_infos = false;
    std::vector<std::string> hrut_ddr_infos;
    bool has_bpu_infos = false;
    int bpu_infos = 0;
    /////////////// > J6E specific  ///////////////

    /////////////// < MDC specific  ///////////////
    AiCoreUsage ai_core_usage;
    bool has_single_ai_core_usage = false;
    std::vector<AiCore> single_ai_core_usage;
    MemBandwidth mem_bandwidth;
    std::vector<Bandwidth> bandwidths;
    /////////////// > MDC specific  ///////////////

    bool has_dus_stat = false;
    std::vector<DuStat> dus;

    DEFINE_SAVE_LOAD_FN(platform,
                        identifier,
                        has_cpus_stat,
                        cpus,
                        has_gpus_stat,
                        gpus,
                        has_memory_stat,
                        memory,
                        has_ios_stat,
                        ios,
                        has_network_stat,
                        networks,
                        network_peers,
                        processes,
                        ti_resource_info,
                        has_ai_core_usages,
                        ai_core_usages,
                        ai_core_usage,
                        has_single_ai_core_usage,
                        single_ai_core_usage,
                        has_hrut_ddr_infos,
                        hrut_ddr_infos,
                        has_bpu_infos,
                        bpu_infos,
                        mem_bandwidth,
                        bandwidths,
                        has_dus_stat,
                        dus)
};

struct SystemHealthSupervisionReport {
    senseAD::diag::SupervisionReport base;
    senseAD::diag::SystemReport sys_report;

    std::unordered_map<std::string, SupervisionStatus::StatusLevel>
        health_check_items;

    DEFINE_SAVE_LOAD_FN(base, sys_report, health_check_items)
};

typedef SupervisionEndpoint<SystemHealthSupervisionReport,
                            SupervisionCppType::SYSTEM_PROFILER>
    SystemHealthSupervisionEndpoint;

}  // namespace diag
}  // namespace senseAD
