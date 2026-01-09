#include <boost/test/unit_test.hpp>
#include "../Headers/ProcessHandler.h"
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(ProcessHandlerTests)

BOOST_AUTO_TEST_CASE(WaitForProcessDoesNotThrowForValidCommand) {
    ProcessHandler handler("cmd.exe /C exit 0");
    handler.waitForProcess();
    bool result = true;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ConstructorThrowsForInvalidCommand) {
    bool threw = false;
    try {
        ProcessHandler handler("DefinitelyNonExistingProgram_12345.exe");
    } catch (const std::exception&) {
        threw = true;
    }
    BOOST_CHECK(threw);
}

BOOST_AUTO_TEST_SUITE_END()
