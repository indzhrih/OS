#include "doctest/doctest.h"
#include <windows.h>
#include <string>
#include <iostream>
#include "../Headers/ExceptionHandler.h"

static DWORD run_process_and_wait(const std::string& cmdline) {
    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);
    std::string cmd = cmdline;

    BOOL ok = CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    if (!ok) return 0xFFFFFFFF;

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return code;
}

TEST_CASE("Sender returns non-zero when launched without args") {
    DWORD code = run_process_and_wait("cmake-build-debug/Sender.exe");
    CHECK(code != 0);
}

TEST_CASE("Sender-like open of missing sync object fails (handle check)") {
    HANDLE h = OpenSemaphoreA(SYNCHRONIZE, FALSE, "NoSuchSemaphore_XYZ");
    CHECK(ExceptionHandler::checkHandle(h, "OpenSemaphore") == false);
    if (h) CloseHandle(h);
}
