cmake_minimum_required(VERSION 3.15)
project(Lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Main main.cpp)
add_executable(Creator creator.cpp)
add_executable(Reporter reporter.cpp)

add_custom_target(Lab1 ALL DEPENDS Main Creator Reporter)
