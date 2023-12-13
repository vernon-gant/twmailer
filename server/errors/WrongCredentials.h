#pragma once

#include <stdexcept>

class WrongCredentials : public std::runtime_error {
public:
    explicit WrongCredentials(const std::string &arg) : runtime_error(arg) {}

};
