#include "Day24-LobbyLayout.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>
#include <boost/functional/hash.hpp>

#include <unordered_set>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

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

    void flipTiles()
    {
        for (const auto& tilePath : m_tilePaths)
        {
            flipTileAtPath(tilePath);
        }
    }

    int getNumTilesWithBlackSideUp() const
    {
        return m_tilesWithBlackSideUp.size();
    }

private:
    std::vector<std::string> m_tilePaths;
    std::unordered_set<Vector2D, Vector2DHash> m_tilesWithBlackSideUp;

    std::unordered_map<std::string, Vector2D> m_directionToUnitVector;

    void flipTileAtPath(const std::string& tilePath)
    {
        Vector2D tileCoordinates = getTileCoordinates(tilePath);
        flipTileAtCoordinates(tileCoordinates);
    }

    void flipTileAtCoordinates(const Vector2D& coordinates)
    {
        const auto insertionResult = m_tilesWithBlackSideUp.insert(coordinates);
        const bool wasInserted = insertionResult.second;
        if (!wasInserted)
        {
            const auto existingElementIterator = insertionResult.first;
            m_tilesWithBlackSideUp.erase(existingElementIterator);
        }
    }

    Vector2D getTileCoordinates(const std::string& tilePath)
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
};

int numTilesWithBlackSideUp(const std::vector<std::string>& tilePathLines)
{
    HexTileFlipper hexTileFlipper{tilePathLines};
    hexTileFlipper.flipTiles();
    return hexTileFlipper.getNumTilesWithBlackSideUp();
}

}
