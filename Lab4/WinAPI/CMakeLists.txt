cmake_minimum_required(VERSION 4.0)
project(Lab4_WinAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(Receiver Receiver.cpp)

add_executable(Sender Sender.cpp)
