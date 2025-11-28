#include "../Headers/PipeServer.h"
#include "../Headers/ExceptionHandler.h"

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
    std::string pipePath = std::string(R"(\\.\pipe\)") + pipeName;
    DWORD bufferSize = 1024;

    pipeHandle = CreateNamedPipeA( pipePath.c_str(), PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, bufferSize, bufferSize, 0, NULL);

    return ExceptionHandler::checkHandle(pipeHandle, "CreateNamedPipe");
}

bool PipeServer::waitForClient() {
    if (pipeHandle == INVALID_HANDLE_VALUE) return false;

    BOOL connected = ConnectNamedPipe(pipeHandle, NULL);
    if (!connected) {
        DWORD errorCode = GetLastError();
        if (errorCode != ERROR_PIPE_CONNECTED) {
            ExceptionHandler::printError("ConnectNamedPipe failed, error: " + std::to_string(errorCode));
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
        ExceptionHandler::printError("ReadFile from pipe failed, error: " + std::to_string(GetLastError()));
        return false;
    }

    return true;
}

bool PipeServer::writeResponse(const PipeResponse& response) {
    if (pipeHandle == INVALID_HANDLE_VALUE) return false;

    DWORD bytesWritten = 0;
    BOOL writeOk = WriteFile(pipeHandle, &response, sizeof(PipeResponse), &bytesWritten, NULL);
    if (!writeOk || bytesWritten != sizeof(PipeResponse)) {
        ExceptionHandler::printError("WriteFile to pipe failed, error: " + std::to_string(GetLastError()));
        return false;
    }

    return true;
}

void PipeServer::disconnect() {
    if (pipeHandle == INVALID_HANDLE_VALUE) return;
    FlushFileBuffers(pipeHandle);
    DisconnectNamedPipe(pipeHandle);
}
