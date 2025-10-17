set(_target "ad_base")

# Include
list(APPEND ${_target}_INCLUDE_DIRS "${Rscl_PATH}/include/${_target}")

# Library
list(APPEND ${_target}_LIBRARIES "${_target}")
list(APPEND ${_target}_LIBRARIES "pthread")

# Debug
# message(STATUS "${_target}_INCLUDE_DIRS: ${${_target}_INCLUDE_DIRS}")
# message(STATUS "${_target}_LIBRARIES: ${${_target}_LIBRARIES}")
