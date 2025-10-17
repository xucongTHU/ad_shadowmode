file(GLOB_RECURSE TRIGGER_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scheduler/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/rule/*.cpp
)

add_library(trigger SHARED ${TRIGGER_SOURCES})

install(TARGETS trigger
        LIBRARY DESTINATION lib)