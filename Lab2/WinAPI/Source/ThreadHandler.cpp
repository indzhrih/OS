#include <iostream>
#include <windows.h>
#include "../Headers/ThreadHandler.h"

void ThreadHandler::createThread(const std::string& threadName, Array& array) {
    HANDLE thread;

    try {
        if (threadName == "min_max") {
            thread = CreateThread(NULL, 0, minMax, &array, 0, NULL);
            if (thread == NULL) {
                throw std::runtime_error("Thread creation failed with error: " + std::to_string(GetLastError()));
            }
        }
        else if (threadName == "average") {
            thread = CreateThread(NULL, 0, average, &array, 0, NULL);
            if (thread == NULL) {
                throw std::runtime_error("Thread creation failed with error: " + std::to_string(GetLastError()));
            }
        }
        else {
            throw std::invalid_argument("Error, wrong thread name");
        }

        clearThreadResources(thread);
    }
    catch (const std::exception& e) {
        std::cerr << "Thread handler error: " << e.what() << std::endl;
        throw;
    }
}

void ThreadHandler::clearThreadResources(HANDLE thread) {
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
}

DWORD WINAPI ThreadHandler::minMax(LPVOID threadParam) {
    try {
        Array* array = static_cast<Array*>(threadParam);

        std::cout << "Min: " << array->findMinElement() << std::endl;
        std::cout << "Max: " << array->findMaxElement() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in minMax thread: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

DWORD WINAPI ThreadHandler::average(LPVOID threadParam) {
    try {
        Array* array = static_cast<Array*>(threadParam);

        std::cout << "Average value: " << array->calculateAverageValue() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in average thread: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}