
#include <iostream>
#include <thread>
#include "Server.h"

void Server::start() {
    _listening_socket->listen();
    _logger->log("\n               SERVER STARTED LISTENING               \n");
    while (true) {
        try {
            std::shared_ptr<ConnectionSocket> new_connection = _listening_socket->accept();
            std::thread connection_thread([this, new_connection]() {
                _connection_handler->handle(new_connection);
            });
            connection_thread.detach();
        } catch (const std::exception &exception) {
            _logger->log_fatal_error(std::string(exception.what()));
        }
    }
}
