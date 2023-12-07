#pragma once

#include <memory>
#include <vector>
#include "../visitor/CommandVisitor.h"
#include "../command/CommanndPipeline.h"

class VisitorPipeline : public CommandPipeline {

private:
    std::vector<std::unique_ptr<CommandVisitor>> _visitors;

public:
    explicit VisitorPipeline(std::vector<std::unique_ptr<CommandVisitor>> visitors) : _visitors(std::move(visitors)) {};

    void process(const Command &command) override;
};
