# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/net/xiaodong/actor/actor-framework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/net/xiaodong/actor/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/distributed_calculator.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/distributed_calculator.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/distributed_calculator.dir/flags.make

examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o: examples/CMakeFiles/distributed_calculator.dir/flags.make
examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o: /home/net/xiaodong/actor/actor-framework/examples/remoting/distributed_calculator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/net/xiaodong/actor/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o"
	cd /home/net/xiaodong/actor/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o -c /home/net/xiaodong/actor/actor-framework/examples/remoting/distributed_calculator.cpp

examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.i"
	cd /home/net/xiaodong/actor/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/net/xiaodong/actor/actor-framework/examples/remoting/distributed_calculator.cpp > CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.i

examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.s"
	cd /home/net/xiaodong/actor/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/net/xiaodong/actor/actor-framework/examples/remoting/distributed_calculator.cpp -o CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.s

examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.requires:
.PHONY : examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.requires

examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.provides: examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/distributed_calculator.dir/build.make examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.provides.build
.PHONY : examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.provides

examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.provides.build: examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o

# Object files for target distributed_calculator
distributed_calculator_OBJECTS = \
"CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o"

# External object files for target distributed_calculator
distributed_calculator_EXTERNAL_OBJECTS =

bin/distributed_calculator: examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o
bin/distributed_calculator: examples/CMakeFiles/distributed_calculator.dir/build.make
bin/distributed_calculator: lib/libcaf_core.so.0.15.0
bin/distributed_calculator: lib/libcaf_io.so.0.15.0
bin/distributed_calculator: lib/libcaf_core.so.0.15.0
bin/distributed_calculator: examples/CMakeFiles/distributed_calculator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/distributed_calculator"
	cd /home/net/xiaodong/actor/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/distributed_calculator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/distributed_calculator.dir/build: bin/distributed_calculator
.PHONY : examples/CMakeFiles/distributed_calculator.dir/build

examples/CMakeFiles/distributed_calculator.dir/requires: examples/CMakeFiles/distributed_calculator.dir/remoting/distributed_calculator.cpp.o.requires
.PHONY : examples/CMakeFiles/distributed_calculator.dir/requires

examples/CMakeFiles/distributed_calculator.dir/clean:
	cd /home/net/xiaodong/actor/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/distributed_calculator.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/distributed_calculator.dir/clean

examples/CMakeFiles/distributed_calculator.dir/depend:
	cd /home/net/xiaodong/actor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/net/xiaodong/actor/actor-framework /home/net/xiaodong/actor/actor-framework/examples /home/net/xiaodong/actor/build /home/net/xiaodong/actor/build/examples /home/net/xiaodong/actor/build/examples/CMakeFiles/distributed_calculator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/distributed_calculator.dir/depend

