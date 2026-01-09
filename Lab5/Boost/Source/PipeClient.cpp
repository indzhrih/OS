#include "../Headers/PipeClient.h"
#include "../Headers/ExceptionHandler.h"
#include <boost/interprocess/exceptions.hpp>

using boost::interprocess::message_queue;
using boost::interprocess::interprocess_exception;

PipeClient::PipeClient(const std::string& pipeNameValue)
    : pipeName(pipeNameValue),
      requestQueueName(pipeNameValue + "_request"),
      responseQueueName(pipeNameValue + "_response"),
      requestQueue(nullptr),
      responseQueue(nullptr) {
}

PipeClient::~PipeClient() {
    requestQueue.reset();
    responseQueue.reset();
}

bool PipeClient::connectToServer() {
    try {
        requestQueue = std::make_unique<message_queue>(
            boost::interprocess::open_only,
            requestQueueName.c_str()
        );
        responseQueue = std::make_unique<message_queue>(
            boost::interprocess::open_only,
            responseQueueName.c_str()
        );

        return true;
    }
    catch (const interprocess_exception& exception) {
        ExceptionHandler::printError("Connect to server failed: " + std::string(exception.what()));
        requestQueue.reset();
        responseQueue.reset();
        return false;
    }
}

bool PipeClient::sendRequest(const PipeRequest& request) const {
    if (!requestQueue) {
        ExceptionHandler::printError("sendRequest: request queue is not initialized");
        return false;
    }

    try {
        requestQueue->send(&request, sizeof(PipeRequest), 0);
        return true;
    }
    catch (const interprocess_exception& exception) {
        ExceptionHandler::printError("Write request failed: " + std::string(exception.what()));
        return false;
    }
}

bool PipeClient::receiveResponse(PipeResponse& response) const {
    if (!responseQueue) {
        ExceptionHandler::printError("receiveResponse: response queue is not initialized");
        return false;
    }

    try {
        message_queue::size_type receivedSize = 0;
        unsigned int priority = 0;
        responseQueue->receive(&response, sizeof(PipeResponse), receivedSize, priority);
        return receivedSize == sizeof(PipeResponse);
    }
    catch (const interprocess_exception& exception) {
        ExceptionHandler::printError("Read response failed: " + std::string(exception.what()));
        return false;
    }
}
