#ifndef LAB5_PIPECLIENT_H
#define LAB5_PIPECLIENT_H

#include <string>
#include <memory>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "PipeRequest.h"
#include "PipeResponse.h"

class PipeClient {
public:
    PipeClient(const std::string& pipeNameValue);
    ~PipeClient();

    bool connectToServer();
    bool sendRequest(const PipeRequest& request) const;
    bool receiveResponse(PipeResponse& response) const;

private:
    std::string pipeName;
    std::string requestQueueName;
    std::string responseQueueName;
    std::unique_ptr<boost::interprocess::message_queue> requestQueue;
    std::unique_ptr<boost::interprocess::message_queue> responseQueue;
};

#endif
