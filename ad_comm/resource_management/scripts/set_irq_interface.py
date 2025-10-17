#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import os
import re
import sys
import json
import psutil
from subprocess import Popen, PIPE
from pprint import PrettyPrinter

BASE_CONFIG_DIR = '/opt/senseauto_active/senseauto-rscl/resource_management/conf'
IRQ_ORIGIN_CONF_PATH = os.path.join(BASE_CONFIG_DIR, 'irq_origin_conf.json')


def first_int(x):
    return int(x.split('-')[0])


def read(path):
    return open(path).read()


def popen(cmd):
    return Popen(cmd, shell=True, stdout=PIPE).communicate()[0]


def execute(cmd):
    print('%s # result %s' % (cmd, popen(cmd)))


def get_irq_map():
    return dict(
        re.findall('^ *(\d+):[0-9 ]+(.+)', read('/proc/interrupts'), re.M))


def get_irq_list(pat):
    return [
        no[0] for no in re.findall('^ *(\d+):.*(%s).*' %
                                   (pat), read('/proc/interrupts'), re.M)
    ]


def bind(irq, cpu_id):
    return 'echo %s > /proc/irq/%s/smp_affinity_list' % (cpu_id, irq)


def __check_cpu_input(spec):
    cpu_num = psutil.cpu_count()
    if spec.isdigit() and int(spec) < cpu_num:
        return spec
    elif '-' in spec:
        cpu_list = spec.split('-')
        if len(cpu_list) == 2 and int(cpu_list[0]) < int(
                cpu_list[1]) < cpu_num:
            return spec
    elif ',' in spec:
        cpu_list = spec.split(',')
        if all(i.isdigit() and int(i) < cpu_num for i in cpu_list):
            return spec
    print("Invalid format or exceed max cpu count:%s" % spec)
    print("Example format: 1 || 2,3,5 || 3-7")
    return None


def __check_irq_save():
    if not (os.path.exists(IRQ_ORIGIN_CONF_PATH)
            and os.path.getsize(IRQ_ORIGIN_CONF_PATH) != 0):
        raise IOError("Can't save origin irq conf!")
    return True


def save_irq_conf():
    if not os.path.exists(IRQ_ORIGIN_CONF_PATH):
        with open(IRQ_ORIGIN_CONF_PATH, 'w') as f:
            json.dump(show_all_hw_irqs(), f)
    # double check
    __check_irq_save()


def bind_irq(pat, cpu_list):
    __check_irq_save()
    cpu_list = __check_cpu_input(cpu_list)
    if cpu_list is not None:
        for irq in get_irq_list(pat):
            execute(bind(irq, cpu_list))


def bind_irq_by_id(hw_id, cpu_list):
    __check_irq_save()
    cpu_list = __check_cpu_input(cpu_list)
    if cpu_list is not None:
        execute(bind(hw_id, cpu_list))


def print_irq_info():
    irq_name = get_irq_map()
    pp = PrettyPrinter(indent=4)
    print("-----------------irq dict-----------------")
    pp.pprint(irq_name)
    print("------------------------------------------")
    return irq_name


def show_irq_bind(pat):
    mask_count = {}
    irq_name = get_irq_map()
    for irq in get_irq_list(pat):
        mask = read('/proc/irq/%s/smp_affinity_list' % (irq)).strip()
        print('%s %s %s' % (irq, mask, irq_name[irq]))
        if mask in mask_count:
            mask_count[mask] += 1
        else:
            mask_count[mask] = 1
    mask_count = sorted(mask_count.items(), key=lambda x: first_int(x[0]))
    print(' '.join('%s/%s' % (k, v) for k, v in mask_count))


def show_all_hw_irqs():
    irq_dict = {}
    irq_name = get_irq_map()
    for key, _ in irq_name.items():
        cpu_affinity = read('/proc/irq/%s/smp_affinity_list' % (key)).strip()
        irq_dict[key] = cpu_affinity
    pp = PrettyPrinter(indent=4)
    pp.pprint(irq_dict)
    return irq_dict


def reset():
    if os.path.exists(IRQ_ORIGIN_CONF_PATH):
        with open(IRQ_ORIGIN_CONF_PATH) as f:
            irq_data = json.load(f)
            for key, value in irq_data.items():
                if key == '0':
                    continue
                bind_irq_by_id(key, value)
    else:
        raise IOError("Can't find origin irq conf!")


def parse_cmd_args(args):
    return [i for i in args if not re.match('^\w+=', i)
            ], dict(i.split('=', 1) for i in args if re.match('^\w+=', i))


if __name__ == '__main__':
    len(sys.argv) > 1 or sys.exit(1)
    func = globals().get(sys.argv.pop(1))
    callable(func) or sys.exit(2)
    args, kw = parse_cmd_args(sys.argv[1:])
    func(*args, **kw)
