#ifndef LAB4_WINAPI_MESSAGE_H
#define LAB4_WINAPI_MESSAGE_H

struct Message {
    Message(char text[20]);
    Message();

    char text[20];
};

#endif //LAB4_WINAPI_MESSAGE_H
