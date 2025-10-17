# Find AD framework root
if (PATH_FW_INSTALLED_DIR)
    # For other project
    set(CYBER_ROOT "${PATH_FW_INSTALLED_DIR}/")
    set(CYBER_LIB_ROOT "${CYBER_ROOT}/lib")
    set(fastrtps_DIR "${CYBER_ROOT}/3rdparty/fastrtps/cmake")
else()
    # For senseauto-rscl
    set(CYBER_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
    set(CYBER_LIB_ROOT "${CYBER_ROOT}/lib/${PLATFORM_NAME}")
    set(fastrtps_DIR "${CYBER_ROOT}/../fastrtps/cmake")
endif()
get_filename_component(CYBER_ROOT "${CYBER_ROOT}" ABSOLUTE)
get_filename_component(CYBER_LIB_ROOT "${CYBER_LIB_ROOT}" ABSOLUTE)

IF(NOT EXISTS ${CYBER_ROOT})
    message(WARNING "Cannot find the installed cyber directory: ${CYBER_ROOT}. DEPENED variable: CYBER_ROOT")
endif()
IF(NOT PLATFORM_NAME)
    message(FATAL_ERROR "Must provide PLATFORM_NAME")
endif()

message(STATUS "Cyber: ${CYBER_ROOT}")

list(APPEND CYBER_INCLUDE_DIRS
    ${CYBER_ROOT}/include
)
get_filename_component(CYBER_LIB_ROOT "${CYBER_LIB_ROOT}" ABSOLUTE)

list(APPEND CYBER_LIBRARY_DIRS ${CYBER_LIB_ROOT})
list(APPEND CYBER_LIBRARIES
    "uuid"
    "cyber"
    "cyber_Sproto_Sliball_Uproto"
)

if (NOT PATH_SOURCE_3RDPARTY)
    message(FATAL_ERROR "Must provide PATH_SOURCE_3RDPARTY to use 3rdparty libs")
endif()
list(APPEND CYBER_LIBRARIES
    "glog.so"
    "gflags.so"
    "protobuf.so"
    "z"
    "rt"
)
list(APPEND CYBER_LIBRARY_DIRS "${PATH_SOURCE_3RDPARTY}/lib")

message(STATUS "fastrtps_DIR: ${fastrtps_DIR}")
find_package(fastrtps REQUIRED)

# For senseauto-rscl
list(APPEND CYBER_LIBRARY_DIRS ${FASTRTPS_LIBRARY_DIRS})
list(APPEND CYBER_LIBRARIES ${FASTRTPS_LIBRARIES})
list(APPEND CYBER_INCLUDE_DIRS ${FASTRTPS_INCLUDE_DIRS})
list(APPEND CYBER_LINK_OPTIONS "LINKER:--no-as-needed" "LINKER:--disable-new-dtags")

function(install_cyber_sense)
    install_fastrtps_sense()
    install(DIRECTORY ${CYBER_INCLUDE_DIRS}
        DESTINATION .
    )
    file(GLOB_RECURSE OUT_LIBS "${CYBER_LIB_ROOT}/*")
    install(FILES ${OUT_LIBS}
        DESTINATION lib
    )
endfunction()
