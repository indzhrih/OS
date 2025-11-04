#include "doctest/doctest.h"
#include "../Headers/ExceptionHandler.h"
#include <sstream>
#include <cstdio>

struct SilenceCerr {
    std::streambuf* old{};
    std::ostringstream sink;
    SilenceCerr()  { old = std::cerr.rdbuf(sink.rdbuf()); }
    ~SilenceCerr() { std::cerr.rdbuf(old); }
};

TEST_CASE("checkHandle returns true for non-null/non-invalid") {
    SetLastError(5);
    CHECK(ExceptionHandler::checkHandle((HANDLE)1, "dummy") == true);
}

TEST_CASE("checkHandle returns false for INVALID_HANDLE_VALUE") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkHandle(INVALID_HANDLE_VALUE, "dummy") == false);
}

TEST_CASE("checkWait returns true for WAIT_OBJECT_0") {
    SetLastError(5);
    CHECK(ExceptionHandler::checkWait(WAIT_OBJECT_0, "wait") == true);
}

TEST_CASE("checkWait returns true for WAIT_TIMEOUT") {
    CHECK(ExceptionHandler::checkWait(WAIT_TIMEOUT, "wait") == true);
}

TEST_CASE("checkWait returns false for other code") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkWait(1234, "wait") == false);
}

TEST_CASE("checkRelease true when BOOL TRUE") {
    SetLastError(5);
    CHECK(ExceptionHandler::checkRelease(TRUE, "rel") == true);
}

TEST_CASE("checkRelease false when BOOL FALSE") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkRelease(FALSE, "rel") == false);
}

TEST_CASE("checkFile true when FILE* valid") {
    SetLastError(5);
    FILE* f = std::fopen("eh_tmp.bin", "wb");
    CHECK(ExceptionHandler::checkFile(f, "file") == true);
    if (f) std::fclose(f);
    std::remove("eh_tmp.bin");
}

TEST_CASE("checkFile false when FILE* null") {
    SilenceCerr guard;
    SetLastError(5);
    CHECK(ExceptionHandler::checkFile(NULL, "file") == false);
}

TEST_CASE("clearInput clears failbit without blocking") {
    std::istringstream fake("garbage\n");
    auto* old = std::cin.rdbuf(fake.rdbuf());
    std::cin.setstate(std::ios::failbit);
    ExceptionHandler::clearInput();
    CHECK(std::cin.fail() == false);
    std::cin.rdbuf(old);
}