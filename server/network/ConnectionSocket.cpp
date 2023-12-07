#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ConnectionSocket.h"
#include "exceptions/NetworkError.h"
#include "exceptions/InternalServerError.h"

void ConnectionSocket::send(const std::string &message) {
    int messageLength = static_cast<int>(message.size());
    if (::send(_socket_descriptor, reinterpret_cast<const char *>(&messageLength), sizeof(int), 0) == -1)
        throw NetworkError("Network error : fail while sending message length.");

    size_t total = 0;
    int bytes_left = message.size();

    while (total < message.size()) {
        int bytes_sent = ::send(_socket_descriptor, message.c_str() + total, bytes_left, 0);
        if (bytes_sent == -1) throw NetworkError("Network error : fail while message sending.");
        total += bytes_sent;
        bytes_left -= bytes_sent;
    }
}

std::string ConnectionSocket::receive() {
    int message_length;
    int bytes_received = recv(_socket_descriptor, &message_length, sizeof(int), 0);

    if (bytes_received == -1) throw NetworkError("Network error : fail while receiving message length");
    if (bytes_received == 0) throw NetworkError("Connection closed by client");

    int total = 0;
    int bytes_left = message_length;
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    std::string message;

    while (total < message_length) {
        bytes_received = recv(_socket_descriptor, buffer, bytes_left, 0);

        if (bytes_received == -1) throw NetworkError("Network error : fail while receiving message");
        if (bytes_received == 0) throw NetworkError("Connection closed by client");

        message.append(buffer);
        bzero(buffer, bytes_received);
        total += bytes_received;
        bytes_left -= bytes_received;
    }

    return message;
}

const std::string& ConnectionSocket::get_client_ip_address() {
    return client_ip_address;
}

std::string ConnectionSocket::compose_ip_address() {
    sockaddr_in addr{};
    socklen_t addrSize = sizeof(sockaddr_in);

    if (getpeername(_socket_descriptor, (sockaddr *) &addr, &addrSize) == -1)
        throw NetworkError("Internal error : failed to get peer name.");

    char clientIP[INET_ADDRSTRLEN];

    if (inet_ntop(AF_INET, &(addr.sin_addr), clientIP, INET_ADDRSTRLEN) == nullptr)
        throw InternalServerError("Internal error : failed to convert IP address.");

    int clientPort = ntohs(addr.sin_port);

    std::string clientAddress = std::string(clientIP) + ":" + std::to_string(clientPort);
    return clientAddress;
}
