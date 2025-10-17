#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import os
import sys
import argparse
import numpy as np

from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


# specializaion
def plot_tuner_profile_csv(file_path: str):
    if not os.path.exists(file_path):
        print("file {} not exist!".format(file_path))
    data = np.genfromtxt(file_path, dtype=None, delimiter=',', names=True)

    draw_label = ["publish_avg_ms", "latency_avg_ms"]

    for label in draw_label:
        fig = plt.figure(dpi=150, figsize=(24, 16))
        fig.suptitle(label + " ranges for pub hz and msg size", fontsize = 12)
        ax = plt.axes(projection='3d')

        ax.plot_trisurf(data["hz"],
                        data["msg_bytes"],
                        data[label],
                        linewidth=1,
                        antialiased=True,
                        edgecolor='Gray',
                        cmap='viridis')
        ax.set_xlabel('hz', fontsize=18)
        ax.set_ylabel('msg_bytes', fontsize=18)
        ax.set_zlabel(label, fontsize=18)

    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description=
        'view 3d figure for tuner',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-s',
                        '--source',
                        dest='source',
                        help='specify result log source dir',
                        default="/tmp/rscl_benchmark_out",
                        required=False)
    args = parser.parse_args()

    tune_log_dir = os.path.join(args.source, "stat", "tuner")
    if not os.path.exists(tune_log_dir):
        print("Not a valid dir: {}".format(tune_log_dir))
        exit(-1)
    for file in os.listdir(tune_log_dir):
        if "transport_tuner_" in file:
            tune_file = os.path.join(tune_log_dir, file)

    plot_tuner_profile_csv(tune_file)
