#ifndef LAB5_CLIENTHANDLER_H
#define LAB5_CLIENTHANDLER_H

#include "PipeClient.h"

class ClientHandler {
public:
    ClientHandler(PipeClient* pipeClientValue);

    void run();

private:
    PipeClient* pipeClient;

    int readEmployeeIdFromConsole();
    void handleModifyCommand();
    void handleReadCommand();
    void handleShutdownCommand();
};

#endif
