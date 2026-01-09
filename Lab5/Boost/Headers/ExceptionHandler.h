#ifndef LAB5_EXCEPTIONHANDLER_H
#define LAB5_EXCEPTIONHANDLER_H

#include <string>
#include <iostream>
#include <cstdio>

class ExceptionHandler {
public:
    static bool checkHandle(bool ok, const std::string& what);
    static bool checkWait(bool ok, const std::string& what);
    static bool checkRelease(bool ok, const std::string& what);
    static bool checkFile(FILE* file, const std::string& what);
    static void printError(const std::string& message);
    static void clearInput();
};

#endif
