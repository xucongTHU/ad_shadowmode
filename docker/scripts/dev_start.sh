TOP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
source "${TOP_DIR}/resource/scripts/shadow_base.sh"

DOCKER_IMAGE="swr.dxh2-region-1.dfhcs.com.cn/dfrd_id_l29/df_thor_image:v25.0919"
if [ -z "$DOCKER_IMAGE" ]; then
    error "Unsupported thor platform"
    exit 1
fi

DEV_CONTAINER="dcl_dev_thor"

function remove_container_if_exists() {
  local container="$1"
  if docker ps -a --format '{{.Names}}' | grep -q "${container}"; then
    info "Removing existing ad_shadowmode container: ${container}"
    docker stop "${container}" > /dev/null
    docker rm -v -f "${container}" 2> /dev/null
  fi
}

function docker_pull() {
  info "Start pulling docker image ${DOCKER_IMAGE} ..."
  if ! docker pull "${DOCKER_IMAGE}" ; then
    error "Failed to pull docker image : ${DOCKER_IMAGE}"
    exit 1
  fi
}

function main() {
  info "Remove existing ad_shadowmode development container ..."
  remove_container_if_exists ${DEV_CONTAINER}

  info "Starting docker container \"${DEV_CONTAINER}\" ..."
  srv_dir="${TOP_DIR}"
  docker run --entrypoint /bin/bash \
             --name $DEV_CONTAINER \
             --privileged  \
             --device /dev/fuse \
             --security-opt apparmor:unconfined \
             --security-opt seccomp=unconfined \
             --network host \
             -v /dev/bus/usb:/dev/bus/usb \
             -v ${srv_dir}:/synaptix \
             -v /etc/passwd:/etc/passwd \
             -w /synaptix \
             -idt "${DOCKER_IMAGE}"

  if [ $? -ne 0 ]; then
      error "Failed to start docker container \"${DEV_CONTAINER}\" based on image: ${DOCKER_IMAGE}"
      exit 1
  fi

  ok "Congratulations! You have successfully finished setting up ad_shadowmode Dev Environment."
  ok "To login into the newly created ${BOLD}${DEV_CONTAINER}${NO_COLOR} container, please run the following command:"
  ok "  ${GREEN}${BOLD}bash docker/scripts/dev_into.sh"
  ok "Enjoy!"
}

main "$@"
