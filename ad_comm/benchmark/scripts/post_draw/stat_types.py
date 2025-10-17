#!/usr/bin/env python3
# coding=utf-8

from enum import Enum
from collections import defaultdict

SIGNAL_TABLE = {
    "1": "SIGHUP",
    "2": "SIGINT",
    "3": "SIGQUIT",
    "4": "SIGILL",
    "5": "SIGTRAP",
    "6": "SIGABRT",
    "7": "SIGBUS",
    "8": "SIGFPE",
    "9": "SIGKILL",
    "b": "SIGSEGV",
    "d": "SIGPIPE",
    "f": "SIGTERM"
}

kCaseAvgCsvName = "case_avg.png"

class TransportType(Enum):
    PUB = 0
    SUB = 1


class DataLog:

    def __init__(self) -> None:
        self.data_mean = {}
        self.csv_path = ""
        self.plot_path = ""

    def __str__(self) -> str:
        return "data_mean:" + str(
            self.data_mean
        ) + "\ncsv_path: " + self.csv_path + "\nplot_path:" + self.plot_path


class BaseStat:

    def __init__(self) -> None:
        self.pid = 0
        self.test_setup = None
        self.exit_code = 0
        self.description = ""

        self.proc_resource = DataLog()
        self.sys_resource = DataLog()

    def __str__(self) -> str:
        return "pid: " + str(self.pid) + "\n" + "test_setup: " + str(
            self.test_setup) + "\n" + "exit_code: " + str(
                self.exit_code) + "\nproc_resource: " + str(
                    self.proc_resource) + "\nsys_resource: " + str(
                        self.sys_resource) + "\n"


class PubStat(BaseStat):

    def __init__(self) -> None:
        super(PubStat, self).__init__()
        self.transport_type = TransportType.PUB

    def __str__(self) -> str:
        return "type: Pub\n" + super().__str__()


class SubStat(BaseStat):

    def __init__(self) -> None:
        super(SubStat, self).__init__()
        self.transport_type = TransportType.SUB
        self.node_topic = ""
        self.transport_stat = None
        self.transport_eval = DataLog()

    def __str__(self) -> str:
        return "type: Sub\n" + "transport_stat: " + str(
            self.transport_stat) + "\ntransport_eval: " + str(
                self.transport_eval) + "\n" + super().__str__()


class BenchmarkCaseResult:

    def __init__(self) -> None:
        self.case_name = ""
        self.pub_stats = {}
        self.sub_stats = {}

    def __str__(self) -> str:
        ret_str = self.case_name + "\n"
        for _, pub in self.pub_stats.items():
            ret_str += pub.__str__() + "\n"
        for _, sub in self.sub_stats.items():
            ret_str += sub.__str__() + "\n"
        return ret_str


class BenchmarkResult:

    def __init__(self, env: str, case_list: list, tune_files: list) -> None:
        self.environment = env
        self.cases = case_list
        self.tune_files = tune_files
