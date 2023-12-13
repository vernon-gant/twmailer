#pragma once

#include "CommandVisitor.h"

class LoggingVisitor : public CommandVisitor {
private:

public:
    void visit_list(const ListCommand &list_command) const override;

    void visit_send(const SendCommand &send_command) const override;

    void visit_read(const ReadCommand &read_command) const override;

    void visit_delete(const DeleteCommand &delete_command) const override;

    void visit_quit(const QuitCommand &quit_command) const override;

};
