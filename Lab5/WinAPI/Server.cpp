#include <iostream>
#include <string>
#include <stdexcept>
#include <windows.h>
#include "Headers/EmployeeStorage.h"
#include "Headers/PipeServer.h"
#include "Headers/ProcessHandler.h"
#include "Headers/ServerHandler.h"
#include "Headers/RecordLockManager.h"
#include "Headers/ExceptionHandler.h"

int main() {
    try {
        std::string binaryFileName;
        std::cout << "Enter binary file name" << std::endl;
        std::getline(std::cin >> std::ws, binaryFileName);

        EmployeeStorage employeeStorage(binaryFileName);
        employeeStorage.createFromConsole();
        employeeStorage.printAll();

        int clientsCount;
        std::cout << "Enter number of clients" << std::endl;
        while (!(std::cin >> clientsCount) || clientsCount <= 0) {
            ExceptionHandler::clearInput();
            std::cout << "Wrong input! Enter positive integer" << std::endl;
        }

        if (clientsCount > MAXIMUM_WAIT_OBJECTS) {
            ExceptionHandler::printError("Too many clients. Maximum allowed is " + std::to_string(MAXIMUM_WAIT_OBJECTS));
            std::cout << "Press any key to exit" << std::endl;
            system("pause");
            return 1;
        }

        PipeServer** pipeServers = new PipeServer*[clientsCount];
        ServerHandler** serverHandlers = new ServerHandler*[clientsCount];
        HANDLE* serverThreads = new HANDLE[clientsCount];
        ProcessHandler** clientProcesses = new ProcessHandler*[clientsCount];
        std::string* pipeNames = new std::string[clientsCount];

        for (int i = 0; i < clientsCount; ++i) {
            pipeServers[i] = NULL;
            serverHandlers[i] = NULL;
            serverThreads[i] = NULL;
            clientProcesses[i] = NULL;
        }

        RecordLockManager recordLockManager;

        for (int i = 0; i < clientsCount; ++i) {
            pipeNames[i] = "EmployeePipe_" + std::to_string(i + 1);
            pipeServers[i] = new PipeServer(pipeNames[i]);
            if (!pipeServers[i]->createPipe()) {
                ExceptionHandler::printError("Cannot create pipe " + pipeNames[i]);
                throw std::runtime_error("CreateNamedPipe failed");
            }
        }

        for (int i = 0; i < clientsCount; ++i) {
            serverHandlers[i] = new ServerHandler(&employeeStorage, pipeServers[i], &recordLockManager);
            serverThreads[i] = CreateThread(NULL, 0, ServerHandlerThread, serverHandlers[i], 0, NULL);
            if (serverThreads[i] == NULL) {
                DWORD errorCode = GetLastError();
                throw std::runtime_error("CreateThread failed, error: " + std::to_string(errorCode));
            }
        }

        for (int i = 0; i < clientsCount; ++i) {
            std::string clientCommand = "Client.exe " + pipeNames[i];
            clientProcesses[i] = new ProcessHandler(clientCommand);
        }

        DWORD waitResult = WaitForMultipleObjects(clientsCount, serverThreads, TRUE, INFINITE);
        if (waitResult == WAIT_FAILED) {
            DWORD errorCode = GetLastError();
            throw std::runtime_error("WaitForMultipleObjects failed, error: " + std::to_string(errorCode));
        }

        for (int i = 0; i < clientsCount; ++i) {
            if (clientProcesses[i] != NULL) clientProcesses[i]->waitForProcess();
        }

        for (int i = 0; i < clientsCount; ++i) {
            if (pipeServers[i] != NULL) pipeServers[i]->disconnect();
        }

        std::cout << "Final file content" << std::endl;
        employeeStorage.printAll();

        for (int i = 0; i < clientsCount; ++i) {
            if (serverThreads[i] != NULL) CloseHandle(serverThreads[i]);
            delete serverHandlers[i];
            delete pipeServers[i];
            delete clientProcesses[i];
        }

        delete[] pipeServers;
        delete[] serverHandlers;
        delete[] serverThreads;
        delete[] clientProcesses;
        delete[] pipeNames;

        std::cout << "Press any key to exit" << std::endl;
        system("pause");

        return 0;
    }
    catch (const std::exception& exception) {
        ExceptionHandler::printError("Server error: " + std::string(exception.what()));
        std::cout << "Press any key to exit" << std::endl;
        system("pause");
        return 1;
    }
    catch (...) {
        ExceptionHandler::printError("Server unknown error");
        std::cout << "Press any key to exit" << std::endl;
        system("pause");
        return 1;
    }
}
