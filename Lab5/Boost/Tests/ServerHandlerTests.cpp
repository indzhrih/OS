#include <boost/test/unit_test.hpp>
#include "../Headers/ServerHandler.h"
#include "../Headers/EmployeeStorage.h"
#include "../Headers/PipeServer.h"
#include "../Headers/PipeClient.h"
#include "../Headers/PipeRequest.h"
#include "../Headers/PipeResponse.h"
#include "../Headers/RecordLockManager.h"
#include <thread>

static std::string generateServerHandlerPipeName(const std::string& suffix) {
    return "ServerHandlerPipe_" + suffix;
}

BOOST_AUTO_TEST_SUITE(ServerHandlerTests)

BOOST_AUTO_TEST_CASE(RunHandlesShutdownRequestAndStops) {
    std::string pipeName = generateServerHandlerPipeName("shutdown");
    PipeServer server(pipeName);
    bool created = server.createPipe();

    EmployeeStorage storage("dummy_file_for_server_handler.bin");
    RecordLockManager lockManager;
    ServerHandler handler(&storage, &server, &lockManager);

    std::thread serverThread([&handler]() { handler.run(); });

    PipeClient client(pipeName);
    bool connected = client.connectToServer();

    PipeRequest request;
    request.type = PIPE_REQUEST_SHUTDOWN;
    request.employeeId = 0;
    bool sent = client.sendRequest(request);

    PipeResponse response;
    bool received = client.receiveResponse(response);

    serverThread.join();
    server.disconnect();

    bool result = created && connected && sent && received && response.status == PIPE_RESPONSE_OK;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
