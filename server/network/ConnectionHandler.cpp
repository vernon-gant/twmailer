#include <thread>
#include "ConnectionHandler.h"
#include "exceptions/ValidationError.h"
#include "exceptions/InternalServerError.h"

void ConnectionHandler::handle_connection(const std::shared_ptr<ConnectionSocket> &connection_socket) {
    std::cout << connection_socket->get_client_ip_address() << "  #############  Started communication"
              << std::endl;
    while (true) {
        try {
            std::string message = connection_socket->receive();
            UserContext user_context = {connection_socket->get_client_ip_address(), ""};
            std::string response = handle_command(message, user_context);
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

std::string ConnectionHandler::handle_command(std::string &message, const UserContext &user_context) {
    try {
        std::unique_ptr<Command> command = _command_parser->create(message, user_context);
        _command_pipeline->process(*command);
        command->execute();
        return command->get_response();
    } catch (const ValidationError &validation_error) {
        return validation_error.what();
    } catch (const InternalServerError &internal_error) {
        return "ERR\n";
    }
}
