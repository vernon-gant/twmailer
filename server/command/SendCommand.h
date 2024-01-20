#pragma once

#include "Command.h"
#include "utils/FileSystemUtils.h"

#include <mutex>
#include <utility>
#include <memory>

typedef struct Mail {
    std::string sender, receiver, subject, content;
} Mail;

class SendCommand : public Command {
private:
    static std::mutex send_mutex; // Static mutex for synchronizing send operations
    Mail _mail;
    std::shared_ptr<FileSystemUtils> _file_system_utils;

public:
    SendCommand(const UserContext &userContext, Mail mail, const std::shared_ptr<FileSystemUtils>& utils);

    void execute() override;

    void accept(const CommandVisitor &visitor) const override;

    [[nodiscard]] const Mail &get_mail() const;
};




