#include "../Headers/ProcessHandler.h"
#include "../Headers/ExceptionHandler.h"
#include <stdexcept>

ProcessHandler::ProcessHandler(const std::string& commandLine) : isCreated(false) {
    ZeroMemory(&processInformation, sizeof(processInformation));

    STARTUPINFOA startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    std::string commandCopy = commandLine;

    BOOL created = CreateProcessA(NULL, commandCopy.data(), NULL, NULL, FALSE,
                                  CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInformation);

    if (!created) {
        DWORD errorCode = GetLastError();
        throw std::runtime_error("CreateProcess failed, error: " + std::to_string(errorCode));
    }

    isCreated = true;
}

ProcessHandler::~ProcessHandler() {
    if (isCreated) {
        if (processInformation.hProcess != NULL) CloseHandle(processInformation.hProcess);
        if (processInformation.hThread != NULL) CloseHandle(processInformation.hThread);
        isCreated = false;
    }
}

void ProcessHandler::waitForProcess() {
    if (!isCreated || processInformation.hProcess == NULL) return;

    DWORD waitResult = WaitForSingleObject(processInformation.hProcess, INFINITE);
    ExceptionHandler::checkWait(waitResult, "WaitForSingleObject(process)");
}
