cmake_minimum_required(VERSION 3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -Wall -pedantic -Wextra -Werror -pthread -O2 -g")
SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -Wall -pedantic -Wextra -Werror -fsanitize=thread -pthread -O2 -g")

#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
message(STATUS "cxx compile flags = ${CMAKE_CXX_FLAGS}")

SET(GCC_COVERAGE_LINK_FLAGS "-lpthread")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
message(STATUS "cxx link flags = ${CMAKE_EXE_LINKER_FLAGS}")

set(GENERAL_LIBS rt)

project(0_threads)
add_executable(0_threads.exe 0_threads.cpp)

project(1_concurrent_access)
add_executable(1_concurrent_access.exe 1_concurrent_access.cpp)

project(2_concurrent_access)
add_executable(2_concurrent_access.exe 2_concurrent_access_class_mutex.cpp)

project(3_concurrent_access_queue)
add_executable(3_concurrent_access_queue.exe 3_concurrent_access_queue.cpp)

project(4_concurrent_access_spmc)
add_executable(4_concurrent_access_spmc.exe 4_concurrent_access_spmc.cpp)

project(5_queue_spsc_cond_vars)
add_executable(5_queue_spsc_cond_vars.exe 5_queue_spsc_cond_vars.cpp)

project(6_queue_spmc_cond_vars)
add_executable(6_queue_spmc_cond_vars.exe 6_queue_spmc_cond_vars.cpp)

project(7_queue_mpmc_cond_vars)
add_executable(7_queue_mpmc_cond_vars.exe 7_queue_mpmc_cond_vars.cpp)

project(8_queue_mpmc_cond_vars)
add_executable(8_queue_mpmc_cond_vars.exe 8_queue_mpmc_cond_vars.cpp)

project(9_cond_vars_example_1)
add_executable(9_cond_vars_example_1.exe 9_cond_vars_example_1.cpp)

project(9_cond_vars_example_2)
add_executable(9_cond_vars_example_2.exe 9_cond_vars_example_2.cpp)

project(9_cond_vars_example_3)
add_executable(9_cond_vars_example_3.exe 9_cond_vars_example_3.cpp)

#project(9_cond_vars_example)
#dd_executable(9_cond_vars_example.exe 9_cond_vars_example.cpp)



# where the libs are compiled into
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#message ("\nsetting library output path:")
#message (${CMAKE_LIBRARY_OUTPUT_DIRECTORY} "\n")


# setting directories
#set(PROJECT_PATH "/home/ethan/projects/Seifenkiste_1")
#set(DIR_KICKSTARTER ${PROJECT_PATH}/kickstarters)
#set(DIR_COMMON ${PROJECT_PATH}/lib)
#set(DIR_PC_LIBS ${DIR_COMMON}/pc_lib)
#set(DIR_PROJECT ${PROJECT_PATH}/PC_cockpit)

#message ("\nusing the following source directories for the main and libs:")
#message (${DIR_KICKSTARTER})
#message (${PROJECT_PATH})
#message (${DIR_COMMON})
#message (${DIR_PC_LIBS})
#message (${DIR_PROJECT})


# include directories
#include_directories( ${DIR_COMMON} ${DIR_PC_LIBS} )
#message ("\n\nheader dirs:")
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()

# set libraries to use for this project
#set(OPENCV_LIBS opencv_core opencv_highgui opencv_imgproc opencv_video opencv_videoio)
#set(MY_LIBS common_stuff message_class tcp_server message_queue_class)

#set(ALL_PROJECT_LIBS ${GENERAL_LIBS} ${MY_LIBS})
#message ("\n\nsetting libraries as :\n" ${ALL_PROJECT_LIBS} "\n")


# define project targets, what will be compiled
#set(PROJECT_SOURCES  ${DIR_COMMON}/message_class.cpp ${DIR_PROJECT}/stream_rpi_camera.cpp )

#set(PROJECT_SOURCES  ${DIR_KICKSTARTER}/threads/1_concurrent_access.cpp)
#set(PROJECT_SOURCES  ${DIR_KICKSTARTER}/threads/2_concurrent_access_class_mutex.cpp)
#set(PROJECT_SOURCES  ${DIR_KICKSTARTER}/threads/3_concurrent_access_queue.cpp)
#set(PROJECT_SOURCES  ${DIR_KICKSTARTER}/threads/4_concurrent_access_spmc.cpp)
#set(PROJECT_SOURCES  ${DIR_KICKSTARTER}/threads/5_queue_spsc_cond_vars.cpp)
#set(PROJECT_SOURCES  ${DIR_KICKSTARTER}/threads/6_queue_spmc_cond_vars.cpp)
#set(PROJECT_SOURCES  ${DIR_KICKSTARTER}/threads/7_queue_mpmc_cond_vars.cpp)


message ("\n\nmain target to compile")
message ("${PROJECT_SOURCES}")

#project(tcp_server_test)
#add_library(tcp_server_class SHARED tcp_server_class.cpp)

#target_link_libraries (tcp_server_test tcp_server_class tcp_lib)

#add_executable(7_queue_mpmc_cond_vars ${PROJECT_SOURCES} )



# add targets 
#add_library(common_stuff SHARED ${DIR_COMMON}/defs.cpp)
#add_library(message_class SHARED ${DIR_COMMON}/message_class.cpp)
#add_library(message_queue_class SHARED ${DIR_COMMON}/message_queue_class.cpp)
#add_library(tcp_server SHARED ${DIR_PC_LIBS}/tcp_server.cpp)

#add_executable(1_concurrent_access ${PROJECT_SOURCES} )
#add_executable(2_concurrent_access_class_mutex ${PROJECT_SOURCES} )
#add_executable(3_concurrent_access_queue ${PROJECT_SOURCES} )
#add_executable(4_concurrent_access_spmc ${PROJECT_SOURCES} )
#add_executable(5_queue_spsc_cond_vars ${PROJECT_SOURCES} )
#add_executable(6_queue_spmc_cond_vars ${PROJECT_SOURCES} )


#target_link_libraries (message_class common_stuff)
#target_link_libraries (message_queue_class common_stuff message_class)
#target_link_libraries (tcp_server common_stuff message_class message_queue_class)

#target_link_libraries (1_concurrent_access ${ALL_PROJECT_LIBS})
#target_link_libraries (2_concurrent_access_class_mutex ${ALL_PROJECT_LIBS})
#target_link_libraries (3_concurrent_access_queue ${ALL_PROJECT_LIBS})
#target_link_libraries (4_concurrent_access_spmc ${ALL_PROJECT_LIBS})
#target_link_libraries (5_queue_spsc_cond_vars ${ALL_PROJECT_LIBS})
#target_link_libraries (6_queue_spmc_cond_vars ${ALL_PROJECT_LIBS})
#target_link_libraries (7_queue_mpmc_cond_vars ${ALL_PROJECT_LIBS})

