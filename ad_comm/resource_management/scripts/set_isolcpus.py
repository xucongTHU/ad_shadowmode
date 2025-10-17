from typing import List
import json
import os

TARGET_FILE = '/etc/default/grub'
CONF_FILE = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                         '../conf/sched_conf.json')


def check_root():
    if os.getuid() != 0:
        raise Exception('Must run with root permission')


def update_grup_conf(isolcpus: List[int]):
    if not os.path.exists(TARGET_FILE):
        raise Exception('The %s file not exist' % TARGET_FILE)
    updated = False
    buffer = []
    isolcpus_str = ''
    if len(isolcpus) > 0:
        isolcpus_str = 'isolcpus=%s' % (','.join([str(w) for w in isolcpus]))
    with open(TARGET_FILE, 'r') as f:
        buffer = f.readlines()
    for idx, buf in enumerate(buffer):
        if buf.startswith('GRUB_CMDLINE_LINUX_DEFAULT='):
            default_v = buf.strip().split('=')[1].strip(' ="')
            default_v = ' '.join(
                [v for v in default_v.split() if not v.startswith('isolcpus')])
            buffer[idx] = 'GRUB_CMDLINE_LINUX_DEFAULT="%s %s"\n' % (
                default_v, isolcpus_str)
            updated = True
            break
    if not updated:
        raise Exception('Failed to update GRUB_CMDLINE_LINUX_DEFAULT')

    print(''.join(buffer))

    with open(TARGET_FILE, 'w') as f:
        f.writelines(buffer)
    os.system('update-grub')


if __name__ == '__main__':
    check_root()
    conf = json.load(open(CONF_FILE))
    update_grup_conf(conf['isolcpus'])
