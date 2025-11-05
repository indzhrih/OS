#ifndef LAB4_WINAPI_EXCEPTIONHANDLER_H
#define LAB4_WINAPI_EXCEPTIONHANDLER_H

#include <windows.h>
#include <string>
#include <iostream>

class ExceptionHandler {
public:
    static bool checkHandle(HANDLE handleValue, const std::string& what);
    static bool checkWait(DWORD waitCode, const std::string& what);
    static bool checkRelease(BOOL ok, const std::string& what);
    static bool checkFile(FILE* file, const std::string& what);
    static void clearInput();
};

#endif //LAB4_WINAPI_EXCEPTIONHANDLER_H
