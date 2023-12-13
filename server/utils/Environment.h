#pragma once

#include <string>
#include <memory>

class Environment {

private:
    const int _port;
    const std::string _directory;

public:
    Environment(int port, std::string directory);

    static std::shared_ptr<Environment> FromArgs(int argc, char **argv);

    int PORT() const;

    const std::string &MAIL_DIRECTORY();
};
