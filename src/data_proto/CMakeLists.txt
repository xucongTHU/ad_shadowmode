add_library(data_proto SHARED
        CurlWrapper.cc
        MqttWrapper.cpp
)

if(BUILD_THOR)
    target_link_libraries(data_proto
            common
            ${PROJECT_ROOT}/3rdparty/paho.mqtt.cpp/thor/lib/libpaho-mqttpp3.so
            ${PROJECT_ROOT}/3rdparty/paho.mqtt.cpp/thor/lib/libpaho-mqtt3as.so
            ${SENSEAUTO_3RDPARTY}/curl/lib/libcurl.so
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libssl.so
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libcrypto.so
    )
else()
    target_link_directories(data_proto PRIVATE
            ${OpenSSL_LIBRARIES_DIR}
            ${CURL_LIBRARIES_DIR}
            ${PAHO.MQTT.C_LIBRARIES_DIR}
            ${PAHO.MQTT.CPP_LIBRARIES_DIR}
    )
    target_link_libraries(data_proto
            common
            ssl
            curl
            crypto
            paho-mqtt3as
            paho-mqttpp3
    )

endif ()


install(TARGETS data_proto
        ARCHIVE DESTINATION lib)