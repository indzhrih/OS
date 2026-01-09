#include <sstream>
#include <iostream>

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
