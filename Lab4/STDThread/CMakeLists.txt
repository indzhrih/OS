cmake_minimum_required(VERSION 3.20)
project(Lab4_STDThread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

add_executable(Receiver
        Receiver.cpp
        Headers/Message.h
        Headers/MessageRingQueue.h
        Headers/ExceptionHandler.h
        Headers/SharedState.h
        Source/Message.cpp
        Source/MessageRingQueue.cpp
        Source/ExceptionHandler.cpp
        Source/SharedState.cpp
)

add_executable(Sender
        Sender.cpp
        Headers/Message.h
        Headers/ExceptionHandler.h
        Headers/SharedState.h
        Source/Message.cpp
        Source/ExceptionHandler.cpp
        Source/SharedState.cpp
)

target_include_directories(Receiver PRIVATE Headers)
target_include_directories(Sender PRIVATE Headers)

include(FetchContent)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

file(GLOB TEST_SOURCES
        Tests/TestsMain.cpp
        Tests/MessageTests.cpp
        Tests/MessageRingQueueTests.cpp
        Tests/ExceptionHandlerTests.cpp
        Tests/ReceiverTests.cpp
        Tests/SharedStateTests.cpp
)

add_executable(lab4_tests
        ${TEST_SOURCES}
        Source/Message.cpp
        Source/MessageRingQueue.cpp
        Source/ExceptionHandler.cpp
        Source/SharedState.cpp
)
target_link_libraries(lab4_tests PRIVATE doctest::doctest)
target_include_directories(lab4_tests PRIVATE Headers)
add_dependencies(lab4_tests Sender Receiver)

include(CTest)
enable_testing()
add_test(NAME unit_tests COMMAND lab4_tests)

set(REPORTS_DIR ${CMAKE_SOURCE_DIR}/Reports)
set(ALLURE_RESULTS_DIR ${REPORTS_DIR}/allure-results)

add_custom_target(run_tests_xml
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ALLURE_RESULTS_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ALLURE_RESULTS_DIR}
        COMMAND $<TARGET_FILE:lab4_tests> --reporters=junit --out=${ALLURE_RESULTS_DIR}/junit.xml
        DEPENDS lab4_tests
        COMMENT "Running doctest (JUnit) and generating ${ALLURE_RESULTS_DIR}/junit.xml"
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
