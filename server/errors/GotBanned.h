#pragma once

#include <stdexcept>
#include <utility>

class GotBanned : public std::runtime_error {
private:
    std::string _user_name;
public:
    [[nodiscard]] const std::string &get_user_name() const {
        return _user_name;
    }

public:
    explicit GotBanned(const std::string &arg, std::string user_name) : runtime_error(arg), _user_name(std::move(user_name)) {}

};