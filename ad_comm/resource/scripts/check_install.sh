#! /bin/bash

check_dependency() {
    local target=$1
    local error_msg=$2

    local search_result=`ldconfig -p | grep $target`
    if [ -z "${search_result}" ]; then
        printf "\n==================================\n"
        printf "Error Message: ${error_msg}\n"
        printf "==================================\n\n"
        exit 1
    fi
}

check_executable() {
    /opt/senseauto_active/senseauto-rscl/bin/rsclbag
    ret=$?
    if [ $ret -ne 0 ]; then
        printf "\n==================================\n"
        printf "Cannot run a minimal executable (ret=$ret). Please try re-install or contact the function owner: fengzhongjie\n"
        printf "==================================\n\n"
        exit 1
    fi
}

main() {
    # # check dependency
    # check_dependency "libz.so.1" "zlib is missing!\n Please run \"sudo apt-get install zlib1g-dev\" to install"
    # check_dependency "liblz4.so.1" "lz4 is missing!\n Please run \"sudo apt-get install liblz4-dev\" to install"
    # check_dependency "libuuid.so.1" "uuid is missing!\n Please run \"sudo apt-get install uuid-dev\" to install"
    # for value in "libcapnp-0.10" "libcapnpc-0.10"
    # do
    #     check_dependency ${value} \
    #         "${value} is missing!\n Please install it following the installation document"
    # done
    # check_dependency "libyaml-cpp.so.0.7" "yaml-cpp is missing!\n Please install it following the installation document"

    # printf "> Dependency is OK\n"

    # run example
    check_executable
    printf "> Execute Test is OK\n"
}

main

exit 0
