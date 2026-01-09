#ifndef LAB5_PIPESERVER_H
#define LAB5_PIPESERVER_H

#include <string>
#include <windows.h>
#include "PipeRequest.h"
#include "PipeResponse.h"

class PipeServer {
public:
    PipeServer(const std::string& pipeNameValue);
    ~PipeServer();

    bool createPipe();
    bool waitForClient();
    bool readRequest(PipeRequest& request);
    bool writeResponse(const PipeResponse& response);
    void disconnect();

private:
    std::string pipeName;
    HANDLE pipeHandle;
};

#endif
