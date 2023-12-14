#pragma once

#include "Command.h"
#include "utils/FileSystemUtils.h"

class DeleteCommand : public Command {
private:
    const int _message_number;
    std::shared_ptr<FileSystemUtils> _file_system_utils;

    static void reorder_message_files(const std::string& user_directory, int deleted_message_number);
public:
    DeleteCommand(const UserContext &userContext, int message_number,
                  const std::shared_ptr<FileSystemUtils> &utils);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] int get_message_number() const;

};
