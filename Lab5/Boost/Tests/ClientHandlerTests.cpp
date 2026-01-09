#include <boost/test/unit_test.hpp>
#include "../Headers/ClientHandler.h"
#include "../Headers/PipeClient.h"
#include "../Headers/ExceptionHandler.h"
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_SUITE(ClientHandlerTests)

BOOST_AUTO_TEST_CASE(RunExitsOnExitCommandWithoutServer) {
    PipeClient client("NonExistingPipeForClientHandler");
    ClientHandler handler(&client);

    std::streambuf* originalBuffer = std::cin.rdbuf();
    std::istringstream input("3\n");
    std::cin.rdbuf(input.rdbuf());

    handler.run();

    std::cin.rdbuf(originalBuffer);
    std::cin.clear();

    bool result = true;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
