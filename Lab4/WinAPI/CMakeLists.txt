cmake_minimum_required(VERSION 4.0)
project(Lab4_WinAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(Receiver Receiver.cpp
                        Headers/Message.h
                        Headers/MessageRingQueue.h
                        Headers/ExceptionHandler.h
                        Source/Message.cpp
                        Source/MessageRingQueue.cpp
                        Source/ExceptionHandler.cpp)

add_executable(Sender Sender.cpp
                      Headers/Message.h
                      Headers/MessageRingQueue.h
                      Headers/ExceptionHandler.h
                      Source/Message.cpp
                      Source/MessageRingQueue.cpp
                      Source/ExceptionHandler.cpp)
