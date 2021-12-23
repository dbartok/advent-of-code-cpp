#include "Day23-Amphipod.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include <unordered_set>
#include <iterator>
#include <queue>
#include <cassert>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int ROOM_START_Y = 2;
const int ROOM_START_X = 3;
const char MIN_AMPHIPOD_CHAR = 'A';
const char MAX_AMPHIPOD_CHAR = 'D';
const char WALL_CHAR = '#';
const char EMPTY_SPACE_CHAR = '.';

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day23
{

using MapRow = std::string;
using Map = std::vector<MapRow>;
using Coordinates = std::pair<int, int>;
using CoordinatesAndDistance = std::pair<Coordinates, int>;

const Map EXTRA_LINES_PART_TWO =
{
    "  #D#C#B#A#",
    "  #D#B#A#C#"
};
const size_t EXTRA_LINES_INSERTION_OFFSET_PART_TWO = 3;

class State
{
public:
    State(Map map, unsigned totalCost)
        : m_map{std::move(map)}
        , m_totalCost{totalCost}
    {

    }

    void move(const Coordinates& coordinatesToMove, const Coordinates& destinationCoordinates)
    {
        std::swap(getValueAt(coordinatesToMove), getValueAt(destinationCoordinates));
    }

    void increaseCost(unsigned costDelta)
    {
        m_totalCost += costDelta;
    }

    bool isAmphipod(const Coordinates& coordinates) const
    {
        return getValueAt(coordinates) >= MIN_AMPHIPOD_CHAR && getValueAt(coordinates) <= MAX_AMPHIPOD_CHAR;
    }

    bool isOrganized() const
    {
        for (char amphipodChar = MIN_AMPHIPOD_CHAR; amphipodChar <= MAX_AMPHIPOD_CHAR; ++amphipodChar)
        {
            if (!isAmphipodOrganized(amphipodChar))
            {
                return false;
            }
        }

        return true;
    }

    unsigned getSingleMoveCost(const Coordinates& coordinates) const
    {
        if (!isAmphipod(coordinates))
        {
            throw std::runtime_error("Only amphipods have move costs");
        }

        const char amphipodChar = getValueAt(coordinates);

        return static_cast<unsigned>(pow(10, amphipodChar - MIN_AMPHIPOD_CHAR));
    }

    bool isValidEndDestination(const Coordinates& coordinatesToMove, const Coordinates& destinationCoordinates) const
    {
        if (!isEmptySpace(destinationCoordinates))
        {
            return false;
        }

        if (isHallway(coordinatesToMove))
        {
            return isCorrectRoom(coordinatesToMove, destinationCoordinates);
        }
        else
        {
            return isHallway(destinationCoordinates);
        }
    }

    bool isEmptySpace(const Coordinates& coordinates) const
    {
        return getValueAt(coordinates) == EMPTY_SPACE_CHAR;
    }

    unsigned getTotalCost() const
    {
        return m_totalCost;
    }

    const Map& getMap() const
    {
        return m_map;
    }

    bool operator==(const State& other) const
    {
        return m_map == other.m_map;
    }

private:
    Map m_map;
    unsigned m_totalCost;

    const char& getValueAt(const Coordinates& coordinates) const
    {
        return m_map.at(coordinates.second).at(coordinates.first);
    }

    char& getValueAt(const Coordinates& coordinates)
    {
        return const_cast<char&>(const_cast<const State&>(*this).getValueAt(coordinates));
    }

    bool isAmphipodOrganized(char amphipodChar) const
    {
        return isOrganizedStartingFrom(amphipodChar, ROOM_START_Y);
    }

    bool isHallway(const Coordinates& coordinates) const
    {
        if (coordinates.second >= ROOM_START_Y)
        {
            return false;
        }

        for (char amphipodChar = MIN_AMPHIPOD_CHAR; amphipodChar <= MAX_AMPHIPOD_CHAR; ++amphipodChar)
        {
            const int amphipodRoomX = getAmphipodRoomX(amphipodChar);
            if (coordinates.first == amphipodRoomX)
            {
                return false;
            }
        }

        return true;
    }

    bool isCorrectRoom(const Coordinates& coordinatesToMove, const Coordinates& destinationCoordinates) const
    {
        const char amphipodChar = getValueAt(coordinatesToMove);
        const int amphipodRoomX = getAmphipodRoomX(amphipodChar);
        if (amphipodRoomX != destinationCoordinates.first)
        {
            return false;
        }

        return isOrganizedStartingFrom(amphipodChar, destinationCoordinates.second + 1);
    }

    bool isOrganizedStartingFrom(char amphipodChar, int amphipodRoomY) const
    {
        const int amphipodRoomX = getAmphipodRoomX(amphipodChar);

        while (true)
        {
            if (getValueAt({amphipodRoomX, amphipodRoomY}) == WALL_CHAR)
            {
                return true;
            }

            if (getValueAt({amphipodRoomX, amphipodRoomY}) != amphipodChar)
            {
                return false;
            }

            ++amphipodRoomY;
        }
    }

    static int getAmphipodRoomX(char amphipodChar)
    {
        const int amphipodIndex = amphipodChar - MIN_AMPHIPOD_CHAR;
        return ROOM_START_X + 2 * amphipodIndex;
    }
};

std::size_t hash_value(const State& state)
{
    boost::hash<Map> hasher;
    return hasher(state.getMap());
}

class AmphipodOrganizer
{
public:
    AmphipodOrganizer(Map initialMap)
        : m_width{initialMap.at(0).size()}
        , m_height{initialMap.size()}
    {
        m_activeStates.insert({initialMap, 0});
        m_visitedStates.insert({std::move(initialMap), 0});
    }

    void organize()
    {
        while (!m_activeStates.empty())
        {
            auto currentState = *m_activeStates.cbegin();
            m_activeStates.erase(m_activeStates.cbegin());

            if (currentState.isOrganized())
            {
                m_minOrganizedStateCost = std::min(m_minOrganizedStateCost.get_value_or(std::numeric_limits<unsigned>::max()), currentState.getTotalCost());
            }
            else if (m_minOrganizedStateCost.has_value() && currentState.getTotalCost() > m_minOrganizedStateCost.value())
            {
                continue;
            }

            auto nextStates = generateNextStates(currentState);
            insertNextStates(std::move(nextStates));
        }
    }

    unsigned getMinEnergyUsed() const
    {
        return m_minOrganizedStateCost.value();
    }

private:
    std::unordered_set<State, boost::hash<State>> m_visitedStates;
    std::unordered_set<State, boost::hash<State>> m_activeStates;
    size_t m_width;
    size_t m_height;

    boost::optional<unsigned> m_minOrganizedStateCost;

    std::vector<State> generateNextStates(const State& currentState)
    {
        std::vector<State> nextStates;

        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
            {
                std::vector<State> nextStatesByMovingTheseCoordinates = generateNextStatesByMoving(currentState, {i, j});
                nextStates.insert(nextStates.end(), std::make_move_iterator(nextStatesByMovingTheseCoordinates.begin()), std::make_move_iterator(nextStatesByMovingTheseCoordinates.end()));
            }
        }

        return nextStates;
    }

    void insertNextStates(std::vector<State> nextStates)
    {
        for (auto& nextState : nextStates)
        {
            auto findResult = m_visitedStates.find(nextState);
            if (findResult == m_visitedStates.end())
            {
                m_activeStates.insert(nextState);
                m_visitedStates.insert(std::move(nextState));
            }
            else
            {
                if (findResult->getTotalCost() > nextState.getTotalCost())
                {
                    m_activeStates.erase(*findResult);
                    m_activeStates.insert(nextState);
                    m_visitedStates.erase(findResult);
                    m_visitedStates.insert(std::move(nextState));
                }
            }
        }
    }

    std::vector<State> generateNextStatesByMoving(const State& currentState, const Coordinates& coordinatesToMove)
    {
        if (!currentState.isAmphipod(coordinatesToMove))
        {
            return {};
        }

        std::vector<State> nextStates;

        std::unordered_set<Coordinates, boost::hash<Coordinates>> visited;
        visited.insert(coordinatesToMove);
        std::queue<CoordinatesAndDistance> bfsQueue;
        bfsQueue.push({coordinatesToMove, 0});

        while (!bfsQueue.empty())
        {
            auto currentNode = bfsQueue.front();
            bfsQueue.pop();

            if (currentState.isValidEndDestination(coordinatesToMove, currentNode.first))
            {
                State currentStateCopy{currentState};
                currentStateCopy.move(coordinatesToMove, currentNode.first);
                currentStateCopy.increaseCost(currentNode.second * currentState.getSingleMoveCost(coordinatesToMove));
                nextStates.push_back(std::move(currentStateCopy));
            }

            const std::vector<Coordinates> allNeighborCoordinates = getAllNeighborCoordinates(currentNode.first);
            for (const auto& neighborCoordinates : allNeighborCoordinates)
            {
                auto insertResult = visited.insert(neighborCoordinates);
                if (!insertResult.second)
                {
                    continue;
                }

                if (currentState.isEmptySpace(neighborCoordinates))
                {
                    bfsQueue.push({neighborCoordinates, currentNode.second + 1});
                }
            }
        }

        return nextStates;
    }

    static std::vector<Coordinates> getAllNeighborCoordinates(const Coordinates& coordinates)
    {
        return
        {
            Coordinates{coordinates.first, coordinates.second - 1},
            Coordinates{coordinates.first, coordinates.second + 1},
            Coordinates{coordinates.first - 1, coordinates.second},
            Coordinates{coordinates.first + 1, coordinates.second}
        };
    }
};

