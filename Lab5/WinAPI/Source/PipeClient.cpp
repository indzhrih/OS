#include "../Headers/PipeClient.h"
#include "../Headers/ExceptionHandler.h"

PipeClient::PipeClient(const std::string& pipeNameValue)
    : pipeName(pipeNameValue), pipeHandle(INVALID_HANDLE_VALUE) {
}

PipeClient::~PipeClient() {
    if (pipeHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(pipeHandle);
        pipeHandle = INVALID_HANDLE_VALUE;
    }
}

bool PipeClient::connectToServer() {
    std::string pipePath = std::string(R"(\\.\pipe\)") + pipeName;

    while (true) {
        pipeHandle = CreateFileA(pipePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (pipeHandle != INVALID_HANDLE_VALUE) break;

        DWORD errorCode = GetLastError();
        if (errorCode == ERROR_PIPE_BUSY || errorCode == ERROR_FILE_NOT_FOUND) {
            BOOL waitResult = WaitNamedPipeA(pipePath.c_str(), 20000);
            if (!waitResult) {
                ExceptionHandler::printError("WaitNamedPipe failed, error: " + std::to_string(GetLastError()));
                return false;
            }
            continue;
        }

        ExceptionHandler::printError("CreateFile for pipe failed, error: " + std::to_string(errorCode));
        return false;
    }

    DWORD mode = PIPE_READMODE_MESSAGE;
    BOOL modeResult = SetNamedPipeHandleState(pipeHandle, &mode, NULL, NULL);
    if (!modeResult) {
        ExceptionHandler::printError("SetNamedPipeHandleState failed, error: " + std::to_string(GetLastError()));
        return false;
    }

    return true;
}

bool PipeClient::sendRequest(const PipeRequest& request) const {
    if (pipeHandle == INVALID_HANDLE_VALUE) {
        ExceptionHandler::printError("sendRequest: pipe handle is invalid");
        return false;
    }

    DWORD bytesWritten = 0;
    BOOL writeOk = WriteFile(pipeHandle, &request, sizeof(PipeRequest), &bytesWritten, NULL);
    if (!writeOk || bytesWritten != sizeof(PipeRequest)) {
        ExceptionHandler::printError("WriteFile to pipe failed, error: " + std::to_string(GetLastError()));
        return false;
    }

    return true;
}

bool PipeClient::receiveResponse(PipeResponse& response) const {
    if (pipeHandle == INVALID_HANDLE_VALUE) {
        ExceptionHandler::printError("receiveResponse: pipe handle is invalid");
        return false;
    }

    DWORD bytesRead = 0;
    BOOL readOk = ReadFile(pipeHandle, &response, sizeof(PipeResponse), &bytesRead, NULL);
    if (!readOk || bytesRead != sizeof(PipeResponse)) {
        ExceptionHandler::printError("ReadFile from pipe failed, error: " + std::to_string(GetLastError()));
        return false;
    }

    return true;
}
