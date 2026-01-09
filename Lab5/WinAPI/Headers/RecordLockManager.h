#ifndef LAB5_RECORDLOCKMANAGER_H
#define LAB5_RECORDLOCKMANAGER_H

#include <mutex>
#include <unordered_map>

class RecordLockManager {
public:
    RecordLockManager();

    bool beginRead(int employeeId);
    bool beginWrite(int employeeId);
    bool endRead(int employeeId);
    bool endWrite(int employeeId);
    bool hasWriter(int employeeId) const;

private:
    struct LockInfo {
        int readersCount = 0;
        bool writeLocked = false;
    };

    mutable std::mutex mutex;
    std::unordered_map<int, LockInfo> locks;
};

#endif
