
#include <iostream>
#include "Server.h"

void Server::start() {
    _listening_socket->listen();
    while (true) {
        try {
            std::shared_ptr<ConnectionSocket> new_connection = _listening_socket->accept();
            _connection_handler->handle_connection(new_connection);
        } catch (const std::exception &exception) {
            std::cout << "Exception" << std::endl;
        }
    }
}
