#include <iostream>
#include <limits>
#include <stdexcept>
#include <windows.h>
#include "Headers/Array.h"
#include "Headers/Thread.h"

int main() {
    int n, markerThreadsNumber;

    try {
        std::cout << "Enter array size " << std::endl;
        while (true) {
            if (std::cin >> n && n > 0) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a positive integer for array size: " << std::endl;
        }

        Array array(n);

        std::cout << "Enter marker threads number" << std::endl;
        while (true) {
            if (std::cin >> markerThreadsNumber && markerThreadsNumber > 0) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a positive integer for marker threads number: " << std::endl;
        }

        HANDLE startThreadsEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (startThreadsEvent == NULL) {
            DWORD ec = GetLastError();
            throw std::runtime_error("CreateEvent(startThreadsEvent) failed, GetLastError=" + std::to_string(ec));
        }

        HANDLE* stopEvents = new HANDLE[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; i++) {
            stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (stopEvents[i] == NULL) {
                DWORD ec = GetLastError();
                throw std::runtime_error("CreateEvent(stopEvents[i]) failed, GetLastError=" + std::to_string(ec));
            }
        }

        HANDLE* endEvents = new HANDLE[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; i++) {
            endEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (endEvents[i] == NULL) {
                DWORD ec = GetLastError();
                throw std::runtime_error("CreateEvent(endEvents[i]) failed, GetLastError=" + std::to_string(ec));
            }
        }

        CRITICAL_SECTION criticalSection;
        InitializeCriticalSection(&criticalSection);

        Thread** threads = new Thread*[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; i++) {
            threads[i] = new Thread(startThreadsEvent, endEvents[i], stopEvents[i], &criticalSection,
                                    &array, i + 1, n);
        }

        bool* isAlive = new bool[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; ++i) isAlive[i] = true;

        if (!SetEvent(startThreadsEvent)) {
            DWORD ec = GetLastError();
            throw std::runtime_error("SetEvent(startThreadsEvent) failed, GetLastError=" + std::to_string(ec));
        }

        int activeThreads = markerThreadsNumber;
        while (activeThreads > 0) {
            EnterCriticalSection(&criticalSection);
            std::cout << "Waiting for threads signals." << std::endl;
            LeaveCriticalSection(&criticalSection);

            if (!ResetEvent(startThreadsEvent)) {
                DWORD ec = GetLastError();
                throw std::runtime_error("ResetEvent(startThreadsEvent) failed, GetLastError=" + std::to_string(ec));
            }

            int waitCount = 0;
            HANDLE* waitList = new HANDLE[markerThreadsNumber];
            for (int i = 0; i < markerThreadsNumber; ++i) {
                if (isAlive[i]) waitList[waitCount++] = stopEvents[i];
            }

            if (waitCount == 0) {
                delete[] waitList;
                break;
            }
            DWORD wr = WaitForMultipleObjects(waitCount, waitList, TRUE, INFINITE);
            delete[] waitList;

            if (wr == WAIT_FAILED) {
                DWORD ec = GetLastError();
                throw std::runtime_error("WaitForMultipleObjects(stopEvents) failed, GetLastError=" + std::to_string(ec));
            }

            EnterCriticalSection(&criticalSection);
            array.printArray();
            LeaveCriticalSection(&criticalSection);

            int threadToStop;
            while (true) {
                EnterCriticalSection(&criticalSection);
                std::cout << "Enter thread number to stop (1-" << markerThreadsNumber << "): ";
                LeaveCriticalSection(&criticalSection);

                if (std::cin >> threadToStop) {
                    if (threadToStop >= 1 && threadToStop <= markerThreadsNumber && isAlive[threadToStop - 1]) {
                        break;
                    }
                    EnterCriticalSection(&criticalSection);
                    std::cout << "This thread is already finished or invalid. Choose an active one." << std::endl;
                    LeaveCriticalSection(&criticalSection);
                }
                else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    EnterCriticalSection(&criticalSection);
                    std::cout << "Please enter a valid integer in range." << std::endl;
                    LeaveCriticalSection(&criticalSection);
                }
            }

            if (!SetEvent(endEvents[threadToStop - 1])) {
                DWORD ec = GetLastError();
                throw std::runtime_error("SetEvent(endEvents[idx]) failed, GetLastError=" + std::to_string(ec));
            }

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
                        if (!ResetEvent(stopEvents[i])) {
                            DWORD ec = GetLastError();
                            throw std::runtime_error("ResetEvent(stopEvents[i]) failed, GetLastError=" + std::to_string(ec));
                        }
                        if (!ResetEvent(endEvents[i])) {
                            DWORD ec = GetLastError();
                            throw std::runtime_error("ResetEvent(endEvents[i]) failed, GetLastError=" + std::to_string(ec));
                        }
                    }
                }
                if (!SetEvent(startThreadsEvent)) {
                    DWORD ec = GetLastError();
                    throw std::runtime_error("SetEvent(startThreadsEvent) failed, GetLastError=" + std::to_string(ec));
                }
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
    catch (const std::exception& ex) {
        std::cerr << "[main] Exception: " << ex.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "[main] Unknown exception" << std::endl;
        return 1;
    }
}