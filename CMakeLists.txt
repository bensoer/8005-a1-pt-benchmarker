cmake_minimum_required(VERSION 3.3)
project(8005_a1_pt_benchmarker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lgmp -g")

set(SOURCE_FILES threads/tmain.cpp common/TaskManager.cpp common/TaskManager.h threads/WorkerThread.cpp threads/WorkerThread.h common/argparcer.h common/Task.cpp common/Task.h common/logger.cpp common/logger.h)
add_executable(8005_a1_pt_benchmarker ${SOURCE_FILES})

set(PSOURCE_FILES processes/pmain.cpp common/argparcer.h common/logger.cpp common/logger.h common/Task.cpp common/Task.h common/TaskManager.cpp common/TaskManager.h processes/WorkerProcess.cpp processes/WorkerProcess.h)
add_executable(p8005_a1_pt_benchmarker ${PSOURCE_FILES})