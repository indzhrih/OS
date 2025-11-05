#include "../Headers/ExceptionHandler.h"
#include <limits>

bool ExceptionHandler::checkHandle(HANDLE handleValue, const std::string& what) {
    if (handleValue && handleValue != INVALID_HANDLE_VALUE) return true;
    std::cerr << what << " failed, err=" << GetLastError() << "\n";
    return false;
}

bool ExceptionHandler::checkWait(DWORD waitCode, const std::string& what) {
    if (waitCode == WAIT_OBJECT_0 || waitCode == WAIT_TIMEOUT) return true;
    std::cerr << what << " failed, error: " << GetLastError() << "\n";
    return false;
}

bool ExceptionHandler::checkRelease(BOOL ok, const std::string& what) {
    if (ok) return true;
    std::cerr << what << " failed, error: " << GetLastError() << "\n";
    return false;
}

bool ExceptionHandler::checkFile(FILE* file, const std::string& what) {
    if (file) return true;
    std::cerr << what << " failed, error: " << GetLastError() << "\n";
    return false;
}

void ExceptionHandler::clearInput() {
    std::cin.clear();
    std::streambuf* rb = std::cin.rdbuf();
    if (rb && rb->in_avail() > 0) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
