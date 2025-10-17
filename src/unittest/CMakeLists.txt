if (CMAKE_MW STREQUAL "RSCL")
    add_executable(1_example_Pub RsclPubExample.cpp)
    target_link_libraries(1_example_Pub
            common
            ${SENSEAUTO_RSCL_LIBS}
            ${SENSEAUTO_MSG_LIBS}
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

    add_executable(1_example_Sub RsclSubExample.cpp)
    target_link_libraries(1_example_Sub
            common
            ${SENSEAUTO_RSCL_LIBS}
            ${SENSEAUTO_MSG_LIBS}
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

    install(TARGETS 1_example_Pub 1_example_Sub
            RUNTIME DESTINATION bin
    )

endif()