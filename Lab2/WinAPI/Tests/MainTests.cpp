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

TEST_CASE("Array methods work correctly") {
    Array a(5);
    a[0] = 5; a[1] = -1; a[2] = 12; a[3] = 2; a[4] = 8;

    int min = a.findMinElement();
    int max = a.findMaxElement();
    int avg = a.calculateAverageValue();

    REQUIRE(min == -1);
    REQUIRE(max == 12);
    REQUIRE(avg == (5 + (-1) + 12 + 2 + 8) / 5);

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[1] == avg);
    REQUIRE(a[2] == avg);
}