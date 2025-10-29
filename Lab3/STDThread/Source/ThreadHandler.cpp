#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "../Headers/ThreadHandler.h"
#include "../Headers/Thread.h"
#include "../Headers/Event.h"

int threadWorker(Thread* data) {
    try {
        if (data == nullptr) {
            throw std::invalid_argument("threadParam is null");
        }
        if (data->startEvent == 0 || data->endEvent == 0 || data->stopEvent == 0) {
            throw std::invalid_argument("One or more thread events are null");
        }
        if (data->criticalSection == 0) {
            throw std::invalid_argument("criticalSection is null");
        }
        if (data->array == 0 || data->arraySize <= 0 || data->threadNumber <= 0) {
            throw std::invalid_argument("Array/threadNumber/arraySize are invalid");
        }

        data->startEvent->wait();

        srand(data->threadNumber);
        int markedCount = 0;
        bool continueWork = true;

        while (continueWork) {
            if (data->endEvent->is_set()) {
                data->criticalSection->lock();
                for (int i = 0; i < data->arraySize; ++i)
                    if ((*data->array)[i] == data->threadNumber) (*data->array)[i] = 0;
                data->criticalSection->unlock();

                data->criticalSection->lock();
                std::cout << "Thread " << data->threadNumber << " finished work." << std::endl;
                data->criticalSection->unlock();
                break;
            }

            int index = rand() % data->arraySize;

            data->criticalSection->lock();
            if ((*data->array)[index] == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                (*data->array)[index] = data->threadNumber;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                markedCount++;
                data->criticalSection->unlock();
            }
            else {
                data->criticalSection->unlock();

                data->criticalSection->lock();
                std::cout << "Thread " << data->threadNumber
                          << " cannot mark element at index " << index
                          << ". Marked " << markedCount << " elements." << std::endl;
                data->criticalSection->unlock();

                data->stopEvent->set();

                if (data->endEvent->is_set()) {
                    data->criticalSection->lock();
                    for (int i = 0; i < data->arraySize; ++i)
                        if ((*data->array)[i] == data->threadNumber) (*data->array)[i] = 0;
                    data->criticalSection->unlock();

                    data->criticalSection->lock();
                    std::cout << "Thread " << data->threadNumber << " finished work." << std::endl;
                    data->criticalSection->unlock();

                    continueWork = false;
                    break;
                }

                data->startEvent->wait();
                if (data->endEvent->is_set()) {
                    data->criticalSection->lock();
                    for (int i = 0; i < data->arraySize; ++i)
                        if ((*data->array)[i] == data->threadNumber) (*data->array)[i] = 0;
                    data->criticalSection->unlock();

                    data->criticalSection->lock();
                    std::cout << "Thread " << data->threadNumber << " finished work." << std::endl;
                    data->criticalSection->unlock();
                    break;
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
