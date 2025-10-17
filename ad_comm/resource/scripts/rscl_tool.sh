#!/bin/bash

################ envs ################
declare -a board_name=()
declare -a board_ip=()
declare -a board_password=()
pc_ip=$(ifconfig -a|grep -v docker|grep inet|grep -v 127.0.0.1|grep -v inet6|grep -v 172.|awk '{print $2}'|tr -d "addr:"|head -n1)
pc_hostname=
# pc workspace 固定这个
pc_workspace="/opt/senseauto_active"
# config 保存目录信息
config_file="/opt/rscltool_config.yaml"
if [[ ${SENSEAUTO_RSCL_CONF_DIR} ]]; then
    config_file="${SENSEAUTO_RSCL_CONF_DIR}/rscltool_config.yaml"
fi
# 可以添加执行板端命令前的source脚本
cmd_prefix=

##alias
shopt -s expand_aliases
alias log_info="echo -e '$(date +%Y-%m-%d-%H:%M:%S) \033[32m[I]\033[0m' "
alias log_warn="echo -e '$(date +%Y-%m-%d-%H:%M:%S) \033[33m[W]\033[0m' "
alias log_err="echo -e '$(date +%Y-%m-%d-%H:%M:%S) \033[31m[E]\033[0m' "
alias wrong_param_exit='wpe() { log_err param error in line ${1}; usage; exit 1; }; wpe'

##install sshpass
if [ "$(which sshpass)" = "" ]; then
    log_info "installing sshpass..."
    sudo apt install sshpass -f
fi

################ function start ################

function dynamic_config() {
    name=$1
    default_value=$(eval echo '$'${name})
    echo_tip=$name
    if [ -n "$2" ]; then
        echo_tip="$2"
    fi
    eval "read -p \"please input $echo_tip(default: ${default_value}): \" $name"
    if [ -z "$(eval echo '$'${name})" ]; then
        eval "${name}=$default_value"
    fi
    log_info  $(format_color_string "$echo_tip: $(eval echo '$'${name})")
}

function config_all() {
    echo "Configuration file not found. Creating new configuration..."
    echo "Attention! You should only config board info which pc will send data to"

    # Prompt user for hostname and host IP
    read -p "Enter hostname(default: ${pc_hostname}): " hostname
    if [ -z $hostname ]; then
        hostname=$pc_hostname
    fi
    read -p "Enter host IP address(default: ${pc_ip}): " host_ip
    if [ -z $host_ip ]; then
        host_ip=$pc_ip
    fi

    # Prompt user for board names and IP addresses
    boardinfo=()
    while true; do
        read -p "Enter board name: " board_name_local
        read -p "Enter board IP address: " board_ip_local
        read -p "Enter board password: " board_password_local
        boardinfo+=("$board_name_local:$board_ip_local:$board_password_local")
        read -p "Do you want to add another board? (y/n) " yn
        case $yn in
            [Nn]* ) break;;
        esac
    done
    read -p "Enter command prefix (would executed on board before each command): " command_pre
    
    # Save configuration to YAML file
    remove_current_config
    echo "hostname: $hostname" | sudo tee -a "$config_file"
    echo "host_ip: $host_ip" | sudo tee -a "$config_file"
    echo "boardinfo:" | sudo tee -a "$config_file"
    for i in "${boardinfo[@]}"; do
        board_name_local="${i%%:*}"
        board_pass_local="${i#*:}"
        board_ip_local="${board_pass_local%:*}"
        board_password_local="${board_pass_local#*:}"
        echo "  - name: $board_name_local" | sudo tee -a "$config_file"
        echo "    ip: $board_ip_local" | sudo tee -a "$config_file"
        echo "    password: $board_password_local" | sudo tee -a "$config_file"
    done
    echo "cmd_prefix: $command_pre" | sudo tee -a "$config_file"

    echo "Configuration saved to $config_file"
}

function load_config() {
    if [ -f "${config_file}" ]; then
        echo "Reading config from $config_file"
        # Read config file
        pc_hostname=$(awk '/^hostname:/ {print $2}' "$config_file" | tr -d '"')
        pc_ip=$(awk '/^host_ip:/ {print $2}' "$config_file" | tr -d '"')
        board_name=($(grep " name" $config_file | awk '{print $3}'))
        board_ip=($(grep " ip" $config_file | awk '{print $2}'))
        board_password=($(grep " password" $config_file | awk '{print $2}'))
    fi
}

