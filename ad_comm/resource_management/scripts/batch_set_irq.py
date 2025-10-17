#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import os
import sys

sys.dont_write_bytecode = True

import set_irq_interface

# change to read conf soon
TMP_CONFIG = {"free_cpu": "0-8", "load_cpu": "9-15"}


def batch_set_irq():
    load_cpu_ids = TMP_CONFIG['load_cpu']
    irq_map = set_irq_interface.get_irq_map()
    for key, _ in irq_map.items():
        if key == '0':
            continue
        set_irq_interface.bind_irq_by_id(key, load_cpu_ids)


if __name__ == '__main__':
    batch_set_irq()
