#include "../Headers/Thread.h"
#include <stdexcept>

Thread::Thread(HANDLE startEvent, HANDLE endEvent, HANDLE stopEvent, CRITICAL_SECTION* criticalSection,
               Array* array, int threadNumber, int arraySize)
    : startEvent(startEvent),
      endEvent(endEvent),
      stopEvent(stopEvent),
      criticalSection(criticalSection),
      array(array),
      arraySize(arraySize),
      threadNumber(threadNumber),
      handle(NULL),
      threadId(0)
{
    if (!this->startEvent || !this->endEvent || !this->stopEvent ||
        !this->criticalSection || !this->array || this->arraySize <= 0 || this->threadNumber <= 0) {

        throw std::invalid_argument("Invalid arguments passed to Thread constructor");
    }

    handle = CreateThread(NULL, 0, threadWorker, this, 0, &threadId);
    if (handle == NULL) {
        DWORD ec = GetLastError();
        throw std::runtime_error("CreateThread failed, GetLastError=" + std::to_string(ec));
    }
}

Thread::~Thread() {
    if (handle) {
        CloseHandle(handle);
        handle = NULL;
    }
}

void Thread::wait() {
    if (!handle) return;
    DWORD wr = WaitForSingleObject(handle, INFINITE);

    if (wr == WAIT_FAILED) {
        DWORD ec = GetLastError();
        throw std::runtime_error("WaitForSingleObject(thread) failed, GetLastError=" + std::to_string(ec));
    }
}
