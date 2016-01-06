cmake_minimum_required(VERSION 3.3)
project(8005_a1_pt_benchmarker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp processes/pmain.cpp threads/tmain.cpp)
add_executable(8005_a1_pt_benchmarker ${SOURCE_FILES})