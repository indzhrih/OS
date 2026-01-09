#include <string>
#include <windows.h>
#include "../Headers/MessageRingQueue.h"
#include "../Headers/Message.h"

static long headerSizeBytes() {
    return (long)(sizeof(int) * 3);
}

static long slotOffsetBytes(int index) {
    return headerSizeBytes() + (long)index * (long)sizeof(Message);
}

MessageRingQueue::MessageRingQueue(const std::string& fileName) : binaryFileName(fileName) {}

void MessageRingQueue::addMessage(Message message) {
    FILE* f = fopen(binaryFileName.c_str(), "rb+");
    if (!f) return;

    int capacity = 0, head = 0, tail = 0;
    fread(&capacity, sizeof(int), 1, f);
    fread(&head, sizeof(int), 1, f);
    fread(&tail, sizeof(int), 1, f);

    long writePos = slotOffsetBytes(tail);
    fseek(f, writePos, SEEK_SET);
    fwrite(&message, sizeof(Message), 1, f);

    tail = (tail + 1) % capacity;

    fseek(f, sizeof(int) * 2, SEEK_SET);
    fwrite(&tail, sizeof(int), 1, f);

    fclose(f);
}

void MessageRingQueue::readMessage(Message& message) {
    FILE* f = fopen(binaryFileName.c_str(), "rb+");
    if (!f) return;

    int capacity = 0, head = 0, tail = 0;
    fread(&capacity, sizeof(int), 1, f);
    fread(&head, sizeof(int), 1, f);
    fread(&tail, sizeof(int), 1, f);

    long readPos = slotOffsetBytes(head);
    fseek(f, readPos, SEEK_SET);
    fread(&message, sizeof(Message), 1, f);

    head = (head + 1) % capacity;

    fseek(f, sizeof(int), SEEK_SET);
    fwrite(&head, sizeof(int), 1, f);

    fclose(f);
}
