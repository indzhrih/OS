#include "../Headers/PipeClient.h"
#include "../Headers/ExceptionHandler.h"
#include <iostream>

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
    std::string fullName = "\\\\.\\pipe\\" + pipeName;

    while (true) {
        pipeHandle = CreateFileA(fullName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (pipeHandle != INVALID_HANDLE_VALUE) {
            break;
        }

        DWORD errorCode = GetLastError();

        if (errorCode == ERROR_PIPE_BUSY || errorCode == ERROR_FILE_NOT_FOUND) {
            BOOL waitResult = WaitNamedPipeA(fullName.c_str(), 20000);
            if (!waitResult) {
                std::cerr << "WaitNamedPipe failed, error: " << GetLastError() << std::endl;
                return false;
            }
            continue;
        }

        std::cerr << "CreateFile for pipe failed, error: " << errorCode << std::endl;
        return false;
    }

    DWORD mode = PIPE_READMODE_MESSAGE;
    BOOL modeResult = SetNamedPipeHandleState(pipeHandle, &mode, NULL, NULL);
    if (!modeResult) {
        std::cerr << "SetNamedPipeHandleState failed, error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

bool PipeClient::sendRequest(const PipeRequest& request) const {
    if (pipeHandle == INVALID_HANDLE_VALUE) {
        std::cerr << "sendRequest: pipe handle is invalid" << std::endl;
        return false;
    }

    DWORD bytesWritten = 0;
    BOOL writeOk = WriteFile(pipeHandle, &request, sizeof(PipeRequest), &bytesWritten, NULL);
    if (!writeOk || bytesWritten != sizeof(PipeRequest)) {
        std::cerr << "WriteFile to pipe failed, error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

bool PipeClient::receiveResponse(PipeResponse& response) const {
    if (pipeHandle == INVALID_HANDLE_VALUE) {
        std::cerr << "receiveResponse: pipe handle is invalid" << std::endl;
        return false;
    }

    DWORD bytesRead = 0;
    BOOL readOk = ReadFile(pipeHandle, &response, sizeof(PipeResponse), &bytesRead, NULL);
    if (!readOk || bytesRead != sizeof(PipeResponse)) {
        std::cerr << "ReadFile from pipe failed, error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}
