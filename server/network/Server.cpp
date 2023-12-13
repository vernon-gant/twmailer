
#include <iostream>
#include "Server.h"

void Server::start() {
    _listening_socket->listen();
    std::cout << "\n               SERVER STARTED LISTENING               \n" << std::endl;
    while (true) {
        try {
            std::shared_ptr<ConnectionSocket> new_connection = _listening_socket->accept();
            _connection_handler->handle(new_connection);
        } catch (const std::exception &exception) {
            std::cout << exception.what() << std::endl;
        }
    }
}
