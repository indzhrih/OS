#include "../Headers/PipeServer.h"
#include "../Headers/ExceptionHandler.h"
#include <iostream>

PipeServer::PipeServer(const std::string& pipeNameValue)
    : pipeName(pipeNameValue), pipeHandle(INVALID_HANDLE_VALUE) {
}

PipeServer::~PipeServer() {
    if (pipeHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(pipeHandle);
        pipeHandle = INVALID_HANDLE_VALUE;
    }
}

bool PipeServer::createPipe() {
    std::string fullName = "\\\\.\\pipe\\" + pipeName;
    DWORD bufferSize = 1024;

    pipeHandle = CreateNamedPipeA(fullName.c_str(), PIPE_ACCESS_DUPLEX,
                                  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                  1, bufferSize, bufferSize, 0, NULL);

    if (!ExceptionHandler::checkHandle(pipeHandle, "CreateNamedPipe")) return false;

    return true;
}

bool PipeServer::waitForClient() {
    if (pipeHandle == INVALID_HANDLE_VALUE) return false;

    BOOL connected = ConnectNamedPipe(pipeHandle, NULL);
    if (!connected) {
        DWORD errorCode = GetLastError();
        if (errorCode != ERROR_PIPE_CONNECTED) {
            std::cerr << "ConnectNamedPipe failed, error: " << errorCode << std::endl;
            return false;
        }
    }

    return true;
}

bool PipeServer::readRequest(PipeRequest& request) {
    if (pipeHandle == INVALID_HANDLE_VALUE) return false;

    DWORD bytesRead = 0;
    BOOL readOk = ReadFile(pipeHandle, &request, sizeof(PipeRequest), &bytesRead, NULL);
    if (!readOk || bytesRead != sizeof(PipeRequest)) {
        std::cerr << "ReadFile from pipe failed, error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

bool PipeServer::writeResponse(const PipeResponse& response) {
    if (pipeHandle == INVALID_HANDLE_VALUE) return false;

    DWORD bytesWritten = 0;
    BOOL writeOk = WriteFile(pipeHandle, &response, sizeof(PipeResponse), &bytesWritten, NULL);
    if (!writeOk || bytesWritten != sizeof(PipeResponse)) {
        std::cerr << "WriteFile to pipe failed, error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

void PipeServer::disconnect() {
    if (pipeHandle == INVALID_HANDLE_VALUE) return;

    FlushFileBuffers(pipeHandle);
    DisconnectNamedPipe(pipeHandle);
}
