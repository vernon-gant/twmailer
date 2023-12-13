#pragma once

#include <utility>
#include <memory>

#include "../network/ConnectionSocket.h"
#include "network/UserContext.h"

class CommandVisitor;

class Command {
protected:
    UserContext _user_context;

    std::string _response;
public:
    explicit Command(UserContext user_context) : _user_context(std::move(user_context)) {};

    virtual void execute() = 0;

    virtual void accept(const CommandVisitor &visitor) const = 0;

    [[nodiscard]] std::string get_response() const {
        return _response;
    }

    [[nodiscard]] const UserContext &get_user_context() const {
        return _user_context;
    }

    virtual ~Command() = default;
};
