#include "Day22-SandSlabs.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>
#include <Eigen/dense>

#include <unordered_set>
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

        for (Vector3D position = m_startPosition; position != m_endPosition + difference; position += difference)
        {
            allOccupiedPositions.push_back(position);
        }

        return allOccupiedPositions;
    }

    bool operator==(const Brick& other) const
    {
        return m_startPosition == other.m_startPosition && m_endPosition == other.m_endPosition;
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

    }

    int determineNumBricksSafeToDisintegrate()
    {
        settleAllIgnoring(boost::none);

        int numBricksSafeToDisintegrate = 0;

        std::vector<Brick> brickCandidatesForDisintegration{m_bricks.begin(), m_bricks.end()};
        for (const auto& brickCandidateForDisintegration : brickCandidatesForDisintegration)
        {
            auto bricksBeforeDisintegration = m_bricks;

            int numBricksMoved = settleAllIgnoring(brickCandidateForDisintegration);

            if (numBricksMoved == 0)
            {
                ++numBricksSafeToDisintegrate;
            }

            m_bricks = bricksBeforeDisintegration;
        }

        return numBricksSafeToDisintegrate;
    }

private:
    std::vector<Brick> m_bricks;
    std::unordered_set<Vector3D, Vector3DHash> m_occupiedPositions;

    // Returns the number of bricks moved during the settling process
    int settleAllIgnoring(boost::optional<const Brick&> ignoredBrick)
    {
        setOccupiedPositions(ignoredBrick);

        std::sort(m_bricks.begin(), m_bricks.end(), [](const auto& lhs, const auto& rhs)
                  {
                      return lhs.getLowestZCoordinate() < rhs.getLowestZCoordinate();
                  });

        int numBricksMoved = 0;

        for (auto& brick : m_bricks)
        {
            if (ignoredBrick.has_value() && ignoredBrick.get() == brick)
            {
                continue;
            }

            if (settleBrick(brick))
            {
                ++numBricksMoved;
            }
        }

        return numBricksMoved;
    }

    // Returns true if the brick has moved as part of settling
    bool settleBrick(Brick& brick)
    {
        eraseBrick(brick);

        int numDownMovements = 0;

        while (!isBrickColliding(brick))
        {
            brick.moveDown();
            ++numDownMovements;
        }

        // We deliberately moved the brick one step too far to find the contradiction, so the settled position is actually the previous position
        brick.moveUp();
        --numDownMovements;

        insertBrick(brick);

        return numDownMovements > 0;
    }

    void setOccupiedPositions(boost::optional<const Brick&> ignoredBrick)
    {
        m_occupiedPositions.clear();
        for (const auto& brick : m_bricks)
        {
            if (ignoredBrick.has_value() && ignoredBrick.get() == brick)
            {
                continue;
            }

            for (const auto& position : brick.getAllOccupiedPositions())
            {
                m_occupiedPositions.insert(position);
            }
        }
    }

    void eraseBrick(const Brick& brick)
    {
        for (const Vector3D& position : brick.getAllOccupiedPositions())
        {
            m_occupiedPositions.erase(position);
        }
    }

    void insertBrick(const Brick& brick)
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

int numBricksSafeToDisintegrate(const std::vector<std::string>& brickSnapshotLines)
{
    std::vector<Brick> bricks = parseBrickSnapshotLines(brickSnapshotLines);
    BrickFallingSimulator brickFallingSimulator{std::move(bricks)};

    return brickFallingSimulator.determineNumBricksSafeToDisintegrate();
}

}
}
}