#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "TestFixtures.cpp"
#include "../Headers/Array.h"
#include "../Headers/ThreadHandler.h"

using Catch::Matchers::ContainsSubstring;

TEST_CASE("Main: Integration contains array header") {
    Array a(5);
    a[0] = 10;
    a[1] = -1;
    a[2] = 6;
    a[3] = 2;
    a[4] = 8;

    ThreadHandler th;
    th.createThread("min_max", a);
    th.waitForThreads();
    th.createThread("average", a);
    th.waitForThreads();

    CoutRedirect out;
    a.printArray();

    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Array: "));
}

TEST_CASE("Main: Integration switchMaxAndMinOnAverage works after threads") {
    Array a(5);
    a[0] = 10;
    a[1] = -1;
    a[2] = 6;
    a[3] = 2;
    a[4] = 8;

    ThreadHandler th;
    th.createThread("min_max", a);
    th.waitForThreads();
    th.createThread("average", a);
    th.waitForThreads();

    int avg = a.calculateAverageValue();
    a.switchMaxAndMinOnAverage();

    CoutRedirect out;
    a.printArray();

    std::string output = out.getOutput();
    size_t countAvg = 0;
    size_t pos = 0;
    while ((pos = output.find(std::to_string(avg), pos)) != std::string::npos) {
        ++countAvg;
        pos += std::to_string(avg).length();
    }

    REQUIRE(countAvg == 2);
}