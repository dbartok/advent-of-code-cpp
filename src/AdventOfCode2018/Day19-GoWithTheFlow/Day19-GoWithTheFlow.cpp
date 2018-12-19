#include "Day19-GoWithTheFlow.h"

#include "AdventOfCode2018/Day16-ChronalClassification/InstructionEvaluator.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct DecodedInstruction
{
    std::string opcodeString;
    Instruction instruction;

    DecodedInstruction(std::string opcodeString, Instruction instruction)
        : opcodeString{std::move(opcodeString)}
        , instruction{std::move(instruction)}
    {

    }
};

class ControlFlowExecutor
{
public:
    ControlFlowExecutor(unsigned instructionPtrRegister, std::vector<DecodedInstruction> decodedInstructions)
        : m_instructionPtrRegister{instructionPtrRegister}
        , m_decodedInstructions(decodedInstructions)
        , m_registers{}
    {

    }

    void run()
    {
        while (true)
        {
            unsigned instructionPointer = m_registers.at(m_instructionPtrRegister);

            if (instructionPointer >= m_decodedInstructions.size())
            {
                break;
            }

            const auto& decodedInstruction = m_decodedInstructions.at(instructionPointer);

            InstructionEvaluator instructionEvaluator{decodedInstruction.instruction, std::move(m_registers)};
            m_registers = instructionEvaluator.evaluateAs(decodedInstruction.opcodeString);
            ++m_registers.at(m_instructionPtrRegister);
        }
    }

    unsigned getValueInFirstRegister() const
    {
        return m_registers.front();
    }

private:
    unsigned m_instructionPtrRegister;
    std::vector<DecodedInstruction> m_decodedInstructions;

    Registers m_registers;
};

DecodedInstruction parseDecodedInstruction(const std::string& decodedInstructionLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, decodedInstructionLine, boost::is_any_of(" "));

    unsigned a = boost::lexical_cast<unsigned>(tokens.at(1));
    unsigned b = boost::lexical_cast<unsigned>(tokens.at(2));
    unsigned c = boost::lexical_cast<unsigned>(tokens.at(3));

    // Always use 0 as the opcode, because the instruction is decoded
    Instruction instruction{0, a, b, c};

    return DecodedInstruction{tokens.at(0), std::move(instruction)};
}

ControlFlowExecutor parseControlFlowExecutor(const std::vector<std::string>& programLines)
{
    std::vector<std::string> tokens;
    boost::split(tokens, programLines.at(0), boost::is_any_of(" "));
    unsigned instructionPtrRegister = boost::lexical_cast<unsigned>(tokens.at(1));

    std::vector<DecodedInstruction> decodedInstructions;
    for (size_t i = 1; i < programLines.size(); ++i)
    {
        decodedInstructions.push_back(parseDecodedInstruction(programLines.at(i)));
    }

    return ControlFlowExecutor{instructionPtrRegister, decodedInstructions};
}

unsigned valueInFirstRegisterAfterProcessHalts(const std::vector<std::string>& programLines)
{
    ControlFlowExecutor controlFlowExecutor = parseControlFlowExecutor(programLines);
    controlFlowExecutor.run();

    return controlFlowExecutor.getValueInFirstRegister();
}

}
