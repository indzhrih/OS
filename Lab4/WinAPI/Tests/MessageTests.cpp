#include "doctest/doctest.h"
#include "../Headers/Message.h"

TEST_CASE("Message default ctor sets first byte to zero") {
    Message m;
    CHECK(m.text[0] == 0);
}

TEST_CASE("Message default ctor sets last byte to zero") {
    Message m;
    CHECK(m.text[19] == 0);
}

TEST_CASE("Message(char[20]) copies first char") {
    char buf[20] = {};
    buf[0] = 'A';
    Message m(buf);
    CHECK(m.text[0] == 'A');
}

TEST_CASE("Message(char[20]) copies 20th char") {
    char buf[20] = {};
    buf[19] = 'Z';
    Message m(buf);
    CHECK(m.text[19] == 'Z');
}
