#include <boost/test/unit_test.hpp>
#include "../Headers/ExceptionHandler.h"
#include <sstream>

BOOST_AUTO_TEST_SUITE(ExceptionHandlerTests)

BOOST_AUTO_TEST_CASE(CheckHandleReturnsTrueWhenOkIsTrue) {
    bool result = ExceptionHandler::checkHandle(true, "CheckHandle true");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CheckHandleReturnsFalseWhenOkIsFalse) {
    bool result = ExceptionHandler::checkHandle(false, "CheckHandle false");
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(CheckWaitReturnsTrueWhenOkIsTrue) {
    bool result = ExceptionHandler::checkWait(true, "CheckWait true");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CheckWaitReturnsFalseWhenOkIsFalse) {
    bool result = ExceptionHandler::checkWait(false, "CheckWait false");
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(CheckReleaseReturnsTrueWhenOkIsTrue) {
    bool result = ExceptionHandler::checkRelease(true, "CheckRelease true");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CheckReleaseReturnsFalseWhenOkIsFalse) {
    bool result = ExceptionHandler::checkRelease(false, "CheckRelease false");
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(CheckFileReturnsTrueForValidFile) {
    const char* name = "exception_handler_test_file.bin";
    FILE* file = std::fopen(name, "wb");
    bool result = ExceptionHandler::checkFile(file, "CheckFile valid");
    if (file) std::fclose(file);
    std::remove(name);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CheckFileReturnsFalseForNullFile) {
    FILE* file = NULL;
    bool result = ExceptionHandler::checkFile(file, "CheckFile null");
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(ClearInputResetsFailStateAndReadsNextValue) {
    std::streambuf* originalBuffer = std::cin.rdbuf();
    std::istringstream input("x\n42\n");
    std::cin.rdbuf(input.rdbuf());

    int value = 0;
    std::cin >> value;
    bool before = std::cin.fail();

    ExceptionHandler::clearInput();
    std::cin >> value;
    bool after = !std::cin.fail() && value == 42;
    std::cin.rdbuf(originalBuffer);
    std::cin.clear();

    bool result = before && after;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
