#include "../Headers/PipeServer.h"
#include "../Headers/ExceptionHandler.h"
#include <boost/interprocess/exceptions.hpp>

using boost::interprocess::message_queue;
using boost::interprocess::interprocess_exception;

PipeServer::PipeServer(const std::string& pipeNameValue)
    : pipeName(pipeNameValue),
      requestQueueName(pipeNameValue + "_request"),
      responseQueueName(pipeNameValue + "_response"),
      requestQueue(nullptr),
      responseQueue(nullptr) {
}

PipeServer::~PipeServer() {
    disconnect();
}

bool PipeServer::createPipe() {
    try {
        message_queue::remove(requestQueueName.c_str());
        message_queue::remove(responseQueueName.c_str());

        requestQueue = std::make_unique<message_queue>(
            boost::interprocess::create_only,
            requestQueueName.c_str(),
            100,
            sizeof(PipeRequest)
        );

        responseQueue = std::make_unique<message_queue>(
            boost::interprocess::create_only,
            responseQueueName.c_str(),
            100,
            sizeof(PipeResponse)
        );

        return true;
    }
    catch (const interprocess_exception& exception) {
        ExceptionHandler::printError("Create message queues failed: " + std::string(exception.what()));
        requestQueue.reset();
        responseQueue.reset();
        return false;
    }
}

bool PipeServer::waitForClient() {
    return requestQueue != nullptr && responseQueue != nullptr;
}

bool PipeServer::readRequest(PipeRequest& request) {
    if (!requestQueue) return false;

    try {
        message_queue::size_type receivedSize = 0;
        unsigned int priority = 0;
        requestQueue->receive(&request, sizeof(PipeRequest), receivedSize, priority);
        return receivedSize == sizeof(PipeRequest);
    }
    catch (const interprocess_exception& exception) {
        ExceptionHandler::printError("Read request failed: " + std::string(exception.what()));
        return false;
    }
}

bool PipeServer::writeResponse(const PipeResponse& response) {
    if (!responseQueue) return false;

    try {
        responseQueue->send(&response, sizeof(PipeResponse), 0);
        return true;
    }
    catch (const interprocess_exception& exception) {
        ExceptionHandler::printError("Write response failed: " + std::string(exception.what()));
        return false;
    }
}

void PipeServer::disconnect() {
    requestQueue.reset();
    responseQueue.reset();
    message_queue::remove(requestQueueName.c_str());
    message_queue::remove(responseQueueName.c_str());
}
