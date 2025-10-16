cmake_minimum_required(VERSION 3.14.0)

project(ad_shadowmode)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ad_3rdparty_mtar SHARED ${CMAKE_SOURCE_DIR}/3rdparty/microtar/microtar.c)

include_directories(
        ${PROJECT_ROOT}
        ${PROJECT_ROOT}/3rdparty
        ${PROJECT_ROOT}/3rdparty/exprtk
        ${PROJECT_ROOT}/3rdparty/nlohmann
)

if(BUILD_THOR)
    include_directories(
            ${PROJECT_ROOT}/3rdparty/paho.mqtt.cpp/thor/include
    )
else()
    include(3rdparty/third_party.cmake)
    include_directories(
            ${PROJECT_ROOT}/3rdparty/paho.mqtt.cpp/orin/include
            ${PROJECT_ROOT}/3rdparty/curl/orin/include
            ${PROJECT_ROOT}/3rdparty/openssl/orin/include
            ${PROJECT_ROOT}/3rdparty/lz4/orin/include
    )
endif ()


set(SENSEAUTO_RSCL /opt/senseauto/senseauto-rscl/active)
set(SENSEAUTO_MSG /opt/senseauto/senseauto-msgs/active)
set(SENSEAUTO_3RDPARTY /opt/senseauto/senseauto-3rdparty/active/3rdparty)
include_directories(
        ${SENSEAUTO_RSCL}
        ${SENSEAUTO_RSCL}/include
        ${SENSEAUTO_MSG}/include
        ${SENSEAUTO_3RDPARTY}/boost/include
        ${SENSEAUTO_3RDPARTY}/capnp/include
        ${SENSEAUTO_3RDPARTY}/gflags/include
        ${SENSEAUTO_3RDPARTY}/glog/include
        ${SENSEAUTO_3RDPARTY}/gtest/include
        ${SENSEAUTO_3RDPARTY}/jsoncpp/include
        ${SENSEAUTO_3RDPARTY}/lz4/include
        ${SENSEAUTO_3RDPARTY}/opencv/include
        ${SENSEAUTO_3RDPARTY}/openssl/include
        ${SENSEAUTO_3RDPARTY}/protobuf/include
        ${SENSEAUTO_3RDPARTY}/uuid/include
        ${SENSEAUTO_3RDPARTY}/yaml-cpp/include
        ${SENSEAUTO_3RDPARTY}/zmq/include
        ${SENSEAUTO_3RDPARTY}/zstd/include
        ${SENSEAUTO_3RDPARTY}/curl/include
)
file(GLOB SENSEAUTO_RSCL_LIBS ${SENSEAUTO_RSCL}/lib/*.so ${SENSEAUTO_RSCL}/lib/*.a)
file(GLOB SENSEAUTO_MSG_LIBS ${SENSEAUTO_MSG}/lib/*.so ${SENSEAUTO_MSG}/lib/*.a)

add_subdirectory(src)


file(GLOB mqttLibs "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/paho.mqtt.cpp/thor/lib/*.so*")

install(TARGETS ad_3rdparty_mtar
        LIBRARY DESTINATION lib
)

install(FILES ${mqttLibs}
        DESTINATION lib
)

install(DIRECTORY  resource/
        DESTINATION resource
)
install(DIRECTORY  config/
        DESTINATION config
)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/release.conf.json
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

#############
##  CPack  ##
#############

set(CPACK_GENERATOR "TGZ")

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${MODULE_VERSION})
set(CPACK_PACKAGE_CONTACT "xucong@t3caic.com")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
set(CPACK_SET_DESTDIR ON)
set(CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${MODULE_VERSION}")

install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/tar_extra/release_helper.sh
        DESTINATION /
)

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
include(CPack)
