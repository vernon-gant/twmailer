#pragma once

#include "Command.h"
#include "utils/FileSystemUtils.h"

#include <utility>

class ListCommand : public Command {
private:
    std::shared_ptr<FileSystemUtils> _file_system_utils;

public:
    explicit ListCommand(const UserContext &userContext, const std::shared_ptr<FileSystemUtils>& utils);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] const std::string &get_user_name() const;

};
