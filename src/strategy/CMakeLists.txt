add_library(strategy SHARED
        StrategyParser.cpp)

target_link_libraries(strategy
		${BOOST_LIBRARIES}
)

install(TARGETS strategy
		ARCHIVE DESTINATION lib)
