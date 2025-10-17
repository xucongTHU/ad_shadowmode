if (PATH_FW_INSTALLED_DIR)
    # For other project
    set(FASTRTPS_ROOT "${PATH_FW_INSTALLED_DIR}/")
    set(FASTRTPS_LIB_ROOT "${FASTRTPS_ROOT}/lib/")
else()
    # For senseauto-rscl
    set(FASTRTPS_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
    set(FASTRTPS_LIB_ROOT "${FASTRTPS_ROOT}/lib/${PLATFORM_NAME}/")
endif()

get_filename_component(FASTRTPS_ROOT "${FASTRTPS_ROOT}" ABSOLUTE)
get_filename_component(FASTRTPS_LIB_ROOT "${FASTRTPS_LIB_ROOT}" ABSOLUTE)
IF(NOT EXISTS ${FASTRTPS_ROOT})
    message(WARNING "Cannot find the fastrtps lib. DEPENED variable: FASTRTPS_ROOT")
endif()
IF(NOT PLATFORM_NAME)
    message(FATAL_ERROR "Must provide PLATFORM_NAME")
endif()

list(APPEND FASTRTPS_INCLUDE_DIRS
    ${FASTRTPS_ROOT}/include
)
list(APPEND FASTRTPS_LIBRARY_DIRS
    ${FASTRTPS_LIB_ROOT}
)
list(APPEND FASTRTPS_LIBRARIES
    "fastcdr"
    "fastrtps"
    "foonathan_memory-0.7.1"
)

function(install_fastrtps_sense)
    install(DIRECTORY ${FASTRTPS_ROOT}/include
        DESTINATION .
    )
    file(GLOB_RECURSE OUT_LIBS "${FASTRTPS_LIB_ROOT}/*")
    install(FILES ${OUT_LIBS}
        DESTINATION lib
    )
endfunction()
