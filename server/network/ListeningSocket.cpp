#include "ListeningSocket.h"
#include <netdb.h>
#include "stdexcept"
#include "errors/InternalServerError.h"


std::unique_ptr<ListeningSocket> ListeningSocket::fromPort(int port) {
    try {
        struct addrinfo *socket_address = get_socket_address(port);
        int socket_descriptor = socket(socket_address->ai_family, socket_address->ai_socktype,
                                       socket_address->ai_protocol);
        int reuseValue = 1;
        setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &reuseValue, sizeof(reuseValue));
        setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEPORT, &reuseValue, sizeof(reuseValue));
        std::unique_ptr<ListeningSocket> listening_socket = std::make_unique<ListeningSocket>(socket_descriptor,
                                                                                              *socket_address->ai_addr);
        freeaddrinfo(socket_address);
        return listening_socket;
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server Error : could not create listening socket!");
    }
}

void ListeningSocket::listen() {
    bind(_socket_descriptor, &_socket_address, sizeof(sockaddr_in));
    ::listen(_socket_descriptor, QUEUE_SIZE);
}

std::shared_ptr<ConnectionSocket> ListeningSocket::accept() {
    struct sockaddr client_address{};
    socklen_t address_size = sizeof(client_address);
    int client_socket_descriptor = ::accept(_socket_descriptor, &client_address, &address_size);
    std::shared_ptr<ConnectionSocket> connection_socket = std::make_shared<ConnectionSocket>(client_socket_descriptor,
                                                                                             client_address);
    return connection_socket;
}

addrinfo *ListeningSocket::get_socket_address(int port) {
    struct addrinfo configuration{}, *socket_address;
    configuration.ai_family = AF_UNSPEC;
    configuration.ai_socktype = SOCK_STREAM;
    configuration.ai_flags = AI_PASSIVE;

    getaddrinfo(nullptr, std::to_string(port).c_str(), &configuration, &socket_address);

    for (struct addrinfo *current_socket_address = socket_address;
         current_socket_address != nullptr; current_socket_address = current_socket_address->ai_next) {
        if (current_socket_address->ai_family == AF_INET) socket_address = current_socket_address;
    }
    return socket_address;
}
