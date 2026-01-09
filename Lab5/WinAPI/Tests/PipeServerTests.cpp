#include <boost/test/unit_test.hpp>
#include "../Headers/PipeServer.h"
#include "../Headers/PipeRequest.h"
#include "../Headers/PipeResponse.h"
#include <windows.h>

static std::string generateServerPipeName(const std::string& suffix) {
    DWORD pid = GetCurrentProcessId();
    return "PipeServerTest_" + std::to_string(pid) + "_" + suffix;
}

BOOST_AUTO_TEST_SUITE(PipeServerTests)

BOOST_AUTO_TEST_CASE(CreatePipeReturnsTrueForValidName) {
    std::string pipeName = generateServerPipeName("create");
    PipeServer server(pipeName);
    bool result = server.createPipe();
    server.disconnect();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ReadRequestReturnsFalseWhenHandleInvalid) {
    PipeServer server("InvalidPipeServer");
    PipeRequest request;
    bool result = !server.readRequest(request);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(WriteResponseReturnsFalseWhenHandleInvalid) {
    PipeServer server("InvalidPipeServer");
    PipeResponse response;
    bool result = !server.writeResponse(response);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
