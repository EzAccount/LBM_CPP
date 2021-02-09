cmake_minimum_required(VERSION 3.10...3.19)
project(LBM_CPP)

set(CMAKE_CXX_STANDARD 11)

add_executable(LBM_CPP main.cpp point.h constants.h)
add_subdirectory(tests)