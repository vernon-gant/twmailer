#pragma once

#include "../command/SendCommand.h"
#include "../command/ReadCommand.h"
#include "../command/DeleteCommand.h"
#include "../command/QuitCommand.h"
#include "../command/ListCommand.h"

class CommandVisitor {
public:
    virtual void visitList(const ListCommand &list_command) const = 0;

    virtual void visitSend(const SendCommand &send_command) const = 0;

    virtual void visitRead(const ReadCommand &read_command) const = 0;

    virtual void visitDelete(const DeleteCommand &delete_command) const = 0;

    virtual void visitQuit(const QuitCommand &quit_command) const = 0;

    virtual ~CommandVisitor() = default;
};
