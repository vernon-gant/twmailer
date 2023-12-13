#pragma once

#include "Command.h"
#include "utils/FileSystemUtils.h"

#include <utility>

class ReadCommand : public Command {
private:
    const int _message_number;

    std::shared_ptr<FileSystemUtils> _file_system_utils;

public:
    ReadCommand(const UserContext &userContext, int message_number, const std::shared_ptr<FileSystemUtils>& utils);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] int get_message_number() const;

};
