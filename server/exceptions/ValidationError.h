#pragma once

#include <stdexcept>

class ValidationError : public std::invalid_argument {
public:
    explicit ValidationError(const std::string &arg) : invalid_argument(arg) {}

};
