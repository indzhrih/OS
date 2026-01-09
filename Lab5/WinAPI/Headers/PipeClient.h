#ifndef LAB5_PIPECLIENT_H
#define LAB5_PIPECLIENT_H

#include <string>
#include <windows.h>
#include "PipeRequest.h"
#include "PipeResponse.h"

class PipeClient {
public:
    PipeClient(const std::string& pipeNameValue);
    ~PipeClient();

    bool connectToServer();
    bool sendRequest(const PipeRequest& request) const;
    bool receiveResponse(PipeResponse& response) const;

private:
    std::string pipeName;
    HANDLE pipeHandle;
};

#endif
