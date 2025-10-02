#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <string>
#include <windows.h>
#include "../Headers/Array.h"

class ThreadHandler {
public:
    HANDLE createThread(const std::string& threadName, Array& array);
    void clearThreadResources(HANDLE thread);

private:
    static DWORD WINAPI minMax(LPVOID threadParam);
    static DWORD WINAPI average(LPVOID threadParam);
};

#endif //LAB2_THREADHANDLER_H
