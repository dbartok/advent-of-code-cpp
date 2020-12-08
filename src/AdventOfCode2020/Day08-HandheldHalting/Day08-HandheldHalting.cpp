#include "Day08-HandheldHalting.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct Instruction
{
    std::string name;
    int arg;
};

class HandheldGameConsole
{
public:
    HandheldGameConsole(std::vector<Instruction> instructions)
        : m_instructions{std::move(instructions)}
        , m_accumulator{0}
        , m_instructionPointer{0}
    {

    }

    void executeCurrentInstruction()
    {
        const auto& currentInstruction = m_instructions.at(m_instructionPointer);

        if (currentInstruction.name == "jmp")
        {
            m_instructionPointer += (currentInstruction.arg - 1);
        }
        else if (currentInstruction.name == "acc")
        {
            m_accumulator += currentInstruction.arg;
        }

        ++m_instructionPointer;
    }

    int getInstructionPointer() const
    {
        return m_instructionPointer;
    }

    int getAccumulator() const
    {
        return m_accumulator;
    }

private:
    std::vector<Instruction> m_instructions;

    int m_accumulator;
    int m_instructionPointer;
};

Instruction parseInstruction(const std::string& instructionLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, instructionLine, boost::is_any_of(" "));

    std::string name = tokens.at(0);
    int arg = std::stoi(tokens.at(1));

    return Instruction{std::move(name), arg};
}

std::vector<Instruction> parseInstructions(const std::vector<std::string>& instructionLines)
{
    std::vector<Instruction> instructions;

    for (const auto& line : instructionLines)
    {
        Instruction instruction = parseInstruction(line);
        instructions.push_back(std::move(instruction));
    }

    return instructions;
}

int valueInAccAfterFirstDuplicateInstruction(const std::vector<std::string>& instructionLines)
{
    std::vector<Instruction> instructions = parseInstructions(instructionLines);
    HandheldGameConsole handheldGameConsole{std::move(instructions)};

    std::unordered_set<int> instructionsExecuted;

    while (true)
    {
        bool wasInserted = instructionsExecuted.insert(handheldGameConsole.getInstructionPointer()).second;
        if (!wasInserted)
        {
            break;
        }

        handheldGameConsole.executeCurrentInstruction();
    }

    return handheldGameConsole.getAccumulator();
}

}
