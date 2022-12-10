#include "Day10-CathodeRayTube.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

int REGISTER_STARTING_VALUE = 1;
unsigned INTERESTING_SIGNAL_STRENGTH_OFFSET = 20;
unsigned INTERESTING_SIGNAL_STRENGTH_MODULUS = 40;
unsigned INTERESTING_SIGNAL_STRENGTH_COUNT = 6;

unsigned ADDX_INSTRUCTION_DURATION = 2;
unsigned NOOP_INSTRUCTION_DURATION = 1;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day10
{

struct Instruction
{
    int offset;
    unsigned duration;
};

class CommunicationDevice
{
public:
    CommunicationDevice(std::vector<Instruction> instructions)
        : m_instructions{std::move(instructions)}
        , m_register{REGISTER_STARTING_VALUE}
        , m_cycleCounter{0u}
    {

    }

    void runProgram()
    {
        for (const auto& instruction : m_instructions)
        {
            runInstruction(instruction);
        }
    }

    int getSumOfInterestingSignalStrengths() const
    {
        if (m_interestingSignalStrengths.size() < INTERESTING_SIGNAL_STRENGTH_COUNT)
        {
            throw std::runtime_error("There are too few interesting signals");
        }

        return std::accumulate(m_interestingSignalStrengths.cbegin(), m_interestingSignalStrengths.cbegin() + INTERESTING_SIGNAL_STRENGTH_COUNT, 0);
    }

private:
    std::vector<Instruction> m_instructions;

    int m_register;
    unsigned m_cycleCounter;
    std::vector<int> m_interestingSignalStrengths;

    void runInstruction(const Instruction& instruction)
    {
        for (unsigned cycleToElapse = 0; cycleToElapse < instruction.duration; ++cycleToElapse)
        {
            elapseCycle();
        }

        m_register += instruction.offset;
    }

    void elapseCycle()
    {
        ++m_cycleCounter;

        if (m_cycleCounter % INTERESTING_SIGNAL_STRENGTH_MODULUS == INTERESTING_SIGNAL_STRENGTH_OFFSET)
        {
            m_interestingSignalStrengths.push_back(m_cycleCounter * m_register);
        }
    }
};

Instruction parseInstructionLine(const std::string& instructionLine)
{
    std::vector<std::string> tokens;
    boost:split(tokens, instructionLine, boost::is_any_of(" "));

    if (tokens.at(0) == "addx")
    {
        return Instruction{std::stoi(tokens.at(1)), ADDX_INSTRUCTION_DURATION};
    }
    else
    {
        return Instruction{0, NOOP_INSTRUCTION_DURATION};
    }
}

std::vector<Instruction> parseInstructionLines(const std::vector<std::string>& instructionLines)
{
    std::vector<Instruction> instructions;

    for (const auto& instructionLine : instructionLines)
    {
        Instruction instruction = parseInstructionLine(instructionLine);
        instructions.push_back(std::move(instruction));
    }

    return instructions;
}

int sumOfInterestingSignalStrengths(const std::vector<std::string>& instructionLines)
{
    std::vector<Instruction> instructions = parseInstructionLines(instructionLines);

    CommunicationDevice communicationDevice{std::move(instructions)};

    communicationDevice.runProgram();

    return communicationDevice.getSumOfInterestingSignalStrengths();
}

}
}
}