# required to have GoogleTest run
enable_testing()

# set the name of this "test suite" as a CMake variable
set(TEST_SUITE "GPSTest")

# create an executable to run the tests
add_executable(${TEST_SUITE}
        GPSTest.cpp
)

# pull in libraries required for testing
target_link_libraries(${TEST_SUITE}
        CDPSDK          # grab the CDP SDK so we can test its code
        gtest_main      # grab GoogleTest so we can do `expect` calls
        gmock           # grab gMock so we can mock out calls to the (nonexistent) hardware
)

# more GoogleTest boilerplate...
include(GoogleTest)
gtest_discover_tests(${TEST_SUITE})