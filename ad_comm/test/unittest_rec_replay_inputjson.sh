#!/bin/bash

set -e

RSCLBAG_BIN=../rsclbag

tdir=$(mktemp -d)

run_master() {
    echo "255" > "$tdir/$BASHPID"
    export DAEMON_slave_ip=127.0.0.1
    $RSCLBAG_BIN rec_replay \
        --master
    echo "$?" > "$tdir/$BASHPID"
}

run_client() {
    echo "255" > "$tdir/$BASHPID"
    $RSCLBAG_BIN rec_replay \
        --no_sim_time
    echo "$?" > "$tdir/$BASHPID"
}

timeout_killer() {
    sleep 15
    pkill -9 rsclbag
}

sudo rm -rf /tmp/input.json
cp input.json /tmp/input.json
run_master &
run_client &
timeout_killer &

wait

for file in "$tdir"/*; do
    printf 'PID %d returned %d\n' "${file##*/}" "$(<"$file")"
    if [ "$(<"$file")" != "0" ]; then
        exit 1
    fi
done
