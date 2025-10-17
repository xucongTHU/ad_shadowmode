#! /usr/bin/env bash
set -e

SHELL_FOLDER=$(dirname $(readlink -f "$0"))

TARGET_ARXML_DIR=$1
WORKING_DIR=$2

# 1. install arxmlc
sudo senseauto-pkm install --module ${SHELL_FOLDER}/senselotu_arxml-*.tar.gz

# 2. copy and process arxml files
if [[ $WORKING_DIR == /tmp/* ]]; then
    rm -rf ${WORKING_DIR}/*
fi
mkdir -p ${WORKING_DIR}/origin/
mkdir -p ${WORKING_DIR}/process/
mkdir -p ${WORKING_DIR}/rscl_tools/
mkdir -p ${WORKING_DIR}/outputs/
cp -r ${TARGET_ARXML_DIR}/*.arxml ${WORKING_DIR}/origin/
python3 /opt/senseauto/senselotu_arxml/active/scripts/rscl_adapter/arxml_modifier.py add_internal_service -i ${WORKING_DIR}/origin/internal_services.arxml -t ${WORKING_DIR}/origin/rscl.arxml -o ${WORKING_DIR}/process/
cp ${WORKING_DIR}/origin/*.arxml ${WORKING_DIR}/process/
mv ${WORKING_DIR}/process/target.arxml ${WORKING_DIR}/process/rscl.arxml
python3 /opt/senseauto/senselotu_arxml/active/scripts/rscl_adapter/arxml_modifier.py add_universal_executable -i ${WORKING_DIR}/process/ -o ${WORKING_DIR}/rscl_tools/
cp ${WORKING_DIR}/rscl_tools/*.arxml ${WORKING_DIR}/process/
