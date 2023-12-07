#pragma once

#include "Command.h"

#include <utility>
#include <memory>

typedef struct Mail {
    std::string sender, receiver, subject, content;
} Mail;

class SendCommand : public Command {
private:
    Mail _mail;

public:
    SendCommand(const UserContext &userContext, const Mail &mail);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] const Mail &get_mail() const;

};


