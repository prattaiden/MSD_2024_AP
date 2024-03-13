cmake_minimum_required(VERSION 3.27)
project(MyMalloc)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyMalloc main.cpp
        HashTable.cpp
        HashTable.h
        MyAlloc.cpp
        MyAlloc.h
        testMalloc.cpp
        testMalloc.h)
