enable_testing()

add_executable(
        GPSTest
        GPSTest.cpp
)
target_link_libraries(
        GPSTest
        GPS
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(GPSTest)