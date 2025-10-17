#!/usr/bin/python3
import os
import logging
from typing import Optional, Tuple
import socket
import yaml

SPECIFIC_CONFIG_DIR = '/opt/senseauto_active/senseauto-config/resource_cfg'
SPECIFIC_CONFIG_MAP_FNAME = '{}/cfg_map.yaml'.format(SPECIFIC_CONFIG_DIR)
VEHICLE_LIST_FNAME = '/opt/senseauto_active/senseauto-vehicle-config/sensor_scheme/vehicle_list.yaml'
PLATFORM_FDIR = '/opt/senseauto_active/senseauto-vehicle-config/sensor_scheme/'
PLATFORM_FNAME = 'ipc.yaml'
DAEMON_LAUNCH_ENV = '/tmp/launcher_cache/cache/launch_env.bash'


def detect_carid() -> str:
    return socket.gethostname().strip()


def detect_platform_sensor_scheme(carid: str) -> Optional[Tuple[str, str]]:
    if not os.path.exists(VEHICLE_LIST_FNAME):
        raise Exception('vehicle_list.yaml not exist')
    with open(VEHICLE_LIST_FNAME, 'r') as f:
        conf_dict = yaml.safe_load(f)
        if carid not in conf_dict:
            logging.error('Failed to find car id: [%s] in vehicle_list.yaml',
                          carid)
            return None
        sensor_scheme = conf_dict[carid]

    ipc_yaml_fname = os.path.join(PLATFORM_FDIR, sensor_scheme, PLATFORM_FNAME)
    logging.info('ipc yaml: %s', ipc_yaml_fname)
    if not os.path.exists(ipc_yaml_fname):
        logging.error('%s not exist', ipc_yaml_fname)
        return None
    with open(ipc_yaml_fname, 'r') as f:
        conf_dict = yaml.safe_load(f)
        platform = conf_dict['platform']

    return (platform, sensor_scheme)


def detect_cfg_dir() -> str:
    # Handle specific cfg in launcer config
    if os.path.exists(DAEMON_LAUNCH_ENV):
        with open(DAEMON_LAUNCH_ENV, 'r') as f:
            for line in f:
                line = line.strip()
                if line.startswith('export launcher_resource_cfg_dir='):
                    cfg_dir = line.split('=')[1]
                    return cfg_dir
    # Auto detect
    carid = detect_carid()
    platform_scheme_pair = detect_platform_sensor_scheme(carid)
    with open(SPECIFIC_CONFIG_MAP_FNAME, 'r') as f:
        conf_dict = yaml.safe_load(f)
        if platform_scheme_pair is None:
            cfg_dir = conf_dict['default']
        else:
            platform, scheme = platform_scheme_pair
            if scheme not in conf_dict[platform]:
                cfg_dir = conf_dict[platform]['default']
            else:
                cfg_dir = conf_dict[platform][scheme]
    return os.path.join(SPECIFIC_CONFIG_DIR, cfg_dir)
