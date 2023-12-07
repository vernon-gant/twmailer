#pragma once

#include "Command.h"
#include "DTO.h"

#include <utility>

class ReadCommand : public Command {
private:
    SingleMessageRequest _message_request;

public:
    ReadCommand(const UserContext &userContext, const SingleMessageRequest &messageRequest);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] const SingleMessageRequest &get_single_message_request() const;

};
