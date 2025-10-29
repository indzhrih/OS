#ifndef LAB3_WINAPI_THREAD_H
#define LAB3_WINAPI_THREAD_H

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

#endif // LAB3_WINAPI_THREAD_H
