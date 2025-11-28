#include <iostream>
#include <string>
#include <windows.h>
#include "Headers/PipeClient.h"
#include "Headers/ClientHandler.h"
#include "Headers/ExceptionHandler.h"

int main(int argc, char* argv[]) {
    try {
        std::string pipeName = "EmployeePipe_1";
        if (argc > 1) pipeName = argv[1];

        PipeClient pipeClient(pipeName);
        bool connected = pipeClient.connectToServer();
        if (!connected) {
            ExceptionHandler::printError("Failed to connect to server");
            std::cout << "Press any key to exit" << std::endl;
            system("pause");
            return 1;
        }

        ClientHandler clientHandler(&pipeClient);
        clientHandler.run();

        std::cout << "Client finished work" << std::endl;
        std::cout << "Press any key to exit" << std::endl;
        system("pause");

        return 0;
    }
    catch (const std::exception& exception) {
        ExceptionHandler::printError("Client error: " + std::string(exception.what()));
        std::cout << "Press any key to exit" << std::endl;
        system("pause");
        return 1;
    }
    catch (...) {
        ExceptionHandler::printError("Client unknown error");
        std::cout << "Press any key to exit" << std::endl;
        system("pause");
        return 1;
    }
}
