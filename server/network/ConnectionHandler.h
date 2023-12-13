#pragma once

#include <memory>
#include <iostream>
#include "ConnectionSocket.h"
#include "exceptions/QuitException.h"
#include "exceptions/NetworkError.h"
#include "command/CommandFactory.h"
#include "command/CommanndPipeline.h"

class ConnectionHandler {
private:
    std::unique_ptr<CommandFactory> _command_parser;
    std::unique_ptr<CommandPipeline> _command_pipeline;

    std::string handle_command(std::string &message, const UserContext &user_context);

public:
    ConnectionHandler(std::unique_ptr<CommandFactory> command_parser, std::unique_ptr<CommandPipeline> command_pipeline)
            : _command_parser(std::move(command_parser)), _command_pipeline(std::move(command_pipeline)) {}

    void handle_connection(const std::shared_ptr<ConnectionSocket> &connection_socket);

    virtual ~ConnectionHandler() = default;
};
