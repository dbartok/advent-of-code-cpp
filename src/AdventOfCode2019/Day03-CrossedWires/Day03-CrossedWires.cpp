#include "Day03-CrossedWires.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct WireSection
{
    Direction direction;
    unsigned length;
};

struct Wire
{
    std::vector<WireSection> wireSections;
};

using WireID = unsigned;

struct FieldWireInfo
{
    std::unordered_set<WireID> wires;
    int numTotalWireSteps = 0;
};

class WirePlanter
{
public:
    WirePlanter(std::vector<Wire> wires)
        : m_wires{std::move(wires)}
    {

    }

    void plant()
    {
        WireID id = 0;

        for (const auto& wire : m_wires)
        {
            Coordinates currentCoordinates{0, 0};
            unsigned numWireSteps = 0;

            for (const auto& wireSection : wire.wireSections)
            {
                for (size_t i = 0; i < wireSection.length; ++i)
                {
                    ++numWireSteps;

                    currentCoordinates = getNeighborCoordinate(currentCoordinates, wireSection.direction);

                    bool isNotSelfIntersection = m_coordinatesToWires[currentCoordinates].wires.insert(id).second;
                    if (isNotSelfIntersection)
                    {
                        m_coordinatesToWires[currentCoordinates].numTotalWireSteps += numWireSteps;
                    }
                }
            }

            ++id;
        }
    }

    int getDistanceToClosestIntersection() const
    {
        std::vector<Coordinates> allIntersectionCoordinates = getAllIntersectionCoordinates();

        auto minDistanceCoordinates = *std::min_element(allIntersectionCoordinates.cbegin(), allIntersectionCoordinates.cend(), [](const auto& lhs, const auto& rhs) noexcept
                                                        {
                                                            return std::abs(lhs.first) + std::abs(lhs.second) < std::abs(rhs.first) + std::abs(rhs.second);
                                                        });

        return std::abs(minDistanceCoordinates.first) + std::abs(minDistanceCoordinates.second);
    }

    int getCombinedWireStepsToClosestIntersection() const
    {
        std::vector<Coordinates> allIntersectionCoordinates = getAllIntersectionCoordinates();

        auto minCombinedWireStepsCoordinates = *std::min_element(allIntersectionCoordinates.cbegin(), allIntersectionCoordinates.cend(), [this](const auto& lhs, const auto& rhs)
                                                                 {
                                                                     const auto& lhsFieldWireInfo = this->m_coordinatesToWires.at(lhs);
                                                                     const auto& rhsFieldWireInfo = this->m_coordinatesToWires.at(rhs);
                                                                     return lhsFieldWireInfo.numTotalWireSteps < rhsFieldWireInfo.numTotalWireSteps;
                                                                 });

        return m_coordinatesToWires.at(minCombinedWireStepsCoordinates).numTotalWireSteps;
    }

private:
    using Coordinates = std::pair<int, int>;

    std::vector<Wire> m_wires;

    std::unordered_map<Coordinates, FieldWireInfo, boost::hash<Coordinates>> m_coordinatesToWires;

    std::vector<Coordinates> getAllIntersectionCoordinates() const
    {
        std::vector<Coordinates> allIntersectionCoordinates;

        for (const auto& coordinatesAndWires : m_coordinatesToWires)
        {
            if (coordinatesAndWires.second.wires.size() > 1)
            {
                allIntersectionCoordinates.push_back(coordinatesAndWires.first);
            }
        }

        return allIntersectionCoordinates;
    }

    static Coordinates getNeighborCoordinate(const Coordinates& coordinates, Direction direction)
    {
        switch (direction)
        {
            case Direction::UP:
                return Coordinates{coordinates.first, coordinates.second - 1};
            case Direction::DOWN:
                return Coordinates{coordinates.first, coordinates.second + 1};
            case  Direction::LEFT:
                return Coordinates{coordinates.first - 1, coordinates.second};
            case Direction::RIGHT:
                return Coordinates{coordinates.first + 1, coordinates.second};
        }

        throw std::runtime_error("Unknown direction: " + std::to_string(static_cast<int>(direction)));
    }
};

Direction parseDirection(char directionChar)
{
    switch (directionChar)
    {
        case 'U':
            return Direction::UP;
        case 'D':
            return Direction::DOWN;
        case 'L':
            return Direction::LEFT;
        case 'R':
            return Direction::RIGHT;
        default:
            throw std::runtime_error("Unknown direction: " + std::string{directionChar});
    }
}

WireSection parseWireSection(const std::string& wireLine)
{
    const char directionChar = wireLine.at(0);
    const Direction direction = parseDirection(directionChar);

    std::string lengthString = wireLine.substr(1);
    const unsigned length = boost::lexical_cast<unsigned>(lengthString);

    return WireSection{direction, length};
}

Wire parseWire(const std::string& wireLine)
{
    std::vector<std::string> wireSectionStrings;
    Utils::splitStringIntoTypedVector(wireLine, wireSectionStrings, ',');

    std::vector<WireSection> wireSections;

    for (const auto& wireSectionString : wireSectionStrings)
    {
        wireSections.push_back(parseWireSection(wireSectionString));
    }

    return Wire{wireSections};
}

std::vector<Wire> parseWires(const std::vector<std::string>& wireLines)
{
    std::vector<Wire> wires;

    for (const auto& wireLine : wireLines)
    {
        wires.push_back(parseWire(wireLine));
    }

    return wires;
}


int distanceToClosestIntersection(const std::vector<std::string>& wireLines)
{
    std::vector<Wire> wires = parseWires(wireLines);

    WirePlanter wp{wires};

    wp.plant();

    return wp.getDistanceToClosestIntersection();
}

int combinedWireStepsToClosestIntersection(const std::vector<std::string>& wireLines)
{
    std::vector<Wire> wires = parseWires(wireLines);

    WirePlanter wp{wires};

    wp.plant();

    return wp.getCombinedWireStepsToClosestIntersection();
}

}
