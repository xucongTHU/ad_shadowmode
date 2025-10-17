#! /bin/bash

set -e

echo "######################################"
echo "# expected to be execute in root dir #"
echo "######################################"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd build

export PATH="$PATH:/opt/senseauto_active/senseauto-3rdparty/3rdparty/bin/lcov"

generate_coverage() {
    rm -rf coverage_result
    echo "# Collect coverage info"
    lcov -c -d $(realpath ../) -o all.info > /dev/null

    echo "# Filter coverage info"
    lcov -r all.info \
        "/usr/*" \
        "/opt/senseauto/*" \
        "*/3rdparty/*" \
        "*/build/*" \
        "*/examples/*" \
        "*/spdlog/*" \
        "*/ad_diag/*" \
        "*/unittests/*" \
        "*/hetero_channel/common/zmq*" \
        -o coverage_result.info > /dev/null

    echo "# Gen html"
    genhtml -s --legend --function-coverage --demangle-cpp \
        -o coverage_result coverage_result.info > coverage_result_overall_stat.txt
}

upload_coverage_report() {
    if [ "${UPLOAD_COVERAGE_REPORT}" == "1" ]; then
        echo "Start Upload coverage report to 10.112.61.61"
        python3 ${SCRIPT_DIR}/upload_ftp_directory.py \
            --ip 10.112.61.61 \
            --user rscl \
            --password rscl \
            --local_src coverage_result \
            --ftp_dest /deployment/rscl_doc/data
        echo "Done Upload coverage report to 10.112.61.61"
    fi
}

generate_coverage
upload_coverage_report

FUNCTION_COVERAGE=$(cat coverage_result_overall_stat.txt | grep 'functions..' | awk '{ print $2 }' | tr -d '%')
FUNCTION_COVERAGE_THRESHOLD=50.0
if (( $(echo "$FUNCTION_COVERAGE_THRESHOLD > $FUNCTION_COVERAGE" |bc -l) )); then
    printf "ERROR: Total Function Coverage(%s) is lower that threshold(%s).\n" "$FUNCTION_COVERAGE" "$FUNCTION_COVERAGE_THRESHOLD"
    exit 1
fi

exit 0
