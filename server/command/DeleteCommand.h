#pragma once

#include "Command.h"
#include "DTO.h"
#include "utils/FileSystemUtils.h"

class DeleteCommand : public Command {
private:
    SingleMessageRequest _message_request;
    std::shared_ptr<FileSystemUtils> _file_system_utils;
public:
    DeleteCommand(const UserContext &userContext, SingleMessageRequest messageRequest,
                  const std::shared_ptr<FileSystemUtils> &utils);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] const SingleMessageRequest &get_single_message_request() const;

};
