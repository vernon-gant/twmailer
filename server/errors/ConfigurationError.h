#pragma once

#include <stdexcept>

class ConfigurationError : public std::runtime_error {
public:
    explicit ConfigurationError(const std::string &arg) : runtime_error(arg) {}

};
