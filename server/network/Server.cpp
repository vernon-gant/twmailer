#include <iostream>
#include <thread> // Include the thread header
#include "Server.h"

void Server::start() {
    _listening_socket->listen();
    while (true) {
        try {
            std::shared_ptr<ConnectionSocket> new_connection = _listening_socket->accept();

            // Create a new thread to handle the connection
            std::thread connectionThread(&ConnectionHandler::handle_connection, _connection_handler.get(), new_connection);

            // Detach the thread so it can operate independently
            connectionThread.detach();
        } catch (const std::exception &exception) {
            std::cout << "Exception: " << exception.what() << std::endl;
        }
    }
}

