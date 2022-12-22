#include "Day22-MonkeyMap.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/dense>
#include <boost/algorithm/string.hpp>

#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int PASSWORD_ROW_MULTIPLIER = 1000;
const int PASSWORD_COLUMN_MULTIPLIER = 4;
const char VOID_TILE = ' ';
const char OPEN_TILE = '.';
const char WALL_TILE = '#';

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day22
{

using MonkeyMap = std::vector<std::string>;

struct Instruction
{
    unsigned numSteps = 0;
    char rotationDirection = '\0';
};

using Instructions = std::vector<Instruction>;

// x points right, y points down
using Vector2D = Eigen::RowVector2i;
using Matrix2D = Eigen::Matrix2i;

const Vector2D START_DIRECTION{1, 0};

class MonkeyMapPathfinder
{
public:
    MonkeyMapPathfinder(MonkeyMap monkeyMap, Instructions instructions)
        : m_monkeyMap{std::move(monkeyMap)}
        , m_width{m_monkeyMap.at(0).size()}
        , m_height{m_monkeyMap.size()}
        , m_instructions{std::move(instructions)}
        , m_currentDirection{START_DIRECTION}
    {
        for (int i = 0; i < m_width; ++i)
        {
            if (m_monkeyMap.at(0).at(i) == OPEN_TILE)
            {
                m_currentPosition = {i, 0};
                break;
            }
        }

        m_leftRotationMatrix <<
            0, -1,
            1, 0;
    }

    void executeInstructions()
    {
        for (const auto& instruction : m_instructions)
        {
            moveForward(instruction.numSteps);
            rotate(instruction.rotationDirection);
        }
    }

    int determineFinalPassword()
    {
        int directionAddend = 0;

        while (m_currentDirection != START_DIRECTION)
        {
            rotateLeft();
            ++directionAddend;
        }

        return (m_currentPosition.y() + 1) * PASSWORD_ROW_MULTIPLIER + (m_currentPosition.x() + 1) * PASSWORD_COLUMN_MULTIPLIER + directionAddend;
    }

private:
    Matrix2D m_leftRotationMatrix;

    MonkeyMap m_monkeyMap;
    size_t m_width;
    size_t m_height;
    Instructions m_instructions;

    Vector2D m_currentPosition;
    Vector2D m_currentDirection;

    void moveForward(unsigned numSteps)
    {
        for (unsigned step = 0; step < numSteps; ++step)
        {
            moveForwardOnce();
        }
    }

    void rotate(char direction)
    {
        if (direction == 'L')
        {
            rotateLeft();
        }
        else if (direction == 'R')
        {
            rotateRight();
        }
    }

    void moveForwardOnce()
    {
        Vector2D nextPosition = getNextPosition();
        if (getTileAt(nextPosition) == OPEN_TILE)
        {
            m_currentPosition = nextPosition;
        }
    }

    void rotateLeft()
    {
        m_currentDirection *= m_leftRotationMatrix;
    }

    void rotateRight()
    {
        rotateLeft();
        rotateLeft();
        rotateLeft();
    }

    Vector2D getNextPosition() const
    {
        Vector2D nextPosition = m_currentPosition;
        do
        {
            nextPosition += m_currentDirection;

            nextPosition.y() = (nextPosition.y() + m_height) % m_height;
            nextPosition.x() = (nextPosition.x() + m_width) % m_width;
        } while (getTileAt(nextPosition) == VOID_TILE);

        return nextPosition;
    }

    char getTileAt(const Vector2D& position) const
    {
        return m_monkeyMap.at(position.y()).at(position.x());
    }
};

using MonkeyNoteTextSection = std::vector<std::string>;

MonkeyMap parseMonkeyMapLines(const std::vector<std::string>& monkeyMapLines)
{
    const auto longestLineIter = std::max_element(monkeyMapLines.cbegin(), monkeyMapLines.cend(), [](const auto& lhs, const auto& rhs)
                                                  {
                                                      return lhs.size() < rhs.size();
                                                  });
    const size_t longestLineSize = longestLineIter->size();

    MonkeyMap monkeyMap;

    std::transform(monkeyMapLines.cbegin(), monkeyMapLines.cend(), std::back_inserter(monkeyMap), [longestLineSize](const auto& line)
                   {
                       auto paddedLine{line};
                       if (line.size() < longestLineSize)
                       {
                           paddedLine.insert(paddedLine.end(), longestLineSize - line.size(), ' ');
                       }
                       return paddedLine;
                   });

    return monkeyMap;
}

Instructions parseInstructionLine(const std::string& instructionLine)
{
    Instructions instructions;
    Instruction currentInstruction;

    for (const char c : instructionLine)
    {
        if (isdigit(c))
        {
            currentInstruction.numSteps *= 10;
            currentInstruction.numSteps += c - '0';
        }
        else
        {
            currentInstruction.rotationDirection = c;
            instructions.push_back(std::move(currentInstruction));
            currentInstruction = {};
        }
    }

    instructions.push_back(std::move(currentInstruction));

    return instructions;
}

std::pair<MonkeyMap, Instructions> parseMonkeyNoteLines(const std::vector<std::string>& monkeyNoteLines)
{
    std::vector<MonkeyNoteTextSection> monkeyNoteTextSections;
    boost::split(monkeyNoteTextSections, monkeyNoteLines, [](const auto& elem) { return elem.empty(); });

    MonkeyMap monkeyMap = parseMonkeyMapLines(monkeyNoteTextSections.at(0));
    Instructions instructions = parseInstructionLine(monkeyNoteTextSections.at(1).front());

    return {std::move(monkeyMap), std::move(instructions)};
}

int finalPassword(const std::vector<std::string>& monkeyNoteLines)
{
    MonkeyMap monkeyMap;
    Instructions instructions;
    std::tie(monkeyMap, instructions) = parseMonkeyNoteLines(monkeyNoteLines);

    MonkeyMapPathfinder monkeyMapPathfinder{std::move(monkeyMap), std::move(instructions)};

    monkeyMapPathfinder.executeInstructions();

    return monkeyMapPathfinder.determineFinalPassword();
}


}
}
}