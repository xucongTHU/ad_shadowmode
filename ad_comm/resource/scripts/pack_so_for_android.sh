#! /bin/bash

set -x

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

OUT_DIR=$(realpath $1)

if [ ! -d ${OUT_DIR} ]; then
    mkdir -p ${OUT_DIR}
fi

sudo cp -rL /opt/senseauto_active/senseauto-3rdparty/ ${OUT_DIR}
sudo cp -rL /opt/senseauto_active/senseauto-msgs/ ${OUT_DIR}
sudo cp -rL /opt/senseauto_active/senseauto-rscl/ ${OUT_DIR}
sudo cp -rL /opt/senseauto_active/senseauto-config/ ${OUT_DIR}

cd ${OUT_DIR}/senseauto-3rdparty/3rdparty &&
    sudo rm -rf bin boost-1.66.0/include/ \
        ffmpeg-n4.2.7_aarch64_gcc_hmi \
        grpc_aarch64/ grpc_aarch64_clang grpc_x86_64 grpc \
        grpc/lib/*.a \
        lib/static/ \
        lib/kestrel_nart.kep \
        pcl-1.9-aarch64 \
        pcl-1.9-aarch64-clang \
        pcl-1.9-x86_64 pcl-1.9/include/ \
        boost-1.66.0/include/

cd ${OUT_DIR}
sudo chmod -R 777 ./senseauto*
find ./senseauto-3rdparty -name "*.so*" -exec mv {} senseauto-rscl/lib \;
find ./senseauto-msgs -name "*.so*" -exec mv {} senseauto-rscl/lib \;

cd ${OUT_DIR}/senseauto-rscl/
# rm -rf include
cd ${OUT_DIR}/senseauto-rscl/3rdparty/apollo/cyber/conf/
sudo rm cyber.pb.conf && mv cyber_rtps_only.pb.conf cyber.pb.conf

cd ${OUT_DIR}
# sudo rm -rf senseauto-3rdparty
# sudo rm -rf senseauto-msgs
