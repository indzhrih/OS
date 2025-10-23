cmake_minimum_required(VERSION 4.0)
project(Kollokvium_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Kollokvium_1 main.cpp
               Headers/Menu.h
               Headers/Factorial.h
               Headers/DuplicateRemover.h
               Headers/Node.h
               Headers/LinkedList.h
               Sources/Menu.cpp
               Sources/Factorial.cpp
               Sources/DuplicateRemover.cpp
               Sources/Node.cpp
               Sources/LinkedList.cpp)
