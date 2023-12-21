#include "Day21-StepCounter.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <unordered_set>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

char START = 'S';
char ROCK = '#';

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day21
{

using Coordinates = std::pair<int, int>;

class GardenTraverser
{
public:
    GardenTraverser(std::vector<std::string> map)
        : m_map{std::move(map)}
        , m_width{m_map.front().size()}
        , m_height{m_map.size()}
    {
        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
            {
                if (getSoilTypeAt({i, j}) == START)
                {
                    m_startCoordinates = {i, j};
                }
            }
        }
    }

    void traverse(int numSteps)
    {
        CoordinatesSet currentReachableCoordinatesQueue;
        CoordinatesSet nextReachableCoordinatesQueue;
        nextReachableCoordinatesQueue.insert(m_startCoordinates);

        for (int step = 0; step < numSteps; ++step)
        {
            std::swap(currentReachableCoordinatesQueue, nextReachableCoordinatesQueue);

            while (!currentReachableCoordinatesQueue.empty())
            {
                const Coordinates currentCoordinates = *currentReachableCoordinatesQueue.begin();
                currentReachableCoordinatesQueue.erase(currentReachableCoordinatesQueue.begin());

                std::vector<Coordinates> allReachableNeighborCoordinates = getAllReachableNeighborCoordinates(currentCoordinates);

                for (const auto& reachableNeighborCoordinates : allReachableNeighborCoordinates)
                {
                    nextReachableCoordinatesQueue.insert(reachableNeighborCoordinates);
                }
            }
        }

        m_numReachablePositions = nextReachableCoordinatesQueue.size();
    }

    int getNumReachablePositions() const
    {
        return m_numReachablePositions;
    }

private:
    using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

    const std::vector<std::string> m_map;
    const size_t m_width;
    const size_t m_height;

    Coordinates m_startCoordinates;

    int m_numReachablePositions;

    char getSoilTypeAt(const Coordinates& coordinates) const
    {
        return m_map.at(mod(coordinates.second, m_height)).at(mod(coordinates.first, m_width));
    }

    std::vector<Coordinates> getAllReachableNeighborCoordinates(const Coordinates& baseCoordinates)
    {
        const std::vector<Coordinates> possibleNeighborCoordinates =
        {
            Coordinates{baseCoordinates.first, baseCoordinates.second - 1},
            Coordinates{baseCoordinates.first, baseCoordinates.second + 1},
            Coordinates{baseCoordinates.first - 1, baseCoordinates.second},
            Coordinates{baseCoordinates.first + 1, baseCoordinates.second}
        };

        std::vector<Coordinates> reachableNeighborCoordinates;

        std::copy_if(possibleNeighborCoordinates.cbegin(),
                     possibleNeighborCoordinates.cend(),
                     std::back_inserter(reachableNeighborCoordinates),
                     [this](const auto& neighborCoords)
                     {
                         return this->getSoilTypeAt(neighborCoords) != ROCK;
                     });

        return reachableNeighborCoordinates;
    }

    static int mod(int dividend, int divisor)
    {
        int modulus = dividend % divisor;

        if (modulus < 0)
        {
            modulus += divisor;
        }

        return modulus;
    }
};

// Calculate value at x according to Lagrange interpolation based on an array of known points in the function
// https://en.wikipedia.org/wiki/Polynomial_interpolation#Constructing_the_interpolation_polynomial
int64_t getInterpolatedValue(std::vector<Coordinates> knownPoints, int x)
{
    int64_t interpolatedValue = 0;

    for (int i = 0; i < knownPoints.size(); ++i)
    {
        int64_t currentTerm = 1;

        for (int j = 0; j < knownPoints.size(); ++j)
        {
            if (j != i)
            {
                currentTerm *= (x - knownPoints.at(j).first) / (knownPoints.at(i).first - knownPoints.at(j).first);
            }
        }

        currentTerm *= knownPoints.at(i).second;
        interpolatedValue += currentTerm;
    }

    return interpolatedValue;
}

int numGardenPlotsReachableAfterSteps(const std::vector<std::string>& mapLines, int numSteps)
{
    GardenTraverser gardenTraverser{mapLines};

    gardenTraverser.traverse(numSteps);

    return gardenTraverser.getNumReachablePositions();
}

int64_t numGardenPlotsReachableAfterSeveralSteps(const std::vector<std::string>& mapLines, int numSteps)
{
    const int gridSize = mapLines.size();
    const int offset = gridSize / 2;

    assert(numSteps % gridSize == offset);

    std::vector<Coordinates> firstFewCoordinatesOfGrowthFunction;
    for (int x = 0; x < 3; ++x)
    {
        const unsigned numCurrentSteps = x * gridSize + offset;

        GardenTraverser gardenTraverser{mapLines};
        gardenTraverser.traverse(numCurrentSteps);
        const int numReachablePositions = gardenTraverser.getNumReachablePositions();

        firstFewCoordinatesOfGrowthFunction.emplace_back(x, numReachablePositions);
    }

    const int desiredXCoordinateOfGrowthFunction = (numSteps - offset) / gridSize;

    return getInterpolatedValue(firstFewCoordinatesOfGrowthFunction, desiredXCoordinateOfGrowthFunction);
}

}
}
}