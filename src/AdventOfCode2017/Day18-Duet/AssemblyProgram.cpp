#include "AssemblyProgram.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

AssemblyProgram::AssemblyProgram(std::vector<AssemblyInstruction::SharedPtr> instructions) noexcept
    : m_instructions{std::move(instructions)}
{

}

void AssemblyProgram::runUntilTerminated()
{
    while (m_state.executionState() != ExecutionState::TERMINATED)
    {
        auto currentInstruction = m_instructions[m_state.instructionIndex()];

        currentInstruction->execute(m_state);

        if (currentInstruction->increasesInstructionIndex())
        {
            ++m_state.instructionIndex();
        }

        if (m_state.instructionIndex() < 0 || m_state.instructionIndex() > m_instructions.size())
        {
            m_state.executionState() = ExecutionState::TERMINATED;
        }
    }
}

int AssemblyProgram::lastRecoveredFrequency() const
{
    boost::optional<int> frequency = m_state.lastPlayedFrequency();

    if (!frequency.is_initialized())
    {
        throw std::runtime_error("No frequency was recovered so far.");
    }

    return frequency.get();
}

}
