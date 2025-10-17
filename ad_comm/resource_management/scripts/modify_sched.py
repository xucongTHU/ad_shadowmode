#!/usr/bin/python3

import os
import sys
import json
import logging
import subprocess
import time
from typing import List

from cfg_dir_detect import detect_cfg_dir

CONF_FILE = 'sched_conf.json'

logging.basicConfig(format='%(asctime)s %(levelname)-8s %(message)s',
                    level=logging.INFO,
                    datefmt='%Y-%m-%d %H:%M:%S')


def get_pids(name: str) -> List[int]:
    try:
        pids = [
            int(v) for v in subprocess.check_output(
                "ps ax | grep -v grep | grep -w %s | awk '{print $1}'" % name,
                shell=True).strip().split()
        ]
        logging.info('[process] %s: %s', name,
                     ', '.join([str(p) for p in pids]))
        return pids
    except subprocess.CalledProcessError as e:
        return []


def set_nice(pid: int, nice_val: int):
    ret = os.system('renice -n %d -p %d' % (nice_val, pid)) == 0
    assert ret, 'Set nice value failed'


def set_fifo(pid: int):
    # Don't set to 99. Avoid soft lockup
    ret = os.system('chrt -f -a -p 98 %d' % (pid)) == 0
    assert ret, 'Set sched_fifo value failed'


def modify_sched(conf: dict) -> bool:
    missing = 0
    # Set nice
    for k, v in conf['priority_process'].items():
        pids = get_pids(k)
        if len(pids) == 0:
            missing += 1
        for pid in pids:
            set_nice(pid, v)

    for k in conf['realtime_process']:
        pids = get_pids(k)
        if len(pids) == 0:
            missing += 1
        for pid in pids:
            set_fifo(pid)
    print(missing)
    return missing == 0


if __name__ == '__main__':
    waited_time = 0
    timeout = 0
    if len(sys.argv) >= 2:
        timeout = int(sys.argv[1])

    cfg_file = os.path.join(detect_cfg_dir(), CONF_FILE)
    if not os.path.exists(cfg_file):
        raise Exception('Missing cfg file: %s' % cfg_file)

    logging.info('conf_file: %s', cfg_file)
    conf = json.load(open(cfg_file))

    while True:
        if modify_sched(conf):
            break
        if waited_time >= timeout:
            logging.warning('Timeout')
            break
        tunit = min(5, timeout - waited_time)
        time.sleep(tunit)
        waited_time += tunit
