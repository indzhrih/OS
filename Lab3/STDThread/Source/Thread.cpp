#include "../Headers/Thread.h"
#include "../Headers/ThreadHandler.h"
#include <stdexcept>

Thread::Thread(Event* startEvent, Event* endEvent, Event* stopEvent, std::mutex* criticalSection,
               Array* array, int threadNumber, int arraySize)
    : startEvent(startEvent),
      endEvent(endEvent),
      stopEvent(stopEvent),
      criticalSection(criticalSection),
      array(array),
      arraySize(arraySize),
      threadNumber(threadNumber) {

    if (this->startEvent == 0 || this->endEvent == 0 || this->stopEvent == 0 ||
        this->criticalSection == 0 || this->array == 0 || this->arraySize <= 0 || this->threadNumber <= 0) {
        throw std::invalid_argument("Invalid arguments passed to Thread constructor");
        }

    handle = std::thread(threadWorker, this);
}

Thread::~Thread() {
    if (handle.joinable()) handle.join();
}

void Thread::wait() {
    if (handle.joinable()) handle.join();
}
