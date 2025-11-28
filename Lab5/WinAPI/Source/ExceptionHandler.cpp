#include <limits>
#include "../Headers/ExceptionHandler.h"

bool ExceptionHandler::checkHandle(HANDLE handle, const std::string& what) {
    if (handle != NULL && handle != INVALID_HANDLE_VALUE) {
        return true;
    }
    std::cerr << what << " failed, error: " << GetLastError() << std::endl;
    return false;
}

bool ExceptionHandler::checkWait(DWORD code, const std::string& what) {
    if (code == WAIT_OBJECT_0) {
        return true;
    }
    if (code == WAIT_TIMEOUT) {
        return true;
    }
    std::cerr << what << " failed, error: " << GetLastError() << std::endl;
    return false;
}

bool ExceptionHandler::checkRelease(BOOL ok, const std::string& what) {
    if (ok) return true;
    std::cerr << what << " failed, error: " << GetLastError() << std::endl;
    return false;
}

bool ExceptionHandler::checkFile(FILE* file, const std::string& what) {
    if (file) {
        return true;
    }
    std::cerr << what << " failed" << std::endl;
    return false;
}

void ExceptionHandler::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}