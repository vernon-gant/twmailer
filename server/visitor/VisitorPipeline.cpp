#include "VisitorPipeline.h"

void VisitorPipeline::process(const Command &command) {
    for (const std::unique_ptr<CommandVisitor> &visitor: _visitors) {
        command.accept(*visitor);
    }
}
