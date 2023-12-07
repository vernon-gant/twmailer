#pragma once

#include <memory>
#include "command/CommandFactory.h"
#include "command/CommanndPipeline.h"

class RequestHandler {
private:
    std::unique_ptr<CommandFactory> _command_parser;
    std::unique_ptr<CommandPipeline> _command_pipeline;

public:
    RequestHandler(std::unique_ptr<CommandFactory> command_parser, std::unique_ptr<CommandPipeline> command_pipeline)
            : _command_parser(std::move(command_parser)), _command_pipeline(std::move(command_pipeline)) {}

    std::string handle(std::string &message, const UserContext &user_context);
};
