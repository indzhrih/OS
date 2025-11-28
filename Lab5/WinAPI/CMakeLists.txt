cmake_minimum_required(VERSION 3.20)
project(Lab5_WinAPI)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Server
        Server.cpp
        Headers/Employee.h
        Headers/PipeRequestType.h
        Headers/PipeResponseStatus.h
        Headers/PipeRequest.h
        Headers/PipeResponse.h
        Headers/EmployeeStorage.h
        Headers/PipeServer.h
        Headers/ProcessHandler.h
        Headers/ExceptionHandler.h
        Headers/ServerHandler.h
        Headers/RecordLockManager.h
        Source/Employee.cpp
        Source/PipeRequestType.cpp
        Source/PipeResponseStatus.cpp
        Source/PipeRequest.cpp
        Source/PipeResponse.cpp
        Source/EmployeeStorage.cpp
        Source/PipeServer.cpp
        Source/ProcessHandler.cpp
        Source/ExceptionHandler.cpp
        Source/ServerHandler.cpp
        Source/RecordLockManager.cpp
)

add_executable(Client
        Client.cpp
        Headers/Employee.h
        Headers/PipeRequestType.h
        Headers/PipeResponseStatus.h
        Headers/PipeRequest.h
        Headers/PipeResponse.h
        Headers/PipeClient.h
        Headers/ExceptionHandler.h
        Headers/ClientHandler.h
        Source/Employee.cpp
        Source/PipeRequestType.cpp
        Source/PipeResponseStatus.cpp
        Source/PipeRequest.cpp
        Source/PipeResponse.cpp
        Source/PipeClient.cpp
        Source/ExceptionHandler.cpp
        Source/ClientHandler.cpp
)

target_include_directories(Server PRIVATE Headers)
target_include_directories(Client PRIVATE Headers)
