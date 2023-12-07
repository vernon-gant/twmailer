#pragma once

#include "Command.h"

#include <utility>

class ListCommand : public Command {

public:
    explicit ListCommand(const UserContext &userContext);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] const std::string &get_user_name() const;

};
