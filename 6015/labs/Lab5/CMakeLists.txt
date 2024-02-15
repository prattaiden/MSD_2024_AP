cmake_minimum_required(VERSION 3.27)
project(Lab5)

set(CMAKE_CXX_STANDARD 14)

include_directories(msdscriptl5)

add_executable(Lab5
        msdscriptl5/catch.h
        msdscriptl5/cmdline.cpp
        msdscriptl5/cmdline.hpp
        msdscriptl5/expr.cpp
        msdscriptl5/expr.h
        msdscriptl5/main.cpp
        msdscriptl5/Makefile
        msdscriptl5/parse.cpp
        msdscriptl5/parse.hpp
        msdscriptl5/parse2.cpp
        msdscriptl5/parse2.h
        msdscriptl5/tests.cpp)
