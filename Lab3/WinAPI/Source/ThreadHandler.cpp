#include <iostream>
#include "../Headers/ThreadHandler.h"
#include "../Headers/Thread.h"

DWORD WINAPI threadWorker(LPVOID threadParam) {
    Thread* data = static_cast<Thread*>(threadParam);

    WaitForSingleObject(data->startEvent, INFINITE);

    srand(data->threadNumber);
    int markedCount = 0;
    bool continueWork = true;

    while (continueWork) {
        int index = rand() % data->arraySize;

        EnterCriticalSection(data->criticalSection);
        if ((*data->array)[index] == 0) {
            Sleep(5);
            (*data->array)[index] = data->threadNumber;
            Sleep(5);
            markedCount++;
            LeaveCriticalSection(data->criticalSection);
        }
        else {
            LeaveCriticalSection(data->criticalSection);

            EnterCriticalSection(data->criticalSection);
            std::cout << "Thread " << data->threadNumber
                      << " cannot mark element at index " << index
                      << ". Marked " << markedCount << " elements."
                      << std::endl;
            LeaveCriticalSection(data->criticalSection);

            SetEvent(data->stopEvent);

            HANDLE waitHandles[2] = { data->endEvent, data->startEvent };
            DWORD result = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

            if (result == WAIT_OBJECT_0) {
                EnterCriticalSection(data->criticalSection);
                for (int i = 0; i < data->arraySize; ++i) {
                    if ((*data->array)[i] == data->threadNumber) {
                        (*data->array)[i] = 0;
                    }
                }
                LeaveCriticalSection(data->criticalSection);

                EnterCriticalSection(data->criticalSection);
                std::cout << "Thread " << data->threadNumber << " finished work." << std::endl;
                LeaveCriticalSection(data->criticalSection);

                continueWork = false;
            }
            else if (result == WAIT_OBJECT_0 + 1) ResetEvent(data->stopEvent);
        }
    }
    return 0;
}