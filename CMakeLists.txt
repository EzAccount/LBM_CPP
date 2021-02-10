cmake_minimum_required(VERSION 3.10...3.19)
project(LBM_CPP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -pg -fno-omit-frame-pointer -DDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG") #-pg
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wshadow -Wstrict-aliasing -Weffc++")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-inline")

#ASan
set(ENV{ASAN_OPTIONS} "strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:symbolize=1")
set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer -fsanitize=address,undefined")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${ASAN_FLAGS}")

add_executable(LBM_CPP main.cpp point.h constants.h)
add_subdirectory(tests)
