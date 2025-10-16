cmake_minimum_required(VERSION 3.14.0)

if (CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
    set(CMAKE_CXX_FLAGS "-std=c++17 -fext-numeric-literals -pipe -O0 -Wall -Wextra -fopenmp -fPIC -pthread")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES arm64)
    set(CMAKE_CXX_FLAGS "-std=c++17 -fext-numeric-literals -pipe -O0 -Wall -Wextra -fopenmp -fPIC -pthread -lrt")
endif()

add_compile_options(-w)

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

if(BUILD_THOR)
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
endif()


