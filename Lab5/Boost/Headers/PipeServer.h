#ifndef LAB5_PIPESERVER_H
#define LAB5_PIPESERVER_H

#include <string>
#include <memory>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "PipeRequest.h"
#include "PipeResponse.h"

class PipeServer {
public:
    PipeServer(const std::string& pipeNameValue);
    ~PipeServer();

    bool createPipe();
    bool waitForClient();
    bool readRequest(PipeRequest& request);
    bool writeResponse(const PipeResponse& response);
    void disconnect();

private:
    std::string pipeName;
    std::string requestQueueName;
    std::string responseQueueName;
    std::unique_ptr<boost::interprocess::message_queue> requestQueue;
    std::unique_ptr<boost::interprocess::message_queue> responseQueue;
};

#endif
