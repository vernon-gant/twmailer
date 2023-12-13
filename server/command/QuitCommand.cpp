#include "QuitCommand.h"
#include "../visitor/CommandVisitor.h"
#include "exceptions/QuitException.h"

void QuitCommand::execute() {
    throw QuitException();
}

void QuitCommand::accept(const CommandVisitor &visitor) const {
    visitor.visit_quit(*this);
}

QuitCommand::QuitCommand(const UserContext &userContext) : Command(userContext) {}
