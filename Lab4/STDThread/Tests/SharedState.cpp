#include "doctest/doctest.h"
#include "../Headers/SharedState.h"
#include <cstdio>

TEST_CASE("SharedState enqueue then tryDequeue returns true") {
    SharedState s("ss_queue.bin");
    Message m;
    m.text[0] = 'A';
    s.enqueue(m);
    Message out;
    bool ok = s.tryDequeue(out);
    CHECK(ok == true);
}

TEST_CASE("SharedState tryDequeue returns false when stopped and empty") {
    SharedState s("ss_empty.bin");
    s.requestStop();
    Message out;
    bool ok = s.tryDequeue(out);
    CHECK(ok == false);
}

TEST_CASE("SharedState appendToFile writes record") {
    SharedState s("ss_file.bin");
    Message m;
    m.text[0] = 'Q';
    s.appendToFile(m);
    FILE* f = std::fopen("ss_file.bin", "rb");
    Message r;
    size_t rc = f ? std::fread(&r, sizeof(Message), 1, f) : 0;
    if (f) std::fclose(f);
    CHECK(rc == 1);
}
