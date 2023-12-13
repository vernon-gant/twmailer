#pragma once

#include "CommandVisitor.h"

class ValidationVisitor : public CommandVisitor {
private:
    std::shared_ptr<FileSystemUtils> _file_system_utils;
public:
    explicit ValidationVisitor(const std::shared_ptr<FileSystemUtils> &fileSystemUtils);

public:
    void visitList(const ListCommand &list_command) const override;

    void visitSend(const SendCommand &send_command) const override;

    void visitRead(const ReadCommand &read_command) const override;

    void visitDelete(const DeleteCommand &delete_command) const override;

    void visitQuit(const QuitCommand &quit_command) const override;

    void validateSingleMessageCommand(const SingleMessageRequest &message_request, const std::string &command_name) const;

};
