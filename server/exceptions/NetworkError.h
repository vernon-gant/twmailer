#pragma once

#include <stdexcept>

class NetworkError : public std::runtime_error {
public:
    explicit NetworkError(const std::string &arg) : runtime_error(arg) {}

};
