#include "Day09-SmokeBasin.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <iterator>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day09
{

using HeightmapRow = std::vector<int>;
using Heightmap = std::vector<HeightmapRow>;

using Coordinates = std::pair<int, int>;

class HeightmapAnalyzer
{
public:
    HeightmapAnalyzer(Heightmap heightmap)
        : m_heightmap{std::move(heightmap)}
        , m_width{m_heightmap.at(0).size()}
        , m_height{m_heightmap.size()}
    {

    }

    int getSumOfRiskLevelsOfAllLowPoints() const
    {
        const std::vector<Coordinates> allLowPointCoordinates = getAllLowPointCoordinates();

        return std::accumulate(allLowPointCoordinates.cbegin(), allLowPointCoordinates.cend(), 0, [this](const auto& acc, const auto& point)
                               {
                                   return acc + this->getHeightAt(point) + 1;
                               });
    }

private:
    const Heightmap m_heightmap;

    const size_t m_width;
    const size_t m_height;

    std::vector<Coordinates> getAllLowPointCoordinates() const
    {
        std::vector<Coordinates> lowPointCoordinates;

        for (size_t j = 0; j < m_height; ++j)
        {
            for (size_t i = 0; i < m_width; ++i)
            {
                Coordinates currentCoordinates{i, j};
                if (isLowPoint(currentCoordinates))
                {
                    lowPointCoordinates.push_back(currentCoordinates);
                }
            }
        }

        return lowPointCoordinates;
    }

    bool isLowPoint(const Coordinates& coordinates) const
    {
        const std::vector<Coordinates> allAdjacentLocationCoordinates = getAllAdjacentLocationCoordinates(coordinates);
        const int currentHeight = getHeightAt(coordinates);

        return std::all_of(allAdjacentLocationCoordinates.cbegin(), allAdjacentLocationCoordinates.cend(), [this, currentHeight](const auto& adjacentLocationCoordinates)
                            {
                                return currentHeight < this->getHeightAt(adjacentLocationCoordinates);
                            });
    }

    int getHeightAt(const Coordinates& coordinates) const
    {
        return m_heightmap.at(coordinates.second).at(coordinates.first);
    }

    std::vector<Coordinates> getAllAdjacentLocationCoordinates(const Coordinates& baseCoordinates) const
    {
        const std::vector<Coordinates> possibleAdjacentLocationCoordinates =
        {
            Coordinates{baseCoordinates.first, baseCoordinates.second - 1},
            Coordinates{baseCoordinates.first, baseCoordinates.second + 1},
            Coordinates{baseCoordinates.first - 1, baseCoordinates.second},
            Coordinates{baseCoordinates.first + 1, baseCoordinates.second}
        };

        std::vector<Coordinates> inBoundsAdjacentLocationCoordinates;

        std::copy_if(possibleAdjacentLocationCoordinates.cbegin(),
                     possibleAdjacentLocationCoordinates.cend(),
                     std::back_inserter(inBoundsAdjacentLocationCoordinates),
                     [this](const auto& coords)
                     {
                         return this->isInBounds(coords);
                     });

        return inBoundsAdjacentLocationCoordinates;
    }

    bool isInBounds(const Coordinates& coordinates) const
    {
        return coordinates.first >= 0 && coordinates.first < m_width&& coordinates.second >= 0 && coordinates.second < m_height;
    }
};

HeightmapRow parseHeightmapLine(const std::string& heightmapLine)
{
    HeightmapRow heightmapRow;

    std::transform(heightmapLine.cbegin(), heightmapLine.cend(), std::back_inserter(heightmapRow), [](const char c)
                   {
                       return c - '0';
                   });

    return heightmapRow;
}

Heightmap parseHeightmapLines(const std::vector<std::string>& heightmapLines)
{
    Heightmap heightmap;

    for (const auto& line : heightmapLines)
    {
        HeightmapRow heightmapRow = parseHeightmapLine(line);
        heightmap.push_back(std::move(heightmapRow));
    }
    
    return heightmap;
}

int sumOfRiskLevelsOfAllLowPoints(const std::vector<std::string>& heightmapLines)
{
    Heightmap heightmap = parseHeightmapLines(heightmapLines);
    HeightmapAnalyzer heightmapAnalyzer{std::move(heightmap)};
    return heightmapAnalyzer.getSumOfRiskLevelsOfAllLowPoints();
}

}
}
}