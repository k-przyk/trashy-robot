# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/przyk/Documents/dev/nanocar/trashy-robot/controller

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/przyk/Documents/dev/nanocar/trashy-robot/controller

# Include any dependencies generated for this target.
include CMakeFiles/point_generator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/point_generator.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/point_generator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/point_generator.dir/flags.make

CMakeFiles/point_generator.dir/src/point_generator.cpp.o: CMakeFiles/point_generator.dir/flags.make
CMakeFiles/point_generator.dir/src/point_generator.cpp.o: src/point_generator.cpp
CMakeFiles/point_generator.dir/src/point_generator.cpp.o: CMakeFiles/point_generator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/przyk/Documents/dev/nanocar/trashy-robot/controller/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/point_generator.dir/src/point_generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/point_generator.dir/src/point_generator.cpp.o -MF CMakeFiles/point_generator.dir/src/point_generator.cpp.o.d -o CMakeFiles/point_generator.dir/src/point_generator.cpp.o -c /home/przyk/Documents/dev/nanocar/trashy-robot/controller/src/point_generator.cpp

CMakeFiles/point_generator.dir/src/point_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/point_generator.dir/src/point_generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/przyk/Documents/dev/nanocar/trashy-robot/controller/src/point_generator.cpp > CMakeFiles/point_generator.dir/src/point_generator.cpp.i

CMakeFiles/point_generator.dir/src/point_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/point_generator.dir/src/point_generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/przyk/Documents/dev/nanocar/trashy-robot/controller/src/point_generator.cpp -o CMakeFiles/point_generator.dir/src/point_generator.cpp.s

# Object files for target point_generator
point_generator_OBJECTS = \
"CMakeFiles/point_generator.dir/src/point_generator.cpp.o"

# External object files for target point_generator
point_generator_EXTERNAL_OBJECTS =

bin/point_generator: CMakeFiles/point_generator.dir/src/point_generator.cpp.o
bin/point_generator: CMakeFiles/point_generator.dir/build.make
bin/point_generator: /usr/lib/x86_64-linux-gnu/libzmq.so
bin/point_generator: CMakeFiles/point_generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/przyk/Documents/dev/nanocar/trashy-robot/controller/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/point_generator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/point_generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/point_generator.dir/build: bin/point_generator
.PHONY : CMakeFiles/point_generator.dir/build

CMakeFiles/point_generator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/point_generator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/point_generator.dir/clean

CMakeFiles/point_generator.dir/depend:
	cd /home/przyk/Documents/dev/nanocar/trashy-robot/controller && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller/CMakeFiles/point_generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/point_generator.dir/depend

