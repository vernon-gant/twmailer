#include <memory>
#include <sstream>
#include <regex>
#include "CredentialsReader.h"
#include "errors/ValidationError.h"
#include "errors/InternalServerError.h"

std::pair<bool, std::string> CredentialsReader::valid_login_input(const LoginInput &login_input) {
    try {
        if (login_input.command != "LOGIN")throw ValidationError("Validation Error : invalid command, you must login firstly...");

        if (login_input.credentials.user_name.empty()) throw ValidationError("Invalid LOGIN command format: missing username...");

        std::regex username_regex("^[a-zA-Z0-9]{1,8}$");
        if (!std::regex_match(login_input.credentials.user_name, username_regex))
            throw ValidationError("Validation error: invalid username format : " + login_input.credentials.user_name +
                                  " -> must consist of numbers with letters + length <= 8)");

        if (login_input.credentials.password.empty())
            throw ValidationError("Invalid LOGIN command format: missing password...");

        return std::make_pair(true, "");
    } catch (const ValidationError &error) {
        return std::make_pair(false, error.what());
    }
}

Credentials CredentialsReader::read(const std::shared_ptr<ConnectionSocket> &connection_socket) {
    LoginInput login_data = read_login_data(connection_socket);

    while (!std::get<0>(valid_login_input(login_data))) {
        connection_socket->send(std::get<1>(valid_login_input(login_data)));
        login_data = read_login_data(connection_socket);
    }

    return login_data.credentials;
}

LoginInput CredentialsReader::read_login_data(const std::shared_ptr<ConnectionSocket> &connection_socket) {
    try {
        std::string credentials_as_string = connection_socket->receive();
        LoginInput login_input;
        Credentials credentials;

        std::istringstream iss(credentials_as_string);
        std::getline(iss, login_input.command);
        std::getline(iss, credentials.user_name);
        std::getline(iss, credentials.password);
        login_input.credentials = credentials;
        return login_input;
    } catch (const std::exception &) {
        throw InternalServerError("Internal Server error : failed to parse credentials from message...");
    }
}
