cmake_minimum_required(VERSION 3.20)
project(Kollokvium_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Kollokvium_1
        main.cpp
        Sources/Menu.cpp
        Sources/Factorial.cpp
        Sources/DuplicateRemover.cpp
        Sources/LinkedList.cpp
        Sources/Node.cpp
)

target_include_directories(Kollokvium_1 PRIVATE Headers)

include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
        tests/TestsMain.cpp
        tests/MenuTests.cpp
        tests/FactorialTests.cpp
        tests/DuplicateRemoverTests.cpp
        tests/LinkedListTests.cpp
        tests/NodeTests.cpp
        Sources/Menu.cpp
        Sources/Factorial.cpp
        Sources/DuplicateRemover.cpp
        Sources/LinkedList.cpp
        Sources/Node.cpp
)

target_include_directories(tests PRIVATE Headers)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(CTest)
enable_testing()
add_test(NAME unit_tests COMMAND tests)

set(REPORTS_DIR ${CMAKE_SOURCE_DIR}/Reports)
set(ALLURE_RESULTS_DIR ${REPORTS_DIR}/allure-results)

set(RUN_TESTS_CMD $<TARGET_FILE:tests> --gtest_output=xml:${ALLURE_RESULTS_DIR}/junit.xml)

add_custom_target(run_tests_xml
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ALLURE_RESULTS_DIR}
        COMMAND ${CMAKE_COMMAND} -E remove -f ${ALLURE_RESULTS_DIR}/*.xml
        COMMAND ${RUN_TESTS_CMD}
        BYPRODUCTS ${ALLURE_RESULTS_DIR}/junit.xml
        DEPENDS tests
        COMMENT "Running tests and generating XML report in ${ALLURE_RESULTS_DIR}/junit.xml"
)

find_program(ALLURE_BIN NAMES allure allure.bat)
if (ALLURE_BIN)
    add_custom_target(allure_report
            COMMAND ${ALLURE_BIN} generate ${ALLURE_RESULTS_DIR} --clean
            -o ${REPORTS_DIR}/allure-report
            DEPENDS run_tests_xml
            COMMENT "Generating Allure report in ${REPORTS_DIR}/allure-report"
    )
else()
    message(WARNING "Allure not found. Install Allure to generate HTML reports.")
endif()

add_custom_target(reports
        DEPENDS run_tests_xml allure_report
        COMMENT "Running tests and generating reports"
)
