#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <sstream>
#include <iostream>
#include "../Headers/ThreadHandler.h"
#include "../Headers/Array.h"

using Catch::Matchers::ContainsSubstring;

struct CoutRedirect {
    std::streambuf* old;
    std::ostringstream oss;
    CoutRedirect() : old(std::cout.rdbuf(oss.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
};
struct CerrRedirect {
    std::streambuf* old;
    std::ostringstream oss;
    CerrRedirect() : old(std::cerr.rdbuf(oss.rdbuf())) {}
    ~CerrRedirect() { std::cerr.rdbuf(old); }
};

TEST_CASE("ThreadHandler: createThread(\"min_max\") computes and prints min then max") {
    ThreadHandler th;
    Array a(3);
    a[0] = 8; a[1] = -2; a[2] = 3;

    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();

    auto s = out.oss.str();
    REQUIRE_THAT(s, ContainsSubstring("Min: -2"));
    REQUIRE_THAT(s, ContainsSubstring("Max: 8"));
    REQUIRE(s.find("Min:") < s.find("Max:"));
}

TEST_CASE("ThreadHandler: createThread(\"average\") computes and prints average") {
    ThreadHandler th;
    Array a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    CoutRedirect out;
    th.createThread("average", a);
    th.waitForThreads();

    REQUIRE_THAT(out.oss.str(), ContainsSubstring("Average value: 2"));
}

TEST_CASE("ThreadHandler: createThread with wrong name throws and logs to cerr") {
    ThreadHandler th;
    Array a(1);
    a[0] = 42;

    CerrRedirect err;
    REQUIRE_THROWS_AS(th.createThread("wrong", a), std::invalid_argument);
    REQUIRE_THAT(err.oss.str(), ContainsSubstring("Thread handler error: Error, wrong thread name"));
}

TEST_CASE("ThreadHandler: Thread handlers work with empty array") {
    ThreadHandler th;

    Array a(2);

    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();

    auto output = out.oss.str();
    REQUIRE_THAT(output, ContainsSubstring("Min:"));
    REQUIRE_THAT(output, ContainsSubstring("Max:"));
}

TEST_CASE("ThreadHandler: Thread handlers work correctly with valid array") {
    ThreadHandler th;
    Array a(5);
    a[0] = 10; a[1] = 20; a[2] = 30; a[3] = 40; a[4] = 50;

    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    th.createThread("average", a);
    th.waitForThreads();

    auto output = out.oss.str();
    REQUIRE_THAT(output, ContainsSubstring("Min: 10"));
    REQUIRE_THAT(output, ContainsSubstring("Max: 50"));
    REQUIRE_THAT(output, ContainsSubstring("Average value: 30"));
}
