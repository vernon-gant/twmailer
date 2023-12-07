#pragma once

#include <memory>
#include <iostream>
#include "ConnectionSocket.h"
#include "RequestHandler.h"
#include "exceptions/QuitException.h"
#include "exceptions/NetworkError.h"

class ConnectionHandler {
protected:
    std::unique_ptr<RequestHandler> _request_handler;

    void process_request(const std::shared_ptr<ConnectionSocket> &connection_socket) {
        std::cout << connection_socket->get_client_ip_address() << "  #############  Started communication" << std::endl;
        while (true) {
            try {
                std::string message = connection_socket->receive();
                UserContext user_context = {connection_socket->get_client_ip_address(), ""};
                std::string response = _request_handler->handle(message, user_context);
                connection_socket->send(response);
            } catch (const NetworkError &exception) {
                std::cout << connection_socket->get_client_ip_address() << "  #############  " << exception.what()
                          << std::endl;
                break;
            } catch (const QuitException &) {
                std::cout << connection_socket->get_client_ip_address() << "  #############  Ended session" << std::endl;
                break;
            }
        }
        connection_socket->close();
    }

public:
    explicit ConnectionHandler(std::unique_ptr<RequestHandler> request_handler) : _request_handler(
            std::move(request_handler)) {};

    virtual void handle(std::shared_ptr<ConnectionSocket> connection_socket) = 0;
};
