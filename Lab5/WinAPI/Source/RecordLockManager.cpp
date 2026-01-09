#include "../Headers/RecordLockManager.h"

RecordLockManager::RecordLockManager() {
}

bool RecordLockManager::beginRead(int employeeId) {
    std::lock_guard<std::mutex> lock(mutex);
    LockInfo& info = locks[employeeId];
    if (info.writeLocked) return false;
    ++info.readersCount;
    return true;
}

bool RecordLockManager::beginWrite(int employeeId) {
    std::lock_guard<std::mutex> lock(mutex);
    LockInfo& info = locks[employeeId];
    if (info.writeLocked || info.readersCount > 0) return false;
    info.writeLocked = true;
    return true;
}

bool RecordLockManager::endRead(int employeeId) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = locks.find(employeeId);
    if (it == locks.end()) return false;
    LockInfo& info = it->second;
    if (info.readersCount <= 0) return false;
    --info.readersCount;
    if (info.readersCount == 0 && !info.writeLocked) locks.erase(it);
    return true;
}

bool RecordLockManager::endWrite(int employeeId) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = locks.find(employeeId);
    if (it == locks.end()) return false;
    LockInfo& info = it->second;
    if (!info.writeLocked) return false;
    info.writeLocked = false;
    if (info.readersCount == 0) locks.erase(it);
    return true;
}

bool RecordLockManager::hasWriter(int employeeId) const {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = locks.find(employeeId);
    if (it == locks.end()) return false;
    return it->second.writeLocked;
}
