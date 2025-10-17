#! /bin/bash

set -e

echo "######################################"
echo "# expected to be execute in root dir #"
echo "######################################"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

export PATH="$PATH:/opt/senseauto_active/senseauto-3rdparty/3rdparty/bin/lcov"

generate_coverage() {
    tar zxf ${TEST_RESULT_TAR}

    result_root_dir=$(find tmp/ -name "senseauto-rscl")
    cp -a ${result_root_dir}/build/* ./
    rm -rf tmp/

    echo "# Collect coverage info"
    lcov -c -d $(realpath ../) -o all.info > /dev/null

    echo "# Filter coverage info"
    lcov -r all.info \
        "/opt/jetson/*" \
        "/usr/*" \
        "/opt/senseauto/*" \
        "*/3rdparty/*" \
        "*/build/*" \
        "*/examples/*" \
        "*/spdlog/*" \
        "*/ad_diag/*" \
        "*/unittests/*" \
        -o coverage_result.info > /dev/null

    echo "# Gen html"
    genhtml -s --legend --function-coverage --demangle-cpp \
        -o coverage_result coverage_result.info > coverage_result_overall_stat.txt
}

upload_coverage_report() {
    echo "Start Upload coverage report to 10.112.61.61"
    mv coverage_result ${UPLOAD_DIRECTORY_NAME}
    python3 ${SCRIPT_DIR}/upload_ftp_directory.py \
        --ip 10.112.61.61 \
        --user rscl \
        --password rscl \
        --local_src ${UPLOAD_DIRECTORY_NAME} \
        --ftp_dest /deployment/rscl_doc/data/coverage_soc/
    rm -rf ${UPLOAD_DIRECTORY_NAME}
    echo "Done Upload coverage report to 10.112.61.61/coverage_soc/${UPLOAD_DIRECTORY_NAME}"
}

echo "####################################"
echo "- Executaion Order"
echo "  1. ENABLE_TEST_COVERAGE=1 make tgz_$\{TARGET_PLATFORM\} in PC"
echo "  2. scp the output package to soc and install it"
echo "  3. Run: bash /opt/senseauto_active/senseauto-rscl/resource/scripts/run_all_unittest_with_coverage_onboard.sh"
echo "  4. scp the result /tmp/rscl_test_result.tar.gz back to current PC"
echo "  5. Run this script: TEST_RESULT_TAR=/tmp/rscl_test_result.tar.gz bash scripts/test_coverage_generate_for_onboard_result.sh"
echo "####################################"

if [ ! -d ${SCRIPT_DIR}/../build ]; then
    echo "[ERROR] You have to call ENABLE_TEST_COVERAGE=1 make tgz_$\{TARGET_PLATFORM\} in current PC"
    exit 1
fi
cd ${SCRIPT_DIR}/../build

if [ -z ${TEST_RESULT_TAR+x} ]; then
    echo "[ERROR] variable TEST_RESULT_TAR have not be set"
    exit 1
fi

echo "> Input arg: TEST_RESULT_TAR=${TEST_RESULT_TAR}"

generate_coverage

if [ ! -z ${UPLOAD_DIRECTORY_NAME+x} ]; then
    upload_coverage_report
fi
rm -rf coverage_result

exit 0
