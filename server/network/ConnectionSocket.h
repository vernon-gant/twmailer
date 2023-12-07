#pragma once

#include <string>
#include "Socket.h"

#include "sys/socket.h"

class ConnectionSocket : public Socket {

private:
    std::string client_ip_address;
    static const int BUFFER_SIZE = 1024;

    std::string compose_ip_address();

public:
    ConnectionSocket(int socket_descriptor, struct sockaddr socket_address) : Socket(socket_descriptor,
                                                                                     socket_address) {
        client_ip_address = compose_ip_address();
    };

    void send(const std::string &message);

    std::string receive();

    const std::string &get_client_ip_address();

};