#include "Day17-ConwayCubes.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>
#include <boost/functional/hash/hash.hpp>

#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char ACTIVE_CHAR = '#';
const size_t BOOT_CYCLE_LENGTH = 6;

}

namespace AdventOfCode
{

using MultiDimensionalVector = Eigen::Matrix<int, Eigen::Dynamic, 1>;

struct MultiDimensionalVectorHash
{
    size_t operator()(const MultiDimensionalVector& vector) const
    {
        size_t seed = 0;

        for (size_t i = 0; i < vector.size(); ++i)
        {
            boost::hash_combine(seed, vector[i]);
        }

        return seed;
    }
};

class MultiDimensionalGameOfLifeSimulator
{
public:
    MultiDimensionalGameOfLifeSimulator(std::vector<MultiDimensionalVector> allStartingActiveCoordinates)
    {
        for (auto& startingActiveCoordinates : allStartingActiveCoordinates)
        {
            m_activeCoordinatesSet.insert(std::move(startingActiveCoordinates));
        }
    }

    void simulateRounds(size_t numRounds)
    {
        for (size_t i = 0; i < numRounds; ++i)
        {
            simulateRound();
        }
    }

    int getNumActiveCoordinates() const
    {
        return m_activeCoordinatesSet.size();
    }

private:
    std::unordered_set<MultiDimensionalVector, MultiDimensionalVectorHash> m_activeCoordinatesSet;

    void simulateRound()
    {
        std::unordered_set<MultiDimensionalVector, MultiDimensionalVectorHash> nextRoundActiveCoordinatesSet;

        for (const auto& coordinates : getAllRelevantCoordinatesForNextRound())
        {
            if (isCoordinateActiveInNextRound(coordinates))
            {
                nextRoundActiveCoordinatesSet.insert(coordinates);
            }
        }

        m_activeCoordinatesSet = nextRoundActiveCoordinatesSet;
    }

    std::unordered_set<MultiDimensionalVector, MultiDimensionalVectorHash> getAllRelevantCoordinatesForNextRound() const
    {
        std::unordered_set<MultiDimensionalVector, MultiDimensionalVectorHash> allRelevantCoordinatesForNextRound;

        for (const auto& activeCoordinates : m_activeCoordinatesSet)
        {
            allRelevantCoordinatesForNextRound.insert(activeCoordinates);

            std::vector<MultiDimensionalVector> allNeighborCoordinates = getAllNeighborCoordinates(activeCoordinates);
            for(const auto& neighborCoordinates : allNeighborCoordinates)
            {
                allRelevantCoordinatesForNextRound.insert(neighborCoordinates);
            }
        }

        return allRelevantCoordinatesForNextRound;
    }

    bool isCoordinateActiveInNextRound(const MultiDimensionalVector& coordinates) const
    {
        const bool isCurrentlyActive = (m_activeCoordinatesSet.find(coordinates) != m_activeCoordinatesSet.cend());
        const int numActiveNeighbors = getNumActiveNeighbors(coordinates);

        const bool isActiveRemainingActive = isCurrentlyActive && (numActiveNeighbors == 2 || numActiveNeighbors == 3);
        const bool isInactiveBecomingActive = !isCurrentlyActive && numActiveNeighbors == 3;

        return isActiveRemainingActive || isInactiveBecomingActive;
    }

    std::vector<MultiDimensionalVector> getAllNeighborCoordinates(const MultiDimensionalVector& coordinates) const
    {
        std::vector<MultiDimensionalVector> allNeighborCoordinates;

        getAllNeighborCoordinatesRecursive(coordinates, coordinates, 0, allNeighborCoordinates);

        return allNeighborCoordinates;
    }

    void getAllNeighborCoordinatesRecursive(const MultiDimensionalVector& originCoordinates,
                                            const MultiDimensionalVector& neighborCandidateCoordinates,
                                            size_t currentDimension,
                                            std::vector<MultiDimensionalVector>& result) const
    {
        if (currentDimension == originCoordinates.size())
        {
            if (originCoordinates != neighborCandidateCoordinates)
            {
                result.push_back(neighborCandidateCoordinates);
            }
            return;
        }

        for (int i = originCoordinates[currentDimension] - 1; i <= originCoordinates[currentDimension] + 1; ++i)
        {
            auto neighborCandidateCoordinatesCopy{neighborCandidateCoordinates};
            neighborCandidateCoordinatesCopy[currentDimension] = i;
            getAllNeighborCoordinatesRecursive(originCoordinates, neighborCandidateCoordinatesCopy, currentDimension + 1, result);
        }
    }

    int getNumActiveNeighbors(const MultiDimensionalVector& coordinates) const
    {
        std::vector<MultiDimensionalVector> allNeighborCoordinates = getAllNeighborCoordinates(coordinates);
        return std::count_if(allNeighborCoordinates.cbegin(), allNeighborCoordinates.cend(), [this](const auto& coords)
                             {
                                 return this->m_activeCoordinatesSet.find(coords) != this->m_activeCoordinatesSet.cend();
                             });
    }
};

std::vector<MultiDimensionalVector> createStartingActiveCoordinatesVector(const std::vector<std::string>& initialStateLines, size_t numDimensions)
{
    std::vector<MultiDimensionalVector> startingActiveCoordinatesVector;

    for (int j = 0; j < initialStateLines.size(); ++j)
    {
        for (int i = 0; i < initialStateLines.at(j).size(); ++i)
        {
            if (initialStateLines.at(j).at(i) == ACTIVE_CHAR)
            {
                MultiDimensionalVector coordinates = MultiDimensionalVector::Zero(numDimensions);
                coordinates[0] = i;
                coordinates[1] = j;
                startingActiveCoordinatesVector.push_back(std::move(coordinates));
            }
        }
    }

    return startingActiveCoordinatesVector;
}

int numCubesInActiveStateAfterBootCycle3D(const std::vector<std::string>& initialStateLines)
{
    std::vector<MultiDimensionalVector> startingActiveCoordinatesVector = createStartingActiveCoordinatesVector(initialStateLines, 3);
    MultiDimensionalGameOfLifeSimulator simulator{std::move(startingActiveCoordinatesVector)};
    simulator.simulateRounds(BOOT_CYCLE_LENGTH);
    return simulator.getNumActiveCoordinates();
}

int numCubesInActiveStateAfterBootCycle4D(const std::vector<std::string>& initialStateLines)
{
    std::vector<MultiDimensionalVector> startingActiveCoordinatesVector = createStartingActiveCoordinatesVector(initialStateLines, 4);
    MultiDimensionalGameOfLifeSimulator simulator{std::move(startingActiveCoordinatesVector)};
    simulator.simulateRounds(BOOT_CYCLE_LENGTH);
    return simulator.getNumActiveCoordinates();
}

}
