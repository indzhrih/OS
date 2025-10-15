#include <iostream>
#include <limits>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <chrono>

#include "Headers/Array.h"
#include "Headers/Thread.h"
#include "Headers/Event.h"

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

        Event* startThreadsEvent = new Event(true, false);

        Event** stopEvents = new Event*[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; i++) {
            stopEvents[i] = new Event(true, false);
        }

        Event** endEvents = new Event*[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; i++) {
            endEvents[i] = new Event(true, false);
        }

        std::mutex criticalSection;

        Thread** threads = new Thread*[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; i++) {
            threads[i] = new Thread(startThreadsEvent, endEvents[i], stopEvents[i],
                                    &criticalSection, &array, i + 1, n);
        }

        bool* isAlive = new bool[markerThreadsNumber];
        for (int i = 0; i < markerThreadsNumber; i++) {
            isAlive[i] = true;
        }

        startThreadsEvent->set();

        int activeThreads = markerThreadsNumber;
        while (activeThreads > 0) {
            criticalSection.lock();
            std::cout << "Waiting for threads signals." << std::endl;
            criticalSection.unlock();

            while (true) {
                bool allSet = true;
                for (int i = 0; i < markerThreadsNumber; i++) {
                    if (isAlive[i]) {
                        if (!stopEvents[i]->is_set()) {
                            allSet = false;
                            break;
                        }
                    }
                }
                if (allSet) break;
            }

            startThreadsEvent->reset();

            criticalSection.lock();
            array.printArray();
            criticalSection.unlock();

            int threadToStop;
            while (true) {
                criticalSection.lock();
                std::cout << "Enter thread number to stop (1-" << markerThreadsNumber << "): ";
                criticalSection.unlock();

                if (std::cin >> threadToStop) {
                    if (threadToStop >= 1 && threadToStop <= markerThreadsNumber && isAlive[threadToStop - 1]) {
                        break;
                    }
                    criticalSection.lock();
                    std::cout << "This thread is already finished or invalid. Choose an active one." << std::endl;
                    criticalSection.unlock();
                }
                else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    criticalSection.lock();
                    std::cout << "Please enter a valid integer in range." << std::endl;
                    criticalSection.unlock();
                }
            }

            endEvents[threadToStop - 1]->set();

            criticalSection.lock();
            std::cout << "Waiting for thread " << threadToStop << " to finish..." << std::endl;
            criticalSection.unlock();

            threads[threadToStop - 1]->wait();
            delete threads[threadToStop - 1];
            threads[threadToStop - 1] = 0;

            isAlive[threadToStop - 1] = false;
            activeThreads--;

            criticalSection.lock();
            std::cout << "Array after thread " << threadToStop << " finished: ";
            array.printArray();
            criticalSection.unlock();

            if (activeThreads > 0) {
                for (int i = 0; i < markerThreadsNumber; i++) {
                    if (isAlive[i]) {
                        stopEvents[i]->reset();
                        endEvents[i]->reset();
                    }
                }
                startThreadsEvent->set();
            }
        }

        criticalSection.lock();
        std::cout << "Final array state: ";
        array.printArray();
        criticalSection.unlock();

        for (int i = 0; i < markerThreadsNumber; i++) {
            if (threads[i] != 0) {
                threads[i]->wait();
                delete threads[i];
            }
        }

        for (int i = 0; i < markerThreadsNumber; i++) {
            delete endEvents[i];
            delete stopEvents[i];
        }
        delete[] endEvents;
        delete[] stopEvents;

        delete startThreadsEvent;
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
