#include "doctest/doctest.h"
#include <windows.h>
#include <string>
#include <iostream>
#include "../Headers/ExceptionHandler.h"
#include "TestFixtures.cpp"

TEST_CASE("Sender returns non-zero when launched without args") {
    DWORD code = TestFixtures::run_process_and_wait("cmake-build-debug/Sender.exe");
    CHECK(code != 0);
}

TEST_CASE("Sender-like open of missing sync object fails (handle check)") {
    HANDLE h = OpenSemaphoreA(SYNCHRONIZE, FALSE, "NoSuchSemaphore_XYZ");
    CHECK(ExceptionHandler::checkHandle(h, "OpenSemaphore") == false);
    if (h) CloseHandle(h);
}
