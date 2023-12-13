#pragma once

#include <memory>
#include <iostream>
#include "ConnectionSocket.h"
#include "RequestHandler.h"
#include "exceptions/QuitException.h"
#include "exceptions/NetworkError.h"

class  ConnectionHandler {
protected:
    std::unique_ptr<RequestHandler> _request_handler;

    void process_connection(const std::shared_ptr<ConnectionSocket> &connection_socket);

public:
    explicit ConnectionHandler(std::unique_ptr<RequestHandler> request_handler) : _request_handler(
            std::move(request_handler)) {};

    virtual void handle(std::shared_ptr<ConnectionSocket> connection_socket) = 0;

    virtual ~ConnectionHandler() = default;
};
