cmake_minimum_required(VERSION 3.14.0)

# 根据处理器架构设置编译标志
if (CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
    set(CMAKE_CXX_FLAGS "-std=c++17 -fext-numeric-literals -pipe -O0 -Wall -Wextra -fopenmp -fPIC -pthread")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES arm64)
    set(CMAKE_CXX_FLAGS "-std=c++17 -fext-numeric-literals -pipe -O0 -Wall -Wextra -fopenmp -fPIC -pthread -lrt")
endif()

# 禁用所有警告
add_compile_options(-w)

#查找依赖包

if(CMAKE_MW STREQUAL "CYBER")
    find_package(Boost REQUIRED COMPONENTS filesystem system iostreams thread regex)
    include_directories(${Boost_INCLUDE_DIRS})
    find_package(Protobuf 3 EXACT REQUIRED)
    include_directories(${Protobuf_INCLUDE_DIRS})
    find_package(gflags REQUIRED)
    include_directories(${GFLAGS_INCLUDE_DIRS})
    find_library(GLOG_LIBRARIES glog)
    find_path(GLOG_INCLUDE_DIRS "glog/logging.h")
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    find_package(yaml-cpp REQUIRED)
    include_directories(${YAML_INCLUDE_DIRS})
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})

    # 查找 CUDA 和 CUDNN
    find_package(CUDA REQUIRED)
    include_directories(${CUDA_INCLUDE_DIRS})
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS}" "-std=c++17")

    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_FIND_ROOT_PATH}/usr/local/cuda")
    find_library(CUDNN_LIBRARY
            NAMES libcudnn.so${__cudnn_ver_suffix} libcudnn${__cudnn_ver_suffix}.dylib ${__cudnn_lib_win_name}
            PATHS $ENV{LD_LIBRARY_PATH} ${__libpath_cudart} ${CUDNN_ROOT_DIR} ${PC_CUDNN_LIBRARY_DIRS} ${CMAKE_INSTALL_PREFIX}
            PATH_SUFFIXES lib lib64 bin
            DOC "CUDNN library."
            REQUIRED
    )
endif ()

if(CMAKE_MW STREQUAL "CYBER")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
        add_compile_definitions(MW_ART_IPC)
        set(TRT_LIB_PATH ${TRT_HOME}/targets/x86_64-linux-gnu/lib)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES arm64)
        add_compile_definitions(MW_ART_ORIN)
        find_library(CUDART cudart ${CUDA_TOOLKIT_ROOT_DIR}/lib64)
        include_directories(${CAICAUTO_3RDPARTY}/tensorrt/include)
        set(TRT_LIB_PATH ${CAICAUTO_3RDPARTY}/tensorrt/targets/aarch64-linux-gnu/lib)
        find_library(NVINFER nvinfer ${TRT_LIB_PATH})
        find_library(NVINFER_PLUGIN nvinfer_plugin ${TRT_LIB_PATH})
        find_library(NVPARSERS nvparsers ${TRT_LIB_PATH})
        find_library(NVONNXPARSERS nvonnxparser ${TRT_LIB_PATH})
    endif()

elseif (CMAKE_MW STREQUAL "RSCL")
    add_compile_definitions(MW_RSCL_THOR)
#    add_subdirectory(channel)
endif()

#添加子目录
add_subdirectory(channel)
add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(data_storage)
add_subdirectory(data_encryption)
add_subdirectory(data_proto)
add_subdirectory(data_upload)
add_subdirectory(trigger)
add_subdirectory(unittest)
add_subdirectory(strategy)

#if(BUILD_THOR)
    add_executable(${PROJECT_NAME}
            ./ShadowModeContext.cpp
            ./main.cpp
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
            ${SENSEAUTO_3RDPARTY}/boost/lib
    )
    target_link_libraries(${PROJECT_NAME}
            channel
            common
            config
            data_encryption
            data_upload
            data_proto
            data_storage
            strategy
            trigger
            boost_filesystem
            boost_system
            ${SENSEAUTO_RSCL_LIBS}
            ${SENSEAUTO_MSG_LIBS}
            ${SENSEAUTO_3RDPARTY}/curl/lib/libcurl.so
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libssl.so
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libcrypto.so
            ${SENSEAUTO_3RDPARTY}/uuid/lib/libuuid.so
            ${SENSEAUTO_3RDPARTY}/capnp/lib/libcapnp.so
            ${SENSEAUTO_3RDPARTY}/lib/libkj.so
            ${SENSEAUTO_3RDPARTY}/yaml-cpp/lib/libyaml-cpp.so
            ${SENSEAUTO_3RDPARTY}/glog/lib/libglog.so
            ${SENSEAUTO_3RDPARTY}/gflags/lib/libgflags.so
            ${SENSEAUTO_3RDPARTY}/protobuf/lib/libprotobuf.so.25
            ${SENSEAUTO_3RDPARTY}/zmq/lib/libzmq.so
            ${SENSEAUTO_3RDPARTY}/zstd/lib/libzstd.so
            ${SENSEAUTO_3RDPARTY}/lz4/lib/liblz4.so
    )

    install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION bin)
#endif()


