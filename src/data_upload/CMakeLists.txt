add_library(data_upload SHARED
        FileStatusManager.cc
        DataReporter.cpp
        DataUploader.cc)

if(BUILD_THOR)
    target_link_libraries(data_upload
            common
            strategy
            ${PROJECT_ROOT}/3rdparty/paho.mqtt.cpp/thor/lib/libpaho-mqttpp3.so
            ${PROJECT_ROOT}/3rdparty/paho.mqtt.cpp/thor/lib/libpaho-mqtt3as.so
            ${SENSEAUTO_3RDPARTY}/curl/lib/libcurl.so
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libssl.so
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libcrypto.so

    )
else()
    target_link_directories(data_upload PRIVATE
            ${OpenSSL_LIBRARIES_DIR}
            ${CURL_LIBRARIES_DIR}
    )
    target_link_libraries(data_upload
            common
            strategy
            paho-mqttpp3
            paho-mqtt3as
            curl
            ssl
            crypto
    )

endif()

install(TARGETS data_upload
        ARCHIVE DESTINATION lib)