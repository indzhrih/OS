#include "../Headers/RecordLockManager.h"

RecordLockManager::RecordLockManager() {
    InitializeCriticalSection(&criticalSection);
    readersCount = 0;
    writeLocked = false;
}

RecordLockManager::~RecordLockManager() {
    DeleteCriticalSection(&criticalSection);
}

bool RecordLockManager::beginRead() {
    EnterCriticalSection(&criticalSection);

    if (writeLocked) {
        LeaveCriticalSection(&criticalSection);
        return false;
    }

    readersCount++;
    LeaveCriticalSection(&criticalSection);
    return true;
}

bool RecordLockManager::beginWrite() {
    EnterCriticalSection(&criticalSection);

    if (writeLocked || readersCount > 0) {
        LeaveCriticalSection(&criticalSection);
        return false;
    }

    writeLocked = true;
    LeaveCriticalSection(&criticalSection);
    return true;
}

bool RecordLockManager::endRead() {
    EnterCriticalSection(&criticalSection);

    if (readersCount <= 0) {
        LeaveCriticalSection(&criticalSection);
        return false;
    }

    readersCount--;
    LeaveCriticalSection(&criticalSection);
    return true;
}

bool RecordLockManager::endWrite() {
    EnterCriticalSection(&criticalSection);

    if (!writeLocked) {
        LeaveCriticalSection(&criticalSection);
        return false;
    }

    writeLocked = false;
    LeaveCriticalSection(&criticalSection);
    return true;
}

bool RecordLockManager::hasWriter() {
    EnterCriticalSection(&criticalSection);
    bool result = writeLocked;
    LeaveCriticalSection(&criticalSection);
    return result;
}
