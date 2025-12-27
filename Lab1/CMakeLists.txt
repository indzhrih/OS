cmake_minimum_required(VERSION 3.15)
project(Lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Main main.cpp)
add_executable(Creator creator.cpp)
add_executable(Reporter reporter.cpp)

add_custom_target(Lab1 ALL DEPENDS Main Creator Reporter)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(main_test tests/main_test.cpp)
target_link_libraries(main_test PRIVATE GTest::gtest_main)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/tests/input_data"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/tests")

include(GoogleTest)
gtest_discover_tests(main_test)

