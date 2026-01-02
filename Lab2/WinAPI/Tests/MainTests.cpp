#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "TestFixtures.cpp"
#include "../Headers/Array.h"
#include "../Headers/ThreadHandler.h"

using Catch::Matchers::ContainsSubstring;

TEST_CASE("Array: findMinElement returns correct minimum") {
    Array a(5);
    a[0] = 5;
    a[1] = -1;
    a[2] = 12;
    a[3] = 2;
    a[4] = 8;

    REQUIRE(a.findMinElement() == -1);
}

TEST_CASE("Array: findMaxElement returns correct maximum") {
    Array a(5);
    a[0] = 5;
    a[1] = -1;
    a[2] = 12;
    a[3] = 2;
    a[4] = 8;

    REQUIRE(a.findMaxElement() == 12);
}

TEST_CASE("Array: calculateAverageValue returns correct average") {
    Array a(5);
    a[0] = 5;
    a[1] = -1;
    a[2] = 12;
    a[3] = 2;
    a[4] = 8;

    int expected = (5 + (-1) + 12 + 2 + 8) / 5;
    REQUIRE(a.calculateAverageValue() == expected);
}

TEST_CASE("Array: switchMaxAndMinOnAverage replaces min element") {
    Array a(5);
    a[0] = 5;
    a[1] = -1;
    a[2] = 12;
    a[3] = 2;
    a[4] = 8;

    a.findMinElement();
    a.findMaxElement();
    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[1] == avg);
}

TEST_CASE("Array: switchMaxAndMinOnAverage replaces max element") {
    Array a(5);
    a[0] = 5;
    a[1] = -1;
    a[2] = 12;
    a[3] = 2;
    a[4] = 8;

    a.findMinElement();
    a.findMaxElement();
    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[2] == avg);
}
