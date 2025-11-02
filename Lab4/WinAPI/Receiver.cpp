#include <iostream>
#include <string>
#include <windows.h>

int main(int argc, char* argv[]) {
    FILE* binaryFile;
    std::string binaryFileName;
    int recordsNumber;
    int sendersNumber;

    std::cout << "Enter binary file name: " << std::endl;
    std::cin >> binaryFileName;
    std::cout << "Enter number of records: " << std::endl;
    std::cin >> recordsNumber;

    binaryFile = fopen(binaryFileName.c_str(), "wb");
    if (binaryFile) {
        fclose(binaryFile);
    }

    std::cout << "Enter number of senders: " << std::endl;
    std::cin >> sendersNumber;

    HANDLE readyEvent = CreateEvent(NULL, FALSE, FALSE, "ReadyEvent");
    HANDLE messageSemaphore = CreateSemaphore(NULL, 0, 1, "MessageSemaphore");
    HANDLE binaryFileMutex = CreateMutex(NULL, FALSE, "BinaryFileMutex");

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
        return 1;
    }

    CloseHandle(piSender.hProcess);
    CloseHandle(piSender.hThread);

    SetEvent(readyEvent);

    int command;

    while (true) {
        std::cout << "Enter command:\n"
                  << "1. Read message\n"
                  << "2. Exit\n";
        std::cin >> command;
        std::cin.ignore();

        switch (command) {
            case 1: {
                std::cout << "Waiting for message..." << std::endl;
                WaitForSingleObject(messageSemaphore, INFINITE);

                WaitForSingleObject(binaryFileMutex, INFINITE);

                binaryFile = fopen(binaryFileName.c_str(), "rb");
                if (binaryFile) {
                    Message message;
                    fread(&message, sizeof(Message), 1, binaryFile);
                    fclose(binaryFile);
                    std::cout << "Message received: " << message.text << std::endl;
                }

                ReleaseMutex(binaryFileMutex);
                break;
            }
            case 2:
                CloseHandle(readyEvent);
                CloseHandle(messageSemaphore);
                CloseHandle(binaryFileMutex);
                std::cout << "Receiver exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Wrong command!" << std::endl;
                std::cin.ignore(10000, '\n');
                break;
        }
    }
}