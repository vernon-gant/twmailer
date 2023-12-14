
#include <iostream>
#include <thread>
#include "Server.h"

void Server::start() {
    _listening_socket->listen();
    std::cout << "\n               SERVER STARTED LISTENING               \n" << std::endl;
    while (true) {
        try {
            std::shared_ptr<ConnectionSocket> new_connection = _listening_socket->accept();
            std::thread connection_thread([this, new_connection]() {
                _connection_handler->handle(new_connection);
            });
            connection_thread.detach();
        } catch (const std::exception &exception) {
            std::cout << exception.what() << std::endl;
        }
    }
}
