#! /usr/bin/env bash
set -e

SUDO_CMD=sudo
if [ $UID = 0 ]
then
    SUDO_CMD=''
fi

SHELL_FOLDER=$(dirname $(readlink -f "$0"))

TARGET_ARXML_DIR=$1
WORKING_DIR=$2

# 1. install arxmlc
$SUDO_CMD senseauto-pkm install --module ${SHELL_FOLDER}/senselotu_arxml-*.tar.gz

# 2. copy and process arxml files
if [[ $WORKING_DIR == /tmp/* ]]; then
    rm -rf ${WORKING_DIR}/*
fi
mkdir -p ${WORKING_DIR}/origin/
mkdir -p ${WORKING_DIR}/process/
mkdir -p ${WORKING_DIR}/rscl_tools/
mkdir -p ${WORKING_DIR}/outputs/
cp -r ${TARGET_ARXML_DIR}/*.arxml ${WORKING_DIR}/origin/
python3 /opt/senseauto/senselotu_arxml/active/scripts/rscl_adapter/arxml_modifier.py add_internal_service -i ${WORKING_DIR}/origin/internal_services.arxml -t ${WORKING_DIR}/origin/rscl.arxml -o ${WORKING_DIR}/process/ >/dev/null 2>&1
cp ${WORKING_DIR}/origin/*.arxml ${WORKING_DIR}/process/
mv ${WORKING_DIR}/process/target.arxml ${WORKING_DIR}/process/rscl.arxml
python3 /opt/senseauto/senselotu_arxml/active/scripts/rscl_adapter/arxml_modifier.py add_universal_executable -i ${WORKING_DIR}/process/ -o ${WORKING_DIR}/rscl_tools/ >/dev/null 2>&1
cp ${WORKING_DIR}/rscl_tools/*.arxml ${WORKING_DIR}/process/

# 3. generate code and copy to current repo
arxmlc -i ${WORKING_DIR}/process/ -o ${WORKING_DIR}/outputs/ -p rscl_lotu >/dev/null 2>&1
$SUDO_CMD rm -rf ${SHELL_FOLDER}/../lotu_generated
$SUDO_CMD mkdir ${SHELL_FOLDER}/../lotu_generated
$SUDO_CMD cp -r ${WORKING_DIR}/outputs/* ${SHELL_FOLDER}/../lotu_generated/

# 4. post process
sed -i 's/ara::core::/ara::lotu::core::/g' $(find ./lotu_generated/ -type f -name "*.cc" -o -name "*.h" -o -name "*.cpp")
sed -i 's/ara\/com\//ara\/lotu_com\//g' $(find ./lotu_generated/ -type f -name "*.cc" -o -name "*.h" -o -name "*.cpp")
sed -i 's/\#include \"ara\/core\//\#include \"ara\/lotu\/core\//g' $(find ./lotu_generated/ -type f -name "*.cc" -o -name "*.h" -o -name "*.cpp")
sed -i 's/ara::com::internal::skeleton::SkeletonAdapter/ara::com::internal::skeleton::lotu::SkeletonAdapter/g' $(find ./lotu_generated/ -type f -name "*.cc" -o -name "*.h" -o -name "*.cpp")
# sed -i 's/DdsTransportQos: UDP_DEFAULT/DdsTransportQos: SHM/g' $(find ./lotu_generated/ -type f -name "*.yaml")
sudo senseauto-pkm remove senselotu_arxml
