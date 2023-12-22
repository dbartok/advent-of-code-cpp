#include "Day22-SandSlabs.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>
#include <Eigen/dense>

#include <unordered_set>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day22
{

using Vector3D = Eigen::Vector3i;

struct Vector3DHash
{
    size_t operator()(const Vector3D& vec) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, vec.x());
        boost::hash_combine(seed, vec.y());
        boost::hash_combine(seed, vec.z());

        return seed;
    }
};

class Brick
{
public:
    Brick(Vector3D startPosition, Vector3D endPosition)
        : m_startPosition{std::move(startPosition)}
        , m_endPosition{std::move(endPosition)}
    {

    }

    void moveDown()
    {
        m_startPosition.z() -= 1;
        m_endPosition.z() -= 1;
    }

    void moveUp()
    {
        m_startPosition.z() += 1;
        m_endPosition.z() += 1;
    }

    int getLowestZCoordinate() const
    {
        return std::min(m_startPosition.z(), m_endPosition.z());
    }

    std::vector<Vector3D> getAllOccupiedPositions() const
    {
        Vector3D difference = m_endPosition - m_startPosition;
        difference.normalize();

        std::vector<Vector3D> allOccupiedPositions;

        for (Vector3D position = m_startPosition; position != m_endPosition; position += difference)
        {
            allOccupiedPositions.push_back(position);
        }

        allOccupiedPositions.push_back(m_endPosition);

        return allOccupiedPositions;
    }

    bool operator==(const Brick& other) const
    {
        return m_startPosition == other.m_startPosition && m_endPosition == other.m_endPosition;
    }

    bool operator!=(const Brick& other) const
    {
        return !(*this == other);
    }

private:
    Vector3D m_startPosition;
    Vector3D m_endPosition;
};

class BrickFallingSimulator
{
public:
    BrickFallingSimulator(std::vector<Brick> bricks)
        : m_bricks{std::move(bricks)}
    {
        for (const auto& brick : m_bricks)
        {
            for (const auto& position : brick.getAllOccupiedPositions())
            {
                m_occupiedPositions.insert(position);
            }
        }
    }

    // Returns the number of bricks moved during the settling process
    int settle()
    {
        std::sort(m_bricks.begin(), m_bricks.end(), [](const auto& lhs, const auto& rhs)
                  {
                      return lhs.getLowestZCoordinate() < rhs.getLowestZCoordinate();
                  });

        int numBricksMoved = 0;

        for (auto& brick : m_bricks)
        {
            if (settleBrick(brick))
            {
                ++numBricksMoved;
            }
        }

        return numBricksMoved;
    }

    const std::vector<Brick>& getBricks() const
    {
        return m_bricks;
    }

private:
    std::vector<Brick> m_bricks;
    std::unordered_set<Vector3D, Vector3DHash> m_occupiedPositions;

    // Returns true if the brick has moved as part of settling
    bool settleBrick(Brick& brick)
    {
        eraseBrickFromOccupiedPositions(brick);

        int numDownwardsMovements = 0;

        while (!isBrickColliding(brick))
        {
            brick.moveDown();
            ++numDownwardsMovements;
        }

        // We deliberately moved the brick one step too far to find the collision, so the settled position is actually the previous position
        brick.moveUp();
        --numDownwardsMovements;

        insertBrickIntoOccupiedPositions(brick);

        return numDownwardsMovements > 0;
    }

    void eraseBrickFromOccupiedPositions(const Brick& brick)
    {
        for (const Vector3D& position : brick.getAllOccupiedPositions())
        {
            m_occupiedPositions.erase(position);
        }
    }

    void insertBrickIntoOccupiedPositions(const Brick& brick)
    {
        for (const Vector3D& position : brick.getAllOccupiedPositions())
        {
            m_occupiedPositions.insert(position);
        }
    }

    bool isBrickColliding(const Brick& brick) const
    {
        // Check for collision with floor
        if (brick.getLowestZCoordinate() < 1)
        {
            return true;
        }

        // Check for collision with another brick
        std::vector<Vector3D> allOccupiedPositions = brick.getAllOccupiedPositions();
        return std::any_of(allOccupiedPositions.cbegin(), allOccupiedPositions.cend(), [this](const auto& pos)
                           {
                               return this->m_occupiedPositions.find(pos) != this->m_occupiedPositions.cend();
                           });
    }
};

Vector3D parsePositionString(const std::string& positionString)
{
    std::vector<int> coordinates;
    AdventOfCode::Utils::splitStringIntoTypedVector(positionString, coordinates, ',');

    return Vector3D{coordinates.data()};
}

Brick parseBrickSnapshotLine(const std::string& brickSnapshotLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, brickSnapshotLine, boost::is_any_of("~"));

    Vector3D startPosition = parsePositionString(tokens.front());
    Vector3D endPosition = parsePositionString(tokens.back());

    return Brick{std::move(startPosition), std::move(endPosition)};
}

std::vector<Brick> parseBrickSnapshotLines(const std::vector<std::string>& brickSnapshotLines)
{
    std::vector<Brick> bricks;

    for (const auto& line : brickSnapshotLines)
    {
        Brick brick = parseBrickSnapshotLine(line);
        bricks.push_back(std::move(brick));
    }

    return bricks;
}

std::vector<Brick> createCopyWithOneElementRemoved(const std::vector<Brick>& initiallySettledBricks, const Brick& brickToDisintegrate)
{
    std::vector<Brick> copyWithOneElementRemoved;

    for (const auto& brick : initiallySettledBricks)
    {
        if (brick != brickToDisintegrate)
        {
            copyWithOneElementRemoved.push_back(brick);
        }
    }

    return copyWithOneElementRemoved;
}

std::vector<int> getNumBricksFallingForAllDisintegrations(const std::vector<std::string>& brickSnapshotLines)
{
    std::vector<Brick> inputBricks = parseBrickSnapshotLines(brickSnapshotLines);
    BrickFallingSimulator brickFallingSimulator{std::move(inputBricks)};

    brickFallingSimulator.settle();

    std::vector<Brick> initiallySettledBricks = brickFallingSimulator.getBricks();

    std::vector<int> numBricksFallingForAllDisintegrations;

    for (const auto& brickToDisintegrate : initiallySettledBricks)
    {
        std::vector<Brick> bricksWithOneBrickDisintegrated = createCopyWithOneElementRemoved(initiallySettledBricks, brickToDisintegrate);
        BrickFallingSimulator brickFallingSimulator{std::move(bricksWithOneBrickDisintegrated)};
        int numBricksMoved = brickFallingSimulator.settle();
        numBricksFallingForAllDisintegrations.push_back(numBricksMoved);
    }

    return numBricksFallingForAllDisintegrations;
}

int numBricksSafeToDisintegrate(const std::vector<std::string>& brickSnapshotLines)
{
    std::vector<int> numBricksFallingForAllDisintegrations = getNumBricksFallingForAllDisintegrations(brickSnapshotLines);

    return std::count(numBricksFallingForAllDisintegrations.cbegin(), numBricksFallingForAllDisintegrations.cend(), 0);
}

int sumOfNumBricksFallingAcrossAllDisintegrations(const std::vector<std::string>& brickSnapshotLines)
{
    std::vector<int> numBricksFallingForAllDisintegrations = getNumBricksFallingForAllDisintegrations(brickSnapshotLines);

    return std::accumulate(numBricksFallingForAllDisintegrations.cbegin(), numBricksFallingForAllDisintegrations.cend(), 0);
}

}
}
}