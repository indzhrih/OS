#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "TestFixtures.cpp"
#include "../Headers/Array.h"

using Catch::Matchers::ContainsSubstring;

TEST_CASE("Array: ctor throws on zero size") {
    REQUIRE_THROWS_AS(Array(0), std::invalid_argument);
}

TEST_CASE("Array: ctor throws on negative size") {
    REQUIRE_THROWS_AS(Array(-3), std::invalid_argument);
}

TEST_CASE("Array: operator[] can set and get values") {
    Array a(3);
    a[0] = 10;
    a[1] = -5;
    a[2] = 7;
    REQUIRE(a[0] == 10);
    REQUIRE(a[1] == -5);
    REQUIRE(a[2] == 7);
}

TEST_CASE("Array: operator[] throws on negative index") {
    Array a(3);
    REQUIRE_THROWS_AS(a[-1], std::out_of_range);
}

TEST_CASE("Array: operator[] throws on index out of bounds") {
    Array a(3);
    REQUIRE_THROWS_AS(a[3], std::out_of_range);
}

TEST_CASE("Array: inputArray correctly fills values") {
    Array a(3);
    std::istringstream testInput("1\n2\n3\n");
    std::cin.rdbuf(testInput.rdbuf());

    CoutRedirect coutr;
    a.inputArray();

    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(a[2] == 3);
}

TEST_CASE("Array: inputArray handles invalid token and prints message") {
    Array a(2);
    std::istringstream testInput("x\n1\n2\n");
    std::cin.rdbuf(testInput.rdbuf());

    CoutRedirect coutr;
    CerrRedirect cerrr;

    a.inputArray();

    REQUIRE_THAT(cerrr.getOutput(), ContainsSubstring("Input error, try again"));
}

TEST_CASE("Array: inputArray prints prompt message") {
    Array a(1);
    std::istringstream testInput("5\n");
    std::cin.rdbuf(testInput.rdbuf());

    CoutRedirect coutr;

    a.inputArray();

    REQUIRE_THAT(coutr.getOutput(), ContainsSubstring("Input 1 numbers"));
}

TEST_CASE("Array: printArray prints all elements") {
    Array a(4);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a[3] = 4;

    CoutRedirect out;
    a.printArray();

    REQUIRE_THAT(out.getOutput(), ContainsSubstring("Array: 1 2 3 4"));
}

TEST_CASE("Array: findMinElement returns correct value") {
    Array a(5);
    a[0] = 5;
    a[1] = 3;
    a[2] = 9;
    a[3] = -4;
    a[4] = 7;

    REQUIRE(a.findMinElement() == -4);
}

TEST_CASE("Array: findMaxElement returns correct value") {
    Array a(5);
    a[0] = 5;
    a[1] = 3;
    a[2] = 9;
    a[3] = -4;
    a[4] = 7;

    REQUIRE(a.findMaxElement() == 9);
}

TEST_CASE("Array: calculateAverageValue returns correct value") {
    Array a(5);
    a[0] = 5;
    a[1] = 3;
    a[2] = 9;
    a[3] = -4;
    a[4] = 7;

    int expected = (5 + 3 + 9 - 4 + 7) / 5;
    REQUIRE(a.calculateAverageValue() == expected);
}

TEST_CASE("Array: switchMaxAndMinOnAverage replaces min element with average") {
    Array a(5);
    a[0] = 5;
    a[1] = 1;
    a[2] = 7;
    a[3] = 5;
    a[4] = 3;

    int oldMin = a.findMinElement();
    int oldMax = a.findMaxElement();
    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[1] == avg);
}

TEST_CASE("Array: switchMaxAndMinOnAverage replaces max element with average") {
    Array a(5);
    a[0] = 5;
    a[1] = 1;
    a[2] = 7;
    a[3] = 5;
    a[4] = 3;

    int oldMin = a.findMinElement();
    int oldMax = a.findMaxElement();
    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[2] == avg);
}

TEST_CASE("Array: switchMaxAndMinOnAverage leaves other elements unchanged") {
    Array a(5);
    a[0] = 5;
    a[1] = 1;
    a[2] = 7;
    a[3] = 5;
    a[4] = 3;

    a.findMinElement();
    a.findMaxElement();
    a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[0] == 5);
    REQUIRE(a[3] == 5);
    REQUIRE(a[4] == 3);
}

TEST_CASE("Array: exactly two elements become average after switch") {
    Array a(6);
    a[0] = 2;
    a[1] = -3;
    a[2] = 5;
    a[3] = 7;
    a[4] = 4;
    a[5] = 6;

    a.findMinElement();
    a.findMaxElement();
    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    int replaced = 0;
    for (int i = 0; i < 6; ++i) {
        if (a[i] == avg) ++replaced;
    }
    REQUIRE(replaced == 2);
}
