#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <sstream>
#include <iostream>
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

struct CinRedirect {
    std::streambuf* old;
    std::istringstream iss;
    explicit CinRedirect(const std::string& data) : old(std::cin.rdbuf()), iss(data) {
        std::cin.rdbuf(iss.rdbuf());
    }
    ~CinRedirect() { std::cin.rdbuf(old); }
};

TEST_CASE("Array: validates size") {
    REQUIRE_THROWS_AS(Array(0), std::invalid_argument);
    REQUIRE_THROWS_AS(Array(-5), std::invalid_argument);
}

TEST_CASE("Array: Default-constructed Array uninitialized and guarded") {
    Array a;

    REQUIRE_THROWS_AS(a.printArray(), std::out_of_range);
    REQUIRE_THROWS_AS(a.inputArray(), std::out_of_range);
    REQUIRE_THROWS_AS(a.findMinElement(), std::out_of_range);
    REQUIRE_THROWS_AS(a.findMaxElement(), std::out_of_range);
    REQUIRE_THROWS_AS(a.calculateAverageValue(), std::out_of_range);
    REQUIRE_THROWS_AS(a.switchMaxAndMinOnAverage(), std::out_of_range);
    REQUIRE_THROWS_AS(a.operator[](0), std::out_of_range);
}

TEST_CASE("Array: operator[] get/set and bounds") {
    Array a(3);
    a[0] = 10;
    a[1] = -5;
    a[2] = 7;
    REQUIRE(a[0] == 10);
    REQUIRE(a[1] == -5);
    REQUIRE(a[2] == 7);

    REQUIRE_THROWS_AS(a[-1], std::out_of_range);
    REQUIRE_THROWS_AS(a[3], std::out_of_range);
}

TEST_CASE("Array: inputArray handles invalid tokens and fills values") {
    Array a(3);

    std::streambuf* origCin = std::cin.rdbuf();
    std::istringstream testInput("x\n1\n2\n3\n");
    std::cin.rdbuf(testInput.rdbuf());

    CoutRedirect coutr;
    CerrRedirect cerrr;

    a.inputArray();

    std::cin.rdbuf(origCin);

    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(a[2] == 3);

    REQUIRE_THAT(coutr.oss.str(), ContainsSubstring("Input 3 numbers"));
    REQUIRE_THAT(cerrr.oss.str(), ContainsSubstring("Input error, try again"));
}

TEST_CASE("Array: printArray prints all elements") {
    Array a(4);
    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4;
    CoutRedirect r;
    a.printArray();
    REQUIRE_THAT(r.oss.str(), ContainsSubstring("Array: 1 2 3 4"));
}

TEST_CASE("Array: findMinElement/findMaxElement/calculateAverageValue work") {
    Array a(5);
    a[0] = 5; a[1] = 3; a[2] = 9; a[3] = -4; a[4] = 7;

    REQUIRE(a.findMinElement() == -4);
    REQUIRE(a.findMaxElement() == 9);

    REQUIRE(a.calculateAverageValue() == (5 + 3 + 9 - 4 + 7) / 5);
}

TEST_CASE("Array: switchMaxAndMinOnAverage replaces exactly the stored min/max indices") {
    Array a(5);
    a[0] = 5; a[1] = 1; a[2] = 7; a[3] = 5; a[4] = 3;

    REQUIRE(a.findMinElement() == 1);
    REQUIRE(a.findMaxElement() == 7);
    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[0] == 5);
    REQUIRE(a[1] == avg);
    REQUIRE(a[2] == avg);
    REQUIRE(a[3] == 5);
    REQUIRE(a[4] == 3);
}

TEST_CASE("Array: exactly two elements replaced with average") {
    Array a(4);
    a[0] = 10; a[1] = -5; a[2] = 3; a[3] = 7;

    a.findMinElement();
    a.findMaxElement();
    int avg = a.calculateAverageValue();

    a.switchMaxAndMinOnAverage();

    REQUIRE(a[0] == avg);
    REQUIRE(a[1] == avg);
    REQUIRE(a[2] == 3);
    REQUIRE(a[3] == 7);
}
