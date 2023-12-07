#include "Environment.h"
#include <cstdlib>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

int Environment::_port = 0;
std::string Environment::_directory;

void Environment::Initialize(int argc, char *argv[]) {
    if (argc != 3) throw std::invalid_argument("Incorrect number of arguments");

    int port = std::atoi(argv[1]);
    if (port <= 1024 || port > 65535) throw std::invalid_argument("Invalid port number");

    _port = port;

    std::string directory = std::string(argv[2]);

    try {
        if (!fs::is_directory(directory) && !fs::create_directory(directory))
            throw std::runtime_error("Failed to create directory");
    } catch (const std::filesystem::filesystem_error &e) {
        throw std::runtime_error(std::string("Filesystem error: ") + e.what());
    }

    _directory = directory;
}

int Environment::PORT() {
    return _port;
}

const std::string &Environment::MAIL_DIRECTORY() {
    return _directory;
}
