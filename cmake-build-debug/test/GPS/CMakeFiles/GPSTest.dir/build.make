# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/krdixson/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/202.7319.62/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/krdixson/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/202.7319.62/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/krdixson/Desktop/CDPSDK

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/krdixson/Desktop/CDPSDK/cmake-build-debug

# Include any dependencies generated for this target.
include test/GPS/CMakeFiles/GPSTest.dir/depend.make

# Include the progress variables for this target.
include test/GPS/CMakeFiles/GPSTest.dir/progress.make

# Include the compile flags for this target's objects.
include test/GPS/CMakeFiles/GPSTest.dir/flags.make

test/GPS/CMakeFiles/GPSTest.dir/GPSTest.cpp.o: test/GPS/CMakeFiles/GPSTest.dir/flags.make
test/GPS/CMakeFiles/GPSTest.dir/GPSTest.cpp.o: ../test/GPS/GPSTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/GPS/CMakeFiles/GPSTest.dir/GPSTest.cpp.o"
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GPSTest.dir/GPSTest.cpp.o -c /Users/krdixson/Desktop/CDPSDK/test/GPS/GPSTest.cpp

test/GPS/CMakeFiles/GPSTest.dir/GPSTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GPSTest.dir/GPSTest.cpp.i"
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/krdixson/Desktop/CDPSDK/test/GPS/GPSTest.cpp > CMakeFiles/GPSTest.dir/GPSTest.cpp.i

test/GPS/CMakeFiles/GPSTest.dir/GPSTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GPSTest.dir/GPSTest.cpp.s"
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/krdixson/Desktop/CDPSDK/test/GPS/GPSTest.cpp -o CMakeFiles/GPSTest.dir/GPSTest.cpp.s

# Object files for target GPSTest
GPSTest_OBJECTS = \
"CMakeFiles/GPSTest.dir/GPSTest.cpp.o"

# External object files for target GPSTest
GPSTest_EXTERNAL_OBJECTS =

test/GPS/GPSTest: test/GPS/CMakeFiles/GPSTest.dir/GPSTest.cpp.o
test/GPS/GPSTest: test/GPS/CMakeFiles/GPSTest.dir/build.make
test/GPS/GPSTest: src/GPS/libGPS.a
test/GPS/GPSTest: lib/libgtest_maind.a
test/GPS/GPSTest: lib/libgtestd.a
test/GPS/GPSTest: test/GPS/CMakeFiles/GPSTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GPSTest"
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GPSTest.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS && "/Users/krdixson/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/202.7319.62/CLion.app/Contents/bin/cmake/mac/bin/cmake" -D TEST_TARGET=GPSTest -D TEST_EXECUTABLE=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS/GPSTest -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=GPSTest_TESTS -D CTEST_FILE=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS/GPSTest[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -P "/Users/krdixson/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/202.7319.62/CLion.app/Contents/bin/cmake/mac/share/cmake-3.17/Modules/GoogleTestAddTests.cmake"

# Rule to build all files generated by this target.
test/GPS/CMakeFiles/GPSTest.dir/build: test/GPS/GPSTest

.PHONY : test/GPS/CMakeFiles/GPSTest.dir/build

test/GPS/CMakeFiles/GPSTest.dir/clean:
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS && $(CMAKE_COMMAND) -P CMakeFiles/GPSTest.dir/cmake_clean.cmake
.PHONY : test/GPS/CMakeFiles/GPSTest.dir/clean

test/GPS/CMakeFiles/GPSTest.dir/depend:
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/krdixson/Desktop/CDPSDK /Users/krdixson/Desktop/CDPSDK/test/GPS /Users/krdixson/Desktop/CDPSDK/cmake-build-debug /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/test/GPS/CMakeFiles/GPSTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/GPS/CMakeFiles/GPSTest.dir/depend
