#pragma once

#include <memory>
#include "BlackListHandler.h"
#include "CredentialsReader.h"
#include "LDAPConnector.h"
#include "network/UserContext.h"

class AuthGuard {
private:

    static const int MAX_ATTEMPTS = 3;

    std::unique_ptr<BlackListHandler> _black_list_handler;
    std::unique_ptr<CredentialsReader> _credentials_reader;
    std::unique_ptr<LDAPConnector> _ldap_connector;

    static void log_success(const std::string &client_address, const std::string &user_name);

    static void log_try(const std::string &client_address, const std::string &user_name);

    static void log_wrong_credentials(const std::string &client_address, const std::string &user_name);

public:
    AuthGuard(std::unique_ptr<BlackListHandler> &,
              std::unique_ptr<CredentialsReader> &,
              std::unique_ptr<LDAPConnector> &);

    [[nodiscard]] UserContext authenticate(const std::shared_ptr<ConnectionSocket> &connection_socket) const;
};
