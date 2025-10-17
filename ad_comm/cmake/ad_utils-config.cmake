set(_target "ad_utils")

# Include
list(APPEND ${_target}_INCLUDE_DIRS "${Rscl_PATH}/include/${_target}")

# Library
list(APPEND ${_target}_LIBRARIES "${_target}")

