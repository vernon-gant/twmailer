#include "Environment.h"
#include <cstdlib>
#include <stdexcept>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;


std::shared_ptr<Environment> Environment::FromArgs(int argc, char *argv[]) {
    if (argc != 3) throw std::invalid_argument("Incorrect number of arguments");

    int port = std::atoi(argv[1]);
    if (port <= 1024 || port > 65535) throw std::invalid_argument("Invalid port number");

    std::string directory = std::string(argv[2]);

    try {
        if (!fs::is_directory(directory) && !fs::create_directory(directory))
            throw std::runtime_error("Failed to create directory");
    } catch (const std::filesystem::filesystem_error &e) {
        throw std::runtime_error(std::string("Filesystem error: ") + e.what());
    }

    return std::make_shared<Environment>(port,directory);
}

int Environment::PORT() const {
    return _port;
}

const std::string &Environment::MAIL_DIRECTORY() {
    return _directory;
}

Environment::Environment(const int port, std::string directory) : _port(port), _directory(std::move(directory)) {}
