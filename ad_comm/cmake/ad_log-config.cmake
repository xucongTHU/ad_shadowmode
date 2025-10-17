set(_target "ad_log")

# Include
list(APPEND ${_target}_INCLUDE_DIRS "${Rscl_PATH}/include/${_target}")

# Library
list(APPEND ${_target}_LIBRARIES "${_target}")

if (ENABLE_GAC_LOG)
    add_definitions(-DSPDLOG_FMT_EXTERNAL)
    set(DEPS_ROOT "${SENSEAUTO_DEV_INSTALLED_ROOT}/${FIND_SENSEAUTO_PACKAGE_PATH}/senseauto_active")
    list(APPEND ${_target}_INCLUDE_DIRS
        ${DEPS_ROOT}/senseauto-3rdparty/3rdparty/include)
    list(APPEND ${_target}_INCLUDE_DIRS "${HOBOT_INCLUDE_DIRS}")
    list(APPEND ${_target}_INCLUDE_DIRS "${HOBOT_INCLUDE_DIRS}/gacrnd/")
    list(APPEND ${_target}_INCLUDE_DIRS "${HOBOT_INCLUDE_DIRS}/gacrnd/gslog")
    list(APPEND ${_target}_LIBRARIES ${HOBOT_ROOT}/lib/gacrnd/libgslog.so)
	# list(APPEND ${_target}_LIBRARIES ${HOBOT_ROOT}/lib/libfmt.so)
endif()


# Debug
# message(STATUS "${_target}_INCLUDE_DIRS: ${${_target}_INCLUDE_DIRS}")
# message(STATUS "${_target}_LIBRARIES: ${${_target}_LIBRARIES}")
