#!/bin/sh

with_perf=false
with_stress=false
specify_case=
total_timeout=0
default_case_timeout=60
enable_tuner=0
start_time=$(date +%s)

# Init Constant
if [ -z ${SENSEAUTO_INSTALLED_ROOT} ]; then
    export SENSEAUTO_INSTALLED_ROOT="/opt/senseauto_active"
fi

PID_FILE="/tmp/benchmark_launcher.pid"
RSCL_DIR="${SENSEAUTO_INSTALLED_ROOT}/senseauto-rscl"
BENCHMARK_DIR="${RSCL_DIR}/benchmark"
EXEC_BIN_DIR="${RSCL_DIR}/bin"
LAUNCHER_EXE_BIN="${EXEC_BIN_DIR}/benchmark_launcher"
TUNER_BIN="${EXEC_BIN_DIR}/autotuner"
DUMMY_MSG_PUBLISHER_EXE_BIN="${EXEC_BIN_DIR}/dummy_msg_publisher"
CASE_DIR="${BENCHMARK_DIR}/resource/cases/mdc"

OUT_DIR="/tmp/rscl_benchmark_out"

STAT_DIR="${OUT_DIR}/stat"

perf_transport() {
    # set perf permission
    echo 0 > /proc/sys/kernel/kptr_restrict

    target_dir=$1
    process_count=$(cat ${PID_FILE} | wc -l)
    if [ $process_count -gt 2 ]; then
        echo -e "\e[31m perf not support more than 2 process, will only perf preceding few. \e[0m"
    fi
    while IFS=',' read -r process_name pid setup_file
    do
        perf_stat() {
            perf stat \
            -e task-clock,cycles,instructions,cache-references,cache-misses,branches,faults \
            -p ${pid} -o ${target_dir}/perf_stat.${process_name}_${pid} sleep 10
        }

        perf_record() {
            perf record -F 900 -g --call-graph fp -p ${pid} sleep 10 2>&1 >> /dev/null
        }

        if [ -e /proc/$pid ]; then
            perf_stat
            perf_record
            sync
            perf script -i $(pwd)/perf.data > ${target_dir}/perf_data.${process_name}_${pid}.unfold
            rm -rf $(pwd)/perf.data
        fi
    done < ${PID_FILE}
}

cleanup() {
    while IFS=',' read -r process_name pid setup_file
    do
        if [ -e /proc/$pid ]; then
            (sleep 1; kill -SIGINT $pid) &
            while [ -e /proc/$pid ]
            do
                echo "Process: $pid is still running"
                sleep .6
            done
        fi
    done < ${PID_FILE}
}

sumup_timeout(){
    for file in `ls -a $1`
    do
        if [ -d $1"/"$file ]
        then
            if [[ $file != '.' && $file != '..' ]]
            then
                sumup_timeout $1"/"$file
            fi
        else
            local case_file=$1"/"$file
            if [ -z "$specify_case" ] || ( [[ -n "$specify_case" && -n "$case_file" ]] && [[ "$case_file" =~ $specify_case ]] ); then
                local max_timeout=$(cat ${case_file} | grep timeout_s | cut -d : -f 2 | sort -rn | head -n 1)
                if [ -z "$max_timeout" ]; then
                    max_timeout=${default_case_timeout}
                fi
                total_timeout=$((max_timeout+total_timeout+5))
            fi
        fi
    done
}

