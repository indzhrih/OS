#ifndef LAB4_WINAPI_SHAREDSTATE_H
#define LAB4_WINAPI_SHAREDSTATE_H

#include <string>
#include <mutex>
#include <condition_variable>
#include <deque>
#include "Message.h"

class SharedState {
public:
    explicit SharedState(std::string fileName);

    void runInputLoop();
    void runWriterLoop();

    void enqueue(const Message& message);
    bool tryDequeue(Message& outMessage);

    void requestStop();
    bool isStopRequested() const;

    void appendToFile(const Message& message);

private:
    std::string fileName;

    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    std::deque<Message> queue;
    bool stopRequested;

    std::mutex fileMutex;
};
#endif //LAB4_WINAPI_SHAREDSTATE_H
