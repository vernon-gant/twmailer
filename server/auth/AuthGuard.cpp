#include "AuthGuard.h"
#include "errors/IsBanned.h"
#include "network/UserContext.h"
#include "errors/GotBanned.h"
#include "errors/AttemptsExceeded.h"

AuthGuard::AuthGuard(std::unique_ptr<BlackListHandler> &black_list_handler,
                     std::unique_ptr<CredentialsReader> &credentials_reader,
                     std::unique_ptr<LDAPConnector> &ldap_connector,
                     const std::shared_ptr<Logger> &logger) : _black_list_handler(std::move(black_list_handler)),
                                                              _credentials_reader(std::move(credentials_reader)),
                                                              _ldap_connector(std::move(ldap_connector)),
                                                              _logger(logger) {}

UserContext AuthGuard::authenticate(const std::shared_ptr<ConnectionSocket> &connection_socket) {
    Credentials credentials = _credentials_reader->read(connection_socket);
    log_attempt(connection_socket->get_client_address(), credentials.user_name);

    if (_black_list_handler->is_banned(credentials.user_name, connection_socket->get_client_address()))
        throw IsBanned("IS BANNED", credentials.user_name);

    while (true) {
        if (_ldap_connector->valid_credentials(credentials)) return authenticate_user(connection_socket, credentials);

        try { handle_failed_attempt(connection_socket, credentials); }
        catch (const AttemptsExceeded &) { break; }

        credentials = _credentials_reader->read(connection_socket);
        log_attempt(connection_socket->get_client_address(), credentials.user_name);
    }

    _black_list_handler->ban(credentials.user_name, connection_socket->get_client_address());
    throw GotBanned("GOT BANNED",credentials.user_name);
}

UserContext AuthGuard::authenticate_user(const std::shared_ptr<ConnectionSocket> &connection_socket,const Credentials &credentials) const {
    log_success(connection_socket->get_client_address(), credentials.user_name);
    connection_socket->send("OK\n");
    return {.client_ip_address = connection_socket->get_client_address(), .user_name = credentials.user_name};
}

void AuthGuard::handle_failed_attempt(const std::shared_ptr<ConnectionSocket> &connection_socket,
                                      const Credentials &credentials) const {
    _black_list_handler->increase_wrong_tries(credentials.user_name, connection_socket->get_client_address());

    int times_banned = _black_list_handler->get_bans_amount(credentials.user_name,connection_socket->get_client_address());
    if (times_banned == MAX_ATTEMPTS) throw AttemptsExceeded();

    connection_socket->send("WRONG\n");
    log_wrong_credentials(connection_socket->get_client_address(), credentials.user_name);
}

void AuthGuard::log_attempt(const std::string &client_address, const std::string &user_name) const {
    _logger->log(client_address, "Attempts to login with \"" + user_name + "\"");
}

void AuthGuard::log_wrong_credentials(const std::string &client_address, const std::string &user_name) const {
    _logger->log(client_address, "Login attempt with \"" + user_name + "\" failed - wrong credentials");
}

void AuthGuard::log_success(const std::string &client_address, const std::string &user_name) const {
    _logger->log(client_address, "Successfully logged in with \"" + user_name + "\"");
}
