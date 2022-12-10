#include "Day10-CathodeRayTube.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int REGISTER_STARTING_VALUE = 1;
const unsigned INTERESTING_SIGNAL_STRENGTH_OFFSET = 20;
const unsigned INTERESTING_SIGNAL_STRENGTH_MODULUS = 40;
const unsigned INTERESTING_SIGNAL_STRENGTH_COUNT = 6;
const unsigned SCREEN_WIDTH = 40;
const unsigned SCREEN_HEIGHT = 6;
const char LIGHT_PIXEL = '#';
const char DARK_PIXEL = '.';

const unsigned ADDX_INSTRUCTION_DURATION = 2;
const unsigned NOOP_INSTRUCTION_DURATION = 1;

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
        , m_screen(SCREEN_HEIGHT * SCREEN_WIDTH)
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

    std::string getImageAppearingOnScreen() const
    {
        std::string output;

        for (size_t i = 0; i < m_screen.size(); ++i)
        {
            const char currentChar = m_screen.at(i) ? LIGHT_PIXEL : DARK_PIXEL;
            output += currentChar;
            if ((i + 1) % SCREEN_WIDTH == 0 && i != m_screen.size() - 1)
            {
                output += "\n";
            }
        }

        return output;
    }

private:
    std::vector<Instruction> m_instructions;

    int m_register;
    unsigned m_cycleCounter;
    std::vector<int> m_interestingSignalStrengths;
    std::vector<bool> m_screen;

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
        drawCurrentPixel();

        ++m_cycleCounter;

        captureInterestingSignalStrength();
    }

    void captureInterestingSignalStrength()
    {
        if (m_cycleCounter % INTERESTING_SIGNAL_STRENGTH_MODULUS == INTERESTING_SIGNAL_STRENGTH_OFFSET)
        {
            m_interestingSignalStrengths.push_back(m_cycleCounter * m_register);
        }
    }

    void drawCurrentPixel()
    {
        const int currentlyRenderedPixelPositionX = m_cycleCounter % SCREEN_WIDTH;
        const bool isCurrentPixelLit = std::abs(currentlyRenderedPixelPositionX - m_register) <= 1;

        m_screen.at(m_cycleCounter) = isCurrentPixelLit;
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

std::string imageAppearingOnScreen(const std::vector<std::string>& instructionLines)
{
    std::vector<Instruction> instructions = parseInstructionLines(instructionLines);

    CommunicationDevice communicationDevice{std::move(instructions)};

    communicationDevice.runProgram();

    return communicationDevice.getImageAppearingOnScreen();
}

}
}
}