#include <iostream>
#include "AuthGuard.h"
#include "errors/IsBanned.h"
#include "network/UserContext.h"
#include "errors/GotBanned.h"

UserContext AuthGuard::authenticate(const std::shared_ptr<ConnectionSocket> &connection_socket) const {
    Credentials credentials = _credentials_reader->read(connection_socket);
    log_try(connection_socket->get_client_ip_address(), credentials.user_name);

    if (_black_list_handler->is_banned(credentials.user_name, connection_socket->get_client_ip_address()))
        throw IsBanned("You are banned : please wait until your ban for " + credentials.user_name + " is released...",credentials.user_name);

    while (true) {
        if (_ldap_connector->valid_credentials(credentials)) {
            log_success(connection_socket->get_client_ip_address(), credentials.user_name);
            connection_socket->send("OK\n");
            return {.client_ip_address = connection_socket->get_client_ip_address(), .user_name = credentials.user_name};
        }
        _black_list_handler->increase_wrong_tries(credentials.user_name, connection_socket->get_client_ip_address());
        int times_banned = _black_list_handler->get_bans_amount(credentials.user_name,connection_socket->get_client_ip_address());
        if (times_banned == MAX_ATTEMPTS) break;
        connection_socket->send("Wrong credentials : wrong username or password. Please, try again...");
        log_wrong_credentials(connection_socket->get_client_ip_address(), credentials.user_name);
        credentials = _credentials_reader->read(connection_socket);
        log_try(connection_socket->get_client_ip_address(), credentials.user_name);
    }

    _black_list_handler->ban(credentials.user_name, connection_socket->get_client_ip_address());
    throw GotBanned("Got banned : you got banned for 1 minute with \"" + credentials.user_name + "\" username...", credentials.user_name);
}

AuthGuard::AuthGuard(std::unique_ptr<BlackListHandler> &black_list_handler,
                     std::unique_ptr<CredentialsReader> &credentials_reader,
                     std::unique_ptr<LDAPConnector> &ldap_connector) : _black_list_handler(
        std::move(black_list_handler)),
                                                                       _credentials_reader(
                                                                               std::move(credentials_reader)),
                                                                       _ldap_connector(std::move(ldap_connector)) {}

void AuthGuard::log_try(const std::string &client_address, const std::string &user_name) {
    std::cout << client_address << "  #############  Tries to login with \"" << user_name << "\"" << std::endl;
}

void AuthGuard::log_wrong_credentials(const std::string &client_address, const std::string &user_name) {
    std::cout << client_address << "  #############  Login with \"" << user_name << "\" failed - wrong credentials"
              << std::endl;
}

void AuthGuard::log_success(const std::string &client_address, const std::string &user_name) {
    std::cout << client_address << "  #############  Successfully logged in with \"" << user_name << "\"" << std::endl;
}
