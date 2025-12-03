#include <iostream>
#include <string>
#include <limits>
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
            std::cout << "Press Enter to exit" << std::endl;
            std::cin.get();
            return 1;
        }

        ClientHandler clientHandler(&pipeClient);
        clientHandler.run();

        std::cout << "Client finished work" << std::endl;
        std::cout << "Press Enter to exit" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        return 0;
    } catch (const std::exception& exception) {
        ExceptionHandler::printError("Client error: " + std::string(exception.what()));
        std::cout << "Press Enter to exit" << std::endl;
        std::cin.get();
        return 1;
    } catch (...) {
        ExceptionHandler::printError("Client unknown error");
        std::cout << "Press Enter to exit" << std::endl;
        std::cin.get();
        return 1;
    }
}
