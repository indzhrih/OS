#include <boost/test/unit_test.hpp>
#include "../Headers/PipeClient.h"
#include "../Headers/PipeServer.h"
#include "../Headers/PipeRequest.h"
#include "../Headers/PipeResponse.h"
#include <windows.h>
#include <thread>

static std::string generatePipeName(const std::string& suffix) {
    DWORD pid = GetCurrentProcessId();
    return "PipeClientTest_" + std::to_string(pid) + "_" + suffix;
}

BOOST_AUTO_TEST_SUITE(PipeClientTests)

BOOST_AUTO_TEST_CASE(ConnectToServerSucceedsWhenServerExists) {
    std::string pipeName = generatePipeName("connect");
    PipeServer server(pipeName);
    bool created = server.createPipe();

    std::thread serverThread([&server]() { server.waitForClient(); });
    Sleep(50);

    PipeClient client(pipeName);
    bool connected = client.connectToServer();

    serverThread.join();
    server.disconnect();

    bool result = created && connected;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(SendRequestReturnsFalseWhenNotConnected) {
    PipeClient client("NotExistingPipe");
    PipeRequest request;
    bool result = !client.sendRequest(request);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ReceiveResponseReturnsFalseWhenNotConnected) {
    PipeClient client("NotExistingPipe");
    PipeResponse response;
    bool result = !client.receiveResponse(response);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
