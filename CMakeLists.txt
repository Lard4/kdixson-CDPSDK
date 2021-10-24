cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)

# save the project name as the variable ${PROJECT_NAME} so we can conveniently access it everywhere (including subdirs!)
project(CDPSDK)

# set project version
set(PROJECT_VER_MAJOR 0)
set(PROJECT_VER_MINOR 1)
set(PROJECT_VER_PATCH 0)
set(PROJECT_VER_STR "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
message("Building Version: " ${PROJECT_VER_STR})

# FetchContent is used to get libaries from git. used to pull third party libraries like GoogleTest and our Crypto lib.
include(FetchContent)

#-----------------------------------------------------------------------------------------------------------------------
# Fetch the Google Test C++ unit testing library from GitHub
FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# make the library available to any other CMakeLists.txt file to pull in
FetchContent_MakeAvailable(googletest)
# End: Google Test setup
#-----------------------


#-----------------------------------------------------------------------------------------------------------------------
# Fetch the Crypto library from GitHub
FetchContent_Declare(cryptolib
        GIT_REPOSITORY https://github.com/Project-Owl/Crypto.git
)
# make it available to subdirectories. used in the src/crypto module
FetchContent_MakeAvailable(cryptolib)
# End: Crypto library setup
#--------------------------


#-----------------------------------------------------------------------------------------------------------------------
# Begin: actual library setup
# note that this comes after setting everything else up!

# "" means we don't have any source files in this top-level directory.
add_library(${PROJECT_NAME} STATIC "")

set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)

# Expose the header files in the ./include directory for all source code to #include directly
# this eliminates the need for writing: #include "../../../../include/gps.h" (or something ridiculous like that)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# tell CMake to go into these subdirectories and run their CMakeLists.txt files
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)