#pragma once

#include "Instruction.h"

namespace AdventOfCode
{

class Processor
{
public:
    Processor(std::vector<Instruction> instructions);
    void run();
    int largestRegister() const;
    int globalMaxValueOfRegisters() const;

private:
    std::vector<Instruction> m_instructions;
    State m_state;
};

}
