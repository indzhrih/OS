#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <thread>
#include <memory>
#include <limits>
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

        std::vector<std::unique_ptr<PipeServer>> pipeServers;
        std::vector<std::unique_ptr<ServerHandler>> serverHandlers;
        std::vector<std::thread> serverThreads;
        std::vector<std::unique_ptr<ProcessHandler>> clientProcesses;
        std::vector<std::string> pipeNames;

        pipeServers.resize(clientsCount);
        serverHandlers.resize(clientsCount);
        clientProcesses.resize(clientsCount);
        pipeNames.resize(clientsCount);
        serverThreads.reserve(clientsCount);

        RecordLockManager recordLockManager;

        for (int i = 0; i < clientsCount; ++i) {
            pipeNames[i] = "EmployeePipe_" + std::to_string(i + 1);
            pipeServers[i] = std::make_unique<PipeServer>(pipeNames[i]);
            if (!pipeServers[i]->createPipe()) {
                ExceptionHandler::printError("Cannot create pipe " + pipeNames[i]);
                throw std::runtime_error("CreatePipe failed");
            }
        }

        for (int i = 0; i < clientsCount; ++i) {
            serverHandlers[i] = std::make_unique<ServerHandler>(&employeeStorage, pipeServers[i].get(), &recordLockManager);
            ServerHandler* handlerPtr = serverHandlers[i].get();
            serverThreads.emplace_back([handlerPtr]() { handlerPtr->run(); });
        }

        for (int i = 0; i < clientsCount; ++i) {
            std::string clientCommand = "Client.exe " + pipeNames[i];
            clientProcesses[i] = std::make_unique<ProcessHandler>(clientCommand);
        }

        for (auto& thread : serverThreads) thread.join();

        for (int i = 0; i < clientsCount; ++i) {
            if (clientProcesses[i]) clientProcesses[i]->waitForProcess();
        }

        for (int i = 0; i < clientsCount; ++i) {
            if (pipeServers[i]) pipeServers[i]->disconnect();
        }

        std::cout << "Final file content" << std::endl;
        employeeStorage.printAll();

        std::cout << "Press Enter to exit" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        return 0;
    } catch (const std::exception& exception) {
        ExceptionHandler::printError("Server error: " + std::string(exception.what()));
        std::cout << "Press Enter to exit" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    } catch (...) {
        ExceptionHandler::printError("Server unknown error");
        std::cout << "Press Enter to exit" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    }
}
