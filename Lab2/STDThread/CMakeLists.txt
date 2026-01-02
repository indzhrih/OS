cmake_minimum_required(VERSION 3.15)
project(Lab2_STDThread)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lab2_STDThread main.cpp
        Headers/Array.h
        Headers/ThreadHandler.h
        Source/Array.cpp
        Source/ThreadHandler.cpp)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

include(Catch)

add_executable(tests
        Tests/ArrayTests.cpp
        Tests/ThreadHandlerTests.cpp
        Tests/MainTests.cpp
        Tests/TestFixtures.cpp
        Source/Array.cpp
        Source/ThreadHandler.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
enable_testing()

catch_discover_tests(tests)

set(ALLURE_RESULTS_DIR ${CMAKE_BINARY_DIR}/allure-results)

add_custom_target(run_tests_xml
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ALLURE_RESULTS_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure --output-junit ${ALLURE_RESULTS_DIR}/junit.xml
        DEPENDS tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests via CTest and exporting JUnit to ${ALLURE_RESULTS_DIR}"
)

find_program(ALLURE_BIN NAMES allure allure.bat)
if (ALLURE_BIN)
    add_custom_target(allure_report
            COMMAND ${ALLURE_BIN} generate ${ALLURE_RESULTS_DIR} --clean -o ${CMAKE_BINARY_DIR}/allure-report
            DEPENDS run_tests_xml
            COMMENT "Generating Allure report"
    )
endif()
