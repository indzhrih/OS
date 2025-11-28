#ifndef LAB5_RECORDLOCKMANAGER_H
#define LAB5_RECORDLOCKMANAGER_H

#include <windows.h>

class RecordLockManager {
public:
    RecordLockManager();
    ~RecordLockManager();

    bool beginRead();
    bool beginWrite();
    bool endRead();
    bool endWrite();
    bool hasWriter();

private:
    CRITICAL_SECTION criticalSection;
    int readersCount;
    bool writeLocked;
};

#endif
