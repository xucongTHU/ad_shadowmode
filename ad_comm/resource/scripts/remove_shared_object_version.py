import subprocess
import os
import shutil
from pathlib import Path
import sys


TARGET_DIR = "./"
SO_DIR = sys.argv[1]


def sys_run(command):
    return subprocess.check_output(command, shell=True, text=True)


replace_target_list = set([f for f in os.listdir(SO_DIR) if ".so" in f])


def replace_all_versioned_so_for_target(target_so):
    deps = sys_run("patchelf --print-needed {}".format(target_so))
    deps = [e.strip() for e in deps.split()]

    for dep in deps:
        if dep in replace_target_list and not dep.endswith(".so"):
            fixed_dep = dep.split(".so")[0] + ".so"
            # print("Replaced so for {}".format(dep))
            sys_run("patchelf --replace-needed {} {} {}".format(dep,
                    fixed_dep, target_so))

def is_elf_file(file_path):
    # ELF 文件的魔数
    elf_magic = b'\x7FELF'

    try:
        with open(file_path, 'rb') as f:  # 以二进制模式打开文件
            file_header = f.read(4)  # 读取前4个字节
            return file_header == elf_magic
    except IOError:
        return False  # 文件无法读取

def replace_all_versioned_so_for_dir(dirname):
    files = os.listdir(dirname)
    for f in files:
        if is_elf_file(f):
            replace_all_versioned_so_for_target(f)
            print("Replaced version so for {}".format(f))


def copy_versioned_so_to_unversion_name(target_so):
    if Path(target_so).is_symlink():
        return
    if target_so.endswith('.so'):
        return
    unver_target_so = target_so.split(".so")[0] + ".so"
    shutil.copyfile(target_so, unver_target_so)
    sys_run("patchelf --set-soname {} {}".format(unver_target_so, unver_target_so))
    os.remove(target_so)


def copy_versioned_so_for_dir(dirname):
    files = os.listdir(dirname)
    for f in files:
        if ".so" in f:
            copy_versioned_so_to_unversion_name(f)


replace_all_versioned_so_for_dir(TARGET_DIR)
copy_versioned_so_for_dir(TARGET_DIR)
