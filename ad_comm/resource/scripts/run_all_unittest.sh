#! /bin/bash

set -e

echo "######################################"
echo "# expected to be execute in root dir #"
echo "######################################"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
TEST_DIR=$( realpath ${SCRIPT_DIR}/../../test )

if [ ! -d ${TEST_DIR} ]; then
    echo "[ERROR] The test directory not exist: ${TEST_DIR}"
    exit 1
fi

cd ${TEST_DIR}

run_all_unittest() {
    cd ${TEST_DIR}
    rm -rf /tmp/rscl_test_result
    mkdir /tmp/rscl_test_result
    touch /tmp/rscl_test_result/test_report
    # generate gcda
    set +e
    for f in unittest_[!bag]*; do
        echo ">>> TestCase: ${f}"
        GCOV_PREFIX_STRIP=1 GCOV_PREFIX=/tmp/rscl_test_result ./${f} --gtest_color=yes | tee -a /tmp/rscl_test_gcda/test_report
    done
    set -e

}

pack_gcda() {
    if [ ! -d /tmp/rscl_test_result ]; then
        echo "directory /tmp/rscl_test_result not exist!"
        exit 1
    fi

    tar czf /tmp/rscl_test_result.tar.gz /tmp/rscl_test_result
    rm -rf /tmp/rscl_test_result
    echo "** output to /tmp/rscl_test_result.tar.gz **"
}

echo "################################"
echo "###### Only tested in 90L ######"
echo "################################"

echo "###### Step 1. Run unittest ######"
run_all_unittest

echo "###### Step 2. Collect all generated file and pack it in tar ######"
pack_gcda

echo "###### Done ######"
exit 0
