#include "QuitCommand.h"
#include "../visitor/CommandVisitor.h"
#include "errors/Quit.h"

void QuitCommand::execute() {
    throw Quit();
}

void QuitCommand::accept(const CommandVisitor &visitor) const {
    visitor.visit_quit(*this);
}

QuitCommand::QuitCommand(const UserContext &userContext) : Command(userContext) {}
