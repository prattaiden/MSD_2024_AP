cmake_minimum_required(VERSION 3.27)
project(msdscript2)

set(CMAKE_CXX_STANDARD 17)

add_executable(msdscript2 main.cpp expr.cpp cmdline.cpp
        tests.cpp
        parse.hpp
        parse.cpp
        parse2.cpp
        parse2.h)
