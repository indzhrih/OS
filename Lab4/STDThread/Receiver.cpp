#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "Headers/Message.h"
#include "Headers/ExceptionHandler.h"

int main() {
    std::string fileName;
    int capacity = 0;
    int senderCount = 0;

    std::cout << "Enter binary file name:\n";
    std::cin >> fileName;

    std::cout << "Enter number of records:\n";
    while (!(std::cin >> capacity) || capacity <= 0) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter positive integer:\n";
    }

    FILE* initFile = std::fopen(fileName.c_str(), "wb");
    if (!ExceptionHandler::checkFile(initFile, "create file")) return 1;
    std::fclose(initFile);

    std::cout << "Enter number of senders:\n";
    while (!(std::cin >> senderCount) || senderCount <= 0) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter positive integer:\n";
    }
    std::cin.ignore(10000, '\n');

    for (int i = 0; i < senderCount; ++i) {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);

        std::string command = "Sender.exe \"" + fileName + "\"";
        std::vector<char> cmd(command.begin(), command.end());
        cmd.push_back('\0');

        BOOL ok = CreateProcessA(NULL, cmd.data(), NULL, NULL,
                                 FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

        if (!ok) std::cerr << "CreateProcess failed\n";
        if (ok) {
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
    }

    long readOffset = 0;

    while (true) {
        int command = 0;
        std::cout << "Enter command:\n1. Read message\n2. Exit\n";
        if (!(std::cin >> command)) {
            ExceptionHandler::clearInput();
            std::cout << "Wrong input!\n";
            continue;
        }
        std::cin.ignore(10000, '\n');

        if (command == 2) break;
        if (command != 1) {
            std::cout << "Wrong input!\n";
            continue;
        }

        FILE* file = std::fopen(fileName.c_str(), "rb");
        if (!ExceptionHandler::checkFile(file, "open input file")) continue;

        if (std::fseek(file, readOffset, SEEK_SET) != 0) {
            std::fclose(file);
            std::cerr << "fseek failed\n";
            continue;
        }

        Message message;
        size_t rc = std::fread(&message, sizeof(Message), 1, file);
        std::fclose(file);

        if (rc != 1) {
            std::cout << "No messages yet.\n";
            continue;
        }

        readOffset += static_cast<long>(sizeof(Message));
        std::cout << "Message received: " << message.text << "\n";
    }

    return 0;
}
