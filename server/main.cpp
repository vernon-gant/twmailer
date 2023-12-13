#include "utils/Environment.h"
#include "network/ListeningSocket.h"
#include "network/ConnectionHandler.h"
#include "visitor/VisitorPipeline.h"
#include "visitor/ValidationVisitor.h"
#include "visitor/LoggingVisitor.h"
#include "network/Server.h"
#include "errors/InternalServerError.h"

int main(int argc, char **argv) {
    std::shared_ptr<Environment> environment = Environment::FromArgs(argc, argv);
    std::unique_ptr<ListeningSocket> listening_socket;
    try {
        listening_socket = ListeningSocket::fromPort(environment->PORT());
    } catch (const InternalServerError &error) {
        std::cout << error.what() << std::endl;
    }
    std::shared_ptr<FileSystemUtils> utils = std::make_shared<FileSystemUtils>(environment);
    std::unique_ptr<CommandVisitor> validation_visitor = std::make_unique<ValidationVisitor>(utils);
    std::unique_ptr<CommandVisitor> logging_visitor = std::make_unique<LoggingVisitor>();
    std::vector<std::unique_ptr<CommandVisitor>> visitors = std::vector<std::unique_ptr<CommandVisitor>>();
    visitors.emplace_back(std::move(validation_visitor));
    visitors.emplace_back(std::move(logging_visitor));
    std::unique_ptr<CommandPipeline> command_pipeline = std::make_unique<VisitorPipeline>(std::move(visitors));
    std::unique_ptr<CommandFactory> command_parser = std::make_unique<CommandFactory>(utils);
    std::unique_ptr<LDAPConnector> ldap_connector = LDAPConnector::initialize();
    std::unique_ptr<BlackListHandler> black_list_handler = std::make_unique<BlackListHandler>();
    std::unique_ptr<CredentialsReader> credentials_reader = std::make_unique<CredentialsReader>();
    std::unique_ptr<AuthGuard> auth_guard = std::make_unique<AuthGuard>(black_list_handler, credentials_reader,
                                                                        ldap_connector);
    std::unique_ptr<ConnectionHandler> connection_handler = std::make_unique<ConnectionHandler>(
            std::move(command_parser), std::move(command_pipeline), std::move(auth_guard));
    Server server(std::move(listening_socket), std::move(connection_handler));
    server.start();
}