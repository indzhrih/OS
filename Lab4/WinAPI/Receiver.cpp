#include <iostream>
#include <string>
#include <windows.h>
#include "Headers/Message.h"
#include "Headers/ExceptionHandler.h"

int main(int argc, char* argv[]) {
    FILE* binaryFile;
    std::string binaryFileName;
    int recordsNumber;
    int sendersNumber;

    std::cout << "Enter binary file name: " << std::endl;
    std::cin >> binaryFileName;

    std::cout << "Enter number of records: " << std::endl;
    while (!(std::cin >> recordsNumber) || recordsNumber <= 0) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter positive integer: " << std::endl;
    }

    binaryFile = fopen(binaryFileName.c_str(), "wb");
    if (!ExceptionHandler::checkFile(binaryFile, "create file")) return 1;
    fclose(binaryFile);

    std::cout << "Enter number of senders: " << std::endl;
    while (!(std::cin >> sendersNumber) || sendersNumber <= 0) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter positive integer: " << std::endl;
    }

    HANDLE readyEvent = CreateEvent(NULL, TRUE, FALSE, "ReadyEvent");
    if (!ExceptionHandler::checkHandle(readyEvent, "CreateEvent(ReadyEvent)")) return 1;

    HANDLE messageSemaphore = CreateSemaphore(NULL, 0, recordsNumber, "MessageSemaphore");
    if (!ExceptionHandler::checkHandle(messageSemaphore, "CreateSemaphore(MessageSemaphore)")) {
        CloseHandle(readyEvent);
        return 1;
    }

    HANDLE binaryFileMutex = CreateMutex(NULL, FALSE, "BinaryFileMutex");
    if (!ExceptionHandler::checkHandle(binaryFileMutex, "CreateMutex(BinaryFileMutex)")) {
        CloseHandle(readyEvent);
        CloseHandle(messageSemaphore);
        return 1;
    }

    HANDLE freeSlotsSemaphore = CreateSemaphore(NULL, recordsNumber, recordsNumber, "FreeSlotsSemaphore");
    if (!ExceptionHandler::checkHandle(freeSlotsSemaphore, "CreateSemaphore(FreeSlotsSemaphore)")) {
        CloseHandle(readyEvent);
        CloseHandle(messageSemaphore);
        CloseHandle(binaryFileMutex);
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
            CloseHandle(freeSlotsSemaphore);
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
            ExceptionHandler::clearInput();
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
                else if (!ExceptionHandler::checkWait(waitResult, "Wait MessageSemaphore")) break;

                WaitForSingleObject(binaryFileMutex, INFINITE);

                binaryFile = fopen(binaryFileName.c_str(), "rb");
                if (!ExceptionHandler::checkFile(binaryFile, "open input file")) {
                    ReleaseMutex(binaryFileMutex);
                    ReleaseSemaphore(messageSemaphore, 1, NULL);
                    break;
                }

                if (fseek(binaryFile, readOffset, SEEK_SET) != 0) {
                    fclose(binaryFile);
                    ReleaseMutex(binaryFileMutex);
                    std::cerr << "fseek failed" << std::endl;
                    ReleaseSemaphore(messageSemaphore, 1, NULL);
                    break;
                }

                Message message;
                size_t readCount = fread(&message, sizeof(Message), 1, binaryFile);
                fclose(binaryFile);

                if (readCount != 1) {
                    ReleaseMutex(binaryFileMutex);
                    std::cerr << "fread failed" << std::endl;
                    ReleaseSemaphore(messageSemaphore, 1, NULL);
                    break;
                }

                readOffset += (long)sizeof(Message);
                ReleaseMutex(binaryFileMutex);

                ReleaseSemaphore(freeSlotsSemaphore, 1, NULL);

                std::cout << "Message received: " << message.text << std::endl;
                break;
            }
            case 2:
                CloseHandle(readyEvent);
                CloseHandle(messageSemaphore);
                CloseHandle(binaryFileMutex);
                CloseHandle(freeSlotsSemaphore);
                std::cout << "Receiver exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Wrong input!" << std::endl;
                break;
        }
    }
}
