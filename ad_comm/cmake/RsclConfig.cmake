# Find AD framework root
if (NOT FIND_SENSEAUTO_PACKAGE_PATH)
    message(WARNING "Please set FIND_SENSEAUTO_PACKAGE_PATH to find AD framework root")
endif()
if(NOT AD_CROSS_COMPILE)
    set(Rscl_PATH ${SENSEAUTO_DEV_INSTALLED_ROOT}/${FIND_SENSEAUTO_PACKAGE_PATH}/senseauto/senseauto-rscl/active)
else()
    set(Rscl_PATH ${SENSEAUTO_DEV_INSTALLED_ROOT}/${FIND_SENSEAUTO_PACKAGE_PATH}/senseauto/senseauto-rscl/active)
endif()

message(STATUS "Rscl: ${Rscl_PATH}")
list(APPEND Rscl_INCLUDE_DIRS ${Rscl_PATH}/include)
list(APPEND Rscl_INCLUDE_DIRS ${SENSEAUTO_DEV_INSTALLED_ROOT}/${FIND_SENSEAUTO_PACKAGE_PATH}/senseauto_active/senseauto-msgs/include/ad_msg_idl/)
list(APPEND Rscl_LIBRARY_DIRS ${Rscl_PATH}/lib)

set(cyber_DIR "${Rscl_PATH}/3rdparty/cyber/cmake")
message(STATUS "cyber_DIR: ${cyber_DIR}")

# Search AD component
foreach(component ${Rscl_FIND_COMPONENTS})
    message("\tUse component: ${component}")
    include(${CMAKE_CURRENT_LIST_DIR}/${component}-config.cmake)
    list(APPEND Rscl_INCLUDE_DIRS ${${component}_INCLUDE_DIRS})
    list(APPEND Rscl_LIBRARIES ${${component}_LIBRARIES})
endforeach()

# Debug
# message(STATUS "Rscl_INCLUDE_DIRS: ${Rscl_INCLUDE_DIRS}")
# message(STATUS "Rscl_LIBRARIES: ${Rscl_LIBRARIES}")
