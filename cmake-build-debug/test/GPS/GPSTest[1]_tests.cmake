add_test( GPSTest.Example /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS/GPSTest [==[--gtest_filter=GPSTest.Example]==] --gtest_also_run_disabled_tests)
set_tests_properties( GPSTest.Example PROPERTIES WORKING_DIRECTORY /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS)
add_test( GPSTest.ParseRMC /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS/GPSTest [==[--gtest_filter=GPSTest.ParseRMC]==] --gtest_also_run_disabled_tests)
set_tests_properties( GPSTest.ParseRMC PROPERTIES WORKING_DIRECTORY /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS)
set( GPSTest_TESTS GPSTest.Example GPSTest.ParseRMC)
