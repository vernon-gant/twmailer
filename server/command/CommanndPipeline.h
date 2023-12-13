#pragma once

#include "Command.h"

class CommandPipeline {
public:
    virtual void process(const Command &command) = 0;

    virtual ~CommandPipeline() = default;
};
