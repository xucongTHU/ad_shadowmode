add_library(channel SHARED
        RsclChannelManager.cpp
)

install(TARGETS channel
        ARCHIVE DESTINATION lib)