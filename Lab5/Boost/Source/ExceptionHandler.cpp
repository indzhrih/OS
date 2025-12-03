#include "../Headers/ExceptionHandler.h"
#include <limits>

bool ExceptionHandler::checkHandle(bool ok, const std::string& what) {
    if (ok) return true;
    printError(what + " failed");
    return false;
}

bool ExceptionHandler::checkWait(bool ok, const std::string& what) {
    if (ok) return true;
    printError(what + " failed");
    return false;
}

bool ExceptionHandler::checkRelease(bool ok, const std::string& what) {
    if (ok) return true;
    printError(what + " failed");
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
