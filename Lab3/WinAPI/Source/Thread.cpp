#include "../Headers/Thread.h"

Thread::Thread(HANDLE startEvent, HANDLE endEvent, HANDLE stopEvent, CRITICAL_SECTION* criticalSection,
               Array* array, int threadNumber, int arraySize)
    : startEvent(startEvent),
      endEvent(endEvent),
      stopEvent(stopEvent),
      criticalSection(criticalSection),
      array(array),
      arraySize(arraySize),
      threadNumber(threadNumber),
      handle(NULL)
{
    handle = CreateThread(NULL, 0, threadWorker, this, 0, NULL);
}

Thread::~Thread() {
    if (handle) {
        CloseHandle(handle);
        handle = NULL;
    }
}

void Thread::wait() {
    if (handle) WaitForSingleObject(handle, INFINITE);
}
