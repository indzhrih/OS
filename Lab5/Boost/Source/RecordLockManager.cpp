#include "../Headers/RecordLockManager.h"

RecordLockManager::RecordLockManager() : readersCount(0), writeLocked(false) {
}

bool RecordLockManager::beginRead() {
    std::lock_guard<std::mutex> lock(mutex);
    if (writeLocked) return false;
    ++readersCount;
    return true;
}

bool RecordLockManager::beginWrite() {
    std::lock_guard<std::mutex> lock(mutex);
    if (writeLocked || readersCount > 0) return false;
    writeLocked = true;
    return true;
}

bool RecordLockManager::endRead() {
    std::lock_guard<std::mutex> lock(mutex);
    if (readersCount <= 0) return false;
    --readersCount;
    return true;
}

bool RecordLockManager::endWrite() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!writeLocked) return false;
    writeLocked = false;
    return true;
}

bool RecordLockManager::hasWriter() const {
    std::lock_guard<std::mutex> lock(mutex);
    return writeLocked;
}
