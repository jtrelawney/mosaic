cmake_minimum_required(VERSION 3.0)

#include_directories(/home/jconnor/Seifenkiste/network/0_tcp_ip_basic_lib_message_exchange/)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -Wall -Wpedantic")
#SET(GCC_COVERAGE_LINK_FLAGS "-pthread")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
message(STATUS "cxx flags = ${CMAKE_CXX_FLAGS}")

project(virt_base_class)
add_executable(virt_base_class.exe virt_base_class.cpp)

#[[
project(server_tcp_test)
add_library(tcp_server_class SHARED tcp_server_class.cpp)
add_executable(server_tcp_test.exe network__server_test.cpp)
target_link_libraries (server_tcp_test.exe message_lib tcp_server_class tcp_lib)
]]
