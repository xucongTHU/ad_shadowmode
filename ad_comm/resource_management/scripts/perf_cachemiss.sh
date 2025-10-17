#!/bin/bash

echo "Usage: bash $0 TARGET_NODE_BINARY_NAME (example: bash $0 ros_control_node)"

perf stat \
    -e cache-misses,cache-references,instructions,cycles,L1-dcache-stores,L1-dcache-store-misses \
    -p $(pidof $1) | tee $1.cache_miss.log
