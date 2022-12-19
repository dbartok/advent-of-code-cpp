#include "Day18-BoilingBoulders.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

#include <unordered_set>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const size_t NUM_DIMENSIONS = 3;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day18
{

using Coordinates = std::array<int, NUM_DIMENSIONS>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

class LavaDropletAnalyzer
{
public:
    LavaDropletAnalyzer(std::vector<Coordinates> allLavaCubeCoordinates)
    {
        m_allLavaCubeCoordinates.insert(std::make_move_iterator(allLavaCubeCoordinates.begin()), std::make_move_iterator(allLavaCubeCoordinates.end()));

        const auto bounds = getBounds();
        m_lowerBound = bounds.first;
        m_upperBound = bounds.second;
    }

    unsigned getSurfaceArea() const
    {
        unsigned surfaceArea = m_allLavaCubeCoordinates.size() * 6;

        for (const auto& lavaCubeCoordinates : m_allLavaCubeCoordinates)
        {
            const std::vector<Coordinates> allNeighborCoordinates = getAllNeighborCoordinates(lavaCubeCoordinates);
            const unsigned numLavaCubeNeighbors = std::count_if(allNeighborCoordinates.cbegin(), allNeighborCoordinates.cend(), [this](const auto& neighborCoords)
                                                                {
                                                                    return this->m_allLavaCubeCoordinates.find(neighborCoords) != this->m_allLavaCubeCoordinates.cend();
                                                                });
            surfaceArea -= numLavaCubeNeighbors;
        }

        return surfaceArea;
    }

    unsigned getOuterSurfaceArea() const
    {
        std::queue<Coordinates> bfsQueue;
        bfsQueue.push(m_lowerBound);
        CoordinatesSet visited{m_lowerBound};
        unsigned numLavaCubeSidesEncountered = 0;

        while (!bfsQueue.empty())
        {
            const Coordinates currentCoordinates = bfsQueue.front();
            bfsQueue.pop();

            std::vector<Coordinates> allInBoundsNeighborCoordinates = getInBoundsNeighborCoordinates(currentCoordinates);

            for (const auto& neighborCoordinates : allInBoundsNeighborCoordinates)
            {
                if (m_allLavaCubeCoordinates.find(neighborCoordinates) != m_allLavaCubeCoordinates.cend())
                {
                    ++numLavaCubeSidesEncountered;
                }
                else if (visited.insert(neighborCoordinates).second)
                {
                    bfsQueue.push(neighborCoordinates);
                }
            }
        }

        return numLavaCubeSidesEncountered;
    }

private:
    CoordinatesSet m_allLavaCubeCoordinates;
    Coordinates m_lowerBound;
    Coordinates m_upperBound;

    std::vector<Coordinates> getInBoundsNeighborCoordinates(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> allNeighborCoordinates = getAllNeighborCoordinates(coordinates);

        std::vector<Coordinates> inBoundsNeighborCoordinates;

        std::copy_if(std::make_move_iterator(allNeighborCoordinates.begin()), std::make_move_iterator(allNeighborCoordinates.end()), std::back_inserter(inBoundsNeighborCoordinates), [this](const auto& coords)
                     {
                         for (size_t i = 0; i < NUM_DIMENSIONS; ++i)
                         {
                             const int currentCoordinate = coords.at(i);
                             const int lowerBoundForCoordinate = this->m_lowerBound.at(i);
                             const int upperBoundForCoordinate = this->m_upperBound.at(i);

                             if (currentCoordinate < lowerBoundForCoordinate || currentCoordinate > upperBoundForCoordinate)
                             {
                                 return false;
                             }
                         }

                         return true;
                     });

        return inBoundsNeighborCoordinates;
    }

    std::pair<Coordinates, Coordinates> getBounds() const
    {
        Coordinates lowerBoundCoordinates;
        Coordinates upperBoundsCoordinates;

        for (int i = 0; i < NUM_DIMENSIONS; ++i)
        {
            const auto minmaxIter = std::minmax_element(m_allLavaCubeCoordinates.cbegin(), m_allLavaCubeCoordinates.cend(), [i](const Coordinates& lhs, const Coordinates& rhs)
                                                        {
                                                            return lhs.at(i) < rhs.at(i);
                                                        });

            const int min = minmaxIter.first->at(i);
            const int max = minmaxIter.second->at(i);

            // Leave an extra offset of 1 around the min and the max
            // This is to make sure that we also discover the outermost sides of the droplet in our search
            lowerBoundCoordinates.at(i) = min - 1;
            upperBoundsCoordinates.at(i) = max + 1;
        }

        return {std::move(lowerBoundCoordinates), std::move(upperBoundsCoordinates)};
    }

    static std::vector<Coordinates> getAllNeighborCoordinates(const Coordinates& coordinates)
    {
        const int x = coordinates.at(0);
        const int y = coordinates.at(1);
        const int z = coordinates.at(2);

        return std::vector<Coordinates>
        {
            { x + 1, y, z },
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

    return lavaDropletAnalyzer.getSurfaceArea();
}

unsigned outerSurfaceAreaOfScannedLavaDroplet(const std::vector<std::string>& lavaCubeCoordinatesLines)
{
    std::vector<Coordinates> allLavaCubeCoordinates = parseLavaCubeCoordinatesLines(lavaCubeCoordinatesLines);

    LavaDropletAnalyzer lavaDropletAnalyzer{std::move(allLavaCubeCoordinates)};

    return lavaDropletAnalyzer.getOuterSurfaceArea();
}

}
}
}