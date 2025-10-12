#include <iostream>
#include <windows.h>
#include "Headers/Array.h"
#include "Headers/Thread.h"

int main() {
    int n, markerThreadsNumber;

    std::cout << "Enter array size " << std::endl;
    std::cin  >> n;
    Array array(n);

    std::cout << "Enter marker threads number" << std::endl;
    std::cin  >> markerThreadsNumber;

    HANDLE startThreadsEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    HANDLE* stopEvents = new HANDLE[markerThreadsNumber];
    for (int i = 0; i < markerThreadsNumber; i++) stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

    HANDLE* endEvents = new HANDLE[markerThreadsNumber];
    for (int i = 0; i < markerThreadsNumber; i++) endEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

    CRITICAL_SECTION criticalSection;
    InitializeCriticalSection(&criticalSection);

    Thread** threads = new Thread*[markerThreadsNumber];
    for (int i = 0; i < markerThreadsNumber; i++) {
        threads[i] = new Thread(startThreadsEvent, endEvents[i], stopEvents[i], &criticalSection,
            &array, i + 1, n);
    }

    bool* isAlive = new bool[markerThreadsNumber];
    for (int i = 0; i < markerThreadsNumber; ++i) isAlive[i] = true;

    SetEvent(startThreadsEvent);

    int activeThreads = markerThreadsNumber;
    while (activeThreads > 0) {
        EnterCriticalSection(&criticalSection);
        std::cout << "Waiting for threads signals." << std::endl;
        LeaveCriticalSection(&criticalSection);

        ResetEvent(startThreadsEvent);

        int waitCount = 0;
        HANDLE* waitList = new HANDLE[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; ++i) {
            if (isAlive[i]) waitList[waitCount++] = stopEvents[i];
        }

        if (waitCount == 0) {
            delete[] waitList;
            break;
        }
        WaitForMultipleObjects(waitCount, waitList, TRUE, INFINITE);
        delete[] waitList;

        EnterCriticalSection(&criticalSection);
        array.printArray();
        LeaveCriticalSection(&criticalSection);

        int threadToStop;
        while (true) {
            EnterCriticalSection(&criticalSection);
            std::cout << "Enter thread number to stop (1-" << markerThreadsNumber << "): ";
            LeaveCriticalSection(&criticalSection);

            std::cin >> threadToStop;

            if (threadToStop >= 1 && threadToStop <= markerThreadsNumber && isAlive[threadToStop - 1]) break;

            EnterCriticalSection(&criticalSection);
            std::cout << "This thread is already finished or invalid. Choose an active one." << std::endl;
            LeaveCriticalSection(&criticalSection);
        }

        SetEvent(endEvents[threadToStop - 1]);

        EnterCriticalSection(&criticalSection);
        std::cout << "Waiting for thread " << threadToStop << " to finish..." << std::endl;
        LeaveCriticalSection(&criticalSection);

        threads[threadToStop - 1]->wait();
        delete threads[threadToStop - 1];
        threads[threadToStop - 1] = nullptr;

        isAlive[threadToStop - 1] = false;
        activeThreads--;

        EnterCriticalSection(&criticalSection);
        std::cout << "Array after thread " << threadToStop << " finished: ";
        array.printArray();
        LeaveCriticalSection(&criticalSection);

        if (activeThreads > 0) {
            for (int i = 0; i < markerThreadsNumber; i++) {
                if (isAlive[i]) {
                    ResetEvent(stopEvents[i]);
                    ResetEvent(endEvents[i]);
                }
            }
            SetEvent(startThreadsEvent);
        }
    }

    EnterCriticalSection(&criticalSection);
    std::cout << "Final array state: ";
    array.printArray();
    LeaveCriticalSection(&criticalSection);

    CloseHandle(startThreadsEvent);
    for (int i = 0; i < markerThreadsNumber; i++) {
        CloseHandle(stopEvents[i]);
        CloseHandle(endEvents[i]);
    }
    DeleteCriticalSection(&criticalSection);

    for (int i = 0; i < markerThreadsNumber; ++i) {
        if (threads[i] != nullptr) {
            threads[i]->wait();
            delete threads[i];
        }
    }

    delete[] endEvents;
    delete[] stopEvents;
    delete[] isAlive;
    delete[] threads;

    return 0;
}