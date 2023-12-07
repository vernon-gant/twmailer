#pragma once

#include <string>

class Environment {

private:
    static int _port;
    static std::string _directory;

public:
    static void Initialize(int argc, char **argv);

    static int PORT();

    static const std::string &MAIL_DIRECTORY();
};
