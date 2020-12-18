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

using Vector3D = Eigen::Matrix<int, 3, 1>;

struct Vector3DHash
{
    size_t operator()(const Vector3D& vector) const
    {
        size_t seed = 0;

        boost::hash_combine(seed, vector[0]);
        boost::hash_combine(seed, vector[1]);
        boost::hash_combine(seed, vector[2]);

        return seed;
    }
};

class ThreeDimensionalGameOfLifeSimulator
{
public:
    ThreeDimensionalGameOfLifeSimulator(std::vector<Vector3D> startingActiveCoordinatesVector)
    {
        for (auto& startingActiveCoordinates : startingActiveCoordinatesVector)
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
    std::unordered_set<Vector3D, Vector3DHash> m_activeCoordinatesSet;

    void simulateRound()
    {
        std::unordered_set<Vector3D, Vector3DHash> nextRoundActiveCoordinatesSet;

        for (const auto& coordinates : getAllRelevantCoordinatesForNextRound())
        {
            if (isCoordinateActiveInNextRound(coordinates))
            {
                nextRoundActiveCoordinatesSet.insert(coordinates);
            }
        }

        m_activeCoordinatesSet = nextRoundActiveCoordinatesSet;
    }

    std::vector<Vector3D> getAllRelevantCoordinatesForNextRound() const
    {
        std::vector<Vector3D> allRelevantCoordinatesForNextRound;

        for (const auto& coordinates : m_activeCoordinatesSet)
        {
            allRelevantCoordinatesForNextRound.push_back(coordinates);

            std::vector<Vector3D> allNeighborCoordinates = getAllNeighborCoordinates(coordinates);
            allRelevantCoordinatesForNextRound.insert(allRelevantCoordinatesForNextRound.end(),
                                                      std::make_move_iterator(allNeighborCoordinates.begin()),
                                                      std::make_move_iterator(allNeighborCoordinates.end()));
        }

        return allRelevantCoordinatesForNextRound;
    }

    bool isCoordinateActiveInNextRound(const Vector3D& coordinates) const
    {
        const bool isCurrentlyActive = (m_activeCoordinatesSet.find(coordinates) != m_activeCoordinatesSet.cend());
        const int numActiveNeighbors = getNumActiveNeighbors(coordinates);

        const bool isActiveRemainingActive = isCurrentlyActive && (numActiveNeighbors == 2 || numActiveNeighbors == 3);
        const bool isInactiveBecomingActive = !isCurrentlyActive && numActiveNeighbors == 3;

        return isActiveRemainingActive || isInactiveBecomingActive;
    }

    std::vector<Vector3D> getAllNeighborCoordinates(const Vector3D& coordinates) const
    {
        std::vector<Vector3D> allNeighborCoordinates;

        for (int i = coordinates[0] - 1; i <= coordinates[0] + 1; ++i)
        {
            for (int j = coordinates[1] - 1; j <= coordinates[1] + 1; ++j)
            {
                for (int k = coordinates[2] - 1; k <= coordinates[2] + 1; ++k)
                {
                    Vector3D neighborCoordinates{i, j, k};
                    if (coordinates != neighborCoordinates)
                    {
                        allNeighborCoordinates.push_back(std::move(neighborCoordinates));
                    }
                }
            }
        }

        return allNeighborCoordinates;
    }

    int getNumActiveNeighbors(const Vector3D& coordinates) const
    {
        std::vector<Vector3D> allNeighborCoordinates = getAllNeighborCoordinates(coordinates);
        return std::count_if(allNeighborCoordinates.cbegin(), allNeighborCoordinates.cend(), [this](const auto& coords)
                             {
                                 return this->m_activeCoordinatesSet.find(coords) != this->m_activeCoordinatesSet.cend();
                             });
    }
};

std::vector<Vector3D> createStartingActiveCoordinatesVector(const std::vector<std::string>& initialStateLines)
{
    std::vector<Vector3D> startingActiveCoordinatesVector;

    for (int j = 0; j < initialStateLines.size(); ++j)
    {
        for (int i = 0; i < initialStateLines.at(j).size(); ++i)
        {
            if (initialStateLines.at(j).at(i) == ACTIVE_CHAR)
            {
                startingActiveCoordinatesVector.emplace_back(i, j, 0);
            }
        }
    }

    return startingActiveCoordinatesVector;
}

int numCubesInActiveStateAfterBootCycle(const std::vector<std::string>& initialStateLines)
{
    std::vector<Vector3D> startingActiveCoordinatesVector = createStartingActiveCoordinatesVector(initialStateLines);
    ThreeDimensionalGameOfLifeSimulator simulator{std::move(startingActiveCoordinatesVector)};
    simulator.simulateRounds(BOOT_CYCLE_LENGTH);
    return simulator.getNumActiveCoordinates();
}

}
