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
include CMakeFiles/CDPSDK.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CDPSDK.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CDPSDK.dir/flags.make

CMakeFiles/CDPSDK.dir/cdp.cpp.o: CMakeFiles/CDPSDK.dir/flags.make
CMakeFiles/CDPSDK.dir/cdp.cpp.o: ../cdp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CDPSDK.dir/cdp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CDPSDK.dir/cdp.cpp.o -c /Users/krdixson/Desktop/CDPSDK/cdp.cpp

CMakeFiles/CDPSDK.dir/cdp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CDPSDK.dir/cdp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/krdixson/Desktop/CDPSDK/cdp.cpp > CMakeFiles/CDPSDK.dir/cdp.cpp.i

CMakeFiles/CDPSDK.dir/cdp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CDPSDK.dir/cdp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/krdixson/Desktop/CDPSDK/cdp.cpp -o CMakeFiles/CDPSDK.dir/cdp.cpp.s

CMakeFiles/CDPSDK.dir/CDPInterface.cpp.o: CMakeFiles/CDPSDK.dir/flags.make
CMakeFiles/CDPSDK.dir/CDPInterface.cpp.o: ../CDPInterface.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CDPSDK.dir/CDPInterface.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CDPSDK.dir/CDPInterface.cpp.o -c /Users/krdixson/Desktop/CDPSDK/CDPInterface.cpp

CMakeFiles/CDPSDK.dir/CDPInterface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CDPSDK.dir/CDPInterface.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/krdixson/Desktop/CDPSDK/CDPInterface.cpp > CMakeFiles/CDPSDK.dir/CDPInterface.cpp.i

CMakeFiles/CDPSDK.dir/CDPInterface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CDPSDK.dir/CDPInterface.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/krdixson/Desktop/CDPSDK/CDPInterface.cpp -o CMakeFiles/CDPSDK.dir/CDPInterface.cpp.s

# Object files for target CDPSDK
CDPSDK_OBJECTS = \
"CMakeFiles/CDPSDK.dir/cdp.cpp.o" \
"CMakeFiles/CDPSDK.dir/CDPInterface.cpp.o"

# External object files for target CDPSDK
CDPSDK_EXTERNAL_OBJECTS =

libCDPSDK.dylib: CMakeFiles/CDPSDK.dir/cdp.cpp.o
libCDPSDK.dylib: CMakeFiles/CDPSDK.dir/CDPInterface.cpp.o
libCDPSDK.dylib: CMakeFiles/CDPSDK.dir/build.make
libCDPSDK.dylib: CMakeFiles/CDPSDK.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/krdixson/Desktop/CDPSDK/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libCDPSDK.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CDPSDK.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CDPSDK.dir/build: libCDPSDK.dylib

.PHONY : CMakeFiles/CDPSDK.dir/build

CMakeFiles/CDPSDK.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CDPSDK.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CDPSDK.dir/clean

CMakeFiles/CDPSDK.dir/depend:
	cd /Users/krdixson/Desktop/CDPSDK/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/krdixson/Desktop/CDPSDK /Users/krdixson/Desktop/CDPSDK /Users/krdixson/Desktop/CDPSDK/cmake-build-debug /Users/krdixson/Desktop/CDPSDK/cmake-build-debug /Users/krdixson/Desktop/CDPSDK/cmake-build-debug/CMakeFiles/CDPSDK.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CDPSDK.dir/depend
