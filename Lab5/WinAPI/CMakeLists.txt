cmake_minimum_required(VERSION 3.20)
project(Lab5_WinAPI)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_INCLUDEDIR "C:/boost_1_89_0")
include_directories("C:/boost_1_89_0")

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

target_include_directories(Server PRIVATE Headers "C:/boost_1_89_0")

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

target_include_directories(Client PRIVATE Headers "C:/boost_1_89_0")

include(CTest)
enable_testing()

add_executable(Lab5Tests
        Tests/TestMain.cpp
        Tests/ExceptionHandlerTests.cpp
        Tests/EmployeeTests.cpp
        Tests/EmployeeStorageTests.cpp
        Tests/RecordLockManagerTests.cpp
        Tests/PipeRequestTests.cpp
        Tests/PipeResponseTests.cpp
        Tests/PipeClientTests.cpp
        Tests/PipeServerTests.cpp
        Tests/ProcessHandlerTests.cpp
        Tests/ServerHandlerTests.cpp
        Tests/ClientHandlerTests.cpp
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
        Source/PipeClient.cpp
        Source/ClientHandler.cpp
)

target_include_directories(Lab5Tests PRIVATE Headers "C:/boost_1_89_0")
add_test(NAME Lab5UnitTests COMMAND Lab5Tests)

set(REPORTS_DIR ${CMAKE_SOURCE_DIR}/Reports)
set(ALLURE_RESULTS_DIR ${REPORTS_DIR}/allure-results)

add_custom_target(run_tests_xml
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ALLURE_RESULTS_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ALLURE_RESULTS_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure --output-junit ${ALLURE_RESULTS_DIR}/junit.xml
        DEPENDS Lab5Tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running Boost tests and generating ${ALLURE_RESULTS_DIR}/junit.xml"
)

set(ALLURE_BIN "" CACHE FILEPATH "Path to allure or allure.bat")
if (NOT ALLURE_BIN)
    find_program(ALLURE_BIN NAMES allure allure.bat)
endif()

if (ALLURE_BIN)
    add_custom_target(allure_report
            COMMAND "${ALLURE_BIN}" generate ${ALLURE_RESULTS_DIR} --clean -o ${REPORTS_DIR}/allure-report
            DEPENDS run_tests_xml
            COMMENT "Generating Allure HTML report in ${REPORTS_DIR}/allure-report"
    )
    add_custom_target(reports
            DEPENDS run_tests_xml allure_report
            COMMENT "Running tests and generating reports (XML + HTML)"
    )
else()
    message(WARNING "Allure not found. Install Allure or set -DALLURE_BIN=... to generate HTML reports.")
    add_custom_target(reports
            DEPENDS run_tests_xml
            COMMENT "Running tests (XML only, Allure not found)"
    )
endif()
