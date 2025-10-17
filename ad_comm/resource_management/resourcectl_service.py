#!/usr/bin/python3
from glob import glob
import os
import logging
from typing import List, Optional, Tuple
import subprocess
import shutil
import signal

from scripts.cfg_dir_detect import detect_cfg_dir

SPECIFIC_CONFIG_MAP_FNAME = '/opt/senseauto_active/senseauto-config/resource_cfg/cfg_map.yaml'
FORCE_TARGETS = ['daemon_server']

logging.basicConfig(format='%(asctime)s %(levelname)-8s %(message)s',
                    level=logging.INFO,
                    datefmt='%Y-%m-%d %H:%M:%S')


def get_pids(name: str) -> List[int]:
    try:
        pids = [
            int(v) for v in subprocess.check_output(
                "ps ax | grep -v grep | grep %s | awk '{print $1}'" % name,
                shell=True).strip().split()
        ]
        logging.info('[process] %s: %s', name,
                     ', '.join([str(p) for p in pids]))
        return pids
    except subprocess.CalledProcessError as e:
        return []


def cgclassify(pid: int, group: Optional[str] = None) -> int:
    if group is None:
        cmd = 'cgclassify --sticky %d' % pid
    else:
        cmd = 'cgclassify --sticky -g %s %d' % (group, pid)
    logging.info('cmd: %s', cmd)
    return os.system(cmd)


def cgconfigparser(conf_file: str) -> int:
    return os.system('cgconfigparser -l %s' % conf_file)


class ConfigInstaller:

    def __init__(self, out_dir: str):
        self.out_dir = out_dir

    def config_file_pair(self, base_dir: str) -> Tuple[str, str]:
        return (os.path.join(base_dir, 'cgconfig.conf'),
                os.path.join(base_dir, 'cgrules.conf'))

    def install(self):
        if not os.path.exists(SPECIFIC_CONFIG_MAP_FNAME):
            raise Exception('Missing %s' % SPECIFIC_CONFIG_MAP_FNAME)

        car_resource_config_dir = detect_cfg_dir()
        if not os.path.exists(car_resource_config_dir):
            raise Exception('Missing cfg dir: %s' % car_resource_config_dir)
        cgconfig, cgrules = self.config_file_pair(car_resource_config_dir)

        logging.info('Selected: %s' % car_resource_config_dir)
        logging.info('Selected: %s' % cgconfig)
        logging.info('Selected: %s' % cgrules)

        out_cgconfig, out_cgrules = self.config_file_pair(self.out_dir)
        shutil.copyfile(cgconfig, out_cgconfig)
        shutil.copyfile(cgrules, out_cgrules)
        os.chmod(out_cgconfig, 644)
        os.chmod(out_cgrules, 644)


class CgrulesgendService:

    def __init__(self, conf_dir: str):
        self.conf_dir = conf_dir
        self.rule_file = os.path.join(conf_dir, 'cgrules.conf')
        self.existing_processes = self.__parse_rules(self.rule_file)
        # Auto remove all rule when exit
        signal.signal(signal.SIGINT, self.cleanup)
        signal.signal(signal.SIGTERM, self.cleanup)

    def __parse_rules(self, rule_file: str) -> List[Tuple[str, str]]:
        rules = []
        with open(rule_file, 'r') as f:
            for line in f:
                if line.startswith('%') or line.startswith('#'):
                    continue
                part = line.strip().split()
                if len(part) == 0 or ':' not in part[0]:
                    continue
                pname = part[0].split(':')[-1].strip()
                rule = '%s:%s' % (part[1].strip(), part[2].strip())
                rules.append((pname, rule))
        return rules

    def move_existing_processes(self, force_targets: List[str] = []):
        handled = set()
        for pname, rule in self.existing_processes:
            for pid in get_pids(pname):
                if pid in handled:
                    continue
                # for the target that cgclassify cannot auto move
                if pname in force_targets:
                    cgclassify(pid, rule)
                else:
                    cgclassify(pid)
                handled.add(pid)

    def start(self):
        if cgconfigparser(os.path.join(self.conf_dir, 'cgconfig.conf')) != 0:
            raise Exception('Failed in cgconfigparser')
        p = subprocess.Popen(['/usr/sbin/cgrulesengd', '-n'],
                             env=dict(os.environ),
                             stdout=subprocess.DEVNULL,
                             stderr=subprocess.DEVNULL)
        self.move_existing_processes(force_targets=FORCE_TARGETS)
        try:
            p.wait()
        except KeyboardInterrupt:
            return

    def cleanup(self, *args):
        logging.info('CleanUp')
        # Clean set_sched
        pids = get_pids('modify_sched.py')
        for pid in pids:
            os.kill(pid, signal.SIGTERM)

        # Clean cgroup
        targets = set()
        with open(self.rule_file, 'r') as f:
            for line in f:
                if line.startswith('#'):
                    continue
                part = line.strip().split()
                if len(part) != 3:
                    continue
                targets.add('%s:/%s' % (part[1], part[2].strip('/')))
        for target in targets:
            cmd = 'cgdelete -r %s 2> /dev/null' % target
            logging.info(cmd)
            os.system(cmd)


if __name__ == '__main__':
    logging.info('Start Service')
    dst_conf_dir = '/etc'

    ConfigInstaller(dst_conf_dir).install()
    service = CgrulesgendService(dst_conf_dir)
    service.start()
    logging.info('Exit Service')
    logging.info('--------------------------')
