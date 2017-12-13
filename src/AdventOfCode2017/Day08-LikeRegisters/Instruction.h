#pragma once

#include "Operation.h"
#include "Condition.h"
#include "State.h"

#include <string>

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
