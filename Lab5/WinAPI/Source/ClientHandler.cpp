#include "../Headers/ClientHandler.h"
#include "../Headers/ExceptionHandler.h"
#include <iostream>
#include <cstring>
#include <string>

ClientHandler::ClientHandler(PipeClient* pipeClientValue) : pipeClient(pipeClientValue) {}

int ClientHandler::readEmployeeIdFromConsole() {
    int employeeId;
    std::cout << "Enter employee id" << std::endl;
    while (!(std::cin >> employeeId)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter integer employee id" << std::endl;
    }
    return employeeId;
}

void ClientHandler::handleModifyCommand() {
    int employeeId = readEmployeeIdFromConsole();

    PipeRequest request;
    request.type = PIPE_REQUEST_BEGIN_MODIFY;
    request.employeeId = employeeId;

    if (!pipeClient->sendRequest(request)) {
        ExceptionHandler::printError("Failed to send modify request to server");
        return;
    }

    PipeResponse response;
    if (!pipeClient->receiveResponse(response)) {
        ExceptionHandler::printError("Failed to receive modify response from server");
        return;
    }

    if (response.status == PIPE_RESPONSE_NOT_FOUND) {
        std::cout << "Employee not found" << std::endl;
        return;
    }
    if (response.status == PIPE_RESPONSE_INVALID_STATE) {
        std::cout << "Server is busy with another employee" << std::endl;
        return;
    }
    if (response.status != PIPE_RESPONSE_OK) {
        std::cout << "Server returned error" << std::endl;
        return;
    }

    std::cout << "Current record:" << std::endl;
    response.employeeData.printInfo();

    employee updatedEmployee = response.employeeData;

    std::cout << "Enter new number" << std::endl;
    while (!(std::cin >> updatedEmployee.num)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter integer number" << std::endl;
    }

    std::string newName;
    std::cout << "Enter new name" << std::endl;
    while (!(std::cin >> newName)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter name" << std::endl;
    }

    if (newName.size() > 9) newName = newName.substr(0, 9);
    std::memset(updatedEmployee.name, 0, sizeof(updatedEmployee.name));
    std::memcpy(updatedEmployee.name, newName.c_str(), newName.size());

    std::cout << "Enter new hours" << std::endl;
    while (!(std::cin >> updatedEmployee.hours)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter hours" << std::endl;
    }

    std::cout << "Enter 1 to send changes, 2 to cancel" << std::endl;
    int command;
    while (!(std::cin >> command) || (command != 1 && command != 2)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter 1 or 2" << std::endl;
    }

    if (command == 1) {
        PipeRequest commitRequest;
        commitRequest.type = PIPE_REQUEST_COMMIT_MODIFY;
        commitRequest.employeeId = employeeId;
        commitRequest.employeeData = updatedEmployee;

        if (!pipeClient->sendRequest(commitRequest)) {
            ExceptionHandler::printError("Failed to send commit request");
        }
        else {
            PipeResponse commitResponse;
            if (!pipeClient->receiveResponse(commitResponse)) {
                ExceptionHandler::printError("Failed to receive commit response");
            }
            else if (commitResponse.status != PIPE_RESPONSE_OK) std::cout << "Commit failed" << std::endl;
            else std::cout << "Record updated" << std::endl;
        }
    }
    else std::cout << "Changes were not sent" << std::endl;

    std::cout << "Enter any number to finish access to this employee" << std::endl;
    int finishAccessCommand;
    while (!(std::cin >> finishAccessCommand)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter integer" << std::endl;
    }

    PipeRequest endRequest;
    endRequest.type = PIPE_REQUEST_END_ACCESS;
    endRequest.employeeId = employeeId;

    if (!pipeClient->sendRequest(endRequest)) {
        ExceptionHandler::printError("Failed to send end access request");
        return;
    }

    PipeResponse endResponse;
    if (!pipeClient->receiveResponse(endResponse)) {
        ExceptionHandler::printError("Failed to receive end access response");
        return;
    }

    if (endResponse.status == PIPE_RESPONSE_OK) std::cout << "Access to employee closed" << std::endl;
    else std::cout << "Failed to close access to employee" << std::endl;
}

void ClientHandler::handleReadCommand() {
    int employeeId = readEmployeeIdFromConsole();

    PipeRequest request;
    request.type = PIPE_REQUEST_READ;
    request.employeeId = employeeId;

    if (!pipeClient->sendRequest(request)) {
        ExceptionHandler::printError("Failed to send read request to server");
        return;
    }

    PipeResponse response;
    if (!pipeClient->receiveResponse(response)) {
        ExceptionHandler::printError("Failed to receive read response from server");
        return;
    }

    if (response.status == PIPE_RESPONSE_NOT_FOUND) {
        std::cout << "Employee not found" << std::endl;
        return;
    }
    if (response.status == PIPE_RESPONSE_INVALID_STATE) {
        std::cout << "Server is busy with another employee" << std::endl;
        return;
    }
    if (response.status != PIPE_RESPONSE_OK) {
        std::cout << "Server returned error" << std::endl;
        return;
    }

    std::cout << "Employee record:" << std::endl;
    response.employeeData.printInfo();

    std::cout << "Enter any number to finish access to this employee" << std::endl;
    int finishAccessCommand;
    while (!(std::cin >> finishAccessCommand)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter integer" << std::endl;
    }

    PipeRequest endRequest;
    endRequest.type = PIPE_REQUEST_END_ACCESS;
    endRequest.employeeId = employeeId;

    if (!pipeClient->sendRequest(endRequest)) {
        ExceptionHandler::printError("Failed to send end access request");
        return;
    }

    PipeResponse endResponse;
    if (!pipeClient->receiveResponse(endResponse)) {
        ExceptionHandler::printError("Failed to receive end access response");
        return;
    }

    if (endResponse.status == PIPE_RESPONSE_OK) std::cout << "Access to employee closed" << std::endl;
    else std::cout << "Failed to close access to employee" << std::endl;
}

void ClientHandler::handleShutdownCommand() {
    PipeRequest request;
    request.type = PIPE_REQUEST_SHUTDOWN;
    request.employeeId = 0;

    if (!pipeClient->sendRequest(request)) {
        ExceptionHandler::printError("Failed to send shutdown request");
        return;
    }

    PipeResponse response;
    pipeClient->receiveResponse(response);
}

void ClientHandler::run() {
    bool running = true;

    while (running) {
        std::cout << "Enter command:" << std::endl;
        std::cout << "1. Modify employee" << std::endl;
        std::cout << "2. Read employee" << std::endl;
        std::cout << "3. Exit" << std::endl;

        int command;
        if (!(std::cin >> command)) {
            ExceptionHandler::clearInput();
            std::cout << "Wrong input!" << std::endl;
            continue;
        }

        if (command == 1) handleModifyCommand();
        else if (command == 2) handleReadCommand();
        else if (command == 3) {
            handleShutdownCommand();
            running = false;
        }
        else std::cout << "Wrong command" << std::endl;
    }
}