# Basic 1_simple.yaml
simple_benchmark() {
    local case_file=$1
    local case_name=$2

    local STAT_CASE_DIR=${STAT_DIR}/${case_name}

    if [ -z "$specify_case" ] || ( [[ -n "$specify_case" && -n "$case_file" ]] && [[ "$case_file" =~ $specify_case ]] ); then
        # execute bin
        mkdir -p ${STAT_CASE_DIR}
        ${LAUNCHER_EXE_BIN} ${case_file} &
        sleep 5

        # calculate rest time and sleep
        local max_timeout=$(cat ${case_file} | grep timeout_s | cut -d : -f 2 | sort -rn | head -n 1)
        local process_count=$(cat ${PID_FILE} | wc -l)
        if [ "$with_perf" = true ] ; then
            mkdir -p ${STAT_CASE_DIR}/perf
            perf_transport ${STAT_CASE_DIR}/perf
        else
            process_count=0
        fi

        if [ ! -z "$max_timeout" ]; then
            max_timeout=$((max_timeout-20*process_count))
            if [ $max_timeout -lt 0 ]; then
                max_timeout=1
            fi
        else
            max_timeout=${default_case_timeout}
        fi
        local rest_time=${max_timeout}
        while [ $rest_time -ge 0 ]
        do
            if [ $((rest_time % 10)) -eq 0 ]; then
                curr_time=$(date +%s)
                cost_time=$((curr_time - start_time))
                echo -e "\033[33m Running case $case_name, estimated $((total_timeout-cost_time)) seconds left \033[0m"
            fi
            sleep 1
            rest_time=$((rest_time-1))
        done

        cleanup

        # collect log files to corresponding dir
        while IFS=',' read -r process_name pid config_file
        do
            mv /tmp/*.$pid.txt ${STAT_CASE_DIR}
            mv /tmp/*.$pid.log ${STAT_CASE_DIR}
            cp $config_file ${STAT_CASE_DIR}/test_setup.yaml.$pid
        done < ${PID_FILE}
    fi
}

pressure_benchmark() {
    local case_file=$1

    if [ -z "$specify_case" ] || ( [[ -n "$specify_case" && -n "$case_file" ]] && [[ "$case_file" =~ $specify_case ]] ); then
        ${BENCHMARK_DIR}/bin/cpu_stress 90 &
        # ${BENCHMARK_DIR}/bin/network_stress 30 &
        ${BENCHMARK_DIR}/bin/io_stress 5 &

        simple_benchmark $@

        pkill -f cpu_stress >/dev/null 2>&1 
        # pkill -f network_stress >/dev/null 2>&1 
        pkill -f io_stress >/dev/null 2>&1 
    fi
}

get_environment() {
    cat /proc/cpuinfo > ${OUT_DIR}/cpuinfo
    cat /proc/meminfo > ${OUT_DIR}/meminfo
    uname -a > ${OUT_DIR}/uname
    basename $(readlink -f /opt/senseauto/senseauto-rscl/active) > ${OUT_DIR}/rscl_version
}

run_benchmark() {
    local prefix=$1

    # basic
    simple_benchmark ${CASE_DIR}/basic/1_200hz_20Bytes.yaml                      ${prefix}1_200hz_20Bytes
    simple_benchmark ${CASE_DIR}/basic/1_200hz_20Bytes_1x5.yaml                  ${prefix}1_200hz_20Bytes_1x5
    simple_benchmark ${CASE_DIR}/basic/2_125hz_100Bytes.yaml                     ${prefix}2_125hz_100Bytes
    simple_benchmark ${CASE_DIR}/basic/3_30hz300KBytes.yaml                      ${prefix}3_30hz300KBytes
    simple_benchmark ${CASE_DIR}/basic/4_10hz800KBytes.yaml                      ${prefix}4_10hz800KBytes
    simple_benchmark ${CASE_DIR}/basic/5_1hz_3MBytes.yaml                        ${prefix}5_1hz_3MBytes

    # With pressure
    if [ "$with_stress" = true ] ; then
        pressure_benchmark ${CASE_DIR}/basic/1_200hz_20Bytes.yaml                      ${prefix}1_200hz_20Bytes_pressure
        pressure_benchmark ${CASE_DIR}/basic/1_200hz_20Bytes_1x5.yaml                  ${prefix}1_200hz_20Bytes_1x5_pressure
        pressure_benchmark ${CASE_DIR}/basic/2_125hz_100Bytes.yaml                     ${prefix}2_125hz_100Bytes_pressure
        pressure_benchmark ${CASE_DIR}/basic/3_30hz300KBytes.yaml                      ${prefix}3_30hz300KBytes_pressure
        pressure_benchmark ${CASE_DIR}/basic/4_10hz800KBytes.yaml                      ${prefix}4_10hz800KBytes_pressure
        pressure_benchmark ${CASE_DIR}/basic/5_1hz_3MBytes.yaml                        ${prefix}5_1hz_3MBytes_pressure
    fi
}

extra_tuner() {
    mkdir -p ${STAT_DIR}/tuner/
    ${TUNER_BIN} -s -t /benchmark_channel &
    sub_pid=$!
    ${TUNER_BIN} -p -t /benchmark_channel
    wait "${sub_pid}"
    mv /tmp/transport_tuner_${sub_pid}.log ${STAT_DIR}/tuner/
}

#
main() {
    rm -rf ${OUT_DIR}
    mkdir ${OUT_DIR}

    sumup_timeout ${CASE_DIR}
    if [ "$with_stress" = true ] ; then
        total_timeout=$((total_timeout*2))
    fi
    echo "Estimate total test time is ${total_timeout}s"
    get_environment

    # normal
    run_benchmark ""

    # tuner
    if [ $enable_tuner -eq 1 ]; then
        extra_tuner
    fi

    # python3 /opt/senseauto/senseauto-rscl/active/benchmark/scripts/post_draw/benchmark_analyser.py -s ${OUT_DIR}

    echo "please scp ${OUT_DIR} to ego x86-pc and run 'python3 /opt/senseauto/senseauto-rscl/active/benchmark/scripts/post_draw/benchmark_analyser.py -s \${benchmark_out_dir}'"
}

function HelpMessage() {
cat << EOF
benchmark_mdc.sh -- Script for benchmark rscl transportation

Usage:
    [-h|--help]         Show the help message
    [-p|--perf]         Start with perf
    [-s|--stress]       Add additional cpu&&network stress running case
    [-l|--list]         List available cases
    [-c|--case]         Specify running case(mdc/basic/1_200hz_20Bytes.yaml)
    [-t|--tuner]        Run autotuner for range hz/msg_size test, 1 enable, 0 disable(default)

Example:
    bash benchmark_mdc.sh -c mdc/basic/1_200hz_20Bytes.yaml -s
This command will run benchmark using mdc/basic/1_200hz_20Bytes.yaml, with stress tool, no perf
EOF
}

function ListCases() {
    local target_dir=$1
    curr_dir=`pwd`
    cd ${BENCHMARK_DIR}/resource/cases
    find ${target_dir} -name "*.yaml"
    cd $curr_dir
}

## Arguments parse
ARGS=$(getopt -o hpslc: --long help,perf,stress,list,case: -n `basename $0` -- "$@")
if [ $? != 0 ]; then ErrorLog " * Failed to parse arguments. $@"; exit 1; fi
eval set -- "${ARGS}"
while true; do
    case $1 in
        -h|--help)
            HelpMessage; exit 0                     ; shift    ;;
        -p|--perf)
            with_perf=true                          ; shift    ;;
        -s|--stress)
            with_stress=true                        ; shift    ;;
        -l|--list)
            ListCases mdc; exit 0                   ; shift    ;;
        -c|--case)
            specify_case=$2                         ; shift 2  ;;
        -t|--tuner)
            shift; enable_tuner="$1" ;;
        --)
            shift; break ;;
        *)
            ErrorLog " * Invalid arguments"; exit 1 ; shift    ;;
    esac
done

main

exit 0
