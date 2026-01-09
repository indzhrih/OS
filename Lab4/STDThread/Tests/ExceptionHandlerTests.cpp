#include "doctest/doctest.h"
#include "../Headers/ExceptionHandler.h"
#include <windows.h>
#include <sstream>
#include <cstdio>
#include <iostream>

struct SilenceCerr {
    std::streambuf* old;
    std::ostringstream sink;
    SilenceCerr() : old(std::cerr.rdbuf(sink.rdbuf())) {}
    ~SilenceCerr() { std::cerr.rdbuf(old); }
};

TEST_CASE("checkHandle returns true for non-null") {
    SetLastError(5);
    CHECK(ExceptionHandler::checkHandle((HANDLE)1, "h") == true);
}

TEST_CASE("checkHandle returns false for invalid") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkHandle(INVALID_HANDLE_VALUE, "h") == false);
}

TEST_CASE("checkWait returns true for object") {
    SetLastError(5);
    CHECK(ExceptionHandler::checkWait(WAIT_OBJECT_0, "w") == true);
}

TEST_CASE("checkWait returns true for timeout") {
    CHECK(ExceptionHandler::checkWait(WAIT_TIMEOUT, "w") == true);
}

TEST_CASE("checkWait returns false for other") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkWait(1234, "w") == false);
}

TEST_CASE("checkRelease returns true") {
    SetLastError(5);
    CHECK(ExceptionHandler::checkRelease(TRUE, "r") == true);
}

TEST_CASE("checkRelease returns false") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkRelease(FALSE, "r") == false);
}

TEST_CASE("checkFile returns true") {
    SetLastError(5);
    FILE* f = std::fopen("eh_tmp.bin", "wb");
    CHECK(ExceptionHandler::checkFile(f, "f") == true);
    if (f) std::fclose(f);
    std::remove("eh_tmp.bin");
}

TEST_CASE("checkFile returns false") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkFile(NULL, "f") == false);
}

TEST_CASE("clearInput clears failbit") {
    std::istringstream fake("x\n");
    auto* old = std::cin.rdbuf(fake.rdbuf());
    std::cin.setstate(std::ios::failbit);
    ExceptionHandler::clearInput();
    CHECK(std::cin.fail() == false);
    std::cin.rdbuf(old);
}
