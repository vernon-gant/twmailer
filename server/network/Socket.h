#pragma once

#include <sys/socket.h>

class Socket {
protected:
    int _socket_descriptor;
    struct sockaddr _socket_address;

public:

    explicit Socket(int socket_descriptor, struct sockaddr socket_address) : _socket_descriptor(socket_descriptor),
                                                                              _socket_address(socket_address) {};

    void close() const;

};
