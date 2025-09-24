#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <sstream>
#include <iostream>
#include "../Headers/Array.h"
#include "../Headers/ThreadHandler.h"

using Catch::Matchers::ContainsSubstring;

struct CoutRedirect {
    std::streambuf* old;
    std::ostringstream oss;
    CoutRedirect() : old(std::cout.rdbuf(oss.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
};

TEST_CASE("Main: Integration: min/max + average + switch + print") {
    Array a(5);
    a[0] = 10; a[1] = -1; a[2] = 6; a[3] = 2; a[4] = 8;

    ThreadHandler th;

    th.createThread("min_max", a);
    th.waitForThreads();

    th.createThread("average", a);
    th.waitForThreads();

    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    CoutRedirect out;
    a.printArray();

    std::ostringstream expected;
    expected << "Array: " << avg << " " << avg << " 6 2 8";

    auto printed = out.oss.str();
    REQUIRE_THAT(printed, ContainsSubstring("Array: "));

    auto pos = printed.find("Array: ");
    REQUIRE(pos != std::string::npos);
    auto tail = printed.substr(pos + 7);
    int countAvg = 0;
    std::istringstream iss(tail);
    int val;
    while (iss >> val) {
        if (val == avg) ++countAvg;
    }
    REQUIRE(countAvg == 2);
}
