#!/bin/bash

set -e

RSCLBAG_BIN=../rsclbag

tdir=$(mktemp -d)

run_master() {
    echo "255" > "$tdir/$BASHPID"
    $RSCLBAG_BIN rec_replay \
        --master \
        -c 127.0.0.1 \
        --no_sim_time \
        -d ./small_bag.00000.rsclbag
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
