#include "../Headers/ServerHandler.h"
#include <iostream>
#include <exception>

ServerHandler::ServerHandler(EmployeeFile* employeeFileValue, PipeServer* pipeServerValue, RecordLockManager* recordLockManagerValue)
    : employeeFile(employeeFileValue),
      pipeServer(pipeServerValue),
      recordLockManager(recordLockManagerValue),
      isRunning(true) {
}

void ServerHandler::run() {
    bool clientConnected = pipeServer->waitForClient();
    if (!clientConnected) {
        return;
    }

    while (isRunning) {
        PipeRequest request;
        bool requestRead = pipeServer->readRequest(request);
        if (!requestRead) {
            break;
        }

        PipeResponse response;

        if (request.type == PIPE_REQUEST_READ) {
            handleReadRequest(request, response);
        } else if (request.type == PIPE_REQUEST_BEGIN_MODIFY) {
            handleBeginModifyRequest(request, response);
        } else if (request.type == PIPE_REQUEST_COMMIT_MODIFY) {
            handleCommitModifyRequest(request, response);
        } else if (request.type == PIPE_REQUEST_END_ACCESS) {
            handleEndAccessRequest(request, response);
        } else if (request.type == PIPE_REQUEST_SHUTDOWN) {
            handleShutdownRequest(response);
        } else {
            response.status = PIPE_RESPONSE_ERROR;
        }

        bool responseWritten = pipeServer->writeResponse(response);
        if (!responseWritten) {
            break;
        }
    }
}

void ServerHandler::handleReadRequest(PipeRequest& request, PipeResponse& response) {
    bool lockGranted = recordLockManager->beginRead();
    if (!lockGranted) {
        response.status = PIPE_RESPONSE_INVALID_STATE;
        return;
    }

    employee record;
    bool found = employeeFile->readEmployee(request.employeeId, record);
    if (!found) {
        recordLockManager->endRead();
        response.status = PIPE_RESPONSE_NOT_FOUND;
        return;
    }

    response.status = PIPE_RESPONSE_OK;
    response.employeeData = record;
}

void ServerHandler::handleBeginModifyRequest(PipeRequest& request, PipeResponse& response) {
    bool lockGranted = recordLockManager->beginWrite();
    if (!lockGranted) {
        response.status = PIPE_RESPONSE_INVALID_STATE;
        return;
    }

    employee record;
    bool found = employeeFile->readEmployee(request.employeeId, record);
    if (!found) {
        recordLockManager->endWrite();
        response.status = PIPE_RESPONSE_NOT_FOUND;
        return;
    }

    response.status = PIPE_RESPONSE_OK;
    response.employeeData = record;
}

void ServerHandler::handleCommitModifyRequest(PipeRequest& request, PipeResponse& response) {
    bool writerExists = recordLockManager->hasWriter();
    if (!writerExists) {
        response.status = PIPE_RESPONSE_INVALID_STATE;
        return;
    }

    bool written = employeeFile->writeEmployee(request.employeeId, request.employeeData);
    if (!written) {
        response.status = PIPE_RESPONSE_NOT_FOUND;
        return;
    }

    response.status = PIPE_RESPONSE_OK;
    response.employeeData = request.employeeData;
}

void ServerHandler::handleEndAccessRequest(PipeRequest& request, PipeResponse& response) {
    bool writerExists = recordLockManager->hasWriter();
    bool lockReleased;

    if (writerExists) {
        lockReleased = recordLockManager->endWrite();
    } else {
        lockReleased = recordLockManager->endRead();
    }

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

DWORD WINAPI ServerHandlerThread(LPVOID parameter) {
    try {
        ServerHandler* handler = static_cast<ServerHandler*>(parameter);
        if (handler != NULL) {
            handler->run();
        }
    }
    catch (const std::exception& exception) {
        std::cerr << "ServerHandlerThread exception: " << exception.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "ServerHandlerThread unknown exception" << std::endl;
        return 1;
    }

    return 0;
}
