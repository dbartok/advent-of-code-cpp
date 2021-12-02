#include "Day02-Dive.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day02
{

enum class Direction
{
    UP,
    DOWN,
    FORWARD,
};

struct Instruction
{
    Direction direction;
    int length;
};

class Submarine
{
public:
    Submarine(std::vector<Instruction> instructions)
        : m_instructions(std::move(instructions))
    {

    }

    void moveToFinalPosition()
    {
        for (const auto& instruction : m_instructions)
        {
            executeInstruction(instruction);
        }
    }

    int getHorizontalPositionAndDepthMultiplied() const
    {
        return m_horizontalPosition * m_depth;
    }

    virtual ~Submarine()
    {

    }

protected:
    int m_horizontalPosition = 0;
    int m_depth = 0;

    virtual void executeInstruction(const Instruction& instruction) = 0;

private:
    std::vector<Instruction> m_instructions;
};

class SimpleCourseSubmarine : public Submarine
{
public:
    using Submarine::Submarine;

    virtual ~SimpleCourseSubmarine()
    {

    }

protected:
    void executeInstruction(const Instruction& instruction) override
    {
        if (instruction.direction == Direction::UP)
        {
            m_depth -= instruction.length;
        }
        else if (instruction.direction == Direction::DOWN)
        {
            m_depth += instruction.length;
        }
        else if (instruction.direction == Direction::FORWARD)
        {
            m_horizontalPosition += instruction.length;
        }
        else
        {
            throw std::runtime_error("Invalid direction: " + std::to_string(static_cast<int>(instruction.direction)));
        }
    }
};

class AdjustedCourseSubmarine : public Submarine
{
public:
    using Submarine::Submarine;

    virtual ~AdjustedCourseSubmarine()
    {

    }

protected:
    void executeInstruction(const Instruction& instruction) override
    {
        if (instruction.direction == Direction::UP)
        {
            m_aim -= instruction.length;
        }
        else if (instruction.direction == Direction::DOWN)
        {
            m_aim += instruction.length;
        }
        else if (instruction.direction == Direction::FORWARD)
        {
            m_horizontalPosition += instruction.length;
            m_depth += m_aim * instruction.length;
        }
        else
        {
            throw std::runtime_error("Invalid direction: " + std::to_string(static_cast<int>(instruction.direction)));
        }
    }

private:
    int m_aim = 0;
};

Direction parseDirection(const std::string& directionString)
{
    if (directionString == "up")
    {
        return Direction::UP;
    }
    else if (directionString == "down")
    {
        return Direction::DOWN;
    }
    else if (directionString == "forward")
    {
        return Direction::FORWARD;
    }
    else
    {
        throw std::runtime_error("Invalid direction string: " + directionString);
    }
}

Instruction parseInstruction(const std::string& instructionLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, instructionLine, boost::is_any_of(" "));

    const Direction direction = parseDirection(tokens.at(0));
    const int length = std::stoi(tokens.at(1));

    return Instruction{direction, length};
}

std::vector<Instruction> parseInstructions(std::vector<std::string> instructionLines)
{
    std::vector<Instruction> instructions;

    for (const auto& line : instructionLines)
    {
        Instruction instruction = parseInstruction(line);
        instructions.push_back(std::move(instruction));
    }

    return instructions;
}

int finalHorizontalPositionAndDepthMultiplied(const std::vector<std::string>& instructionLines)
{
    std::vector<Instruction> instructions = parseInstructions(instructionLines);
    SimpleCourseSubmarine submarine{instructions};
    submarine.moveToFinalPosition();
    return submarine.getHorizontalPositionAndDepthMultiplied();
}

int finalHorizontalPositionAndDepthMultipliedWithAdjustedCourse(const std::vector<std::string>& instructionLines)
{
    std::vector<Instruction> instructions = parseInstructions(instructionLines);
    AdjustedCourseSubmarine submarine{instructions};
    submarine.moveToFinalPosition();
    return submarine.getHorizontalPositionAndDepthMultiplied();
}

}
}
}