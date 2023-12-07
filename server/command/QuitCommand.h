#pragma once

#include "Command.h"

class QuitCommand : public Command {
public:
    explicit QuitCommand(const UserContext &userContext);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

};
