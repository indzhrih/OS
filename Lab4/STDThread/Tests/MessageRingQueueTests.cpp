#include "doctest/doctest.h"
#include "../Headers/MessageRingQueue.h"
#include <cstdio>
#include <string>

static void prepareRing(const std::string& name, int capacity) {
    FILE* f = std::fopen(name.c_str(), "wb");
    int head = 0, tail = 0;
    std::fwrite(&capacity, sizeof(int), 1, f);
    std::fwrite(&head, sizeof(int), 1, f);
    std::fwrite(&tail, sizeof(int), 1, f);
    std::fclose(f);
}

TEST_CASE("MessageRingQueue basic roundtrip") {
    std::string file = "ring_basic.bin";
    prepareRing(file, 3);
    MessageRingQueue q(file);
    Message in; in.text[0] = 'h'; in.text[1] = 'i';
    q.addMessage(in);
    Message out;
    q.readMessage(out);
    CHECK(out.text[0] == 'h');
}

TEST_CASE("MessageRingQueue wrap-around") {
    std::string file = "ring_wrap.bin";
    prepareRing(file, 2);
    MessageRingQueue q(file);
    Message a; a.text[0] = 'A';
    Message b; b.text[0] = 'B';
    q.addMessage(a);
    q.addMessage(b);
    Message out;
    q.readMessage(out);
    CHECK(out.text[0] == 'A');
}

TEST_CASE("MessageRingQueue order preserved") {
    std::string file = "ring_order.bin";
    prepareRing(file, 2);
    MessageRingQueue q(file);
    Message a; a.text[0] = 'X';
    Message b; b.text[0] = 'Y';
    q.addMessage(a);
    q.addMessage(b);
    Message o1, o2;
    q.readMessage(o1);
    q.readMessage(o2);
    CHECK(o2.text[0] == 'Y');
}
