#include "../Headers/Message.h"

Message::Message(char text_[20]) {
    for (int i = 0; i < 20; ++i) {
        text[i] = text_[i];
    }
}

Message::Message() {
    for (int i = 0; i < 20; ++i) {
        text[i] = 0;
    }
}
