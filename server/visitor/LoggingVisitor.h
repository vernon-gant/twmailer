#pragma once

#include "CommandVisitor.h"

class LoggingVisitor : public CommandVisitor {
private:

public:
    void visitList(const ListCommand &list_command) const override;

    void visitSend(const SendCommand &send_command) const override;

    void visitRead(const ReadCommand &read_command) const override;

    void visitDelete(const DeleteCommand &delete_command) const override;

    void visitQuit(const QuitCommand &quit_command) const override;

};
