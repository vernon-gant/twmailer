#pragma once

#include <string>
#include "network/ConnectionSocket.h"

typedef struct Credentials {
    std::string user_name, password;
} Credentials;

typedef struct LoginInput {
    std::string command;
    Credentials credentials;
} LoginInput;

class CredentialsReader {
private:
    static std::pair<bool, std::string> valid_login_input(const LoginInput &login_input);

    static LoginInput read_login_data(const std::shared_ptr<ConnectionSocket> &connection_socket);
public:

    static Credentials read(const std::shared_ptr<ConnectionSocket>&);
};
