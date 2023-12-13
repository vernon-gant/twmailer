#pragma once

#include "CommandVisitor.h"

class ValidationVisitor : public CommandVisitor {
private:
    std::shared_ptr<FileSystemUtils> _file_system_utils;
public:
    explicit ValidationVisitor(const std::shared_ptr<FileSystemUtils> &fileSystemUtils);

public:
    void visit_list(const ListCommand &list_command) const override;

    void visit_send(const SendCommand &send_command) const override;

    void visit_read(const ReadCommand &read_command) const override;

    void visit_delete(const DeleteCommand &delete_command) const override;

    void visit_quit(const QuitCommand &) const override;

    void validate_single_message_command(const std::string &command_name, int message_number,
                                         const std::string &user_name) const;

};
