#!/bin/bash

config_file="/opt/had-tool/bin/had-tool.ini"

if [ ! -f "${config_file}" ]; then
    echo "${config_file} not exist, not support."
    exit 1
fi

# Function to execute command in remote using sshpass
function execute_remote() {
    local command="$1"
    while IFS=' = ' read -r key value || [[ -n "$key" ]]; do
        if [[ -n $key ]]; then
            if [[ $key == "deploy"*"_ip" ]]; then
                platform="${key#deploy_}"
                platform="${platform%_*}"
                if [[ $platform == "90L" ]] || [[ $platform == "x86" ]]; then
                    continue
                fi

                ip_key="deploy_${platform}_ip"
                user_key="deploy_${platform}_user_name"
                pass_key="deploy_${platform}_password"
                path_key="deploy_${platform}_path"

                ip=$(grep -oP "(?<=^$ip_key = ).*" "$config_file" | awk '$1=$1')
                user=$(grep -oP "(?<=^$user_key = ).*" "$config_file" | awk '$1=$1')
                pass=$(grep -oP "(?<=^$pass_key = ).*" "$config_file" | awk '$1=$1')
                path=$(grep -oP "(?<=^$path_key = ).*" "$config_file" | awk '$1=$1')

                if [[ -n $ip && -n $user && -n $pass && -n $path ]]; then
                    eval "sshpass -p ${pass} ssh -n -o StrictHostKeyChecking=no ${user}@${ip} \"cd ${path}/senseauto/ad_home/apps/ad_rscl_tools/ && source /etc/profile && ${command}\" &"
                fi
            fi
        fi
    done < "$config_file"
}

function fetch_remote() {
    local r_file_path="$1"
    local l_file_path="$2"
    while IFS=' = ' read -r key value || [[ -n "$key" ]]; do
        if [[ -n $key ]]; then
            if [[ $key == "deploy"*"_ip" ]]; then
                platform="${key#deploy_}"
                platform="${platform%_*}"
                if [[ $platform == "90L" ]] || [[ $platform == "x86" ]]; then
                    continue
                fi

                rm -rf ${l_file_path}/${platform}_graph/
                mkdir ${l_file_path}/${platform}_graph/

                ip_key="deploy_${platform}_ip"
                user_key="deploy_${platform}_user_name"
                pass_key="deploy_${platform}_password"

                ip=$(grep -oP "(?<=^$ip_key = ).*" "$config_file" | awk '$1=$1')
                user=$(grep -oP "(?<=^$user_key = ).*" "$config_file" | awk '$1=$1')
                pass=$(grep -oP "(?<=^$pass_key = ).*" "$config_file" | awk '$1=$1')

                if [[ -n $ip && -n $user && -n $pass ]]; then
                    eval "sshpass -p ${pass} scp -o StrictHostKeyChecking=no ${user}@${ip}:${r_file_path} ${l_file_path}/${platform}_graph"
                fi
            fi
        fi
    done < "$config_file"
}

function InstallNetworkx() {
    if python3 -c "import networkx" >/dev/null 2>&1;then
        echo "networkx already installed"
        return
    fi
    # install networkx
    pip3 --timeout 60 install networkx --quiet --exists-action i >/dev/null 2>&1
    if python3 -c "import networkx" >/dev/null 2>&1;then
        echo "networkx install success"
    else
        echo "networkx install failed"
    fi
}

function InstallNetgraph() {
    if python3 -c "import netgraph" >/dev/null 2>&1;then
        echo "netgraph already installed"
        return
    fi
    # install netgraph
    pip3 --timeout 60 install netgraph --quiet --exists-action i >/dev/null 2>&1
    if python3 -c "import netgraph" >/dev/null 2>&1;then
        echo "netgraph install success"
    else
        echo "netgraph install failed"
    fi
}

execute_remote "timeout 20 ./tools.sh rscl_graph"
pushd /opt/senseauto/senseauto-framework-sdk/active/bin/
timeout 25 ./rscl_graph
popd

fetch_remote "/tmp/state_json.txt" "/tmp/"
eval "ls -d /tmp/*/|grep _graph|xargs -i cat {}state_json.txt >> /tmp/state_json.txt"

InstallNetworkx
InstallNetgraph
python3 /opt/senseauto/senseauto-framework-sdk/active/resource/scripts/graph_gen.py /tmp/state_json.txt /tmp/graph_output/
if [[ $? == 0 ]]; then
    echo "Please check /tmp/graph_output/"
    ls /tmp/graph_output/
else
    echo "Parse error!"
fi
