#include <boost/test/unit_test.hpp>
#include "../Headers/ExceptionHandler.h"
#include <sstream>

BOOST_AUTO_TEST_SUITE(ExceptionHandlerTests)

BOOST_AUTO_TEST_CASE(CheckHandleReturnsTrueForValidHandle) {
    HANDLE handle = CreateEventA(NULL, FALSE, FALSE, NULL);
    bool result = ExceptionHandler::checkHandle(handle, "CheckHandle valid");
    if (handle) CloseHandle(handle);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CheckHandleReturnsFalseForInvalidHandle) {
    HANDLE handle = INVALID_HANDLE_VALUE;
    bool result = ExceptionHandler::checkHandle(handle, "CheckHandle invalid");
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

BOOST_AUTO_TEST_CASE(CheckReleaseReturnsTrueForSuccess) {
    bool result = ExceptionHandler::checkRelease(TRUE, "CheckRelease true");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CheckReleaseReturnsFalseForFailure) {
    bool result = ExceptionHandler::checkRelease(FALSE, "CheckRelease false");
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(CheckWaitReturnsTrueForObjectAndTimeout) {
    bool objectResult = ExceptionHandler::checkWait(WAIT_OBJECT_0, "CheckWait object");
    bool timeoutResult = ExceptionHandler::checkWait(WAIT_TIMEOUT, "CheckWait timeout");
    bool result = objectResult && timeoutResult;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CheckWaitReturnsFalseForFailedCode) {
    bool result = ExceptionHandler::checkWait(WAIT_FAILED, "CheckWait failed");
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
