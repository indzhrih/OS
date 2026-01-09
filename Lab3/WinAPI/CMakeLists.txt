cmake_minimum_required(VERSION 3.14)
project(Lab3_WinAPI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lab3_WinAPI
        main.cpp
        Headers/Array.h Headers/Thread.h Headers/ThreadHandler.h
        Source/Array.cpp Source/Thread.cpp Source/ThreadHandler.cpp)

target_include_directories(Lab3_WinAPI PRIVATE ${CMAKE_SOURCE_DIR}/Headers)

include(FetchContent)

FetchContent_Declare(bandit
        GIT_REPOSITORY https://github.com/banditcpp/bandit.git
        GIT_TAG f297efd8aecc6e9ce8f84e33264898bf77f9cb73
)

FetchContent_Declare(snowhouse
        GIT_REPOSITORY https://github.com/banditcpp/snowhouse.git
        GIT_TAG 8db10c8
)

FetchContent_Populate(bandit)
FetchContent_Populate(snowhouse)

add_library(bandit_headers INTERFACE)
target_include_directories(bandit_headers INTERFACE
        ${bandit_SOURCE_DIR}
        ${snowhouse_SOURCE_DIR}/include
)

add_executable(tests
        Tests/MainTests.cpp
        Tests/ArrayTests.cpp
        Tests/ThreadTests.cpp
        Tests/ThreadHandlerTests.cpp
        Source/Array.cpp
        Source/Thread.cpp
        Source/ThreadHandler.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/Headers)
target_link_libraries(tests PRIVATE bandit_headers)

include(CTest)
enable_testing()
add_test(NAME unit COMMAND tests)

set(ALLURE_RESULTS_DIR ${CMAKE_BINARY_DIR}/allure-results)

set(RUN_BANDIT_CMD cmd /c "\"$<TARGET_FILE:tests>\" --reporter=xunit > \"${ALLURE_RESULTS_DIR}\\junit.xml\"")

add_custom_target(run_tests_xml
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ALLURE_RESULTS_DIR}
        COMMAND ${CMAKE_COMMAND} -E remove -f ${ALLURE_RESULTS_DIR}/*.xml
        COMMAND ${RUN_BANDIT_CMD}
        BYPRODUCTS ${ALLURE_RESULTS_DIR}/junit.xml
        DEPENDS tests
        COMMENT "Run bandit and export JUnit to ${ALLURE_RESULTS_DIR}/junit.xml"
)

find_program(ALLURE_BIN NAMES allure allure.bat)
if (ALLURE_BIN)
    add_custom_target(allure_report
            COMMAND ${ALLURE_BIN} generate ${ALLURE_RESULTS_DIR} --clean
            -o ${CMAKE_BINARY_DIR}/allure-report
            DEPENDS run_tests_xml
            COMMENT "Generating Allure report"
    )
endif()

