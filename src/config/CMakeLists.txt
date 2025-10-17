add_library(config SHARED
        AppConfig.cpp
)
target_link_libraries(config)
install(TARGETS config
        ARCHIVE DESTINATION lib)
