#ifndef LAB3_WINAPI_THREAD_H
#define LAB3_WINAPI_THREAD_H

#include <thread>
#include <mutex>
#include "Array.h"
#include "Event.h"

class Thread {
    friend int threadWorker(Thread* threadParam);

public:
    Thread(Event* startEvent,
           Event* endEvent,
           Event* stopEvent,
           std::mutex* criticalSection,
           Array* array,
           int threadNumber,
           int arraySize);
    ~Thread();

    void wait();

private:
    Event* startEvent;
    Event* endEvent;
    Event* stopEvent;
    std::mutex* criticalSection;

    Array* array;
    int arraySize;
    int threadNumber;

    std::thread handle;
};

#endif // LAB3_WINAPI_THREAD_H
