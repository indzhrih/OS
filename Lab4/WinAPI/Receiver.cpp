#include <iostream>
#include <string>
#include <windows.h>
#include "Headers/Message.h"

int main(int argc, char* argv[]) {
    FILE* binaryFile;
    std::string binaryFileName;
    int recordsNumber;
    int sendersNumber;

    std::cout << "Enter binary file name: " << std::endl;
    std::cin >> binaryFileName;

    std::cout << "Enter number of records: " << std::endl;
    while (!(std::cin >> recordsNumber) || recordsNumber <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Wrong input! Enter positive integer: " << std::endl;
    }

    binaryFile = fopen(binaryFileName.c_str(), "wb");
    if (!binaryFile) {
        std::cerr << "Failed to create file" << std::endl;
        return 1;
    }
    fclose(binaryFile);

    std::cout << "Enter number of senders: " << std::endl;
    while (!(std::cin >> sendersNumber) || sendersNumber <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Wrong input! Enter positive integer: " << std::endl;
    }

    HANDLE readyEvent = CreateEvent(NULL, TRUE, FALSE, "ReadyEvent");
    if (!readyEvent) {
        std::cerr << "CreateEvent failed, err=" << GetLastError() << std::endl;
        return 1;
    }

    HANDLE messageSemaphore = CreateSemaphore(NULL, 0, recordsNumber, "MessageSemaphore");
    if (!messageSemaphore) {
        std::cerr << "CreateSemaphore failed, err=" << GetLastError() << std::endl;
        CloseHandle(readyEvent);
        return 1;
    }

    HANDLE binaryFileMutex = CreateMutex(NULL, FALSE, "BinaryFileMutex");
    if (!binaryFileMutex) {
        std::cerr << "CreateMutex failed, error: " << GetLastError() << std::endl;
        CloseHandle(readyEvent);
        CloseHandle(messageSemaphore);
        return 1;
    }

    for (int i = 0; i < sendersNumber; ++i) {
        STARTUPINFO siSender;
        PROCESS_INFORMATION piSender;
        ZeroMemory(&siSender, sizeof(STARTUPINFO));
        siSender.cb = sizeof(STARTUPINFO);

        std::string senderCmd = "Sender.exe " + binaryFileName;

        if (!CreateProcess(NULL, const_cast<LPSTR>(senderCmd.c_str()), NULL, NULL, FALSE,
                       CREATE_NEW_CONSOLE, NULL, NULL, &siSender, &piSender))
        {
            DWORD error = GetLastError();
            std::cerr << "CreateProcess failed! code=" << error << std::endl;
            CloseHandle(readyEvent);
            CloseHandle(messageSemaphore);
            CloseHandle(binaryFileMutex);
            return 1;
        }

        CloseHandle(piSender.hProcess);
        CloseHandle(piSender.hThread);
    }

    SetEvent(readyEvent);

    int command;

    long readOffset = 0;

    std::cin.ignore(10000, '\n');

    while (true) {
        std::cout << "Enter command:\n"
                  << "1. Read message\n"
                  << "2. Exit\n";

        if (!(std::cin >> command)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Wrong input!" << std::endl;
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (command) {
            case 1: {
                DWORD waitResult = WaitForSingleObject(messageSemaphore, 0);
                if (waitResult == WAIT_TIMEOUT) {
                    std::cout << "No messages yet." << std::endl;
                    break;
                }
                else if (waitResult != WAIT_OBJECT_0) {
                    std::cerr << "Wait failed, err=" << GetLastError() << std::endl;
                    break;
                }

                WaitForSingleObject(binaryFileMutex, INFINITE);

                binaryFile = fopen(binaryFileName.c_str(), "rb");
                if (!binaryFile) {
                    ReleaseMutex(binaryFileMutex);
                    std::cerr << "Failed to open input file" << std::endl;
                    break;
                }

                if (fseek(binaryFile, readOffset, SEEK_SET) != 0) {
                    fclose(binaryFile);
                    ReleaseMutex(binaryFileMutex);
                    std::cerr << "fseek failed" << std::endl;
                    break;
                }
                Message message;
                size_t read = fread(&message, sizeof(Message), 1, binaryFile);
                fclose(binaryFile);

                if (read != 1) {
                    ReleaseMutex(binaryFileMutex);
                    std::cerr << "fread failed" << std::endl;
                    break;
                }

                readOffset += (long)sizeof(Message);
                ReleaseMutex(binaryFileMutex);

                std::cout << "Message received: " << message.text << std::endl;
                break;
            }
            case 2:
                CloseHandle(readyEvent);
                CloseHandle(messageSemaphore);
                CloseHandle(binaryFileMutex);
                std::cout << "Receiver exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Wrong input!" << std::endl;
                break;
        }
    }
}
