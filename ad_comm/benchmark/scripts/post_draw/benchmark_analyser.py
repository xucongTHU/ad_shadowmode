#!/usr/bin/env python3
# coding=utf-8

from python_gen_doc import DocxGenerator
from stat_types import *
from tools import *
from collections import OrderedDict, defaultdict
import matplotlib.pyplot as plt
from unittest import case
from enum import Enum
import argparse
import csv
import os
import re
import sys

sys.dont_write_bytecode = True


class BenchmarkPlatform(Enum):
    LINUX = 0
    QNX = 1


# read case running environment
class EnvReader:

    def __init__(self, env_path: str, platform = BenchmarkPlatform.LINUX) -> None:
        self.env_property = OrderedDict()
        self.env_file_path = env_path
        self.platform = platform

    @property
    def EnvInfo(self):
        if self.platform == BenchmarkPlatform.QNX:
            return "qnx not supported"
        if len(self.env_property) == 0:
            self.ReadCpuInfo()
            self.ReadMemInfo()
            self.ReadRsclInfo()
            self.ReadLinuxInfo()
        return YamlFormatter.encode(self.env_property)

    def ReadCpuInfo(self):
        cpu_file = os.path.join(self.env_file_path, "cpuinfo")
        with open(cpu_file, "r") as f:
            content = f.readlines()
            for line in content:
                if "processor" in line:
                    self.env_property["CPU数量"] = int(
                        line.split(':')[-1].strip()) + 1
                elif "model name" in line:
                    self.env_property["配置"] = line.split(':')[-1].strip()

    def ReadLinuxInfo(self):
        linux_file = os.path.join(self.env_file_path, "uname")
        with open(linux_file, "r") as f:
            self.env_property["系统信息"] = f.readline().strip()

    def ReadMemInfo(self):
        mem_file = os.path.join(self.env_file_path, "meminfo")
        with open(mem_file, "r") as f:
            content = f.readlines()
            for line in content:
                if "MemTotal" in line:
                    self.env_property["总内存(KB)"] = line.split(':')[-1].strip()
                elif "MemFree" in line:
                    self.env_property["空闲内存(KB)"] = line.split(':')[-1].strip()
                elif "MemAvailable" in line:
                    self.env_property["可用内存(KB)"] = line.split(':')[-1].strip()

    def ReadRsclInfo(self):
        rscl_file = os.path.join(self.env_file_path, "rscl_version")
        with open(rscl_file, "r") as f:
            self.env_property["rscl版本"] = f.read().replace("\n", "-").strip()


