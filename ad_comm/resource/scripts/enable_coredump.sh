#!/bin/sh
rm -rf $CORE_DUMP_PATH
mkdir -p $CORE_DUMP_PATH
sysctl -w kernel.core_pattern=$CORE_DUMP_PATH/core.%e.%p.%s.%E.%t > /dev/null 2>&1
sysctl -w kernel.core_uses_pid=1 > /dev/null 2>&1
ulimit -c unlimited > /dev/null 2>&1

ulimit -a > /dev/null 2>&1
echo "coredump generate path is $CORE_DUMP_PATH !!"