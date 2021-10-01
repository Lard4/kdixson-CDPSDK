cmake_minimum_required(VERSION 3.17)
project(CDPSDK)

set(PROJECT_VER_MAJOR 1)
set(PROJECT_VER_MINOR 0)
set(PROJECT_VER_PATCH 0)
set(PROJECT_VER_STR "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
message("Building Version: " ${PROJECT_VER_STR})

set(CMAKE_CXX_STANDARD 14)

#-----------------------------------------------------------------------------------------------------------------------
# Set up Google Test C++ unit testing library

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# End: Google Test setup
#-----------------------------------------------------------------------------------------------------------------------

add_library(CDPSDK SHARED cdp.cpp cdp.h CDPInterface.cpp)

# Include the generic includes for everybody
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Finally, add sub directories after configuration is complete
add_subdirectory(src)
add_subdirectory(test)