# iterate case benchmark log files, get and draw
class CaseIterator:

    def __init__(self, case_path: str, platform=BenchmarkPlatform.LINUX) -> None:
        self.case_path = case_path
        self.__case_list = []
        self.tune_files = []
        self.platform = platform
        self.__Iterate()

    @property
    def CaseList(self):
        return sorted(self.__case_list, key=lambda x: x.case_name.lower())

    def __Iterate(self):
        casename_list = os.listdir(self.case_path)
        for casename in casename_list:
            case_eval = BenchmarkCaseResult()
            case_eval.case_name = casename
            if casename == "tuner":
                tuner_path = os.path.join(self.case_path, casename)
                file_list = [file for file in os.listdir(
                    tuner_path) if "png" not in file]
                if len(file_list) != 0:
                    print("Parsing tuner {}".format(file_list[0]))
                    self.tune_files = plot_tuner_profile_csv(
                        os.path.join(tuner_path, file_list[0]))
                continue
            print("Parsing case {}".format(casename))
            logfile_list = os.listdir(os.path.join(self.case_path, casename))
            all_pids = set([
                file.split(".")[-1] for file in logfile_list
                if "test_setup.yaml" in file
            ])
            sub_pids = set([
                file.split(".")[1] for file in logfile_list
                if file.startswith("transport_")
            ])
            if len(sub_pids) == 0:
                print(
                    "No transport log found, sub process maybe unexpected exit!"
                )
            pub_pids = all_pids.difference(sub_pids)

            # dict init
            for sub_pid in sub_pids:
                case_eval.sub_stats[sub_pid] = SubStat()
            for pub_pid in pub_pids:
                case_eval.pub_stats[pub_pid] = PubStat()

            for logfile in logfile_list:
                curr_file_path = os.path.join(self.case_path, casename,
                                              logfile)
                if ".png" in logfile:
                    continue
                if logfile == "perf":
                    perf_data_files = os.listdir(curr_file_path)
                    for file in perf_data_files:
                        if "perf_data" in file and "unfold" in file:
                            self.__perf_draw(os.path.join(
                                curr_file_path, file))

                for pid in sub_pids:
                    if pid not in logfile:
                        continue
                    self.__select_sub(case_eval.sub_stats[pid], logfile, pid,
                                      curr_file_path)
                for pid in pub_pids:
                    if pid not in logfile:
                        continue
                    self.__select_pub(case_eval.pub_stats[pid], logfile, pid,
                                      curr_file_path)
            self.__case_list.append(case_eval)

    def __select_sub(self, sub_ret: SubStat, logfile: str, sub_pid: str,
                     curr_file_path: str):
        if "proc_resource" in logfile:
            sub_ret.pid = sub_pid
            sub_ret.proc_resource.csv_path = curr_file_path
            sub_ret.proc_resource.data_mean, sub_ret.proc_resource.plot_path = plot_proc_res_csv(
                curr_file_path)
        elif "system_cpu" in logfile:
            sub_ret.sys_resource.csv_path = curr_file_path
            sub_ret.sys_resource.data_mean, sub_ret.sys_resource.plot_path = plot_ts_csv(
                curr_file_path)
        elif "test_setup" in logfile:
            with open(curr_file_path, 'r') as f:
                sub_ret.test_setup = YamlFormatter.decode(f.read())
        elif "transport_summary" in logfile:
            with open(curr_file_path, 'r') as f:
                content = ""
                for line in f.readlines():
                    content += line + "\n"
                sub_ret.transport_stat = YamlFormatter.decode(content)
        elif logfile.startswith("transport_"):
            sub_ret.node_topic = logfile.split("_", 1)[1].split(".", 1)[0]
            sub_ret.transport_eval.csv_path = curr_file_path
            sub_ret.transport_eval.data_mean, sub_ret.transport_eval.plot_path = plot_ts_csv(
                curr_file_path)
        elif "running_report" in logfile:
            with open(curr_file_path, 'r') as f:
                for line in f.readlines():
                    if "ret_status" in line:
                        r_exit_code = line.split(":")[1].strip()
                        sub_ret.exit_code = r_exit_code[-1]
                    elif "description" in line:
                        sub_ret.description = line.split(":")[1].strip()

    def __select_pub(self, pub_ret: PubStat, logfile: str, pub_pid: str,
                     curr_file_path: str):
        if "proc_resource" in logfile:
            pub_ret.pid = pub_pid
            pub_ret.proc_resource.csv_path = curr_file_path
            pub_ret.proc_resource.data_mean, pub_ret.proc_resource.plot_path = plot_proc_res_csv(
                curr_file_path)
        elif "system_cpu" in logfile:
            pub_ret.sys_resource.csv_path = curr_file_path
        elif "test_setup" in logfile:
            with open(curr_file_path, 'r') as f:
                pub_ret.test_setup = YamlFormatter.decode(f.read())
        elif "running_report" in logfile:
            with open(curr_file_path, 'r') as f:
                for line in f.readlines():
                    if "ret_status" in line:
                        r_exit_code = line.split(":")[1].strip()
                        pub_ret.exit_code = r_exit_code[-1]
                    elif "description" in line:
                        pub_ret.description = line.split(":")[1].strip()
        return pub_ret

    def __perf_draw(self, perf_data: str):
        stackcollapse = os.path.join(
            os.path.dirname(os.path.realpath(__file__)),
            "stackcollapse-perf.pl")
        flamegraph = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                  "flamegraph.pl")
        perf_dir = perf_data.rsplit("/", 1)[0]
        file_name = perf_data.rsplit("/", 1)[1].rsplit(".", 1)[0]
        stackcollapse_cmd = "{} {} > {}.fold".format(
            stackcollapse, perf_data, os.path.join(perf_dir, file_name))
        flame_cmd = "{} {}.fold > {}.svg".format(
            flamegraph, os.path.join(perf_dir, file_name),
            os.path.join(perf_dir, file_name))
        os.system(stackcollapse_cmd)
        os.system(flame_cmd)

    def average_draw(self, dump_file_path: str):
        avg_data = defaultdict(list)
        for case in self.CaseList:
            if len(case.sub_stats) == 0:
                print("{} has no substat log, skip".format(case.case_name))
                continue

            # only support single sub
            first_case_key = list(case.sub_stats.keys())[0]
            if case.sub_stats[first_case_key].transport_stat is None:
                print(
                    "case {} sub pid {} has no transport_summary log, maybe unexpected exit? skip"
                    .format(case.case_name, first_case_key))
                continue
            avg_data["case_name"].append(case.case_name)
            # avg_data["publish_max_ms"].append(
            #     float(case.sub_stats[first_case_key].
            #           transport_stat[u"最高发送耗时(ms)"]))
            # avg_data["latency_max_ms"].append(
            #     float(case.sub_stats[first_case_key].
            #           transport_stat[u"最高接收延迟(ms)"]))
            avg_data["publish_avg_ms"].append(
                float(case.sub_stats[first_case_key].
                      transport_stat[u"平均发送耗时(ms)"]))
            avg_data["latency_avg_ms"].append(
                float(case.sub_stats[first_case_key].
                      transport_stat[u"平均接收延迟(ms)"]))
            avg_data["jitter_avg_ms"].append(case.sub_stats[first_case_key].
                                             transport_eval.data_mean["jitter(ms)"])
            avg_data["cpu_used"].append(case.sub_stats[first_case_key].
                                        proc_resource.data_mean["cpu_used"])
            avg_data["rss_kb"].append(case.sub_stats[first_case_key].
                                      proc_resource.data_mean["rss_kb"])
        plot_case_avg(avg_data, dump_file_path)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='benchmark result parser, output png and report.docx to input dir',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-s',
                        '--source',
                        dest='source',
                        help='specify result log source dir',
                        default="/tmp/rscl_benchmark_out",
                        required=False)
    parser.add_argument('-p',
                        '--platform',
                        dest='platform',
                        help='specify benchmark platform',
                        default="x86",
                        required=False)
    args = parser.parse_args()

    target_platform = BenchmarkPlatform.LINUX
    if args.platform == "qnx":
        target_platform = BenchmarkPlatform.QNX

    log_dir = args.source
    if not os.path.exists(log_dir):
        print("Invalid log path: " + log_dir)
        exit(-1)
    reader = EnvReader(log_dir, target_platform)

    case_iter = CaseIterator(os.path.join(log_dir, "stat"), target_platform)
    if len(case_iter.CaseList) > 2:
        case_iter.average_draw(os.path.join(log_dir, kCaseAvgCsvName))

    ret = BenchmarkResult(
        reader.EnvInfo, case_iter.CaseList, case_iter.tune_files)
    doc = DocxGenerator(ret, os.path.join(log_dir, "report.docx"))
    doc.generate_docx()
