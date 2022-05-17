# CMakeList.txt : CMake project for SW, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories("../include")
# Add source to this project"../inc/SW.h"cutable."../include/logic_t.h"
add_executable (riscv "logic_t.cpp"  "main.cpp")

# TODO: Add tests and install targets if needed.
