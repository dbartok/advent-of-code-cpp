#pragma once

#include "Operation.h"
#include "Condition.h"
#include "State.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class Instruction
{
public:
    Instruction(Operation operation, Condition condition);
    void execute(State& state) const;

    static Instruction fromString(const std::string& instructionString);

private:
    Operation m_operation;
    Condition m_condition;
};

}
