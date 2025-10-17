#!/bin/bash

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

chmod +x "${SCRIPT_DIR}/resourcectl"
chmod +x ${SCRIPT_DIR}/*.sh
chmod +x ${SCRIPT_DIR}/*.py
rm -f /usr/bin/resourcectl
ln -s "${SCRIPT_DIR}/resourcectl" /usr/bin
