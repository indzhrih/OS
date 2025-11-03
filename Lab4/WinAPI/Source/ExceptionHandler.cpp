#include "../Headers/ExceptionHandler.h"

bool ExceptionHandler::checkHandle(HANDLE h, const std::string& what) {
    if (h != NULL && h != INVALID_HANDLE_VALUE) return true;
    std::cerr << what << " failed, err=" << GetLastError() << std::endl;
    return false;
}

bool ExceptionHandler::checkWait(DWORD code, const std::string& what) {
    if (code == WAIT_OBJECT_0) return true;
    if (code == WAIT_TIMEOUT) return true;
    std::cerr << what << " failed, error: " << GetLastError() << std::endl;
    return false;
}

bool ExceptionHandler::checkRelease(BOOL ok, const std::string& what) {
    if (ok) return true;
    std::cerr << what << " failed, error: " << GetLastError() << std::endl;
    return false;
}

bool ExceptionHandler::checkFile(FILE* f, const std::string& what) {
    if (f) return true;
    std::cerr << what << " failed, error: " << std::endl;
    return false;
}

void ExceptionHandler::clearInput() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}
