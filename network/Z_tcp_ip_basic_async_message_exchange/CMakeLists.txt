cmake_minimum_required(VERSION 3.0)

include_directories(/home/jconnor/Seifenkiste/network/1_tcp_ip_basic_async_message_exchange)
#include_directories(/usr/include/linux)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -Wall")
#SET(GCC_COVERAGE_LINK_FLAGS "-pthread")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
message(STATUS "cxx flags = ${CMAKE_CXX_FLAGS}")

project(tcp_lib)
add_library(tcp_lib SHARED tcp_lib.cpp)
get_target_property(INCLUDE_DIRS tcp_lib INCLUDE_DIRECTORIES)
message("include dirs : ${INCLUDE_DIRS}")

project(tcp_client_test)
add_library(tcp_client_class SHARED tcp_client_class.cpp)
add_executable(tcp_client_test network__client_test.cpp)
target_link_libraries (tcp_client_test tcp_client_class tcp_lib)

project(tcp_server_test)
add_library(tcp_server_class SHARED tcp_server_class.cpp)
add_executable(tcp_server_test network__server_test.cpp)
target_link_libraries (tcp_server_test tcp_server_class tcp_lib)
