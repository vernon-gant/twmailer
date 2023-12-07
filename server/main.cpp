#include "utils/Environment.h"
#include "network/ListeningSocket.h"
#include "network/ConnectionHandler.h"
#include "network/SequentialHandler.h"
#include "visitor/VisitorPipeline.h"
#include "visitor/ValidationVisitor.h"
#include "visitor/LoggingVisitor.h"
#include "network/Server.h"

int main(int argc, char **argv) {
    Environment::Initialize(argc, argv);
    std::unique_ptr<ListeningSocket> listening_socket = ListeningSocket::fromPort(Environment::PORT());
    std::unique_ptr<CommandVisitor> validation_visitor = std::make_unique<ValidationVisitor>();
    std::unique_ptr<CommandVisitor> logging_visitor = std::make_unique<LoggingVisitor>();
    std::vector<std::unique_ptr<CommandVisitor>> visitors = std::vector<std::unique_ptr<CommandVisitor>>();
    visitors.emplace_back(std::move(validation_visitor));
    visitors.emplace_back(std::move(logging_visitor));
    std::unique_ptr<CommandPipeline> command_pipeline = std::make_unique<VisitorPipeline>(std::move(visitors));
    std::unique_ptr<CommandFactory> command_parser = std::make_unique<CommandFactory>();
    std::unique_ptr<RequestHandler> request_handler = std::make_unique<RequestHandler>(std::move(command_parser),
                                                                                       std::move(command_pipeline));
    std::unique_ptr<ConnectionHandler> connection_handler = std::make_unique<SequentialHandler>(
            std::move(request_handler));
    Server server(std::move(listening_socket), std::move(connection_handler));
    server.start();
}