#include <iostream>
#include <string>
#include <windows.h>
#include "Headers/Message.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Not enough args!";
        return 1;
    }

    int command;
    FILE* binaryFile;

    std::string binaryFileName = argv[1];

    try {
        HANDLE readyEvent = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, "ReadyEvent");
        HANDLE messageSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "MessageSemaphore");
        HANDLE binaryFileMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "BinaryFileMutex");

        if (!readyEvent || !messageSemaphore || !binaryFileMutex) {
            std::cerr << "Error opening synchronization objects" << std::endl;
            return 1;
        }

        std::cout << "Waiting for Receiver to be ready..." << std::endl;
        WaitForSingleObject(readyEvent, INFINITE);
        std::cout << "Receiver is ready!" << std::endl;

        while (true) {
            std::cout << "Enter command:\n"
                      << "1. Send\n"
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
                    Message message;
                    std::cout << "Enter your message: " << std::endl;

                    std::cin.getline(message.text, sizeof(message.text));

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(10000, '\n');
                    }

                    WaitForSingleObject(binaryFileMutex, INFINITE);

                    binaryFile = fopen(binaryFileName.c_str(), "ab");
                    if (!binaryFile) {
                        ReleaseMutex(binaryFileMutex);
                        std::cerr << "Failed to open output file\n";
                        break;
                    }

                    fwrite(&message, sizeof(Message), 1, binaryFile);
                    fclose(binaryFile);

                    ReleaseMutex(binaryFileMutex);

                    if (!ReleaseSemaphore(messageSemaphore, 1, NULL)) {
                        std::cerr << "ReleaseSemaphore failed, err=" << GetLastError() << std::endl;
                    }

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
    catch (const std::exception& ex) {
        std::cerr << "Sender error: " << ex.what() << "\n";
        std::cerr << "Press any key to close this window...";
        system("pause");
        return 1;
    }
}
