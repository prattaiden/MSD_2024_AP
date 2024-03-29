cmake_minimum_required(VERSION 3.27)
project(A5Queue)

set(CMAKE_CXX_STANDARD 17)

add_executable(A5Queue
        SerialQueue.hpp
        SerialQueue.cpp
        ConcurrentQueue.hpp
        ConcurrentQueueTests.cpp
        ConcurrentQueueTests.h
)
