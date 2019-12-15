#include "Day15-OxygenSystem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;

using CoordinatesToDistance = std::unordered_map<Coordinates, int, boost::hash<Coordinates>>;

enum class DroidStatusCode
{
    HIT_WALL = 0,
    MOVED = 1,
    MOVED_AND_OXYGEN_FOUND = 2,
};

enum class Direction
{
    NORTH = 1,
    SOUTH = 2,
    WEST = 3,
    EAST = 4,
};

class OxygenRepairDroid
{
public:
    OxygenRepairDroid(IntcodeInterpreter interpreter)
        : m_interpreter{std::move(interpreter)}
        , m_currentPosition{0, 0}
        , m_numStepsFromStart{0}
    {
        m_coordinatesToDistance[{0, 0}] = 0;
    }

    void traverse()
    {
        traverseRecursive();
    }

    void moveToOxygen()
    {
        traverseRecursive(true /* shouldHaltOnOxygen */);

        m_coordinatesToDistance.clear();
        m_coordinatesToDistance[m_oxygenCoordinates] = 0;
        m_numStepsFromStart = 0;
        m_halted = false;
    }

    int getDistanceToOxygen() const
    {
        return m_coordinatesToDistance.at({m_oxygenCoordinates});
    }

    int getMaxDistance() const
    {
        const auto maxElementIter = std::max_element(m_coordinatesToDistance.cbegin(), m_coordinatesToDistance.cend(), [](const auto& lhs, const auto& rhs)
                                                     {
                                                         return lhs.second < rhs.second;
                                                     });

        return maxElementIter->second;
    }

private:
    IntcodeInterpreter m_interpreter;

    Coordinates m_currentPosition;
    int m_numStepsFromStart;
    CoordinatesToDistance m_coordinatesToDistance;

    Coordinates m_oxygenCoordinates;

    bool m_halted = false;

    void traverseRecursive(bool shouldHaltOnOxygen = false)
    {
        for (auto direction : getAllDirections())
        {
            if (isHalted())
            {
                return;
            }

            DroidStatusCode statusCode = tryMove(direction);

            if (statusCode == DroidStatusCode::HIT_WALL)
            {
                continue;
            }

            if (updateDataAfterMove(direction))
            {
                traverseRecursive(shouldHaltOnOxygen);
            }

            if (statusCode == DroidStatusCode::MOVED_AND_OXYGEN_FOUND)
            {
                m_oxygenCoordinates = m_currentPosition;

                if (shouldHaltOnOxygen)
                {
                    m_halted = true;
                }
            }

            if (!isHalted())
            {
                undoMove(direction);
            }
        }
    }

    DroidStatusCode tryMove(Direction direction)
    {
        m_interpreter.addInput(static_cast<IntcodeNumberType>(direction));
        m_interpreter.execute();
        int output = m_interpreter.getOutputs().back();
        return static_cast<DroidStatusCode>(output);
    }

    bool updateDataAfterMove(Direction direction)
    {
        ++m_numStepsFromStart;
        updatePosition(direction);
        bool isNewLowestDistance = updateMap();
        return isNewLowestDistance;
    }

    void updatePosition(Direction direction)
    {
        switch (direction)
        {
            case Direction::WEST:
                m_currentPosition.first -= 1;
                break;
            case Direction::EAST:
                m_currentPosition.first += 1;
                break;
            case Direction::NORTH:
                m_currentPosition.second -= 1;
                break;
            case Direction::SOUTH:
                m_currentPosition.second += 1;
                break;
            default:
                throw std::runtime_error("Invalid direction");
        }
    }

    bool updateMap()
    {
        auto findResult = m_coordinatesToDistance.find(m_currentPosition);
        if (findResult == m_coordinatesToDistance.end())
        {
            m_coordinatesToDistance.emplace(m_currentPosition, m_numStepsFromStart);
            return true;
        }

        if (findResult->second > m_numStepsFromStart)
        {
            findResult->second = m_numStepsFromStart;
            return true;
        }

        return false;
    }

    void undoMove(Direction direction)
    {
        --m_numStepsFromStart;

        Direction oppositeDirection = getOppositeDirection(direction);
        DroidStatusCode statusCode = tryMove(oppositeDirection);
        assert(statusCode != DroidStatusCode::HIT_WALL);

        updatePosition(oppositeDirection);
    }

    bool isHalted() const
    {
        return m_halted || m_interpreter.getExecutionState() == IntcodeProgramExecutionState::TERMINATED;
    }

    static std::vector<Direction> getAllDirections()
    {
        return {Direction::NORTH, Direction::SOUTH, Direction::WEST, Direction::EAST};
    }

    static Direction getOppositeDirection(Direction direction)
    {
        switch (direction)
        {
            case Direction::WEST:
                return Direction::EAST;
            case Direction::EAST:
                return Direction::WEST;
            case Direction::NORTH:
                return Direction::SOUTH;
            case Direction::SOUTH:
                return Direction::NORTH;
            default:
                throw std::runtime_error("Invalid direction");
        }
    }
};

int fewestNumberOfMovementsToOxygen(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    OxygenRepairDroid droid{std::move(interpreter)};

    droid.traverse();

    return droid.getDistanceToOxygen();
}

int numMinutesUntilOxygenSpreads(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    OxygenRepairDroid droid{std::move(interpreter)};

    droid.moveToOxygen();
    droid.traverse();

    return droid.getMaxDistance();
}

}
