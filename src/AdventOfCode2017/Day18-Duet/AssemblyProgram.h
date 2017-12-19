#pragma once

#include "AssemblyProgramState.h"
#include "AssemblyInstruction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class AssemblyProgram
{
public:
    AssemblyProgram(std::vector<AssemblyInstruction::SharedPtr> instructions) noexcept;

    void runUntilTerminated();
    int lastRecoveredFrequency() const;

private:
    std::vector<AssemblyInstruction::SharedPtr> m_instructions;
    AssemblyProgramState m_state;
};

}
