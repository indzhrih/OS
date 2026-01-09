#ifndef LAB5_PROCESSHANDLER_H
#define LAB5_PROCESSHANDLER_H

#include <string>
#include <windows.h>

class ProcessHandler {
public:
    ProcessHandler(const std::string& commandLine);
    ~ProcessHandler();

    void waitForProcess();

private:
    std::string command;
    PROCESS_INFORMATION processInfo;
    bool isStarted;
};

#endif
