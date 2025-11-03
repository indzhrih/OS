#include <iostream>
#include <string>
#include <windows.h>
#include "Headers/Message.h"
#include "Headers/ExceptionHandler.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Not enough args!";
        return 1;
    }

    int command;
    FILE* binaryFile;

    std::string binaryFileName = argv[1];

    try {
        HANDLE readyEvent = OpenEvent(SYNCHRONIZE, FALSE, "ReadyEvent");
        HANDLE messageSemaphore = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, "MessageSemaphore");
        HANDLE binaryFileMutex = OpenMutex(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, "BinaryFileMutex");
        HANDLE freeSlotsSemaphore = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, "FreeSlotsSemaphore");

        if (!ExceptionHandler::checkHandle(readyEvent, "OpenEvent(ReadyEvent)")) return 1;
        if (!ExceptionHandler::checkHandle(messageSemaphore, "OpenSemaphore(MessageSemaphore)")) return 1;
        if (!ExceptionHandler::checkHandle(binaryFileMutex, "OpenMutex(BinaryFileMutex)")) return 1;
        if (!ExceptionHandler::checkHandle(freeSlotsSemaphore, "OpenSemaphore(FreeSlotsSemaphore)")) return 1;

        std::cout << "Waiting for Receiver to be ready..." << std::endl;
        WaitForSingleObject(readyEvent, INFINITE);
        std::cout << "Receiver is ready!" << std::endl;

        while (true) {
            std::cout << "Enter command:\n"
                      << "1. Send\n"
                      << "2. Exit\n";

            if (!(std::cin >> command)) {
                ExceptionHandler::clearInput();
                std::cout << "Wrong input!" << std::endl;
                continue;
            }
            std::cin.ignore(10000, '\n');

            switch (command) {
                case 1: {
                    Message message;
                    std::cout << "Enter your message: " << std::endl;

                    std::cin.getline(message.text, sizeof(message.text));
                    if (std::cin.fail()) ExceptionHandler::clearInput();

                    DWORD waitFree = WaitForSingleObject(freeSlotsSemaphore, INFINITE);
                    if (!ExceptionHandler::checkWait(waitFree, "Wait FreeSlotsSemaphore")) break;

                    WaitForSingleObject(binaryFileMutex, INFINITE);

                    binaryFile = fopen(binaryFileName.c_str(), "ab");
                    if (!ExceptionHandler::checkFile(binaryFile, "open output file")) {
                        ReleaseMutex(binaryFileMutex);
                        ReleaseSemaphore(freeSlotsSemaphore, 1, NULL);
                        break;
                    }

                    fwrite(&message, sizeof(Message), 1, binaryFile);
                    fclose(binaryFile);

                    ReleaseMutex(binaryFileMutex);

                    BOOL rel = ReleaseSemaphore(messageSemaphore, 1, NULL);
                    ExceptionHandler::checkRelease(rel, "ReleaseSemaphore(MessageSemaphore)");

                    std::cout << "Message sent" << std::endl;
                    break;
                }
                case 2:
                    std::cout << "Exiting..." << std::endl;
                    return 0;
                default:
                    std::cout << "Wrong input!" << std::endl;
                    break;
            }
        }
    }
    catch (...) {
        std::cerr << "Sender unexpected error\n";
        return 1;
    }
}
