#include "Day23-UnstableDiffusion.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <deque>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned NUM_DAYS_TO_SIMULATE_FIRST_PART = 10;
const char OCCUPIED_POSITION = '#';

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day23
{

class ElfMovementSimulator
{
public:
    ElfMovementSimulator(const std::vector<std::string>& initialPositions)
    {
        for (int j = 0; j < initialPositions.size(); ++j)
        {
            for (int i = 0; i < initialPositions.at(j).size(); ++i)
            {
                if (initialPositions.at(j).at(i) == OCCUPIED_POSITION)
                {
                    m_occupiedPositions.insert({i, j});
                }
            }
        }
    }

    void simulate(unsigned numMaxRoundsToSimulate = 0)
    {
        for (m_numRoundsSimulated = 1; numMaxRoundsToSimulate == 0 || m_numRoundsSimulated <= numMaxRoundsToSimulate; ++m_numRoundsSimulated)
        {
            if (!simulateRound())
            {
                break;
            }
        };
    }

    unsigned getNumGroundTilesInBoundingRectangle() const
    {
        const auto minmaxPositionXIter = std::minmax_element(m_occupiedPositions.cbegin(), m_occupiedPositions.cend(), [](const auto& lhs, const auto& rhs)
                                                             {
                                                                 return lhs.first < rhs.first;
                                                             });

        const int minX = minmaxPositionXIter.first->first;
        const int maxX = minmaxPositionXIter.second->first;

        const auto minmaxPositionYIter = std::minmax_element(m_occupiedPositions.cbegin(), m_occupiedPositions.cend(), [](const auto& lhs, const auto& rhs)
                                                             {
                                                                 return lhs.second < rhs.second;
                                                             });

        const int minY = minmaxPositionYIter.first->second;
        const int maxY = minmaxPositionYIter.second->second;

        unsigned numGroundTiles = 0;

        for (int j = minY; j <= maxY; ++j)
        {
            for (int i = minX; i <= maxX; ++i)
            {
                if (m_occupiedPositions.find({i, j}) == m_occupiedPositions.cend())
                {
                    ++numGroundTiles;
                }
            }
        }

        return numGroundTiles;
    }

    unsigned getNumFirstRoundWhereNoElfMoves() const
    {
        return m_numRoundsSimulated;
    }

private:
    using Coordinates = std::pair<int, int>;

    const Coordinates SOUTH = {0, 1};
    const Coordinates NORTH = {0, -1};
    const Coordinates EAST = {1, 0};
    const Coordinates WEST = {-1, 0};

    std::unordered_set<Coordinates, boost::hash<Coordinates>> m_occupiedPositions;
    std::deque<Coordinates> m_proposedDirectionsInOrder{NORTH, SOUTH, WEST, EAST};
    unsigned m_numRoundsSimulated = 0;

    bool simulateRound()
    {
        std::unordered_map<Coordinates, std::vector<Coordinates>, boost::hash<Coordinates>> proposedPositionToCurrentPositions;

        for (const auto& currentPosition : m_occupiedPositions)
        {
            auto proposedPosition = getProposedPosition(currentPosition);
            if (proposedPosition.has_value())
            {
                proposedPositionToCurrentPositions[proposedPosition.get()].push_back(currentPosition);
            }
        }

        // No more movement
        if (proposedPositionToCurrentPositions.empty())
        {
            return false;
        }

        for (const auto& proposedPositionAndCurrentPositions : proposedPositionToCurrentPositions)
        {
            const auto& proposedPosition = proposedPositionAndCurrentPositions.first;
            const auto& currentPositions = proposedPositionAndCurrentPositions.second;

            if (currentPositions.size() == 1)
            {
                m_occupiedPositions.erase(currentPositions.front());
                m_occupiedPositions.insert(proposedPosition);
            }
        }

        auto firstProposedDirection = m_proposedDirectionsInOrder.front();
        m_proposedDirectionsInOrder.pop_front();
        m_proposedDirectionsInOrder.push_back(std::move(firstProposedDirection));

        return true;
    }

    boost::optional<Coordinates> getProposedPosition(const Coordinates& currentPosition) const
    {
        if (areAllNeighborPositionsFree(currentPosition))
        {
            return boost::none;
        }

        for (const auto& proposedDirection : m_proposedDirectionsInOrder)
        {
            if (isFreeTowardsProposedDirection(currentPosition, proposedDirection))
            {
                return Coordinates{currentPosition.first + proposedDirection.first, currentPosition.second + proposedDirection.second};
            }
        }

        return boost::none;
    }

    bool areAllNeighborPositionsFree(const Coordinates& currentPosition) const
    {
        std::vector<Coordinates> allNeighborPositions;

        for (int j = -1; j <= 1; ++j)
        {
            for (int i = -1; i <= 1; ++i)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }

                allNeighborPositions.emplace_back(currentPosition.first + i, currentPosition.second + j);
            }
        }

        return areAllPositionsFree(allNeighborPositions);
    }

    bool areAllPositionsFree(const std::vector<Coordinates>& positions) const
    {
        return std::all_of(positions.cbegin(), positions.cend(), [this](const auto& coords)
                           {
                               return this->m_occupiedPositions.find(coords) == this->m_occupiedPositions.cend();
                           });
    }

    bool isFreeTowardsProposedDirection(const Coordinates& currentPosition, const Coordinates& proposedDirection) const
    {
        std::vector<Coordinates> relevantPositionsToCheck;

        Coordinates relevantPositionToCheck{currentPosition.first + proposedDirection.first, currentPosition.second + proposedDirection.second};

        relevantPositionsToCheck.push_back(relevantPositionToCheck);

        if (proposedDirection.second != 0)
        {
            relevantPositionsToCheck.emplace_back(relevantPositionToCheck.first - 1, relevantPositionToCheck.second);
            relevantPositionsToCheck.emplace_back(relevantPositionToCheck.first + 1, relevantPositionToCheck.second);
        }
        else
        {
            relevantPositionsToCheck.emplace_back(relevantPositionToCheck.first, relevantPositionToCheck.second + 1);
            relevantPositionsToCheck.emplace_back(relevantPositionToCheck.first, relevantPositionToCheck.second - 1);
        }

        return areAllPositionsFree(relevantPositionsToCheck);
    }
};

unsigned numGroundTilesInBoundingRectangle(const std::vector<std::string>& initialPositionsLines)
{
    ElfMovementSimulator elfMovementSimulator{initialPositionsLines};

    elfMovementSimulator.simulate(NUM_DAYS_TO_SIMULATE_FIRST_PART);

    return elfMovementSimulator.getNumGroundTilesInBoundingRectangle();
}

unsigned numFirstRoundWhereNoElfMoves(const std::vector<std::string>& initialPositionsLines)
{
    ElfMovementSimulator elfMovementSimulator{initialPositionsLines};

    elfMovementSimulator.simulate();

    return elfMovementSimulator.getNumFirstRoundWhereNoElfMoves();
}

}
}
}