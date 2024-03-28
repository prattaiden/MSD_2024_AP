cmake_minimum_required(VERSION 3.27)
project(msdscript2)

set(CMAKE_CXX_STANDARD 17)

add_executable(msdscript2 main.cpp expr.cpp cmdline.cpp
        tests.cpp
        parse.hpp
        parse.cpp
        exec.cpp
        test_msdscript.cpp
        test_msdscript.h
        Val.cpp
        Val.h
        pointer.h
        Env.cpp
        Env.h
)
