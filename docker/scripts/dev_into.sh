DOCKER_USER="root"

DEV_CONTAINER="dcl_dev_thor"
if [ -z "$DEV_CONTAINER" ]; then
    echo "Unsupported thor platform"
    exit 1
fi

function restart_stopped_container {
    if docker ps -f status=exited -f name="${DEV_CONTAINER}" | grep "${DEV_CONTAINER}"; then
        docker start "${DEV_CONTAINER}"
    fi
}

xhost +local:root 1>/dev/null 2>&1

restart_stopped_container

docker exec \
    -u "${DOCKER_USER}" \
    -it "${DEV_CONTAINER}" \
    /bin/bash

xhost -local:root 1>/dev/null 2>&1