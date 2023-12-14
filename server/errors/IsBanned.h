#pragma once

#include <stdexcept>

class IsBanned : public std::runtime_error {
private:
    std::string _user_name;
public:
    [[nodiscard]] const std::string &get_user_name() const {
        return _user_name;
    }

public:
    explicit IsBanned(const std::string &arg, const std::string &user_name) : runtime_error(arg),
                                                                              _user_name(user_name) {}

};
