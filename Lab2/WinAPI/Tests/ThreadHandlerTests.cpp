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

TEST_CASE("ThreadHandler: wrong name throws and logs") {
    ThreadHandler th;
    Array a(1);
    a[0] = 42;

    CerrRedirect err;
    REQUIRE_THROWS_AS(th.createThread("wrong", a), std::invalid_argument);
    REQUIRE_THAT(err.oss.str(), ContainsSubstring("Thread handler error: Error, wrong thread name"));
}
