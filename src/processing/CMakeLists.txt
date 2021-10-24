# Headers listed as sources don’t get compiled directly on their own, but the effect of adding them is for the benefit
# of IDE generators causing those headers to be listed in the project’s file list within the IDE, even if no source file
# refers to it via #include. This can make those headers easier to find during development and potentially aid things
# like refactoring functionality, etc.
set(FILES
        Packet.cpp
        Packet.h
)

target_sources(${PROJECT_NAME} PRIVATE ${FILES})