#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <string>
#include <thread>
#include "../Headers/Array.h"

class ThreadHandler {
public:
    void createThread(const std::string& threadName, Array& array);
    void waitForThreads();

private:
    static void minMax(Array* array);
    static void average(Array* array);

    std::thread currentThread;
};

#endif //LAB2_THREADHANDLER_H
