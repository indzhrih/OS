#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "TestFixtures.cpp"
#include "../Headers/ThreadHandler.h"
#include "../Headers/Array.h"

using Catch::Matchers::ContainsSubstring;

TEST_CASE("ThreadHandler: createThread(\"min_max\") prints min") {
    ThreadHandler th;
    Array a(3);
    a[0] = 8;
    a[1] = -2;
    a[2] = 3;
    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Min: -2"));
}

TEST_CASE("ThreadHandler: createThread(\"min_max\") prints max") {
    ThreadHandler th;
    Array a(3);
    a[0] = 8;
    a[1] = -2;
    a[2] = 3;
    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Max: 8"));
}

TEST_CASE("ThreadHandler: createThread(\"min_max\") prints min before max") {
    ThreadHandler th;
    Array a(3);
    a[0] = 8;
    a[1] = -2;
    a[2] = 3;
    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    auto s = out.getOutput();
    REQUIRE(s.find("Min:") < s.find("Max:"));
}

TEST_CASE("ThreadHandler: createThread(\"average\") computes and prints average") {
    ThreadHandler th;
    Array a(3);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    CoutRedirect out;
    th.createThread("average", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Average value: 2"));
}

TEST_CASE("ThreadHandler: createThread with wrong name throws") {
    ThreadHandler th;
    Array a(1);
    a[0] = 42;
    REQUIRE_THROWS_AS(th.createThread("wrong", a), std::invalid_argument);
}

TEST_CASE("ThreadHandler: createThread with wrong name logs to cerr") {
    ThreadHandler th;
    Array a(1);
    a[0] = 42;
    CerrRedirect err;
    try {
        th.createThread("wrong", a);
    } catch (const std::invalid_argument&) {}
    REQUIRE_THAT(err.getOutput(), ContainsSubstring("Thread handler error: Error, wrong thread name"));
}

TEST_CASE("ThreadHandler: min_max thread prints Min with empty array") {
    ThreadHandler th;
    Array a(2);
    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Min:"));
}

TEST_CASE("ThreadHandler: min_max thread prints Max with empty array") {
    ThreadHandler th;
    Array a(2);
    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Max:"));
}

TEST_CASE("ThreadHandler: min_max thread prints correct min") {
    ThreadHandler th;
    Array a(5);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    a[3] = 40;
    a[4] = 50;
    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Min: 10"));
}

TEST_CASE("ThreadHandler: min_max thread prints correct max") {
    ThreadHandler th;
    Array a(5);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    a[3] = 40;
    a[4] = 50;
    CoutRedirect out;
    th.createThread("min_max", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Max: 50"));
}

TEST_CASE("ThreadHandler: average thread prints correct average") {
    ThreadHandler th;
    Array a(5);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    a[3] = 40;
    a[4] = 50;
    CoutRedirect out;
    th.createThread("average", a);
    th.waitForThreads();
    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Average value: 30"));
}
