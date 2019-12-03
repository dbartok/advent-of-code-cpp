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

            for (const auto& wireSection : wire.wireSections)
            {
                for (size_t i = 0; i < wireSection.length; ++i)
                {
                    currentCoordinates = getNeighborCoordinate(currentCoordinates, wireSection.direction);
                    m_coordinatesToWires[currentCoordinates].insert(id);
                }
            }

            ++id;
        }
    }

    int getDistanceToClosestIntersection() const
    {
        std::vector<Coordinates> intersectionCoordinates;

        for (const auto& coordinatesAndWires : m_coordinatesToWires)
        {
            if (coordinatesAndWires.second.size() > 1)
            {
                intersectionCoordinates.push_back(coordinatesAndWires.first);
            }
        }

        auto minDistanceCoord = *std::min_element(intersectionCoordinates.cbegin(), intersectionCoordinates.cend(), [](const auto& lhs, const auto& rhs) noexcept
                                                  {
                                                      return std::abs(lhs.first) + std::abs(lhs.second) < std::abs(rhs.first) + std::abs(rhs.second);
                                                  });

        return std::abs(minDistanceCoord.first) + std::abs(minDistanceCoord.second);
    }

private:
    using Coordinates = std::pair<int, int>;
    using WireID = unsigned;

    std::vector<Wire> m_wires;

    std::unordered_map<Coordinates, std::unordered_set<WireID>, boost::hash<Coordinates>> m_coordinatesToWires;

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

}
