#include <thread>
#include "ConnectionHandler.h"
#include "errors/ValidationError.h"
#include "errors/InternalServerError.h"
#include "errors/NetworkError.h"
#include "errors/IsBanned.h"
#include "errors/GotBanned.h"

void ConnectionHandler::handle(const std::shared_ptr<ConnectionSocket> &connection_socket) {
    _logger->log(connection_socket->get_client_address(),"Started connection");

    try {
        std::optional<UserContext> user_context = authenticate(connection_socket);
        if (user_context.has_value()) handle_connection(connection_socket, user_context.value());
    } catch (const NetworkError &exception) {
        _logger->log(connection_socket->get_client_address(), exception.what());
    } catch (const InternalServerError &error) {
        _logger->log(connection_socket->get_client_address(), error.what());
        connection_socket->send("ERR\n");
    }

    _logger->log(connection_socket->get_client_address(), "Closed connection");
    connection_socket->close();
}

std::optional<UserContext> ConnectionHandler::authenticate(const std::shared_ptr<ConnectionSocket> &connection_socket) {
    try {
        return std::make_optional<UserContext>(_auth_guard->authenticate(connection_socket));
    } catch (const GotBanned &error) {
        std::string log_message = "Last login attempt for \"" + error.get_user_name() + "\" used - got banned";
        _logger->log(connection_socket->get_client_address(), log_message);
        connection_socket->send(error.what());
    } catch (const IsBanned &error) {
        std::string log_message = "Login with \"" + error.get_user_name() + "\" failed - currently banned";
        _logger->log(connection_socket->get_client_address(), log_message);
        connection_socket->send(error.what());
    }
    return std::nullopt;
}

void ConnectionHandler::handle_connection(const std::shared_ptr<ConnectionSocket> &connection_socket,
                                          const UserContext &user_context) {
    while (true) {
        try {
            std::string message = connection_socket->receive();
            std::string response = handle_command(message, user_context);
            connection_socket->send(response);
        } catch (const Quit &) {
            _logger->log(connection_socket->get_client_address(), "Ended session");
            break;
        }
    }
}

std::string ConnectionHandler::handle_command(std::string &message, const UserContext &user_context) {
    try {
        std::unique_ptr<Command> command = _command_parser->create(message, user_context);
        _command_pipeline->process(*command);
        command->execute();
        return command->get_response();
    } catch (const ValidationError &validation_error) {
        _logger->log(user_context.client_ip_address, validation_error.what());
        return validation_error.what();
    }
}
