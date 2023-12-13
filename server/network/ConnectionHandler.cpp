#include "ConnectionHandler.h"

void ConnectionHandler:: process_connection(const std::shared_ptr<ConnectionSocket> &connection_socket) {
    std::cout << connection_socket->get_client_ip_address() << "  #############  Started communication"
              << std::endl;
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
            std::cout << connection_socket->get_client_ip_address() << "  #############  Ended session"
                      << std::endl;
            break;
        }
    }
    connection_socket->close();
}
