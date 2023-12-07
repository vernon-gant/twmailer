#pragma once

#include <exception>
#include <stdexcept>

class InternalServerError : public std::runtime_error {
public:
    explicit InternalServerError(const std::string &arg) : runtime_error(arg) {}

};
