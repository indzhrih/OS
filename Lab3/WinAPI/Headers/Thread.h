#ifndef LAB3_WINAPI_THREAD_H
#define LAB3_WINAPI_THREAD_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include "Array.h"

DWORD WINAPI threadWorker(LPVOID threadParam);

class Thread {
    friend DWORD WINAPI threadWorker(LPVOID threadParam);

public:
    Thread(HANDLE startEvent,
           HANDLE endEvent,
           HANDLE stopEvent,
           CRITICAL_SECTION* criticalSection,
           Array* array,
           int threadNumber,
           int arraySize);
    ~Thread();

    void wait();

private:
    HANDLE startEvent;
    HANDLE endEvent;
    HANDLE stopEvent;
    CRITICAL_SECTION* criticalSection;

    Array* array;
    int arraySize;
    int threadNumber;

    HANDLE handle;
    DWORD threadId;
};

#endif
