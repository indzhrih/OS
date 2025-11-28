#ifndef LAB5_EXCEPTIONHANDLER_H
#define LAB5_EXCEPTIONHANDLER_H

#include <windows.h>
#include <string>
#include <iostream>

class ExceptionHandler {
public:
    static bool checkHandle(HANDLE handle, const std::string& what);
    static bool checkWait(DWORD code, const std::string& what);
    static bool checkRelease(BOOL ok, const std::string& what);
    static bool checkFile(FILE* file, const std::string& what);
    static void printError(const std::string& message);
    static void clearInput();
};

#endif
