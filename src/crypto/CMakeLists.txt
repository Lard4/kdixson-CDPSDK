# Headers listed as sources don’t get compiled directly on their own, but the effect of adding them is for the benefit
# of IDE generators causing those headers to be listed in the project’s file list within the IDE, even if no source file
# refers to it via #include. This can make those headers easier to find during development and potentially aid things
# like refactoring functionality, etc.
set(FILES
        duckcrypto.cpp
        duckcrypto.h
)

target_sources(${PROJECT_NAME} PRIVATE ${FILES})

# tell CMake that we're going to need cryptolib to build (INTERFACE means we need it to build)
add_library(cryptolib INTERFACE)

# include the header files from the source directory of the cryptolib library to the cryptolib library itself
target_include_directories(cryptolib INTERFACE ${cryptolib_SOURCE_DIR})

# link the cryptolib library to our project
target_link_libraries(${PROJECT_NAME} cryptolib)