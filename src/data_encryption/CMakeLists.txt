add_library(data_encryption SHARED
        DataEncryption.cpp)

if(BUILD_THOR)
    target_link_libraries(data_encryption
            common
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libssl.so
            ${SENSEAUTO_3RDPARTY}/openssl/lib64/libcrypto.so
    )
else()
    target_link_directories(data_encryption PRIVATE
            ${OpenSSL_LIBRARIES_DIR}
    )

    target_link_libraries(data_encryption
            common
            ssl
            crypto
    )
endif()

install(TARGETS data_encryption
        ARCHIVE DESTINATION lib)