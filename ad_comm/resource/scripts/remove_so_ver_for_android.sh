#! /bin/bash

set -x

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

OUT_DIR=$(realpath $1)

echo "!!!! MUST use patchelf version >= 0.10 !!!"

cd ${OUT_DIR}/senseauto-rscl/test
python3 ${SCRIPT_DIR}/remove_shared_object_version.py ../lib
cd ${OUT_DIR}/senseauto-rscl/example
python3 ${SCRIPT_DIR}/remove_shared_object_version.py ../lib
cd ${OUT_DIR}/senseauto-rscl/bin
python3 ${SCRIPT_DIR}/remove_shared_object_version.py ../lib
cd ${OUT_DIR}/senseauto-rscl/lib
python3 ${SCRIPT_DIR}/remove_shared_object_version.py ./
