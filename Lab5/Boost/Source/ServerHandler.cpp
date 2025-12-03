#include "../Headers/ServerHandler.h"
#include "../Headers/ExceptionHandler.h"

ServerHandler::ServerHandler(EmployeeStorage* employeeStorageValue, PipeServer* pipeServerValue, RecordLockManager* recordLockManagerValue)
    : employeeStorage(employeeStorageValue),
      pipeServer(pipeServerValue),
      recordLockManager(recordLockManagerValue),
      isRunning(true) {
}

void ServerHandler::run() {
    if (!pipeServer->waitForClient()) return;

    while (isRunning) {
        PipeRequest request;
        if (!pipeServer->readRequest(request)) break;

        PipeResponse response;

        if (request.type == PIPE_REQUEST_READ) handleReadRequest(request, response);
        else if (request.type == PIPE_REQUEST_BEGIN_MODIFY) handleBeginModifyRequest(request, response);
        else if (request.type == PIPE_REQUEST_COMMIT_MODIFY) handleCommitModifyRequest(request, response);
        else if (request.type == PIPE_REQUEST_END_ACCESS) handleEndAccessRequest(request, response);
        else if (request.type == PIPE_REQUEST_SHUTDOWN) handleShutdownRequest(response);
        else response.status = PIPE_RESPONSE_ERROR;

        if (!pipeServer->writeResponse(response)) break;
    }
}

void ServerHandler::handleReadRequest(PipeRequest& request, PipeResponse& response) {
    if (!recordLockManager->beginRead()) {
        response.status = PIPE_RESPONSE_INVALID_STATE;
        return;
    }

    employee record;
    bool found = employeeStorage->readEmployee(request.employeeId, record);
    if (!found) {
        recordLockManager->endRead();
        response.status = PIPE_RESPONSE_NOT_FOUND;
        return;
    }

    response.status = PIPE_RESPONSE_OK;
    response.employeeData = record;
}

void ServerHandler::handleBeginModifyRequest(PipeRequest& request, PipeResponse& response) {
    if (!recordLockManager->beginWrite()) {
        response.status = PIPE_RESPONSE_INVALID_STATE;
        return;
    }

    employee record;
    bool found = employeeStorage->readEmployee(request.employeeId, record);
    if (!found) {
        recordLockManager->endWrite();
        response.status = PIPE_RESPONSE_NOT_FOUND;
        return;
    }

    response.status = PIPE_RESPONSE_OK;
    response.employeeData = record;
}

void ServerHandler::handleCommitModifyRequest(PipeRequest& request, PipeResponse& response) {
    if (!recordLockManager->hasWriter()) {
        response.status = PIPE_RESPONSE_INVALID_STATE;
        return;
    }

    bool written = employeeStorage->writeEmployee(request.employeeId, request.employeeData);
    if (!written) {
        response.status = PIPE_RESPONSE_NOT_FOUND;
        return;
    }

    response.status = PIPE_RESPONSE_OK;
    response.employeeData = request.employeeData;
}

void ServerHandler::handleEndAccessRequest(PipeRequest&, PipeResponse& response) {
    bool writerExists = recordLockManager->hasWriter();
    bool lockReleased = writerExists ? recordLockManager->endWrite() : recordLockManager->endRead();

    if (!lockReleased) {
        response.status = PIPE_RESPONSE_INVALID_STATE;
        return;
    }

    response.status = PIPE_RESPONSE_OK;
}

void ServerHandler::handleShutdownRequest(PipeResponse& response) {
    response.status = PIPE_RESPONSE_OK;
    isRunning = false;
}
