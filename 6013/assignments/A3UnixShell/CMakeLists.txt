cmake_minimum_required(VERSION 3.27)
project(A3UnixShell)

set(CMAKE_CXX_STANDARD 17)

add_executable(A3UnixShell main.cpp
        shelpers.hpp
        shelpers.cpp
        shelpers.cpp)
