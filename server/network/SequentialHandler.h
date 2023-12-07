#pragma once

#include "ConnectionHandler.h"

class SequentialHandler : public ConnectionHandler {
public:
    explicit SequentialHandler(std::unique_ptr<RequestHandler> request_handler) : ConnectionHandler(
            std::move(request_handler)) {}

    void handle(std::shared_ptr<ConnectionSocket> connection_socket) override;
};
