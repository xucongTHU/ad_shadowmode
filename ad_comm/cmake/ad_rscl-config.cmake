set(_target "ad_rscl")

if (NOT TARGET ad_rscl_interface)
    ################# Other senseauto project dep ###################
    if (NOT PATH_SOURCE_3RDPARTY)
        set(PATH_SOURCE_3RDPARTY ${SENSEAUTO_DEV_INSTALLED_ROOT}/${FIND_SENSEAUTO_PACKAGE_PATH}/senseauto_active/senseauto-3rdparty/3rdparty)
    endif()

    find_senseauto_package(senseauto-msgs ADMsgIDL REQUIRED)
    #################################################################
    set(PATH_FW_INSTALLED_DIR ${SENSEAUTO_DEV_INSTALLED_ROOT}/${FIND_SENSEAUTO_PACKAGE_PATH}/senseauto_active/senseauto-rscl)

    include(${CMAKE_CURRENT_LIST_DIR}/ad_mw.cmake)

    add_library(ad_rscl_interface INTERFACE)
    ADD_AD_MW_DEPENDENCIES()
    target_link_directories(ad_rscl_interface INTERFACE
        ${PATH_SOURCE_3RDPARTY}/lib

        "${Rscl_PATH}/lib"
        ${ADMsgIDL_LIBRARY_DIRS}
        ${THOR_NVLIB_PATH}
        )
    target_include_directories(ad_rscl_interface SYSTEM INTERFACE
        ${PATH_SOURCE_3RDPARTY}/include

        ${Rscl_PATH}/include
        ${ADMsgIDL_INCLUDE_DIRS}
        )

    set(DEPS_ROOT "/opt/senseauto_active")
    list(APPEND RSCL_DEPS_LINK_DIRS "${DEPS_ROOT}/senseauto-rscl/lib")
    list(APPEND RSCL_DEPS_LINK_DIRS "${DEPS_ROOT}/senseauto-3rdparty/3rdparty/lib")
    list(APPEND RSCL_DEPS_LINK_DIRS "${DEPS_ROOT}/senseauto-msgs/lib")

    list(JOIN RSCL_DEPS_LINK_DIRS ":" RSCL_DEPS_LINK_DIRS_STR)
    target_link_options(ad_rscl_interface INTERFACE "-Wl,-rpath-link=${RSCL_DEPS_LINK_DIRS_STR}")
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_link_options(ad_rscl_interface INTERFACE "-Wl,--copy-dt-needed-entries")
    endif()

    if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
        set(PTHREAD_ONLY_LIB_NAME -pthread)
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
        set(PTHREAD_ONLY_LIB_NAME -pthread)
    else()
        set(PTHREAD_ONLY_LIB_NAME -lpthread)
    endif()

    target_link_libraries(ad_rscl_interface INTERFACE
        protobuf
        kj
        capnp

        ad_rscl
        ad_rscl_pb
        ad_rscl_capnp
        ad_diag
        ad_bag
        ad_serde
        ad_serde_capnp
        ad_base_common
        ad_base_classloader
        std_msgs_capnp
        ad_monitor_capnp
        zmq

        ${PTHREAD_ONLY_LIB_NAME}
        ${THOR_NVLIBS}
        )
    add_library(ad_rscl_cuda_interface INTERFACE)
    target_link_libraries(ad_rscl_cuda_interface INTERFACE ad_rscl_interface ad_rscl_cuda)
endif()

# Library
list(APPEND ${_target}_LIBRARIES "ad_rscl_interface")

# Debug
# message(STATUS "${_target}_INCLUDE_DIRS: ${${_target}_INCLUDE_DIRS}")
# message(STATUS "${_target}_LIBRARIES: ${${_target}_LIBRARIES}")
