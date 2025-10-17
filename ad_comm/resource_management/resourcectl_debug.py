#!/usr/bin/python3
import sys
import os
from time import sleep
from typing import Dict, List
import logging
import subprocess
import psutil
from collections import OrderedDict

logging.basicConfig(format='%(asctime)s %(levelname)-8s %(message)s',
                    level=logging.INFO,
                    datefmt='%Y-%m-%d %H:%M:%S')


def parse_rules(rule_file: str) -> List[str]:
    pnames = []
    with open(rule_file, 'r') as f:
        for line in f:
            if line.startswith('%') or line.startswith('#'):
                continue
            part = line.strip().split()
            if len(part) == 0:
                continue
            if ':' not in part[0]:
                continue
            pname = part[0].split(':')[-1].strip()
            pnames.append(pname)
    return pnames


def pids_from_process_name(name: str) -> List[int]:
    try:
        pids = [
            int(v) for v in subprocess.check_output(
                "ps ax | grep -v grep | grep %s | awk '{print $1}'" % name,
                shell=True).strip().split()
        ]
        return pids
    except subprocess.CalledProcessError as e:
        return []


def print_process_cgroup(process_name: str, pids: List[int],
                         subsystem: List[str]):
    logging.info('[Process]: %s', process_name)
    for pid in pids:
        cgroup_file = '/proc/%d/cgroup' % pid
        if not os.path.exists(cgroup_file):
            logging.warning(
                '  |- [PID]: %d - Cannot find cgroup. Maybe process is exited',
                pid)
            continue
        logging.info('  |- [PID]: %d', pid)
        with open(cgroup_file, 'r') as f:
            for line in f:
                eles = line.split(':')
                for s in eles[1].split(','):
                    if s in subsystem:
                        logging.info('    |-- %s', line.strip())
                        break


def get_process_maps(pids: List[int]) -> Dict[int, psutil.Process]:
    out = {}
    for pid in pids:
        try:
            p = psutil.Process(pid)
            out[pid] = p
            p.cpu_percent(interval=None)
        except psutil.NoSuchProcess:
            logging.warning(
                '  |- [PID]: %d - Cannot find cgroup. Maybe process is exited',
                pid)
    return out


def print_process_running_cpuset(process_name: str, pids: List[int],
                                 process_maps: Dict[int, psutil.Process]):
    logging.info('[Process]: %s', process_name)
    for pid in pids:
        if pid not in process_maps:
            continue
        p = process_maps[pid]
        stat_file = '/proc/%d/stat' % pid
        with p.oneshot():
            rss = round(p.memory_full_info().rss / 1024.0 / 1024.0, 2)
            cpu_percent = p.cpu_percent(interval=None)

        # https://man7.org/linux/man-pages/man5/proc.5.html
        with open(stat_file, 'r') as f:
            content = f.readline()
            parts = content.split()
            processerid = parts[38]
            logging.warning(
                '  |- [PID]: %6d - cpu id: %2s, cpu percent: %5.2f, rss: %7.2f MB, ',
                pid, processerid, cpu_percent, rss)


if __name__ == '__main__':
    if len(sys.argv) != 2:
        logging.error('Usage: python %s path/to/cgrules.conf', sys.argv[0])
        exit(1)

    # Get pids
    process_names = parse_rules(sys.argv[1])
    pid_maps = OrderedDict()
    for pn in process_names:
        pids = pids_from_process_name(pn)
        if len(pids) == 0:
            logging.warn('Process: %s is not running', pn)
            continue
        pid_maps[pn] = pids

    # Print Assigned cgroup
    logging.info('------------- [cgroup] -------------')
    for pname, pids in pid_maps.items():
        print_process_cgroup(pname, pids, ['cpuset', 'cpu', 'memory'])

    # Print running cpuset
    logging.info('------------- [running cpuset] -------------')
    allpids = []
    for pids in pid_maps.values():
        allpids.extend(pids)
    process_maps = get_process_maps(allpids)
    # Ensure the stat of the process have 1s interval
    sleep(1)
    for pname, pids in pid_maps.items():
        print_process_running_cpuset(pname, pids, process_maps)
