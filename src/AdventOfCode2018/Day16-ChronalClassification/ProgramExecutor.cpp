#include "ProgramExecutor.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

ProgramExecutor::ProgramExecutor(std::vector<Instruction> instructions, OpcodeToOpcodeString opcodeToOpcodeString)
    : m_instructions{std::move(instructions)}
    , m_opcodeToOpcodeString{std::move(opcodeToOpcodeString)}
    , m_registers{0, 0, 0, 0}
{

}

void ProgramExecutor::execute()
{
    for (const auto& instruction : m_instructions)
    {
        InstructionEvaluator instructionEvalutator{instruction, std::move(m_registers)};

        std::string opcodeString = m_opcodeToOpcodeString.at(instruction.opcode);
        m_registers = instructionEvalutator.evaluateAs(opcodeString);
    }
}

unsigned ProgramExecutor::getValueInFirstRegister() const
{
    return m_registers.front();
}

}
