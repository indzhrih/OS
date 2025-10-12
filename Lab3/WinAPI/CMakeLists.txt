cmake_minimum_required(VERSION 4.0)
project(Lab3_WinAPI)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab3_WinAPI main.cpp
        Headers/Array.h
        Headers/Thread.h
        Headers/ThreadHandler.h
        Source/Array.cpp
        Source/Thread.cpp
        Source/ThreadHandler.cpp)
