include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_MW STREQUAL "CYBER")
    add_library(cyber_recorder SHARED
            ./cyber_recorder/CyberRecorder.cpp
    )
    if(CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
        target_link_libraries(cyber_recorder /lib/x86_64-linux-gnu/libtinfo.so.5.9)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES arm64)
        target_link_libraries(cyber_recorder
                common
                ${cAD_CM_SDK_LIBS}
                ${Boost_LIBRARIES}
                ${GLOG_LIBRARIES}
                ${PROTOBUF_LIBRARIES}
                ${ORIN_LIB_LINK_DIRECTORY}/libncurses.so
                ${ORIN_LIB_LINK_DIRECTORY}/libtinfo.so.6.2
        )
    endif()

    install(TARGETS cyber_recorder
            LIBRARY DESTINATION lib)
elseif (CMAKE_MW STREQUAL "RSCL")
    add_library(rscl_recorder SHARED
                ./rscl_recorder/RsclRecorder.cpp
    )

    target_link_libraries(rscl_recorder
            ${SENSEAUTO_RSCL_LIBS}
    )

    install(TARGETS rscl_recorder
            LIBRARY DESTINATION lib)
endif()

add_library(data_storage SHARED
        ./DataStorage.cpp
        ./common/FileCompress.cpp
        ./common/FileRoller.cpp
)
if(BUILD_THOR)
    target_link_directories(data_storage PUBLIC
            ${SENSEAUTO_3RDPARTY}/boost/lib
    )

    target_link_libraries(data_storage
            ad_3rdparty_mtar
            rscl_recorder
            boost_filesystem
            boost_system
    )
else()
    target_link_libraries(data_storage
            ad_3rdparty_mtar
            cyber_recorder
            #        data_report
    )

endif()

install(TARGETS data_storage
        ARCHIVE DESTINATION lib)
