#pragma once

#include <memory>
#include <iostream>
#include <optional>
#include "ConnectionSocket.h"
#include "errors/Quit.h"
#include "errors/NetworkError.h"
#include "command/CommandFactory.h"
#include "command/CommanndPipeline.h"
#include "auth/AuthGuard.h"
#include "utils/Logger.h"

class ConnectionHandler {
private:
    std::unique_ptr<CommandFactory> _command_parser;
    std::unique_ptr<CommandPipeline> _command_pipeline;
    std::unique_ptr<AuthGuard> _auth_guard;
    std::shared_ptr<Logger> _logger;

    std::string handle_command(std::string &message, const UserContext &user_context);

    void handle_connection(const std::shared_ptr<ConnectionSocket> &connection_socket, const UserContext &user_context);

    std::optional<UserContext> authenticate(const std::shared_ptr<ConnectionSocket> &connection_socket);

public:
    ConnectionHandler(std::unique_ptr<CommandFactory> command_parser, std::unique_ptr<CommandPipeline> command_pipeline,
                      std::unique_ptr<AuthGuard> auth_guard, const std::shared_ptr<Logger> &logger)
            : _command_parser(std::move(command_parser)), _command_pipeline(std::move(command_pipeline)),
              _auth_guard(std::move(auth_guard)), _logger(logger) {}

    void handle(const std::shared_ptr<ConnectionSocket> &connection_socket);

    virtual ~ConnectionHandler() = default;
};