function echo_current_config() {
    if [ -f "${config_file}" ]; then
        cat $config_file
    else
        log_warn "/opt/rscltool_config.yaml doesn't exist, run \"rscl_tool config\" command"
        echo "Current configuration is:"
        echo "  {"
        echo "      \"hostname\": \"${pc_hostname}\","
        echo "      \"host_ip\": \"${pc_ip}\","
        for ((i=0; i<${#board_name[@]}; i++)); do
        echo ""
        echo "            \"board_name\": \"${board_name[$i]}\","
        echo "            \"board_ip\": \"${board_ip[$i]}\","
        echo "            \"board_passward\": \"${board_password[$i]}\","
        done
        echo "  }"
    fi
}

function remove_current_config() {
    if [ -f "${config_file}" ]; then
        sudo mv $config_file /tmp/
    fi
}

function analyze_zmq_check() {
    local ip=""
    local result=""
    local success=""
    local fail=""
    while IFS= read -r line; do
    if [[ $line =~ Executing\ command\ on\ ([^@]+)@([^[:space:]]+) ]]; then
        if [[ ! -z $ip ]]; then
        if [[ ! -z $success ]]; then
            result+="$ip: Zmq server is already running. "
            result+=$'\n'
        else
            result+="$ip: Zmq server is not running! Try run command: rscl_tool start "
            result+=$'\n'
        fi
        success=""
        fail=""
        fi
        ip="${BASH_REMATCH[1]}@${BASH_REMATCH[2]}"
    elif [[ $line =~ rscl_polyfill_core ]]; then
        success="true"
    elif [[ $line =~ comand\ run\ on\ [^[:space:]]+\ failed\ with\ [0-9]+ ]]; then
        fail="true"
    fi
    done <<< "$1"
    if [[ ! -z $ip ]]; then
    if [[ ! -z $success ]]; then
        result+="$ip: Zmq server is already running. "
    else
        result+="$ip: Zmq server is not running! Try run command: rscl_tool start "
    fi
    fi
    echo "$result"
}

function run_on_board() {
    cmd=$1
    show_ret="${2:-false}"
    into_docker="${3:-true}"

    # log_scope "run_on_board" "start"
    # Use sshpass to execute command on each board
    ret=0
    for ((i=0; i<${#board_name[@]}; i++)); do
        # for L4 hmi-pc compatible. todo: replace this hardcode with sshpass docker check
        if [[ "${board_name[$i]}" == "sensetime" && "$into_docker" = true ]]; then
            cmd="docker exec -d unified-senseauto bash -ic \"$1\""
        else
            cmd=$(echo "$cmd" | sed "s/\\\\\\$/$/g")
        fi
        if [ ! -z $cmd_prefix ]; then
            cmd="${cmd_prefix};${cmd}"
        fi
	    log_info "Executing command on ${board_name[$i]}@${board_ip[$i]}"
        if ${show_ret}; then
	        sshpass -p "${board_password[$i]}" ssh -o StrictHostKeyChecking=no "${board_name[$i]}"@"${board_ip[$i]}" "${cmd}"
        else
            sshpass -p "${board_password[$i]}" ssh -o StrictHostKeyChecking=no "${board_name[$i]}"@"${board_ip[$i]}" "${cmd}" > /dev/null 2>&1
        fi
	    ssh_ret=$?
        if [ $ssh_ret -ne 0 ]; then
            log_err "comand run on ${board_ip[$i]} failed with $ssh_ret!!!"
            ret=1||ret
        fi
        sleep 1
    done
    # log_scope "run_on_board" "end"
    return $ret
}

function usage() {
    echo "Usage: $0 [COMMAND]"
    echo "Commands: "
    echo "      config              guide you to config the environment, or echo"
    echo "                          pattern: $0 config [option]"
    echo "                          example: $0 config"
    echo "                          example(echo current config): $0 config info"
    echo ""
    echo "      start               start rscl related process(rscl_polyfill_core) in both host and remote"
    echo "                          pattern: $0 start [option]"
    echo "                          example: $0 start"
    echo "                          example(set server exclusive): $0 start -e"
    echo ""
    echo "      stop                stop rscl related process(rscl_polyfill_core) in both host and remote"
    echo "                          pattern: $0 stop"
    echo "                          example: $0 stop"
    echo ""
    echo "      check               check rscl related process status in both host and remote"
    echo "                          pattern: $0 check [option]"
    echo "                          example(check zmq): $0 check zmq"
}

################ function end ################

# locad configuration
load_config

##operation
operation=$1
shift
if [[ "${operation}" =~ ^"config" ]]; then
    if [[ "$1" == "info" ]]; then
        echo_current_config
    else
        config_all
        load_config
    fi
elif [[ "${operation}" =~ ^"start" ]]; then
    start_zmq_cmd="LOG_path=/tmp/ LOG_level=info rscl_polyfill_core > /dev/null 2>&1 < /dev/null &"
    eval $start_zmq_cmd
    if [[ "$1" == "-e" ]]; then
        start_zmq_cmd="LOG_path=/tmp/ LOG_level=info rscl_polyfill_core --exclusive > /dev/null 2>&1 < /dev/null &"
    fi
    run_on_board "${start_zmq_cmd}" false true
elif [[ "${operation}" =~ ^"stop" ]]; then
    run_on_board "ps -ef | grep rscl_polyfill_core | grep -v grep | awk '{print \\\$2}'|xargs sudo kill -15" false true
    ps -ef | grep rscl_polyfill_core | grep -v grep | awk '{print $2}'|xargs sudo kill -15 > /dev/null 2>&1
elif [[ "${operation}" =~ ^"check" ]]; then
    if [[ "$1" == "zmq" ]]; then
        board_result=$(run_on_board "ps -ef|grep rscl_polyfill_core|grep -v grep" true false)
        host_result=$(ps -ef|grep rscl_polyfill_core|grep -v grep)
        if [[ $host_result =~ "rscl_polyfill_core" ]]; then
            echo "host: Zmq server is already running."
        else
            echo "host: Zmq server is not running! Try run command: rscl_tool start"
        fi
        analyze_zmq_check "$board_result"
    elif [[ "$1" == "all" ]]; then
        echo "todo"
    fi
else
    usage
fi
