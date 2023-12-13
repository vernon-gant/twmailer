#pragma once

#include "../command/SendCommand.h"
#include "../command/ReadCommand.h"
#include "../command/DeleteCommand.h"
#include "../command/QuitCommand.h"
#include "../command/ListCommand.h"

class CommandVisitor {
public:
    virtual void visit_list(const ListCommand &list_command) const = 0;

    virtual void visit_send(const SendCommand &send_command) const = 0;

    virtual void visit_read(const ReadCommand &read_command) const = 0;

    virtual void visit_delete(const DeleteCommand &delete_command) const = 0;

    virtual void visit_quit(const QuitCommand &quit_command) const = 0;

    virtual ~CommandVisitor() = default;
};
