#include "doctest/doctest.h"
#include "../Headers/MessageRingQueue.h"
#include <cstdio>
#include <string>

static void prepare_ring_file(const std::string& fname, int capacity) {
    FILE* f = std::fopen(fname.c_str(), "wb");
    int head = 0, tail = 0;
    std::fwrite(&capacity, sizeof(int), 1, f);
    std::fwrite(&head, sizeof(int), 1, f);
    std::fwrite(&tail, sizeof(int), 1, f);
    std::fclose(f);
}

TEST_CASE("MessageRingQueue ctor + add/read basic roundtrip") {
    const std::string file = "test_ring_basic.bin";
    prepare_ring_file(file, 3);

    MessageRingQueue q(file);
    Message in{}; in.text[0] = 'h'; in.text[1] = 'i';
    q.addMessage(in);

    Message out{};
    q.readMessage(out);

    CHECK(out.text[0] == 'h');
}

TEST_CASE("MessageRingQueue wrap-around works on small capacity") {
    const std::string file = "test_ring_wrap.bin";
    prepare_ring_file(file, 2);

    MessageRingQueue q(file);

    Message a{}; a.text[0] = 'A';
    Message b{}; b.text[0] = 'B';

    q.addMessage(a);
    q.addMessage(b);

    Message out{};
    q.readMessage(out);

    CHECK(out.text[0] == 'A');
}

TEST_CASE("MessageRingQueue second read returns second enqueued") {
    const std::string file = "test_ring_order.bin";
    prepare_ring_file(file, 2);

    MessageRingQueue q(file);

    Message a{}; a.text[0] = 'X';
    Message b{}; b.text[0] = 'Y';

    q.addMessage(a);
    q.addMessage(b);

    Message out1{}, out2{};
    q.readMessage(out1);
    q.readMessage(out2);

    CHECK(out2.text[0] == 'Y');
}
