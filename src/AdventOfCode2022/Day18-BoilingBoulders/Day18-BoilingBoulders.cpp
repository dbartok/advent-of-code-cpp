#include "Day18-BoilingBoulders.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2022
{
namespace Day18
{

using Coordinates = std::tuple<int, int, int>;

class LavaDropletAnalyzer
{
public:
    LavaDropletAnalyzer(std::vector<Coordinates> allLavaCubeCoordinates)
    {
        m_allLavaCubeCoordinates.insert(std::make_move_iterator(allLavaCubeCoordinates.begin()), std::make_move_iterator(allLavaCubeCoordinates.end()));
    }

    void analyze()
    {
        m_surfaceArea = m_allLavaCubeCoordinates.size() * 6;

        for (const auto& lavaCubeCoordinates : m_allLavaCubeCoordinates)
        {
            const std::vector<Coordinates> allNeighborCoordinates = getAllNeighborCoordinates(lavaCubeCoordinates);
            const unsigned numLavaCubeNeighbors = std::count_if(allNeighborCoordinates.cbegin(), allNeighborCoordinates.cend(), [this](const auto& neighborCoords)
                                                                {
                                                                    return this->m_allLavaCubeCoordinates.find(neighborCoords) != this->m_allLavaCubeCoordinates.cend();
                                                                });
            m_surfaceArea -= numLavaCubeNeighbors;
        }
    }

    unsigned getSurfaceArea() const
    {
        return m_surfaceArea;
    }

private:
    std::unordered_set<Coordinates, boost::hash<Coordinates>> m_allLavaCubeCoordinates;
    unsigned m_surfaceArea = 0;

    static std::vector<Coordinates> getAllNeighborCoordinates(const Coordinates& coordinates)
    {
        int x, y, z;
        std::tie(x, y, z) = coordinates;

        return std::vector<Coordinates>
        {
            {x + 1, y, z},
            {x - 1, y, z},
            {x, y + 1, z},
            {x, y - 1, z},
            {x, y, z + 1},
            {x, y, z - 1},
        };
    }
};

Coordinates parseLavaCubeCoordinatesLine(const std::string& lavaCubeCoordinatesLine)
{
    std::vector<int> coordinates;
    Utils::splitStringIntoTypedVector(lavaCubeCoordinatesLine, coordinates, ',');

    return Coordinates{coordinates.at(0), coordinates.at(1), coordinates.at(2)};
}

std::vector<Coordinates> parseLavaCubeCoordinatesLines(const std::vector<std::string>& lavaCubeCoordinatesLines)
{
    std::vector<Coordinates> allLavaCubeCoordinates;

    for (const auto& lavaCubeCoordinatesLine : lavaCubeCoordinatesLines)
    {
        Coordinates lavaCubeCoordinates = parseLavaCubeCoordinatesLine(lavaCubeCoordinatesLine);
        allLavaCubeCoordinates.push_back(std::move(lavaCubeCoordinates));
    }

    return allLavaCubeCoordinates;
}

unsigned surfaceAreaOfScannedLavaDroplet(const std::vector<std::string>& lavaCubeCoordinatesLines)
{
    std::vector<Coordinates> allLavaCubeCoordinates = parseLavaCubeCoordinatesLines(lavaCubeCoordinatesLines);

    LavaDropletAnalyzer lavaDropletAnalyzer{std::move(allLavaCubeCoordinates)};

    lavaDropletAnalyzer.analyze();

    return lavaDropletAnalyzer.getSurfaceArea();
}

}
}
}