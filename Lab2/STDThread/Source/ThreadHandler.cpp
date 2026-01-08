#include <iostream>
#include <thread>
#include "../Headers/ThreadHandler.h"

void ThreadHandler::createThread(const std::string& threadName, Array& array) {
    waitForThreads();

    try {
        if (threadName == "min_max") currentThread = std::thread(minMax, &array);
        else if (threadName == "average") currentThread = std::thread(average, &array);
        else throw std::invalid_argument("Error, wrong thread name");
    }
    catch (const std::exception& e) {
        std::cerr << "Thread handler error: " << e.what() << std::endl;
        throw;
    }
}

void ThreadHandler::minMax(Array* array) {
    try {
        std::cout << "Min: " << array->findMinElement() << std::endl;
        std::cout << "Max: " << array->findMaxElement() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in minMax thread: " << e.what() << std::endl;
    }
}

void ThreadHandler::waitForThreads() {
    if (currentThread.joinable()) currentThread.join();
}

void ThreadHandler::average(Array* array) {
    try {
        std::cout << "Average value: " << array->calculateAverageValue() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in average thread: " << e.what() << std::endl;
    }
}