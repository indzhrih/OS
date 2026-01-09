#include "../Headers/ProcessHandler.h"
#include <stdexcept>

ProcessHandler::ProcessHandler(const std::string& commandLine)
    : command(commandLine), processInfo(), isStarted(false) {
    STARTUPINFOA startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    std::string cmdLineCopy = command;
    char* mutableCmdLine = cmdLineCopy.empty() ? nullptr : cmdLineCopy.data();

    BOOL created = CreateProcessA(nullptr, mutableCmdLine, nullptr,
        nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInfo);

    if (!created) {
        DWORD errorCode = GetLastError();
        throw std::runtime_error("CreateProcess failed, error: " + std::to_string(errorCode));
    }

    isStarted = true;
}

ProcessHandler::~ProcessHandler() {
    if (processInfo.hProcess) CloseHandle(processInfo.hProcess);
    if (processInfo.hThread) CloseHandle(processInfo.hThread);
    isStarted = false;
}

void ProcessHandler::waitForProcess() {
    if (!isStarted) return;
    if (processInfo.hProcess) WaitForSingleObject(processInfo.hProcess, INFINITE);
    isStarted = false;
}
