# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add source to this project's executable.
#add_executable (CMakeProject1 "src/CMakeProject1.cpp" "src/CMakeProject1.h" "hello_test.cpp")

include_directories("include")
# TODO: Add tests and install targets if needed.
enable_testing()

add_executable(
  hello_test
  #"hello_test.cpp"
  "../src/logic_t.cpp"
  "../src/cl.cpp"
  "../src/seq_queue.cpp"
  "../src/core.cpp"
  "../src/inst_field.cpp"
)
target_link_libraries(
  hello_test
  gtest_main
)

include(GoogleTest)

gtest_discover_tests(hello_test)