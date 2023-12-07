#pragma once

#include "Command.h"
#include "DTO.h"

class DeleteCommand : public Command {
private:
    SingleMessageRequest _message_request;
public:
    DeleteCommand(const UserContext &userContext, SingleMessageRequest messageRequest);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] const SingleMessageRequest &get_single_message_request() const;

};
