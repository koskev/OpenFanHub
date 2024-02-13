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
CMAKE_SOURCE_DIR = /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src/build

# Utility rule file for CMSIS_LD_F101VF.

# Include any custom commands dependencies for this target.
include CMakeFiles/CMSIS_LD_F101VF.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CMSIS_LD_F101VF.dir/progress.make

CMakeFiles/CMSIS_LD_F101VF: F101VF.ld

F101VF.ld:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating F101VF.ld"
	/usr/bin/cmake -DFLASH_ORIGIN="0x8000000" -DRAM_ORIGIN="0x20000000" -DCCRAM_ORIGIN="0x10000000" -DRAM_SHARE_ORIGIN="0x20030000" -DFLASH_SIZE="768K" -DRAM_SIZE="80K" -DCCRAM_SIZE="0K" -DRAM_SHARE_SIZE="NOTFOUND" -DSTACK_SIZE="0x400" -DHEAP_SIZE="0x200" -DLINKER_SCRIPT="/mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src/build/F101VF.ld" -P /home/kevin/cmake/stm32/stm32/linker_ld.cmake

CMSIS_LD_F101VF: CMakeFiles/CMSIS_LD_F101VF
CMSIS_LD_F101VF: F101VF.ld
CMSIS_LD_F101VF: CMakeFiles/CMSIS_LD_F101VF.dir/build.make
.PHONY : CMSIS_LD_F101VF

# Rule to build all files generated by this target.
CMakeFiles/CMSIS_LD_F101VF.dir/build: CMSIS_LD_F101VF
.PHONY : CMakeFiles/CMSIS_LD_F101VF.dir/build

CMakeFiles/CMSIS_LD_F101VF.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CMSIS_LD_F101VF.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CMSIS_LD_F101VF.dir/clean

CMakeFiles/CMSIS_LD_F101VF.dir/depend:
	cd /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src/build /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src/build /mnt/hdd/home/kevin/Dokumente/Projekte/Github/OpenFanHub/Firmware/src/build/CMakeFiles/CMSIS_LD_F101VF.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CMSIS_LD_F101VF.dir/depend

