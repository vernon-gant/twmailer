#pragma once

#include <memory>
#include <netdb.h>
#include "Socket.h"
#include "ConnectionSocket.h"

class ListeningSocket : public Socket {

private:

    static const int QUEUE_SIZE = 20;

    static addrinfo * get_socket_address(int port);

public:

    ListeningSocket(int socket_descriptor, struct sockaddr socket_address) : Socket(socket_descriptor, socket_address) {}

    static std::unique_ptr<ListeningSocket> fromPort(int port);

    void listen();

    std::shared_ptr<ConnectionSocket> accept();


};
