#include "../Headers/ClientHandler.h"
#include "../Headers/ExceptionHandler.h"
#include <iostream>
#include <limits>
#include <cstring>
#include <string>

ClientHandler::ClientHandler(PipeClient* pipeClientValue) : pipeClient(pipeClientValue) {
}

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

    bool sent = pipeClient->sendRequest(request);
    if (!sent) {
        std::cout << "Failed to send request to server" << std::endl;
        return;
    }

    PipeResponse response;
    bool received = pipeClient->receiveResponse(response);
    if (!received) {
        std::cout << "Failed to receive response from server" << std::endl;
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
    std::cout << "num = " << response.employeeData.num
              << ", name = " << response.employeeData.name
              << ", hours = " << response.employeeData.hours << std::endl;

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

        bool commitSent = pipeClient->sendRequest(commitRequest);
        if (!commitSent) std::cout << "Failed to send commit request" << std::endl;
        else {
            PipeResponse commitResponse;
            bool commitReceived = pipeClient->receiveResponse(commitResponse);
            if (!commitReceived) std::cout << "Failed to receive commit response" << std::endl;
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

    bool endSent = pipeClient->sendRequest(endRequest);
    if (!endSent) {
        std::cout << "Failed to send end access request" << std::endl;
        return;
    }

    PipeResponse endResponse;
    bool endReceived = pipeClient->receiveResponse(endResponse);
    if (!endReceived) {
        std::cout << "Failed to receive end access response" << std::endl;
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

    bool sent = pipeClient->sendRequest(request);
    if (!sent) {
        std::cout << "Failed to send request to server" << std::endl;
        return;
    }

    PipeResponse response;
    bool received = pipeClient->receiveResponse(response);
    if (!received) {
        std::cout << "Failed to receive response from server" << std::endl;
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
    std::cout << "num = " << response.employeeData.num
              << ", name = " << response.employeeData.name
              << ", hours = " << response.employeeData.hours << std::endl;

    std::cout << "Enter any number to finish access to this employee" << std::endl;
    int finishAccessCommand;
    while (!(std::cin >> finishAccessCommand)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter integer" << std::endl;
    }

    PipeRequest endRequest;
    endRequest.type = PIPE_REQUEST_END_ACCESS;
    endRequest.employeeId = employeeId;

    bool endSent = pipeClient->sendRequest(endRequest);
    if (!endSent) {
        std::cout << "Failed to send end access request" << std::endl;
        return;
    }

    PipeResponse endResponse;
    bool endReceived = pipeClient->receiveResponse(endResponse);
    if (!endReceived) {
        std::cout << "Failed to receive end access response" << std::endl;
        return;
    }

    if (endResponse.status == PIPE_RESPONSE_OK) std::cout << "Access to employee closed" << std::endl;
    else std::cout << "Failed to close access to employee" << std::endl;
}

void ClientHandler::handleShutdownCommand() {
    PipeRequest request;
    request.type = PIPE_REQUEST_SHUTDOWN;
    request.employeeId = 0;

    bool sent = pipeClient->sendRequest(request);
    if (!sent) {
        std::cout << "Failed to send shutdown request" << std::endl;
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
