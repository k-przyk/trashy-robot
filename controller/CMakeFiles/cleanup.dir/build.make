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

# Utility rule file for cleanup.

# Include any custom commands dependencies for this target.
include CMakeFiles/cleanup.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cleanup.dir/progress.make

CMakeFiles/cleanup:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/przyk/Documents/dev/nanocar/trashy-robot/controller/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Cleaning build artifacts"
	/usr/bin/cmake -E remove_directory /home/przyk/Documents/dev/nanocar/trashy-robot/controller/bin
	/usr/bin/cmake -E remove_directory /home/przyk/Documents/dev/nanocar/trashy-robot/controller/CMakeFiles
	/usr/bin/cmake -E remove /home/przyk/Documents/dev/nanocar/trashy-robot/controller/cmake_install.cmake
	/usr/bin/cmake -E remove /home/przyk/Documents/dev/nanocar/trashy-robot/controller/Makefile
	/usr/bin/cmake -E remove /home/przyk/Documents/dev/nanocar/trashy-robot/controller/CMakeCache.txt

cleanup: CMakeFiles/cleanup
cleanup: CMakeFiles/cleanup.dir/build.make
.PHONY : cleanup

# Rule to build all files generated by this target.
CMakeFiles/cleanup.dir/build: cleanup
.PHONY : CMakeFiles/cleanup.dir/build

CMakeFiles/cleanup.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cleanup.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cleanup.dir/clean

CMakeFiles/cleanup.dir/depend:
	cd /home/przyk/Documents/dev/nanocar/trashy-robot/controller && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller /home/przyk/Documents/dev/nanocar/trashy-robot/controller/CMakeFiles/cleanup.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cleanup.dir/depend
