#include "Day11-SpacePolice.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;

enum class PaintColor
{
    BLACK = 0,
    WHITE = 1,
};

using CoordinatesMap = std::unordered_map<Coordinates, PaintColor, boost::hash<Coordinates>>;

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class HullPaintingRobot
{
public:
    HullPaintingRobot(IntcodeInterpreter interpreter)
        : m_interpreter{std::move(interpreter)}
        , m_currentCoordinates{0, 0}
        , m_direction{Direction::UP}
    {

    }

    void paint()
    {
        while (m_interpreter.getExecutionState() != IntcodeProgramExecutionState::TERMINATED)
        {
            m_interpreter.addInput(static_cast<IntcodeNumberType>(getCurrentColor()));
            m_interpreter.execute();

            const auto& outputs = m_interpreter.getOutputs();
            assert(outputs.size() == 0 || outputs.size() == 2);

            if (outputs.size() == 2)
            {
                m_coordinatesToColor[m_currentCoordinates] = static_cast<PaintColor>(outputs.front());

                if (outputs.back() == 0)
                {
                    turnLeft();
                }
                else
                {
                    turnRight();
                }

                moveForward();
                m_interpreter.clearOutputs();
            }
        }
    }

    unsigned getNumPanelsPaintedAtLeastOnce()
    {
        return m_coordinatesToColor.size();
    }

private:
    IntcodeInterpreter m_interpreter;

    Coordinates m_currentCoordinates;
    Direction m_direction;

    CoordinatesMap m_coordinatesToColor;

    void moveForward()
    {
        switch (m_direction)
        {
            case Direction::LEFT:
                m_currentCoordinates.first -= 1;
                break;
            case Direction::RIGHT:
                m_currentCoordinates.first += 1;
                break;
            case Direction::UP:
                m_currentCoordinates.second += 1;
                break;
            case Direction::DOWN:
                m_currentCoordinates.second -= 1;
                break;
            default:
                throw std::runtime_error("Invalid direction");
        }
    }

    void turnLeft()
    {
        switch (m_direction)
        {
            case Direction::LEFT:
                m_direction = Direction::DOWN;
                break;
            case Direction::RIGHT:
                m_direction = Direction::UP;
                break;
            case Direction::UP:
                m_direction = Direction::LEFT;
                break;
            case Direction::DOWN:
                m_direction = Direction::RIGHT;
                break;
            default:
                throw std::runtime_error("Invalid direction");
        }
    }

    void turnRight()
    {
        for (int i = 1; i <= 3; ++i)
        {
            turnLeft();
        }
    }

    PaintColor getCurrentColor() const
    {
        const auto currentCoordinatesAndColorIter = m_coordinatesToColor.find(m_currentCoordinates);
        if (currentCoordinatesAndColorIter == m_coordinatesToColor.cend())
        {
            return PaintColor::BLACK;
        }
        return currentCoordinatesAndColorIter->second;
    }
};

unsigned numPanelsPaintedAtLeastOnce(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    HullPaintingRobot robot{std::move(interpreter)};

    robot.paint();

    return robot.getNumPanelsPaintedAtLeastOnce();
}

}
