#! /bin/bash

set -e

OUT_DIR=$1

if [ ! -d ${OUT_DIR} ]; then
    mkdir -p ${OUT_DIR}
fi

RSCL_DIR="/opt/senseauto_active/senseauto-rscl"
THIRDPARTY_DIR="/opt/senseauto_active/senseauto-3rdparty"
echo "** RSCL is expected to be installed in ${RSCL_DIR}**"

# cpp part
cd ${OUT_DIR}
mkdir include
mkdir lib

cp -a ${RSCL_DIR}/include/ad_base include
cp -a ${RSCL_DIR}/include/ad_bag include
cp -a ${RSCL_DIR}/include/ad_bag_c include
cp -a ${RSCL_DIR}/include/ad_serde include

copy_lib() {
    local LIB_NAME=$1
    cp -a ${RSCL_DIR}/lib/${LIB_NAME} lib
    ldd lib/${LIB_NAME} | grep senseauto | awk 'NF == 4 { system("cp " $3 " lib") }'
}

copy_lib libad_bag.so
copy_lib libad_bag_c.so
copy_lib libad_serde.so
copy_lib libad_serde_capnp.so

cat <<EOT > test.cc
#include "ad_bag/bag_reader.h"
int main() {
    senseAD::bag::BagReaderAttribute attr;
    attr.iterator_type =
        senseAD::bag::BagIteratorType::READ_NEXT_BY_TIMESTAMP_ORDER;

    return 0;
}
EOT

echo "** Test whether the exported file can compile"

g++ -std=c++14 test.cc -o test -Iinclude -Llib -lad_bag
./test
rm test test.cc

# py part
cp -a ${RSCL_DIR}/python/ad_bag_py ./
echo "** Test whether the exported python api can use"
export PYTHONPATH=$(pwd):$PYTHONPATH
python3 -c "import ad_bag_py"

echo "** DONE **"
