add_library(common SHARED
        logger/Logger.cpp
		time/Timer.cpp
		utils/utils.cpp
		utils/statusUtils.cpp
		utils/sRegex.cpp
		utils/debouncer.cpp
)
target_link_libraries(common
		${YAML_CPP_LIBRARIES}
		${PROTOBUF_LIBRARIES}
		${SENSEAUTO_3RDPARTY}/boost/lib/libboost_regex.so

)
install(TARGETS common
		ARCHIVE DESTINATION lib)
