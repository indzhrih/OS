#ifndef LAB4_WINAPI_MESSAGERINGQUEUE_H
#define LAB4_WINAPI_MESSAGERINGQUEUE_H

#include <string>
#include "Message.h"

class MessageRingQueue {
public:
    MessageRingQueue(const std::string& fileName);
    void addMessage(Message message);
    void readMessage(Message& message);
private:
    std::string binaryFileName;
};

#endif //LAB4_WINAPI_MESSAGERINGQUEUE_H
