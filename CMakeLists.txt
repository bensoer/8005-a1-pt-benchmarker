cmake_minimum_required(VERSION 3.3)
project(8005_a1_pt_benchmarker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lgmp")

set(SOURCE_FILES processes/pmain.cpp threads/tmain.cpp common/TaskManager.cpp common/TaskManager.h threads/WorkerThread.cpp threads/WorkerThread.h common/argparcer.h common/Task.cpp common/Task.h)
add_executable(8005_a1_pt_benchmarker ${SOURCE_FILES})