Map parseInitialPositionsLines(const std::vector<std::string>& initialPositionsLines)
{
    Map map = Map(initialPositionsLines.size(), MapRow(initialPositionsLines.at(0).size(), WALL_CHAR));

    for (int j = 0; j < initialPositionsLines.size(); ++j)
    {
        for (int i = 0; i < initialPositionsLines.at(j).size(); ++i)
        {
            const char initialPositionsChar = initialPositionsLines.at(j).at(i);
            if (initialPositionsChar != ' ')
            {
                map.at(j).at(i) = initialPositionsLines.at(j).at(i);
            }
        }
    }

    return map;
}

std::vector<std::string> getExtendedLines(const std::vector<std::string>& initialPositionsLines)
{
    auto extendedLines{initialPositionsLines};
    extendedLines.insert(extendedLines.begin() + EXTRA_LINES_INSERTION_OFFSET_PART_TWO, EXTRA_LINES_PART_TWO.cbegin(), EXTRA_LINES_PART_TWO.cend());
    return extendedLines;
}

unsigned leastEnergyRequiredToOrganize(const std::vector<std::string>& initialPositionsLines)
{
    Map map = parseInitialPositionsLines(initialPositionsLines);
    AmphipodOrganizer organizer{std::move(map)};
    organizer.organize();
    return organizer.getMinEnergyUsed();
}

unsigned leastEnergyRequiredToOrganizeExtendedMap(const std::vector<std::string>& initialPositionsLines)
{
    std::vector<std::string> extendedLines = getExtendedLines(initialPositionsLines);
    Map map = parseInitialPositionsLines(std::move(extendedLines));
    AmphipodOrganizer organizer{std::move(map)};
    organizer.organize();
    return organizer.getMinEnergyUsed();
}

}
}
}