#include <iostream>
#include <stdexcept>
#include "../Headers/ThreadHandler.h"
#include "../Headers/Thread.h"

DWORD WINAPI threadWorker(LPVOID threadParam) {
    try {
        if (!threadParam) throw std::invalid_argument("threadParam is null");

        Thread* data = static_cast<Thread*>(threadParam);

        if (!data->startEvent || !data->endEvent || !data->stopEvent) {
            throw std::invalid_argument("One or more thread events are null");
        }

        if (!data->criticalSection) {
            throw std::invalid_argument("criticalSection is null");
        }

        if (!data->array || data->arraySize <= 0 || data->threadNumber <= 0) {
            throw std::invalid_argument("Array/threadNumber/arraySize are invalid");
        }

        DWORD wr = WaitForSingleObject(data->startEvent, INFINITE);
        if (wr == WAIT_FAILED) {
            DWORD ec = GetLastError();
            throw std::runtime_error("WaitForSingleObject(startEvent) failed, GetLastError=" + std::to_string(ec));
        }

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
                          << ". Marked " << markedCount << " elements." << std::endl;
                LeaveCriticalSection(data->criticalSection);

                if (!SetEvent(data->stopEvent)) {
                    DWORD ec = GetLastError();
                    throw std::runtime_error("SetEvent(stopEvent) failed, GetLastError=" + std::to_string(ec));
                }

                HANDLE waitHandles[2] = { data->endEvent, data->startEvent };
                DWORD result = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

                if (result == WAIT_FAILED) {
                    DWORD ec = GetLastError();
                    throw std::runtime_error("WaitForMultipleObjects failed, GetLastError=" + std::to_string(ec));
                }

                if (result == WAIT_OBJECT_0) {
                    EnterCriticalSection(data->criticalSection);
                    for (int i = 0; i < data->arraySize; ++i) {
                        if ((*data->array)[i] == data->threadNumber) (*data->array)[i] = 0;
                    }
                    LeaveCriticalSection(data->criticalSection);

                    EnterCriticalSection(data->criticalSection);
                    std::cout << "Thread " << data->threadNumber << " finished work." << std::endl;
                    LeaveCriticalSection(data->criticalSection);

                    continueWork = false;
                }
            }
        }

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "[threadWorker] Exception: " << ex.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "[threadWorker] Unknown exception" << std::endl;
        return 1;
    }
}
