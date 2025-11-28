#ifndef LAB5_SERVERHANDLER_H
#define LAB5_SERVERHANDLER_H

#include <windows.h>
#include "PipeServer.h"
#include "EmployeeStorage.h"
#include "RecordLockManager.h"

class ServerHandler {
public:
    ServerHandler(EmployeeStorage* employeeStorageValue, PipeServer* pipeServerValue, RecordLockManager* recordLockManagerValue);

    void run();

private:
    EmployeeStorage* employeeStorage;
    PipeServer* pipeServer;
    RecordLockManager* recordLockManager;
    bool isRunning;

    void handleReadRequest(PipeRequest& request, PipeResponse& response);
    void handleBeginModifyRequest(PipeRequest& request, PipeResponse& response);
    void handleCommitModifyRequest(PipeRequest& request, PipeResponse& response);
    void handleEndAccessRequest(PipeRequest& request, PipeResponse& response);
    void handleShutdownRequest(PipeResponse& response);
};

DWORD WINAPI ServerHandlerThread(LPVOID parameter);

#endif
