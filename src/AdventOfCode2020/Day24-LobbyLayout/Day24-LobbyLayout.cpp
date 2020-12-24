#include "Day24-LobbyLayout.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>
#include <boost/functional/hash.hpp>

#include <unordered_set>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
    unsigned NUM_DAYS_TO_SIMULATE = 100;
}

namespace AdventOfCode
{

using Vector2D = Eigen::Matrix<int, 2, 1>;

struct Vector2DHash
{
    size_t operator()(const Vector2D& vec) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, vec.x());
        boost::hash_combine(seed, vec.y());

        return seed;
    }
};

// Coordinates are defined as following:
// (x-asis points towards east, y-axis  points towards southeast)
//
//           +     +
//          / \   / \
//         /   \ /   \
//        +     +     +
//        |0, -1|1, -1|
//        +     +     +
//       / \   / \   / \
//      /   \ /   \ /   \
//     +     +     +     +
//     |-1, 0|0, 0 |1, 0 |
//     +     +     +     +
//      \   / \   / \   /
//       \ /   \ /   \ /
//        +     +     +
//        |-1, 1|0, 1 |
//        +     +     +
//         \   / \   /
//          \ /   \ /
//           +     +
//
class HexTileFlipper
{
public:
    HexTileFlipper(std::vector<std::string> tilePaths)
        : m_tilePaths{std::move(tilePaths)}
    {
        m_directionToUnitVector =
        {
            {"e", {1, 0}},
            {"w", {-1, 0}},
            {"nw", {0, -1}},
            {"ne", {1, -1}},
            {"sw", {-1, 1}},
            {"se", {0, 1}},
        };
    }

    void setupInitialState()
    {
        for (const auto& tilePath : m_tilePaths)
        {
            flipTileAtPath(tilePath);
        }
    }

    void simulateMultipleDays(unsigned numDaysToSimulate)
    {
        for (unsigned i = 0; i < numDaysToSimulate; ++i)
        {
            simulateSingleDay();
        }
    }

    int getNumBlackTiles() const
    {
        return m_blackTileCoordinatesSet.size();
    }

private:
    std::vector<std::string> m_tilePaths;
    std::unordered_set<Vector2D, Vector2DHash> m_blackTileCoordinatesSet;

    std::unordered_map<std::string, Vector2D> m_directionToUnitVector;

    void flipTileAtPath(const std::string& tilePath)
    {
        Vector2D tileCoordinates = getTileCoordinatesForPath(tilePath);
        flipTileAtCoordinates(tileCoordinates);
    }

    void flipTileAtCoordinates(const Vector2D& coordinates)
    {
        const auto insertionResult = m_blackTileCoordinatesSet.insert(coordinates);
        const bool wasInserted = insertionResult.second;
        if (!wasInserted)
        {
            const auto existingElementIterator = insertionResult.first;
            m_blackTileCoordinatesSet.erase(existingElementIterator);
        }
    }

    void simulateSingleDay()
    {
        std::unordered_set<Vector2D, Vector2DHash> nextRoundBlackTileCoordinatesSet;

        for (const auto& coordinates : getAllRelevantCoordinatesForNextRound())
        {
            if (isTileBlackInNextRound(coordinates))
            {
                nextRoundBlackTileCoordinatesSet.insert(coordinates);
            }
        }

        m_blackTileCoordinatesSet = nextRoundBlackTileCoordinatesSet;
    }

    Vector2D getTileCoordinatesForPath(const std::string& tilePath) const
    {
        Vector2D coordinates{0, 0};
        std::string direction;

        for (const auto c : tilePath)
        {
            direction += c;
            const auto findResult = m_directionToUnitVector.find(direction);
            if (findResult != m_directionToUnitVector.cend())
            {
                coordinates += findResult->second;
                direction = "";
            }
        }

        return coordinates;
    }

    std::unordered_set<Vector2D, Vector2DHash> getAllRelevantCoordinatesForNextRound() const
    {
        std::unordered_set<Vector2D, Vector2DHash> allRelevantCoordinatesForNextRound;

        for (const auto& blackTileCoordinates : m_blackTileCoordinatesSet)
        {
            allRelevantCoordinatesForNextRound.insert(blackTileCoordinates);

            std::vector<Vector2D> allNeighborCoordinates = getAllAdjacentTileCoordinates(blackTileCoordinates);
            for (const auto& neighborCoordinates : allNeighborCoordinates)
            {
                allRelevantCoordinatesForNextRound.insert(neighborCoordinates);
            }
        }

        return allRelevantCoordinatesForNextRound;
    }

    bool isTileBlackInNextRound(const Vector2D& coordinates) const
    {
        const bool isTileCurrentlyBlack = (m_blackTileCoordinatesSet.find(coordinates) != m_blackTileCoordinatesSet.cend());
        const int numAdjacentBlackTiles = getNumAdjacentBlackTiles(coordinates);

        const bool isBlackTileRemainingBlack = isTileCurrentlyBlack && (numAdjacentBlackTiles == 1 || numAdjacentBlackTiles == 2);
        const bool isWhiteTileTurningOverToBlack = !isTileCurrentlyBlack && numAdjacentBlackTiles == 2;

        return isBlackTileRemainingBlack || isWhiteTileTurningOverToBlack;
    }

    std::vector<Vector2D> getAllAdjacentTileCoordinates(const Vector2D& coordinates) const
    {
        std::vector<Vector2D> allAdjacentTileCoordinates;

        for (const auto& directionAndUnitVector : m_directionToUnitVector)
        {
            const auto& unitVector = directionAndUnitVector.second;
            allAdjacentTileCoordinates.push_back(coordinates + unitVector);
        }

        return allAdjacentTileCoordinates;
    }

    int getNumAdjacentBlackTiles(const Vector2D& coordinates) const
    {
        std::vector<Vector2D> allAdjacentTileCoordinates = getAllAdjacentTileCoordinates(coordinates);
        return std::count_if(allAdjacentTileCoordinates.cbegin(), allAdjacentTileCoordinates.cend(), [this](const auto& coords)
                             {
                                 return this->m_blackTileCoordinatesSet.find(coords) != this->m_blackTileCoordinatesSet.cend();
                             });
    }
};

int numTilesWithBlackSideUpInInitialState(const std::vector<std::string>& tilePathLines)
{
    HexTileFlipper hexTileFlipper{tilePathLines};
    hexTileFlipper.setupInitialState();
    return hexTileFlipper.getNumBlackTiles();
}

int numTilesWithBlackSideUpAfterMultipleDays(const std::vector<std::string>& tilePathLines)
{
    HexTileFlipper hexTileFlipper{tilePathLines};
    hexTileFlipper.setupInitialState();
    hexTileFlipper.simulateMultipleDays(NUM_DAYS_TO_SIMULATE);
    return hexTileFlipper.getNumBlackTiles();
}

}
