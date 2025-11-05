#include "../Headers/SharedState.h"
#include "../Headers/ExceptionHandler.h"
#include <cstdio>
#include <iostream>

SharedState::SharedState(std::string fileNameValue)
    : fileName(std::move(fileNameValue)), stopRequested(false) {}

void SharedState::runInputLoop() {
    while (true) {
        int command = 0;
        std::cout << "Enter command:\n1. Send\n2. Exit\n";
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

        Message message;
        std::cout << "Enter your message:\n";
        std::cin.getline(message.text, sizeof(message.text));
        if (std::cin.fail()) {
            ExceptionHandler::clearInput();
            message.text[0] = '\0';
        }
        enqueue(message);
    }
}

void SharedState::runWriterLoop() {
    while (true) {
        Message message;
        if (!tryDequeue(message)) break;
        appendToFile(message);
        std::cout << "Message sent\n";
    }
}

void SharedState::enqueue(const Message& message) {
    std::lock_guard<std::mutex> lock(queueMutex);
    queue.push_back(message);
    queueCondVar.notify_one();
}

bool SharedState::tryDequeue(Message& outMessage) {
    std::unique_lock<std::mutex> lock(queueMutex);
    queueCondVar.wait(lock, [&] { return !queue.empty() || stopRequested; });
    if (queue.empty() && stopRequested) return false;
    outMessage = queue.front();
    queue.pop_front();
    return true;
}

void SharedState::requestStop() {
    std::lock_guard<std::mutex> lock(queueMutex);
    stopRequested = true;
    queueCondVar.notify_all();
}

bool SharedState::isStopRequested() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(queueMutex));
    return stopRequested;
}

void SharedState::appendToFile(const Message& message) {
    std::lock_guard<std::mutex> lock(fileMutex);
    FILE* file = std::fopen(fileName.c_str(), "ab");
    if (!ExceptionHandler::checkFile(file, "open output file")) return;
    std::fwrite(&message, sizeof(Message), 1, file);
    std::fclose(file);
}
