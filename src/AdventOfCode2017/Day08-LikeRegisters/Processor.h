#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "Instruction.h"
END_LIBRARIES_DISABLE_WARNINGS

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
