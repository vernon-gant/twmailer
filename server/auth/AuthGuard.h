#pragma once

#include <memory>
#include "BlackListHandler.h"
#include "CredentialsReader.h"
#include "LDAPConnector.h"
#include "network/UserContext.h"
#include "utils/Logger.h"

class AuthGuard {

private:
    static const int MAX_ATTEMPTS = 3;

    std::unique_ptr<BlackListHandler> _black_list_handler;
    std::unique_ptr<CredentialsReader> _credentials_reader;
    std::unique_ptr<LDAPConnector> _ldap_connector;
    std::shared_ptr<Logger> _logger;

    void log_success(const std::string &client_address, const std::string &user_name) const;

    void log_attempt(const std::string &client_address, const std::string &user_name) const;

    void log_wrong_credentials(const std::string &client_address, const std::string &user_name) const;

    [[nodiscard]] UserContext authenticate_user(const std::shared_ptr<ConnectionSocket> &connection_socket, const Credentials &credentials) const;

    void handle_failed_attempt(const std::shared_ptr<ConnectionSocket> &connection_socket, const Credentials &credentials) const;

public:
    AuthGuard(std::unique_ptr<BlackListHandler> &,
              std::unique_ptr<CredentialsReader> &,
              std::unique_ptr<LDAPConnector> &,
              const std::shared_ptr<Logger> &);

    [[nodiscard]] UserContext authenticate(const std::shared_ptr<ConnectionSocket> &connection_socket);
};
