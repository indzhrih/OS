#ifndef LAB5_RECORDLOCKMANAGER_H
#define LAB5_RECORDLOCKMANAGER_H

#include <mutex>

class RecordLockManager {
public:
    RecordLockManager();

    bool beginRead();
    bool beginWrite();
    bool endRead();
    bool endWrite();
    bool hasWriter() const;

private:
    mutable std::mutex mutex;
    int readersCount;
    bool writeLocked;
};

#endif
