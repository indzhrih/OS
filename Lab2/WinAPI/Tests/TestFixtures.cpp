#include <sstream>
#include <iostream>

struct CoutRedirect {
    std::streambuf* old;
    std::ostringstream oss;
    CoutRedirect() : old(std::cout.rdbuf(oss.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
    std::string getOutput() const { return oss.str(); }
};

struct CerrRedirect {
    std::streambuf* old;
    std::ostringstream oss;
    CerrRedirect() : old(std::cerr.rdbuf(oss.rdbuf())) {}
    ~CerrRedirect() { std::cerr.rdbuf(old); }
    std::string getOutput() const { return oss.str(); }
};

struct CinRedirect {
    std::streambuf* old;
    std::istringstream iss;
    explicit CinRedirect(const std::string& data) : old(std::cin.rdbuf()), iss(data) {
        std::cin.rdbuf(iss.rdbuf());
    }
    ~CinRedirect() { std::cin.rdbuf(old); }
};
