if(EXISTS "/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS/hello_test[1]_tests.cmake")
  include("/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS/hello_test[1]_tests.cmake")
else()
  add_test(hello_test_NOT_BUILT hello_test_NOT_BUILT)
endif()