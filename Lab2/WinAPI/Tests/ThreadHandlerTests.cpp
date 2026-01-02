#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "TestFixtures.cpp"
#include "../Headers/ThreadHandler.h"
#include "../Headers/Array.h"

using Catch::Matchers::ContainsSubstring;

TEST_CASE("ThreadHandler: wrong name throws exception") {
    ThreadHandler th;
    Array a(1);
    a[0] = 42;

    REQUIRE_THROWS_AS(th.createThread("wrong", a), std::invalid_argument);
}

TEST_CASE("ThreadHandler: wrong name logs error message") {
    ThreadHandler th;
    Array a(1);
    a[0] = 42;

    CerrRedirect err;

    try {
        th.createThread("wrong", a);
    }
    catch (const std::invalid_argument&) {}

    REQUIRE_THAT(err.getOutput(), ContainsSubstring("Thread handler error: Error, wrong thread name"));
}
