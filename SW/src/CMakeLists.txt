# CMakeList.txt : CMake project for SW, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../include")
# Add source to this project"../inc/SW.h"cutable."../include/logic_t.h"
add_executable (ama-riscv-sim 
"logic_t.cpp"  
"main.cpp" 
"cl.cpp" 
"seq_queue.cpp" 
"core.cpp" 
"inst_field.cpp" 
"decoder.cpp" 
"control.cpp"
"op_fwd.cpp"
"reg_file.cpp"
 "intf_cfg.cpp" "modules.cpp" "cpu.cpp" "memory.cpp" "perf_cpu.cpp")

# TODO: Add tests and install targets if needed.
