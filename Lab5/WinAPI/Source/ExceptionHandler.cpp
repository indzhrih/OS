#include <limits>
#include "../Headers/ExceptionHandler.h"

bool ExceptionHandler::checkHandle(HANDLE handle, const std::string& what) {
    if (handle != NULL && handle != INVALID_HANDLE_VALUE) return true;
    printError(what + " failed, error: " + std::to_string(GetLastError()));
    return false;
}

bool ExceptionHandler::checkWait(DWORD code, const std::string& what) {
    if (code == WAIT_OBJECT_0 || code == WAIT_TIMEOUT) return true;
    printError(what + " failed, error: " + std::to_string(GetLastError()));
    return false;
}

bool ExceptionHandler::checkRelease(BOOL ok, const std::string& what) {
    if (ok) return true;
    printError(what + " failed, error: " + std::to_string(GetLastError()));
    return false;
}

bool ExceptionHandler::checkFile(FILE* file, const std::string& what) {
    if (file) return true;
    printError(what + " failed");
    return false;
}

void ExceptionHandler::printError(const std::string& message) {
    std::cerr << message << std::endl;
}

void ExceptionHandler::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
