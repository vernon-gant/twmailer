#pragma once

#include <memory>
#include "ListeningSocket.h"
#include "ConnectionHandler.h"

class Server {

private:
    std::unique_ptr<ConnectionHandler> _connection_handler;

    std::unique_ptr<ListeningSocket> _listening_socket;

public:
    Server(std::unique_ptr<ListeningSocket> listening_socket, std::unique_ptr<ConnectionHandler> connection_handler)
            : _connection_handler(std::move(connection_handler)), _listening_socket(std::move(listening_socket)) {};

    void start();

};
