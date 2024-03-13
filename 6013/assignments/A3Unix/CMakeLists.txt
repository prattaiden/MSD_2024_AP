cmake_minimum_required(VERSION 3.27)
project(A3Unix)

set(CMAKE_CXX_STANDARD 17)

add_executable(A3Unix main.cpp
        shelpers.hpp
        shelpers.cpp)
